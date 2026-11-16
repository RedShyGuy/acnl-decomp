#pragma once

#include "decomp.h"
#include "nn/math/math_Vector3.h"

namespace nw {
namespace math {
// A sphere (the class and member names are ours, after AABB and PLANE).
struct SPHERE
{
    nn::math::VEC3 center; // 0x0
    f32 radius;            // 0xC
};
ASSERT_SIZE(SPHERE, 0x10);
} // namespace math
} // namespace nw
