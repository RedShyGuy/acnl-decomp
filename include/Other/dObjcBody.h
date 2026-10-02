#pragma once

#include "decomp.h"
#include "collision/dWorld.h"
#include "collision/dWorld_Body.h"

// RTTI 8ObjcBody @ 0x008CD528
// vtable 0x008F9D1C (vptr 0x008F9D24), offset_to_top 0, 2 entries
class ObjcBody : public ::collision::World::Body
{
public:
    class Functor;
    ObjcBody(); // ctor candidate(s) 0x006AD550 (unverified)
    virtual void vf_0x00(); // 0x006AD594 slot 0x00 | virtual slot, introduced by collision::World::Body
    virtual void vf_0x04(); // 0x006AD584 slot 0x04 | virtual slot, introduced by collision::World::Body
};
