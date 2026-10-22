#pragma once

#include "decomp.h"
#include "nn/math/math_Vector3.h"

namespace nn {
namespace math {
// a scale, a rotation (angles in radians around X, Y, Z) and a translation (the name is from the
// reference symbols, member names are ours)
struct Transform3
{
    VEC3 scale;     // 0x00
    VEC3 rotate;    // 0x0C
    VEC3 translate; // 0x18
};
ASSERT_SIZE(Transform3, 0x24);
} // namespace math
} // namespace nn
