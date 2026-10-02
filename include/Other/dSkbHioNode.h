#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

// RTTI 10SkbHioNode @ 0x008CB1DC
// vtable 0x008EC490 (vptr 0x008EC498), offset_to_top 0, 1 entries
class SkbHioNode : public ::sead::hostio::Node
{
public:
    SkbHioNode(); // ctor candidate(s) 0x00784778 (unverified)
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
};
