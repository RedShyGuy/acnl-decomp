#pragma once

#include "decomp.h"
#include "collision/dWorld.h"
#include "collision/dWorld_Body.h"

namespace bgcheck {
// RTTI N7bgcheck15DynamicCylinderE @ 0x008D3EE8
// vtable 0x0090BC84 (vptr 0x0090BC8C), offset_to_top 0, 2 entries
class DynamicCylinder : public ::collision::World::Body
{
public:
    DynamicCylinder(); // ctor candidate(s) 0x00615040 (unverified)
    virtual void vf_0x00(); // 0x00615084 slot 0x00 | virtual slot, introduced by collision::World::Body
    virtual void vf_0x04(); // 0x00615060 slot 0x04 | virtual slot, introduced by collision::World::Body
};
} // namespace bgcheck
