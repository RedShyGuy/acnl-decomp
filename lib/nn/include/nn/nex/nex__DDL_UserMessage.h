#pragma once

#include "decomp.h"
#include "nn/nex/nex_Data.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex16_DDL_UserMessageE @ 0x008CE65C
// vtable 0x008FD304 (vptr 0x008FD30C), offset_to_top 0, 8 entries
class _DDL_UserMessage : public ::nn::nex::Data
{
public:
    _DDL_UserMessage(); // ctor address unknown
    virtual void vf_0x00(); // 0x0038462C slot 0x00 | virtual slot, introduced by nn::nex::Data
    virtual ~_DDL_UserMessage(); // 0x00384604 slot 0x04 | slot vf_0x04 of nn::nex::Data
    virtual void Clone() const; // 0x0072BBA8 slot 0x08 | slot vf_0x08 of nn::nex::Data
    virtual void GetDataType() const; // 0x0072BB60 slot 0x0C | slot vf_0x0C of nn::nex::Data
    virtual void vf_0x10(); // 0x0072BB80 slot 0x10 | virtual slot, introduced by nn::nex::Data
    virtual void vf_0x14(); // 0x0072BCB0 slot 0x14 | virtual slot, introduced by nn::nex::Data
    virtual void StreamIn(nn::nex::Message*) const; // 0x003842C4 slot 0x18 | slot vf_0x18 of nn::nex::Data
    virtual void vf_0x1C(); // 0x0038443C slot 0x1C | virtual slot, introduced by nn::nex::Data
};
} // namespace nex
} // namespace nn
