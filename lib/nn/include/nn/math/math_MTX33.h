#pragma once

#include "decomp.h"

namespace nn {
namespace math {
struct MTX33
{
    MTX33() {}
    MTX33(f32 m00, f32 m01, f32 m02, f32 m10, f32 m11, f32 m12, f32 m20, f32 m21, f32 m22)
    {
        m[0][0] = m00;
        m[0][1] = m01;
        m[0][2] = m02;
        m[1][0] = m10;
        m[1][1] = m11;
        m[1][2] = m12;
        m[2][0] = m20;
        m[2][1] = m21;
        m[2][2] = m22;
    }

    f32 m[3][3]; // row-major

    // the identity (inline: a function-local static, e.g. in the static initializer of
    // hid_GyroscopeReader.cpp; the name follows MTX34::Identity)
    static const MTX33& Identity()
    {
        // 0x00AF91D0
        static const MTX33 s_Identity(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        return s_Identity;
    }
};
ASSERT_SIZE(MTX33, 0x24);
} // namespace math
} // namespace nn
