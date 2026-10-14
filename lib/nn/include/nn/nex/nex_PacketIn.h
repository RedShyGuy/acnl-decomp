#pragma once

#include "decomp.h"
#include "nn/nex/nex_Packet.h"

namespace nn {
namespace nex {
// A received packet.
// RTTI N2nn3nex8PacketInE @ 0x008CF6FC
// vtable 0x008FFCA8 (vptr 0x008FFCB0), offset_to_top 0, 2 entries
class PacketIn : public ::nn::nex::Packet
{
public:
    // (inline)
    PacketIn() {}
    virtual ~PacketIn(); // 0x003D57A0 slot 0x00
    // 0x003D56FC slot 0x04 | fefates:bytes (deleting dtor)
};
ASSERT_SIZE(PacketIn, 0x118);
} // namespace nex
} // namespace nn
