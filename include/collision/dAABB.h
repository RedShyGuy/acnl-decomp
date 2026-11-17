#pragma once

#include "decomp.h"
#include "collision/dShape.h"

namespace collision {
// an axis-aligned box of the collision world (names are ours)
// RTTI N9collision4AABBE @ 0x008D40F0
// vtable 0x0090C278 (vptr 0x0090C280), offset_to_top 0, 3 entries
class AABB : public ::collision::Shape
{
public:
    AABB(); // 0x006F8910
    virtual ~AABB(); // 0x006F892C slot 0x00
    // 0x006F8928 (deleting dtor)
    void Translate(const nn::math::VEC3& offset) override; // 0x006F87AC slot 0x08

    void Set(const nn::math::VEC3& min, const nn::math::VEC3& max); // 0x006F88DC
    // a box of width (x) and depth (z) around base, from base up to base.y + height
    void SetFromBase(const nn::math::VEC3& base, f32 width, f32 depth, f32 height); // 0x006F8884

    nn::math::VEC3 m_Min; // 0x08
    nn::math::VEC3 m_Max; // 0x14
};
ASSERT_SIZE(AABB, 0x20);
} // namespace collision
