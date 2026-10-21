#pragma once

#include "decomp.h"
#include "nn/math/math_Vector3.h"

namespace nw {
namespace math {
// A plane n . p + d = 0 with a unit normal. Member names are ours.
class PLANE
{
public:
    // (out of line: the game constructs arrays of planes with __aeabi_vec_ctor)
    PLANE(); // 0x004D9D0C (name is ours)

    // the plane through the three points; the normal is (p2 - p0) x (p1 - p0), normalized
    void Set(const nn::math::VEC3* p0, const nn::math::VEC3* p1, const nn::math::VEC3* p2); // 0x004D9C54 | fefates:bytes [tier B]

    nn::math::VEC3 m_Normal; // 0x0
    f32 m_D;                 // 0xC
};
ASSERT_SIZE(PLANE, 0x10);
} // namespace math
} // namespace nw
