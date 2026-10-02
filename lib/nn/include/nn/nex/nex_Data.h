#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_Data.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex4DataE @ 0x008CF520
// vtable 0x008FF9A0 (vptr 0x008FF9A8), offset_to_top 0, 8 entries
class Data : public ::nn::nex::_DDL_Data
{
public:
    Data(); // ctor candidate(s) 0x003CE054 (unverified)
    virtual void vf_0x00(); // 0x003CE070 slot 0x00 | virtual slot, introduced by nn::nex::Data
    virtual ~Data(); // 0x003CE06C slot 0x04 | slot vf_0x04 of nn::nex::Data
    virtual void Clone() const; // 0x0072EB7C slot 0x08 | fefates:bytes
    virtual void GetDataType() const; // 0x0072EB4C slot 0x0C | mk7dlp:bytes
    virtual void vf_0x10(); // 0x0072EB60 slot 0x10 | virtual slot, introduced by nn::nex::Data
    virtual void vf_0x14(); // 0x0072EBA8 slot 0x14 | virtual slot, introduced by nn::nex::Data
    virtual void StreamIn(nn::nex::Message*) const; // 0x003D9AF4 slot 0x18 | slot vf_0x18 of nn::nex::Data
    virtual void vf_0x1C(); // 0x003D9B68 slot 0x1C | virtual slot, introduced by nn::nex::Data
};
} // namespace nex
} // namespace nn
