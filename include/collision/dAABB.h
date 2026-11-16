#pragma once

#include "decomp.h"
#include "collision/dShape.h"

namespace collision {
// RTTI N9collision4AABBE @ 0x008D40F0
// vtable 0x0090C278 (vptr 0x0090C280), offset_to_top 0, 3 entries
class AABB : public ::collision::Shape
{
public:
    AABB(); // 0x006F8910
    virtual ~AABB(); // 0x006F892C slot 0x00
    // 0x006F8928 (deleting dtor)
    void Translate(const nn::math::VEC3& offset) override; // 0x006F87AC slot 0x08
};
} // namespace collision
