#pragma once

#include "decomp.h"

namespace nn {
namespace math {
// How a projection matrix is turned for a side of the LCD (the type name is from the binary; the
// values and their names are ours): 0 and 4 keep it, 1 turns by 90 degrees (row 0 becomes row
// 1, row 1 the negated row 0), 2 by 180 degrees, 3 by 270 degrees
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
    MTX44() {}
    MTX44(f32 m00, f32 m01, f32 m02, f32 m03, f32 m10, f32 m11, f32 m12, f32 m13, f32 m20, f32 m21, f32 m22, f32 m23,
          f32 m30, f32 m31, f32 m32, f32 m33)
    {
        m[0][0] = m00;
        m[0][1] = m01;
        m[0][2] = m02;
        m[0][3] = m03;
        m[1][0] = m10;
        m[1][1] = m11;
        m[1][2] = m12;
        m[1][3] = m13;
        m[2][0] = m20;
        m[2][1] = m21;
        m[2][2] = m22;
        m[2][3] = m23;
        m[3][0] = m30;
        m[3][1] = m31;
        m[3][2] = m32;
        m[3][3] = m33;
    }

    // the identity (a function-local static)
    static const MTX44& Identity(); // 0x0047E51C | nintendogs:callgraph [tier A]

    f32 m[4][4];    // row-major
};
ASSERT_SIZE(MTX44, 0x40);
} // namespace math
} // namespace nn
