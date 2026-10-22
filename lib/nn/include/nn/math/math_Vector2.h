#pragma once

#include "decomp.h"

namespace nn {
namespace math {
struct VEC2
{
    VEC2() {}
    constexpr VEC2(f32 fx, f32 fy) : x(fx), y(fy) {}

    f32 x;
    f32 y;
};
ASSERT_SIZE(VEC2, 0x8);
} // namespace math
} // namespace nn
