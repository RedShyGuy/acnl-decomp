#pragma once

#include "decomp.h"

namespace nn {
namespace math {

struct VEC3
{
    VEC3() {}
    VEC3(f32 fx, f32 fy, f32 fz) : x(fx), y(fy), z(fz) {}

    f32 x;
    f32 y;
    f32 z;

    VEC3 operator+(const VEC3& v) const { VEC3 r = { x + v.x, y + v.y, z + v.z }; return r; }
    VEC3 operator-(const VEC3& v) const { VEC3 r = { x - v.x, y - v.y, z - v.z }; return r; }
    VEC3 operator*(f32 f) const { VEC3 r = { x * f, y * f, z * f }; return r; }

    // the zero vector (inline: a function-local static, e.g. in the static initializer of
    // hid_GyroscopeReader.cpp; the name is ours)
    static const VEC3& Zero()
    {
        // 0x00AF91C4
        static const VEC3 s_Zero(0.0f, 0.0f, 0.0f);
        return s_Zero;
    }
};
ASSERT_SIZE(VEC3, 0xC);

} // namespace math
} // namespace nn
