#pragma once

#include "decomp.h"
#include "collision/dShape.h"

namespace collision {
// RTTI N9collision6SphereE @ 0x008D410C
// vtable 0x0090C29C (vptr 0x0090C2A4), offset_to_top 0, 3 entries
class Sphere : public ::collision::Shape
{
public:
    Sphere(); // ctor candidate(s) 0x006F8C84, 0x006F8CAC (unverified)
    virtual ~Sphere(); // 0x006F8CD4 slot 0x00
    // 0x006F8CD0 (deleting dtor)
    void Translate(const nn::math::VEC3& offset) override; // 0x006F8C2C slot 0x08
};
} // namespace collision
