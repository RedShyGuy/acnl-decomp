#pragma once

#include "decomp.h"
#include "nn/nex/nex_Buffer.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex12PacketBufferE @ 0x008CE150
// vtable 0x008FC614 (vptr 0x008FC61C), offset_to_top 0, 2 entries
class PacketBuffer : public ::nn::nex::Buffer
{
public:
    PacketBuffer(); // ctor candidate(s) 0x0035D64C (unverified)
    virtual ~PacketBuffer(); // 0x003D0074 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x0035D6B0 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    PacketBuffer(unsigned int); // 0x0035D64C | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
