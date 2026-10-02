#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex6StreamE @ 0x008CF5D0
// vtable 0x008FFA5C (vptr 0x008FFA64), offset_to_top 0, 5 entries
class Stream : public ::nn::nex::RootObject
{
public:
    struct Type { u32 _unknown; }; // TODO: real type unknown (placeholder)
    Stream(); // ctor address unknown
    virtual ~Stream(); // 0x003D113C slot 0x00 | slot vf_0x00 of nn::nex::Stream
    // 0x003D1124 slot 0x04 | slot vf_0x04 of nn::nex::Stream (deleting dtor)
    virtual void ReceiveIncomingPacket(unsigned short, unsigned char, nn::nex::Packet*); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void DoWork(); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void IsDuplicateReorderingPacket(nn::nex::Packet*); // 0x003D111C slot 0x10 | slot vf_0x10 of nn::nex::Stream
};
} // namespace nex
} // namespace nn
