#pragma once

#include "decomp.h"
#include "nn/math/math_Vector3.h"

namespace nw {
namespace math {
// An axis aligned box. Member names are ours.
class AABB
{
public:
    // the box around the points
    void Set(const nn::math::VEC3* pPoints, unsigned int count); // 0x004D9B4C | fefates:bytes [tier B]
    // min and max are swapped where min is the larger one (name is ours)
    void Normalize(); // 0x004D9C08 (name is ours)

    nn::math::VEC3 m_Min; // 0x00
    nn::math::VEC3 m_Max; // 0x0C
};
ASSERT_SIZE(AABB, 0x18);
} // namespace math
} // namespace nw
