#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

namespace sead {
// RTTI N4sead15DrawLockContextE @ 0x008D179C
// vtable 0x00905964 (vptr 0x0090596C), offset_to_top 0, 1 entries
class DrawLockContext : public ::sead::hostio::Node
{
public:
    DrawLockContext(); // ctor candidate(s) 0x001208F4 (unverified)
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
};
} // namespace sead
