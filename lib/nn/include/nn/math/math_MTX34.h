#pragma once

#include "decomp.h"

namespace nn {
namespace math {
class MTX34
{
public:
    MTX34() {}
    MTX34(f32 m00, f32 m01, f32 m02, f32 m03, f32 m10, f32 m11, f32 m12, f32 m13, f32 m20, f32 m21, f32 m22, f32 m23)
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
    }

    // the identity (a function-local static)
    static const MTX34& Identity(); // 0x0047E49C | nintendogs:callgraph [tier A]

    f32 m[3][4];    // row-major, column 3 is the translation
};
ASSERT_SIZE(MTX34, 0x30);
} // namespace math
} // namespace nn
