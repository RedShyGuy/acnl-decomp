#pragma once

#include "decomp.h"

namespace nn {
namespace math {
// How a projection matrix is turned for a side of the LCD (the type name is from the binary; the
// values and their names are ours, MTX44PivotC_FAST is not analyzed yet)
enum PivotDirection : u8 {
    PIVOT_DIRECTION_0,
    PIVOT_DIRECTION_1,
    PIVOT_DIRECTION_2,
    PIVOT_DIRECTION_3,
    PIVOT_DIRECTION_4
};

class MTX44
{
public:
    void Identity(); // 0x0047E51C | nintendogs:callgraph [tier A]

    f32 m[4][4];    // row-major
};
ASSERT_SIZE(MTX44, 0x40);
} // namespace math
} // namespace nn
