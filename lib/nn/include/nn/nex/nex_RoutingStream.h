#pragma once

#include "decomp.h"
#include "nn/nex/nex_Stream.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex13RoutingStreamE @ 0x008CE284
// vtable 0x008FC974 (vptr 0x008FC97C), offset_to_top 0, 5 entries
class RoutingStream : public ::nn::nex::Stream
{
public:
    virtual ~RoutingStream(); // 0x0036D22C slot 0x00 | fefates:callseq
    // 0x0036D1FC slot 0x04 | slot vf_0x04 of nn::nex::Stream (deleting dtor)
    virtual void ReceiveIncomingPacket(unsigned short, unsigned char, nn::nex::Packet*); // 0x0036C2DC slot 0x08 | fefates:callseq
    virtual void DoWork(); // 0x0036D004 slot 0x0C | slot vf_0x0C of nn::nex::Stream
    RoutingStream(); // 0x0036D064 | fefates:bytes-fuzzy [tier B]
};
} // namespace nex
} // namespace nn
