#pragma once

#include "decomp.h"
#include "collision/dShape.h"

namespace collision {
// a capsule of the collision world: the points within radius of the segment from m_Start to m_End (names are ours)
// RTTI N9collision7CapsuleE @ 0x008D4118
// vtable 0x0090C2B0 (vptr 0x0090C2B8), offset_to_top 0, 3 entries
class Capsule : public ::collision::Shape
{
public:
    Capsule(); // 0x006F8D78
    virtual ~Capsule(); // 0x006F8DA0 slot 0x00
    // 0x006F8D9C (deleting dtor)
    void Translate(const nn::math::VEC3& offset) override; // 0x006F8CD8 slot 0x08

    void Set(const nn::math::VEC3& start, const nn::math::VEC3& end, f32 radius); // 0x006F8D40

    nn::math::VEC3 m_Start; // 0x08
    nn::math::VEC3 m_End;   // 0x14
    f32 m_Radius;           // 0x20
};
ASSERT_SIZE(Capsule, 0x24);
} // namespace collision
