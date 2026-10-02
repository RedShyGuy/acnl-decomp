#pragma once

#include "decomp.h"
#include "nn/nex/nex_Gathering.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex21_DDL_DynamicGatheringE @ 0x008CEB74
// vtable 0x008FDF9C (vptr 0x008FDFA4), offset_to_top 0, 9 entries
class _DDL_DynamicGathering : public ::nn::nex::Gathering
{
public:
    _DDL_DynamicGathering(); // ctor address unknown
    virtual ~_DDL_DynamicGathering(); // 0x0039B6EC slot 0x00 | slot vf_0x00 of nn::nex::_DDL_Gathering
    // 0x0039B6B8 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void Clone() const; // 0x0072CECC slot 0x08 | fefates:bytes
    virtual void GetGatheringType() const; // 0x0072CE6C slot 0x0C | mk7dlp:bytes
    virtual void vf_0x10(); // 0x0072CE98 slot 0x10 | virtual slot, introduced by nn::nex::_DDL_Gathering
    virtual void vf_0x14(); // 0x0072CF48 slot 0x14 | virtual slot, introduced by nn::nex::_DDL_Gathering
    virtual void StreamIn(nn::nex::Message*) const; // 0x003846C8 slot 0x18 | mk7dlp:callseq
    virtual void StreamOut(nn::nex::Message*); // 0x0039B66C slot 0x1C | fefates:bytes
};
} // namespace nex
} // namespace nn
