#pragma once

#include "decomp.h"

namespace nn {
namespace math {
struct VEC4
{
    VEC4() {}
    constexpr VEC4(f32 fx, f32 fy, f32 fz, f32 fw) : x(fx), y(fy), z(fz), w(fw) {}

    f32 x;
    f32 y;
    f32 z;
    f32 w;
};
ASSERT_SIZE(VEC4, 0x10);
} // namespace math
} // namespace nn
