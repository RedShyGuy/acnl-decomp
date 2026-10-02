#pragma once

#include "decomp.h"
#include "nn/nex/nex_UserMessage.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex18_DDL_BinaryMessageE @ 0x008CE76C
// vtable 0x008FD5A4 (vptr 0x008FD5AC), offset_to_top 0, 9 entries
class _DDL_BinaryMessage : public ::nn::nex::UserMessage
{
public:
    _DDL_BinaryMessage(); // ctor address unknown
    virtual void vf_0x00(); // 0x00390F84 slot 0x00 | virtual slot, introduced by nn::nex::Data
    virtual ~_DDL_BinaryMessage(); // 0x00390F44 slot 0x04 | slot vf_0x04 of nn::nex::Data
    virtual void Clone() const; // 0x0072C5B8 slot 0x08 | slot vf_0x08 of nn::nex::Data
    virtual void GetDataType() const; // 0x0072C568 slot 0x0C | slot vf_0x0C of nn::nex::Data
    virtual void vf_0x10(); // 0x0072C58C slot 0x10 | virtual slot, introduced by nn::nex::Data
    virtual void vf_0x14(); // 0x0072C77C slot 0x14 | virtual slot, introduced by nn::nex::Data
    virtual void StreamIn(nn::nex::Message*) const; // 0x0072C6AC slot 0x18 | slot vf_0x18 of nn::nex::Data
    virtual void vf_0x1C(); // 0x00390E1C slot 0x1C | virtual slot, introduced by nn::nex::Data
};
} // namespace nex
} // namespace nn
