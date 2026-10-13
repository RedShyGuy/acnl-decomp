#pragma once

#include "decomp.h"
#include "nn/nex/nex_Buffer.h"

namespace nn {
namespace nex {
// A Buffer from the pools of the PacketBufferManager.
// RTTI N2nn3nex12PacketBufferE @ 0x008CE150
// vtable 0x008FC614 (vptr 0x008FC61C), offset_to_top 0, 2 entries
class PacketBuffer : public ::nn::nex::Buffer
{
public:
    explicit PacketBuffer(unsigned int size); // 0x0035D64C | fefates:bytes [tier B]
    // (only a nop in the original that falls into ~Buffer)
    virtual ~PacketBuffer(); // 0x003D0074 slot 0x00
    // 0x0035D6B0 slot 0x04 (deleting dtor)
};
} // namespace nex
} // namespace nn
