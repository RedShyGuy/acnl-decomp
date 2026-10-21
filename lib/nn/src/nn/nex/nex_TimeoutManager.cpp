#include "nn/nex/nex_TimeoutManager.h"
#include "nn/nex/nex_ConnectionOrientedStream.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_PRUDPEndPoint.h"
#include "nn/nex/nex_PacketOut.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_StreamSettings.h"
#include "nn/nex/nex_Timeout.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
namespace {
// the types of the packets (the low 4 bits of the type and flags; the names are ours)
const u16 TYPE_SYN = 0;
const u16 TYPE_CONNECT = 1;
const u16 TYPE_DISCONNECT = 3;
const u16 TYPE_PING = 4;
const u16 TYPE_MASK = 0xF;
// the fault reasons of a packet that is not resent any more
const u8 FAULT_REASON_EXPIRED = 12;
const u8 FAULT_REASON_RETRANSMISSIONS = 13;
// the fault event of a lost connection
const u32 FAULT_EVENT_TIMEOUT = 7;

// (inline) the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    return Scheduler::GetInstanceInline()->m_Lock;
}
} // namespace

// 0x00374A10 (name is ours)
bool nn::nex::TimeoutManager::UnschedulePacketTimeout(nn::nex::PacketOut* pPacket)
{
    ScopedCS lock(GetSchedulerLock());
    std::pair<std::multimap<Time, PacketOut*>::iterator, std::multimap<Time, PacketOut*>::iterator> range =
        m_Timeouts.equal_range(pPacket->m_pTimeout->m_Unknown0x08);
    for (std::multimap<Time, PacketOut*>::iterator it = range.first; it != range.second; ++it) {
        if (it->second == pPacket) {
            m_Timeouts.erase(it);
            m_Packets.erase(pPacket);
            pPacket->ReleaseRef();
            return true;
        }
    }
    return false;
}

// 0x00374CBC
void nn::nex::TimeoutManager::SchedulePacketTimeout(nn::nex::PacketOut* pPacket)
{
    ScopedCS lock(GetSchedulerLock());
    if (m_Unknown0x3C) {
        u16 type = pPacket->m_TypeFlags & TYPE_MASK;
        if (type == TYPE_SYN || type == TYPE_CONNECT || type == TYPE_DISCONNECT) {
            pPacket->m_pTimeout->m_Interval = 1;
            pPacket->m_pTimeout->m_ExpirationTime = Time(1);
        }
    }
    Timeout* pTimeout = pPacket->m_pTimeout;
    pTimeout->m_Unknown0x08 = SystemClock::GetTime() + static_cast<int>(pTimeout->m_Interval);
    pPacket->AcquireRef();
    if (m_Packets.find(pPacket) != m_Packets.end()) {
        // the packet was scheduled before: the old timeout is removed
        for (std::multimap<Time, PacketOut*>::iterator it = m_Timeouts.begin(); it != m_Timeouts.end(); ++it) {
            if (it->second == pPacket) {
                m_Timeouts.erase(it);
                m_Packets.erase(pPacket);
                pPacket->ReleaseRef();
                break;
            }
        }
    }
    m_Packets.InsertInline(pPacket);
    m_Timeouts.m_Tree.InsertInline(std::pair<const Time, PacketOut*>(pPacket->m_pTimeout->m_Unknown0x08, pPacket));
}

// 0x00375244 (name is ours)
void nn::nex::TimeoutManager::ProcessTimeouts()
{
    ScopedCS lock(GetSchedulerLock());
    qVector<PacketOut*> expired;
    std::multimap<Time, PacketOut*>::iterator it = m_Timeouts.begin();
    while (it != m_Timeouts.end()) {
        PacketOut* pPacket = it->second;
        Timeout* pTimeout = pPacket->m_pTimeout;
        if (static_cast<s64>(pTimeout->m_Unknown0x08.m_Value - SystemClock::s_LastTime.m_Value) >= 0
            && (pTimeout->m_ExpirationTime.m_Value == 0 || static_cast<s64>(pTimeout->m_ExpirationTime.m_Value - SystemClock::GetTime().m_Value) >= 0)) {
            Core* pCore = pTimeout->m_pCore;
            if (!pCore->m_IsInterrupted || static_cast<s64>(pCore->m_InterruptTime.m_Value - SystemClock::GetTime().m_Value) >= 0) {
                break;
            }
        }
        std::multimap<Time, PacketOut*>::iterator current = it;
        ++it;
        m_Timeouts.erase(current);
        m_Packets.erase(pPacket);
        expired.push_back(pPacket);
    }
    for (PacketOut** ppPacket = expired.m_pBegin; ppPacket != expired.m_pEnd; ppPacket++) {
        PacketOut* pPacket = *ppPacket;
        PRUDPEndPoint* pEndPoint = pPacket->m_pEndPoint;
        if ((pPacket->m_TypeFlags & TYPE_MASK) == TYPE_PING) {
            pEndPoint->TimeToPing();
        } else if (pEndPoint->m_pStream->GetThisSettings()->m_Unknown0x60 > pPacket->m_Unknown0x120 && !pPacket->m_pTimeout->IsExpired()) {
            pEndPoint->TransmitPacket(pPacket);
        } else {
            u16 type = pPacket->m_TypeFlags & TYPE_MASK;
            if (type == TYPE_DISCONNECT && pEndPoint->GetConnectionState() == EndPoint::STATE_DISCONNECTING) {
                pEndPoint->SetConnectionState(EndPoint::STATE_NOT_CONNECTED);
            }
            type = pPacket->m_TypeFlags & TYPE_MASK;
            if ((type == TYPE_CONNECT || type == TYPE_SYN) && pEndPoint->GetConnectionState() == EndPoint::STATE_CONNECTING) {
                pEndPoint->SetConnectionState(EndPoint::STATE_NOT_CONNECTED);
            }
            if (pPacket->m_pTimeout->IsExpired()) {
                pEndPoint->m_Statistics.m_Unknown0x38++;
                pEndPoint->m_FaultReason = FAULT_REASON_EXPIRED;
                pEndPoint->IsFaulty();
            } else {
                pEndPoint->m_FaultReason = FAULT_REASON_RETRANSMISSIONS;
                pEndPoint->IsFaulty();
            }
            if (!pEndPoint->IsFaulty()) {
                pEndPoint->SignalFaultEvent(FAULT_EVENT_TIMEOUT, false);
            }
        }
        pPacket->ReleaseRef();
    }
}

// 0x00375728 (name is ours)
void nn::nex::TimeoutManager::LimitExpirationTimes(const nn::nex::Time& deadline)
{
    ScopedCS lock(GetSchedulerLock());
    for (std::multimap<Time, PacketOut*>::iterator it = m_Timeouts.begin(); it != m_Timeouts.end(); ++it) {
        Timeout* pTimeout = it->second->m_pTimeout;
        if (pTimeout->m_ExpirationTime.m_Value != 0 && deadline.m_Value < pTimeout->m_ExpirationTime.m_Value) {
            pTimeout->m_ExpirationTime = deadline;
        }
    }
}

// 0x00375828 | fefates:bytes [tier B]
nn::nex::TimeoutManager::TimeoutManager() : m_Unknown0x3C(false)
{
    m_Timeouts.m_Tree.m_IsInsertAlways = true;
}

// 0x0037594C | fefates:bytes [tier B]
nn::nex::TimeoutManager::~TimeoutManager()
{
    // nothing to do: the trees are destroyed by the compiler
}

} // namespace nex
} // namespace nn

namespace __rw {
// 0x00801F98 | fefates:callgraph [tier C]
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::PacketOut*, nn::nex::PacketOut*, __ident<nn::nex::PacketOut*, nn::nex::PacketOut*>, std::less<nn::nex::PacketOut*>,
                          nn::nex::MemAllocator<nn::nex::PacketOut*> >::iterator
__rb_tree<nn::nex::PacketOut*, nn::nex::PacketOut*, __ident<nn::nex::PacketOut*, nn::nex::PacketOut*>, std::less<nn::nex::PacketOut*>,
          nn::nex::MemAllocator<nn::nex::PacketOut*> >::erase(iterator first, iterator last)
{
    return EraseRangeAllInline(first, last);
}

// 0x00802740 (name is ours)
template __rb_tree<nn::nex::PacketOut*, nn::nex::PacketOut*, __ident<nn::nex::PacketOut*, nn::nex::PacketOut*>, std::less<nn::nex::PacketOut*>,
                   nn::nex::MemAllocator<nn::nex::PacketOut*> >::iterator
__rb_tree<nn::nex::PacketOut*, nn::nex::PacketOut*, __ident<nn::nex::PacketOut*, nn::nex::PacketOut*>, std::less<nn::nex::PacketOut*>,
          nn::nex::MemAllocator<nn::nex::PacketOut*> >::_C_insert(Node* pNode, Node* pParent, nn::nex::PacketOut* const& value);

// 0x007FBFA0 | fefates:callgraph [tier C]
template __rb_tree<nn::nex::Time, std::pair<const nn::nex::Time, nn::nex::PacketOut*>, __select1st<std::pair<const nn::nex::Time, nn::nex::PacketOut*>, nn::nex::Time>,
                   std::less<nn::nex::Time>, nn::nex::MemAllocator<std::pair<const nn::nex::Time, nn::nex::PacketOut*> > >::iterator
__rb_tree<nn::nex::Time, std::pair<const nn::nex::Time, nn::nex::PacketOut*>, __select1st<std::pair<const nn::nex::Time, nn::nex::PacketOut*>, nn::nex::Time>,
          std::less<nn::nex::Time>, nn::nex::MemAllocator<std::pair<const nn::nex::Time, nn::nex::PacketOut*> > >::erase(iterator it);

// 0x007FC5FC | fefates:callgraph [tier C]
template void __rb_tree<nn::nex::Time, std::pair<const nn::nex::Time, nn::nex::PacketOut*>, __select1st<std::pair<const nn::nex::Time, nn::nex::PacketOut*>, nn::nex::Time>,
                        std::less<nn::nex::Time>, nn::nex::MemAllocator<std::pair<const nn::nex::Time, nn::nex::PacketOut*> > >::_C_erase(Node* pNode);

// 0x007FC638 (name is ours)
template __rb_tree<nn::nex::Time, std::pair<const nn::nex::Time, nn::nex::PacketOut*>, __select1st<std::pair<const nn::nex::Time, nn::nex::PacketOut*>, nn::nex::Time>,
                   std::less<nn::nex::Time>, nn::nex::MemAllocator<std::pair<const nn::nex::Time, nn::nex::PacketOut*> > >::iterator
__rb_tree<nn::nex::Time, std::pair<const nn::nex::Time, nn::nex::PacketOut*>, __select1st<std::pair<const nn::nex::Time, nn::nex::PacketOut*>, nn::nex::Time>,
          std::less<nn::nex::Time>, nn::nex::MemAllocator<std::pair<const nn::nex::Time, nn::nex::PacketOut*> > >::_C_insert(Node* pNode, Node* pParent,
                                                                                                                          const std::pair<const nn::nex::Time, nn::nex::PacketOut*>& value);
} // namespace __rw
