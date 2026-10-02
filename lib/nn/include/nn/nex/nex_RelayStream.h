#pragma once

#include "decomp.h"
#include "nn/nex/nex_Stream.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11RelayStreamE @ 0x008CE074
// vtable 0x008FC420 (vptr 0x008FC428), offset_to_top 0, 5 entries
class RelayStream : public ::nn::nex::Stream
{
public:
    RelayStream(); // ctor address unknown
    virtual ~RelayStream(); // 0x0035B9A4 slot 0x00 | slot vf_0x00 of nn::nex::Stream
    // 0x0035B8B4 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void ReceiveIncomingPacket(unsigned short, unsigned char, nn::nex::Packet*); // 0x0035B774 slot 0x08 | fefates:bytes
    virtual void DoWork(); // 0x0036037C slot 0x0C | slot vf_0x0C of nn::nex::Stream
};
} // namespace nex
} // namespace nn
