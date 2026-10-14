#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_PacketQueue.h"

namespace nn {
namespace nex {
// A PacketQueue whose functions run under a lock (the lock of the scheduler in SocketTransport).
// The member names are ours.
// RTTI N2nn3nex20ProtectedPacketQueueE @ 0x008CE8FC
// vtable 0x008FD8C0 (vptr 0x008FD8C8), offset_to_top 0, 7 entries
class ProtectedPacketQueue : public ::nn::nex::PacketQueue
{
public:
    // (inline)
    explicit ProtectedPacketQueue(CriticalSection* pLock) : m_pLock(pLock) {}
    virtual ~ProtectedPacketQueue(); // 0x00397CDC slot 0x00
    // 0x00397C60 slot 0x04 (deleting dtor)
    virtual void Purge(); // 0x00397B88 slot 0x08 | slot vf_0x08 of nn::nex::PacketQueue
    virtual void Queue(nn::nex::Packet* pPacket, bool setTime); // 0x00397BC0 slot 0x0C | slot vf_0x0C of nn::nex::PacketQueue
    virtual void QueueFront(nn::nex::Packet* pPacket, bool setTime); // 0x00397B40 slot 0x10 | slot vf_0x10 of nn::nex::PacketQueue
    virtual nn::nex::PacketQueue::PacketChain::iterator Dequeue(nn::nex::PacketQueue::PacketChain::iterator it); // 0x00397C08 slot 0x14 | slot vf_0x14 of nn::nex::PacketQueue
    virtual nn::nex::CriticalSection* GetLock(); // 0x00397C58 slot 0x18 | slot vf_0x18 of nn::nex::PacketQueue

    CriticalSection* m_pLock; // 0x18
};
ASSERT_SIZE(ProtectedPacketQueue, 0x1C);
} // namespace nex
} // namespace nn
