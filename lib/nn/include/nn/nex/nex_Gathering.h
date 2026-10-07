#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_Gathering.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex9GatheringE @ 0x008CF750
// vtable 0x008FFD14 (vptr 0x008FFD1C), offset_to_top 0, 9 entries
class Gathering : public ::nn::nex::_DDL_Gathering
{
public:
    Gathering(); // ctor candidate(s) 0x003D6A90 (unverified)
    virtual ~Gathering(); // 0x003D6B0C slot 0x00 | fefates:callgraph
    // 0x003D6AEC slot 0x04 | slot vf_0x04 of nn::nex::_DDL_Gathering (deleting dtor)
    virtual void vf_0x20(); // 0x0072EA04 slot 0x20 | virtual slot, introduced by nn::nex::Gathering
    void Reset(); // 0x003D6A5C | fefates:bytes [tier B]
    // (name is ours)
    void SetDescription(const String& description); // 0x003D6A14
};
} // namespace nex
} // namespace nn
