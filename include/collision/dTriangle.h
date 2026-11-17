#pragma once

#include "decomp.h"
#include "collision/dShape.h"

namespace collision {
// a triangle of the collision world with its normal (names are ours; the normal is set by its users)
// RTTI N9collision8TriangleE @ 0x008D4130
// vtable 0x0090C2D8 (vptr 0x0090C2E0), offset_to_top 0, 3 entries
class Triangle : public ::collision::Shape
{
public:
    Triangle(); // 0x006F9030
    virtual ~Triangle(); // 0x006F904C slot 0x00
    // 0x006F9048 (deleting dtor)
    void Translate(const nn::math::VEC3& offset) override; // 0x006F8F98 slot 0x08

    nn::math::VEC3 m_Vertices[3]; // 0x08
    nn::math::VEC3 m_Normal;      // 0x2C
};
ASSERT_SIZE(Triangle, 0x38);
} // namespace collision
