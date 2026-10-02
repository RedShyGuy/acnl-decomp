#pragma once

#include "decomp.h"
#include "nn/nex/nex_Packet.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex9PacketOutE @ 0x008CF780
// vtable 0x008FFD7C (vptr 0x008FFD84), offset_to_top 0, 2 entries
class PacketOut : public ::nn::nex::Packet
{
public:
    PacketOut(); // TODO: default ctor added so derived stubs compile - may not exist
    virtual ~PacketOut(); // 0x003D7884 slot 0x00 | fefates:bytes
    // 0x003D7850 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    void Initialize(nn::nex::PRUDPEndPoint*, nn::nex::PacketType, unsigned short, nn::nex::Buffer*, unsigned int, nn::nex::Timeout*); // 0x003D7580 | fefates:bytes [tier B]
    PacketOut(nn::nex::PRUDPEndPoint*, nn::nex::PacketType, unsigned short, nn::nex::Buffer*, unsigned int); // 0x003D7780 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
