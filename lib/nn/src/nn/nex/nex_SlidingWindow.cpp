#include "nn/nex/nex_SlidingWindow.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_PacketOut.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
namespace {
// the lock of the scheduler (the functions of the window run under it)
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}
} // namespace

// 0x0036E1AC | fefates:bytes [tier B]
nn::nex::SlidingWindow::SlidingWindow(unsigned short windowSize, unsigned int maxPackets)
    : m_WindowSize(windowSize), m_NextSequenceId(1), m_NextToSend(1), m_FirstUnacked(1), m_DataPacketCount(0), m_MaxPackets(maxPackets),
      m_Lock(0x20000000)
{
}

// 0x0036E298 (name is ours)
nn::nex::SlidingWindow::~SlidingWindow()
{
    ScopedCS lock(GetSchedulerLock());
    Purge();
}

// 0x0036D8D8 (name is ours)
void nn::nex::SlidingWindow::Acknowledge(const nn::nex::LogicalClockTmpl<unsigned short, short>& sequenceId, nn::nex::qMap<unsigned int, unsigned int>* pIdCounts)
{
    ScopedCS lock(GetSchedulerLock());
    PacketMap::iterator it = m_Packets.find(sequenceId);
    if (it == m_Packets.end()) {
        return;
    }
    PacketOut* pPacket = it->second;
    u32 id = pPacket->m_Unknown0x148;
    if (id != 0) {
        qMap<u32, u32>::iterator idIt = pIdCounts->find(id);
        if (idIt != pIdCounts->end() && --idIt->second == 0) {
            ScopedCS idLock(Network::GetInstance()->m_IdLock);
            Network::GetInstance()->m_ReleasedIds.push_back(id);
            pIdCounts->EraseInline(id);
        }
    }
    pPacket->m_Unknown0x131 = 1;
    m_Packets.erase(it++);
    if (sequenceId.m_Value == m_FirstUnacked.m_Value) {
        m_FirstUnacked.m_Value = it == m_Packets.end() ? m_NextSequenceId.m_Value : it->first.m_Value;
    }
    if (pPacket->GetType() == Packet::TYPE_DATA) {
        m_DataPacketCount--;
    }
    pPacket->ReleaseRef();
}

// 0x0036DC5C (name is ours)
nn::nex::PacketOut* nn::nex::SlidingWindow::GetNextPacketToSend()
{
    ScopedCS lock(GetSchedulerLock());
    PacketOut* pPacket = nullptr;
    if (static_cast<s16>(m_NextToSend.m_Value - m_FirstUnacked.m_Value) < m_WindowSize) {
        PacketMap::iterator it = m_Packets.find(m_NextToSend);
        if (it != m_Packets.end()) {
            m_NextToSend.m_Value++;
            pPacket = it->second;
        }
    }
    return pPacket;
}

// 0x0036DD2C (name is ours)
bool nn::nex::SlidingWindow::AddPacket(nn::nex::PacketOut* pPacket)
{
    ScopedCS lock(GetSchedulerLock());
    if (m_Packets.find(m_NextSequenceId) != m_Packets.end() || static_cast<s32>(m_MaxPackets - m_Packets.size()) <= 0) {
        return false;
    }
    pPacket->AcquireRef();
    if (pPacket->GetType() == Packet::TYPE_DATA) {
        m_DataPacketCount++;
    }
    m_Packets.IndexInline(m_NextSequenceId) = pPacket;
    pPacket->m_SequenceId.m_Value = m_NextSequenceId.m_Value;
    m_NextSequenceId.m_Value++;
    return true;
}

// 0x0036DFB0 (name is ours)
void nn::nex::SlidingWindow::Purge()
{
    ScopedCS lock(GetSchedulerLock());
    PacketMap::iterator it = m_Packets.begin();
    while (it != m_Packets.end()) {
        it->second->ReleaseRef();
        m_Packets.erase(it++);
    }
    m_NextToSend.m_Value = m_NextSequenceId.m_Value;
    m_FirstUnacked.m_Value = m_NextSequenceId.m_Value;
    m_DataPacketCount = 0;
}

// 0x0036E0E0 (name is ours)
nn::nex::PacketOut* nn::nex::SlidingWindow::GetPacket(const nn::nex::LogicalClockTmpl<unsigned short, short>& sequenceId)
{
    ScopedCS lock(GetSchedulerLock());
    PacketMap::iterator it = m_Packets.find(sequenceId);
    if (it == m_Packets.end()) {
        return nullptr;
    }
    return it->second;
}

} // namespace nex
} // namespace nn

namespace __rw {
// the functions of the tree of the packets
// 0x007FA9E4 (name is ours)
template __rb_tree<nn::nex::LogicalClockTmpl<u16, s16>, std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketOut*>, __select1st<std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketOut*>, nn::nex::LogicalClockTmpl<u16, s16> >, std::less<nn::nex::LogicalClockTmpl<u16, s16> >, nn::nex::MemAllocator<std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketOut*> > >::iterator __rb_tree<nn::nex::LogicalClockTmpl<u16, s16>, std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketOut*>, __select1st<std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketOut*>, nn::nex::LogicalClockTmpl<u16, s16> >, std::less<nn::nex::LogicalClockTmpl<u16, s16> >, nn::nex::MemAllocator<std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketOut*> > >::erase(iterator it);
// 0x007FB044 (name is ours)
template void __rb_tree<nn::nex::LogicalClockTmpl<u16, s16>, std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketOut*>, __select1st<std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketOut*>, nn::nex::LogicalClockTmpl<u16, s16> >, std::less<nn::nex::LogicalClockTmpl<u16, s16> >, nn::nex::MemAllocator<std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketOut*> > >::_C_erase(Node* pNode);
// 0x007FB080 (name is ours)
template __rb_tree<nn::nex::LogicalClockTmpl<u16, s16>, std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketOut*>, __select1st<std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketOut*>, nn::nex::LogicalClockTmpl<u16, s16> >, std::less<nn::nex::LogicalClockTmpl<u16, s16> >, nn::nex::MemAllocator<std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketOut*> > >::iterator __rb_tree<nn::nex::LogicalClockTmpl<u16, s16>, std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketOut*>, __select1st<std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketOut*>, nn::nex::LogicalClockTmpl<u16, s16> >, std::less<nn::nex::LogicalClockTmpl<u16, s16> >, nn::nex::MemAllocator<std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketOut*> > >::_C_insert(Node* pNode, Node* pParent, const std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketOut*>& value);

// the functions of the u32 -> u32 tree (std::map and std::multimap)
// 0x0080A72C | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE __rb_tree<u32, std::pair<const u32, u32>, __select1st<std::pair<const u32, u32>, u32>, std::less<u32>, nn::nex::MemAllocator<std::pair<const u32, u32> > >::iterator
__rb_tree<u32, std::pair<const u32, u32>, __select1st<std::pair<const u32, u32>, u32>, std::less<u32>, nn::nex::MemAllocator<std::pair<const u32, u32> > >::erase(iterator first, iterator last)
{
    return EraseRangeInline(first, last);
}
// 0x0080A0D0 (name is ours)
template __rb_tree<u32, std::pair<const u32, u32>, __select1st<std::pair<const u32, u32>, u32>, std::less<u32>, nn::nex::MemAllocator<std::pair<const u32, u32> > >::iterator __rb_tree<u32, std::pair<const u32, u32>, __select1st<std::pair<const u32, u32>, u32>, std::less<u32>, nn::nex::MemAllocator<std::pair<const u32, u32> > >::erase(iterator it);
// 0x0080A998 (name is ours)
template void __rb_tree<u32, std::pair<const u32, u32>, __select1st<std::pair<const u32, u32>, u32>, std::less<u32>, nn::nex::MemAllocator<std::pair<const u32, u32> > >::_C_erase(Node* pNode);
} // namespace __rw
