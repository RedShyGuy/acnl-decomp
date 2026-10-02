#pragma once

#include "decomp.h"
#include "nn/nex/nex_PacketIn.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex20PacketBufferPacketInE @ 0x008CE8F0
// vtable 0x008FD8B0 (vptr 0x008FD8B8), offset_to_top 0, 2 entries
class PacketBufferPacketIn : public ::nn::nex::PacketIn
{
public:
    PacketBufferPacketIn(); // ctor candidate(s) 0x0036968C (unverified)
    virtual ~PacketBufferPacketIn(); // 0x0039797C slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003978DC slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
};
} // namespace nex
} // namespace nn
