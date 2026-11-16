#pragma once

#include "decomp.h"
#include "collision/dShape.h"
#include "nn/math/math_Vector3.h"

namespace collision {
// a vertical cylinder of the collision world (names are ours)
// RTTI N9collision8CylinderE @ 0x008D4124
// vtable 0x0090C2C4 (vptr 0x0090C2CC), offset_to_top 0, 3 entries
class Cylinder : public ::collision::Shape
{
public:
    Cylinder(); // 0x006F8F48
    Cylinder(const nn::math::VEC3& center, f32 radius, f32 height); // 0x006F8F1C
    virtual ~Cylinder(); // 0x006F8F94 slot 0x00
    // 0x006F8F70 (deleting dtor)
    void Translate(const nn::math::VEC3& offset) override; // 0x006F8EB8 slot 0x08

    void Set(const nn::math::VEC3* center, f32 radius, f32 height); // 0x006F8EF8

    nn::math::VEC3 m_Center; // 0x08
    f32 m_Radius;            // 0x14
    f32 m_Height;            // 0x18
};
ASSERT_SIZE(Cylinder, 0x1C);
} // namespace collision
