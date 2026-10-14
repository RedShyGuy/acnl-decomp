#include "nn/nex/nex_PacketQueue.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
// 0x0035B610 slot 0x00
// 0x0035B5D0 (deleting dtor)
nn::nex::PacketQueue::~PacketQueue()
{
    Purge();
}

// 0x0035B408 slot 0x08 | mk7dlp:bytes
void nn::nex::PacketQueue::Purge()
{
    PacketChain::iterator it = m_Packets.begin();
    while (it != m_Packets.end()) {
        it = Dequeue(it);
    }
}

// 0x0035B454 slot 0x0C | slot vf_0x0C of nn::nex::PacketQueue
void nn::nex::PacketQueue::Queue(nn::nex::Packet* pPacket, bool setTime)
{
    if (pPacket->m_pChainPrev != nullptr || pPacket->m_pChainNext != nullptr || m_Packets.m_pFirst == pPacket) {
        return;
    }
    pPacket->AcquireRef();
    m_Packets.push_back(pPacket);
    if (setTime) {
        pPacket->m_Unknown0x108 = SystemClock::s_LastTime;
    }
}

// 0x0035B344 slot 0x10 | fefates:bytes
void nn::nex::PacketQueue::QueueFront(nn::nex::Packet* pPacket, bool setTime)
{
    if (pPacket->m_pChainPrev != nullptr || pPacket->m_pChainNext != nullptr || m_Packets.m_pFirst == pPacket) {
        return;
    }
    pPacket->AcquireRef();
    m_Packets.push_front(pPacket);
    if (setTime) {
        pPacket->m_Unknown0x108 = SystemClock::GetTime();
    }
}

// 0x0035B514 slot 0x14 | fefates:bytes
nn::nex::PacketQueue::PacketChain::iterator nn::nex::PacketQueue::Dequeue(nn::nex::PacketQueue::PacketChain::iterator it)
{
    Packet* pPacket = *it;
    it = m_Packets.EraseInline(it);
    pPacket->m_Unknown0x108 = 0;
    pPacket->ReleaseRef();
    return it;
}

// 0x0035B5C8 slot 0x18 | slot vf_0x18 of nn::nex::PacketQueue
nn::nex::CriticalSection* nn::nex::PacketQueue::GetLock()
{
    return nullptr;
}

// 0x007E46FC
// 0x007E46CC (deleting dtor)
template qChain<Packet*, DefaultChainPolicy<Packet*> >::~qChain();

} // namespace nex
} // namespace nn
