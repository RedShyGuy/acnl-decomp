#pragma once

#include "decomp.h"
#include "collision/dShape.h"

namespace collision {
// RTTI N9collision8TriangleE @ 0x008D4130
// vtable 0x0090C2D8 (vptr 0x0090C2E0), offset_to_top 0, 3 entries
class Triangle : public ::collision::Shape
{
public:
    Triangle(); // 0x006F9030
    virtual ~Triangle(); // 0x006F904C slot 0x00
    // 0x006F9048 (deleting dtor)
    void Translate(const nn::math::VEC3& offset) override; // 0x006F8F98 slot 0x08
};
} // namespace collision
