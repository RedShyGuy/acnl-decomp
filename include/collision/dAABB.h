#pragma once

#include "decomp.h"
#include "collision/dShape.h"

namespace collision {
// RTTI N9collision4AABBE @ 0x008D40F0
// vtable 0x0090C278 (vptr 0x0090C280), offset_to_top 0, 3 entries
class AABB : public ::collision::Shape
{
public:
    AABB(); // ctor candidate(s) 0x006F8910 (unverified)
    virtual void vf_0x00(); // 0x006F892C slot 0x00 | virtual slot, introduced by collision::AABB
    virtual void vf_0x04(); // 0x006F8928 slot 0x04 | virtual slot, introduced by collision::AABB
    virtual void vf_0x08(); // 0x006F87AC slot 0x08 | virtual slot, introduced by collision::AABB
};
} // namespace collision
