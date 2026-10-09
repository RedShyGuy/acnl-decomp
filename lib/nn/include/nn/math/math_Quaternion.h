#pragma once

#include "decomp.h"

namespace nn {
namespace math {
// a quaternion (the type name is from the symbols; the layout from MTX34ToQUATC_FAST)
struct QUAT
{
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};
ASSERT_SIZE(QUAT, 0x10);
} // namespace math
} // namespace nn
