#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

namespace time {
// RTTI N4time11TimeHioNodeE @ 0x008D254C
// vtable 0x009073FC (vptr 0x00907404), offset_to_top 0, 1 entries
class TimeHioNode : public ::sead::hostio::Node
{
public:
    TimeHioNode(); // ctor candidate(s) 0x007A0B0C (unverified)
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
};
} // namespace time
