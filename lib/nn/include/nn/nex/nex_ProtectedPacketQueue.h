#pragma once

#include "decomp.h"
#include "nn/nex/nex_PacketQueue.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex20ProtectedPacketQueueE @ 0x008CE8FC
// vtable 0x008FD8C0 (vptr 0x008FD8C8), offset_to_top 0, 7 entries
class ProtectedPacketQueue : public ::nn::nex::PacketQueue
{
public:
    ProtectedPacketQueue(); // ctor address unknown
    virtual void vf_0x00(); // 0x00397CDC slot 0x00 | virtual slot, introduced by nn::nex::PacketQueue
    virtual ~ProtectedPacketQueue(); // 0x00397C60 slot 0x04 | fefates:callseq
    virtual void Purge(); // 0x00397B88 slot 0x08 | slot vf_0x08 of nn::nex::PacketQueue
    virtual void Queue(nn::nex::Packet*, bool); // 0x00397BC0 slot 0x0C | slot vf_0x0C of nn::nex::PacketQueue
    virtual void QueueFront(nn::nex::Packet*, bool); // 0x00397B40 slot 0x10 | slot vf_0x10 of nn::nex::PacketQueue
    virtual void vf_0x14(); // 0x00397C08 slot 0x14 | slot vf_0x14 of nn::nex::PacketQueue (was Dequeue)
    virtual void GetLock(); // 0x00397C58 slot 0x18 | slot vf_0x18 of nn::nex::PacketQueue
};
} // namespace nex
} // namespace nn
