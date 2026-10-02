#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

// RTTI 17HandCursorHioNode @ 0x008CC65C
// vtable 0x008F3AA8 (vptr 0x008F3AB0), offset_to_top 0, 1 entries
class HandCursorHioNode : public ::sead::hostio::Node
{
public:
    HandCursorHioNode(); // ctor candidate(s) 0x00796F5C (unverified)
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
};
