#include "nn/nex/nex_ProtectedPacketQueue.h"

namespace nn {
namespace nex {
// 0x00397CDC slot 0x00
// 0x00397C60 (deleting dtor)
nn::nex::ProtectedPacketQueue::~ProtectedPacketQueue()
{
    ScopedCS lock(*m_pLock);
    Purge();
}

// 0x00397B88 slot 0x08 | slot vf_0x08 of nn::nex::PacketQueue
void nn::nex::ProtectedPacketQueue::Purge()
{
    ScopedCS lock(*m_pLock);
    PacketQueue::Purge();
}

// 0x00397BC0 slot 0x0C | slot vf_0x0C of nn::nex::PacketQueue
void nn::nex::ProtectedPacketQueue::Queue(nn::nex::Packet* pPacket, bool setTime)
{
    ScopedCS lock(*m_pLock);
    PacketQueue::Queue(pPacket, setTime);
}

// 0x00397B40 slot 0x10 | slot vf_0x10 of nn::nex::PacketQueue
void nn::nex::ProtectedPacketQueue::QueueFront(nn::nex::Packet* pPacket, bool setTime)
{
    ScopedCS lock(*m_pLock);
    PacketQueue::QueueFront(pPacket, setTime);
}

// 0x00397C08 slot 0x14 | slot vf_0x14 of nn::nex::PacketQueue
nn::nex::PacketQueue::PacketChain::iterator nn::nex::ProtectedPacketQueue::Dequeue(nn::nex::PacketQueue::PacketChain::iterator it)
{
    ScopedCS lock(*m_pLock);
    return PacketQueue::Dequeue(it);
}

// 0x00397C58 slot 0x18 | slot vf_0x18 of nn::nex::PacketQueue
nn::nex::CriticalSection* nn::nex::ProtectedPacketQueue::GetLock()
{
    return m_pLock;
}

} // namespace nex
} // namespace nn
