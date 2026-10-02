#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

// RTTI 9VolumeHIO @ 0x008CD7FC
// vtable 0x008FA848 (vptr 0x008FA850), offset_to_top 0, 1 entries
class VolumeHIO : public ::sead::hostio::Node
{
public:
    VolumeHIO(); // ctor candidate(s) 0x00788830 (unverified)
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
};
