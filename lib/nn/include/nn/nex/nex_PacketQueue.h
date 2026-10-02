#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11PacketQueueE @ 0x008CE05C
// vtable 0x008FC3C4 (vptr 0x008FC3CC), offset_to_top 0, 7 entries
class PacketQueue : public ::nn::nex::RootObject
{
public:
    PacketQueue(); // ctor address unknown
    virtual void vf_0x00(); // 0x0035B610 slot 0x00 | virtual slot, introduced by nn::nex::PacketQueue
    virtual ~PacketQueue(); // 0x0035B5D0 slot 0x04 | fefates:bytes
    virtual void Purge(); // 0x0035B408 slot 0x08 | mk7dlp:bytes
    virtual void Queue(nn::nex::Packet*, bool); // 0x0035B454 slot 0x0C | slot vf_0x0C of nn::nex::PacketQueue
    virtual void QueueFront(nn::nex::Packet*, bool); // 0x0035B344 slot 0x10 | fefates:bytes
    virtual void vf_0x14(); // 0x0035B514 slot 0x14 | fefates:bytes (was Dequeue)
    virtual void GetLock(); // 0x0035B5C8 slot 0x18 | slot vf_0x18 of nn::nex::PacketQueue
};
} // namespace nex
} // namespace nn
