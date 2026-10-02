#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_BinaryMessage.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex13BinaryMessageE @ 0x008CE20C
// vtable 0x008FC7A4 (vptr 0x008FC7AC), offset_to_top 0, 9 entries
class BinaryMessage : public ::nn::nex::_DDL_BinaryMessage
{
public:
    BinaryMessage(); // ctor address unknown
    virtual void vf_0x00(); // 0x00361C40 slot 0x00 | virtual slot, introduced by nn::nex::Data
    virtual ~BinaryMessage(); // 0x00361C00 slot 0x04 | slot vf_0x04 of nn::nex::Data
    virtual void vf_0x20(); // 0x00361B44 slot 0x20 | virtual slot, introduced by nn::nex::UserMessage
};
} // namespace nex
} // namespace nn
