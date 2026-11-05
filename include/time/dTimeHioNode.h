#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

namespace time {
// the host I/O node of the clock (an instance in time_Api.cpp)
// RTTI N4time11TimeHioNodeE @ 0x008D254C
// vtable 0x009073FC (vptr 0x00907404), offset_to_top 0, 1 entries
class TimeHioNode : public ::sead::hostio::Node
{
public:
    TimeHioNode() : _4(0) {}
    // (the code is shared with other nodes)
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00

    u32 _4; // 0x4
};
} // namespace time
