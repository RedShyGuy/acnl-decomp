#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex6PacketE @ 0x008CF5A0
// vtable 0x008FFA1C (vptr 0x008FFA24), offset_to_top 0, 2 entries
class Packet : public ::nn::nex::RefCountedObject
{
public:
    Packet(); // ctor address unknown
    virtual ~Packet(); // 0x003D0560 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003D04C0 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
};
} // namespace nex
} // namespace nn
