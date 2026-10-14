#pragma once

#include "decomp.h"
#include "nn/nex/nex_PacketIn.h"

namespace nn {
namespace nex {
// A packet in the memory of the PacketBufferManager.
// (0x0036968C, named as a ctor candidate before, is a function of another class)
// RTTI N2nn3nex20PacketBufferPacketInE @ 0x008CE8F0
// vtable 0x008FD8B0 (vptr 0x008FD8B8), offset_to_top 0, 2 entries
class PacketBufferPacketIn : public ::nn::nex::PacketIn
{
public:
    // (inline)
    PacketBufferPacketIn() {}
    virtual ~PacketBufferPacketIn(); // 0x0039797C slot 0x00
    // 0x003978DC slot 0x04 (deleting dtor)
};
ASSERT_SIZE(PacketBufferPacketIn, 0x118);
} // namespace nex
} // namespace nn
