#pragma once

#include "decomp.h"
#include "collision/dShape.h"

namespace collision {
// a sphere of the collision world (names are ours)
// RTTI N9collision6SphereE @ 0x008D410C
// vtable 0x0090C29C (vptr 0x0090C2A4), offset_to_top 0, 3 entries
class Sphere : public ::collision::Shape
{
public:
    Sphere(); // 0x006F8CAC
    Sphere(const nn::math::VEC3& center, f32 radius); // 0x006F8C84
    virtual ~Sphere(); // 0x006F8CD4 slot 0x00
    // 0x006F8CD0 (deleting dtor)
    void Translate(const nn::math::VEC3& offset) override; // 0x006F8C2C slot 0x08

    void Set(const nn::math::VEC3& center, f32 radius); // 0x006F8C64

    nn::math::VEC3 m_Center; // 0x08
    f32 m_Radius;            // 0x14
};
ASSERT_SIZE(Sphere, 0x18);
} // namespace collision
