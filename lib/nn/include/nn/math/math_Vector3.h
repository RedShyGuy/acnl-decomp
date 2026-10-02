#pragma once

#include "decomp.h"

namespace nn {
namespace math {

struct VEC3
{
    f32 x;
    f32 y;
    f32 z;

    VEC3 operator+(const VEC3& v) const { VEC3 r = { x + v.x, y + v.y, z + v.z }; return r; }
    VEC3 operator-(const VEC3& v) const { VEC3 r = { x - v.x, y - v.y, z - v.z }; return r; }
    VEC3 operator*(f32 f) const { VEC3 r = { x * f, y * f, z * f }; return r; }
};
ASSERT_SIZE(VEC3, 0xC);

} // namespace math
} // namespace nn
