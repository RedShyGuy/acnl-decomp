#pragma once

#include "decomp.h"
#include "collision/dShape.h"

namespace collision {
// RTTI N9collision8TriangleE @ 0x008D4130
// vtable 0x0090C2D8 (vptr 0x0090C2E0), offset_to_top 0, 3 entries
class Triangle : public ::collision::Shape
{
public:
    Triangle(); // ctor candidate(s) 0x006F9030 (unverified)
    virtual void vf_0x00(); // 0x006F904C slot 0x00 | virtual slot, introduced by collision::Triangle
    virtual void vf_0x04(); // 0x006F9048 slot 0x04 | virtual slot, introduced by collision::Triangle
    virtual void vf_0x08(); // 0x006F8F98 slot 0x08 | virtual slot, introduced by collision::Triangle
};
} // namespace collision
