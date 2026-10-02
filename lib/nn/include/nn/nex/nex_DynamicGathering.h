#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DynamicGathering.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex16DynamicGatheringE @ 0x008CE578
// vtable 0x008FD0B4 (vptr 0x008FD0BC), offset_to_top 0, 9 entries
class DynamicGathering : public ::nn::nex::_DDL_DynamicGathering
{
public:
    DynamicGathering(); // ctor candidate(s) 0x003870D0 (unverified)
    virtual ~DynamicGathering(); // 0x0037F188 slot 0x00 | slot vf_0x00 of nn::nex::_DDL_Gathering
    // 0x0037F140 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void GetGatheringType() const; // 0x003D12D0 slot 0x0C | slot vf_0x0C of nn::nex::_DDL_Gathering
};
} // namespace nex
} // namespace nn
