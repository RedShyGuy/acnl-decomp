// The fast C versions of the math functions (ARMv6 *C_FAST); the assembly versions are in
// math_ARMv6Asm.cpp.

#include "nn/math/math_ARMv6.h"
#include <math.h>
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Quaternion.h"
#include "nn/math/math_Vector3.h"

namespace nn {
namespace math {
// 0x0047DEF0 | nintendogs:bytes [tier A]
f32 nn::math::ARMv6::HermiteC_FAST(f32 p1, f32 t1, f32 p2, f32 t2, f32 s, f32 d)
{
    f32 u = s / d;
    f32 u1 = u - 1.0f;
    return p1 + (p1 - p2) * (2.0f * u - 3.0f) * (u * u) + s * u1 * (u1 * t1 + u * t2);
}

// 0x0047DF3C (name is ours)
nn::math::MTX44* nn::math::ARMv6::MTX44OrthoC_FAST(nn::math::MTX44* pOut, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f)
{
    f32 width = 1.0f / (r - l);
    f32 height = 1.0f / (t - b);
    f32 depth = 1.0f / (f - n);
    pOut->m[0][1] = 0.0f;
    pOut->m[0][2] = 0.0f;
    pOut->m[1][0] = 0.0f;
    pOut->m[1][2] = 0.0f;
    pOut->m[2][0] = 0.0f;
    pOut->m[2][1] = 0.0f;
    pOut->m[3][0] = 0.0f;
    pOut->m[3][1] = 0.0f;
    pOut->m[3][2] = 0.0f;
    pOut->m[3][3] = 1.0f;
    pOut->m[0][0] = 2.0f * width;
    pOut->m[0][3] = -(r + l) * width;
    pOut->m[1][1] = 2.0f * height;
    pOut->m[1][3] = -(t + b) * height;
    pOut->m[2][2] = depth;
    pOut->m[2][3] = n * depth;
    return pOut;
}

// 0x0047DFCC | mk7dlp:bytes [tier A]
nn::math::MTX44* nn::math::ARMv6::MTX44PivotC_FAST(nn::math::MTX44* pM, nn::math::PivotDirection pivot)
{
    f32 (*m)[4] = pM->m;
    switch (pivot) {
    case PIVOT_DIRECTION_0:
    case PIVOT_DIRECTION_4:
        break;
    case PIVOT_DIRECTION_2: {
        f32 m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
        f32 m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
        m[0][0] = -m00;
        m[0][1] = -m01;
        m[0][2] = -m02;
        m[0][3] = -m03;
        m[1][0] = -m10;
        m[1][1] = -m11;
        m[1][2] = -m12;
        m[1][3] = -m13;
        break;
    }
    case PIVOT_DIRECTION_3: {
        f32 m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
        f32 m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
        m[0][0] = -m10;
        m[0][1] = -m11;
        m[0][2] = -m12;
        m[0][3] = -m13;
        m[1][0] = m00;
        m[1][1] = m01;
        m[1][2] = m02;
        m[1][3] = m03;
        break;
    }
    default: {
        f32 m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
        f32 m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
        m[0][0] = m10;
        m[0][1] = m11;
        m[0][2] = m12;
        m[0][3] = m13;
        m[1][0] = -m00;
        m[1][1] = -m01;
        m[1][2] = -m02;
        m[1][3] = -m03;
        break;
    }
    }
    return pM;
}

// 0x0047E088 | nintendogs:bytes [tier A]
nn::math::MTX34* nn::math::ARMv6::MTX34LookAtC_FAST(nn::math::MTX34* pOut, const nn::math::VEC3* pCameraPosition, const nn::math::VEC3* pCameraUp, const nn::math::VEC3* pTarget)
{
    // z: from the target to the camera
    f32 zx = pCameraPosition->x - pTarget->x;
    f32 zy = pCameraPosition->y - pTarget->y;
    f32 zz = pCameraPosition->z - pTarget->z;
    f32 inverse = 1.0f / sqrtf(zx * zx + zy * zy + zz * zz);
    zz *= inverse;
    zy *= inverse;
    zx *= inverse;
    // x: up cross z
    f32 xx = pCameraUp->y * zz - pCameraUp->z * zy;
    f32 xy = pCameraUp->z * zx - pCameraUp->x * zz;
    f32 xz = pCameraUp->x * zy - pCameraUp->y * zx;
    inverse = 1.0f / sqrtf(xx * xx + xy * xy + xz * xz);
    xx *= inverse;
    xy *= inverse;
    xz *= inverse;
    // y: z cross x
    f32 yx = zy * xz - zz * xy;
    f32 yy = zz * xx - zx * xz;
    f32 yz = zx * xy - zy * xx;
    pOut->m[0][0] = xx;
    pOut->m[0][1] = xy;
    pOut->m[0][2] = xz;
    pOut->m[0][3] = -(pCameraPosition->x * xx + pCameraPosition->y * xy + pCameraPosition->z * xz);
    pOut->m[1][0] = yx;
    pOut->m[1][1] = yy;
    pOut->m[1][2] = yz;
    pOut->m[1][3] = -(pCameraPosition->x * yx + pCameraPosition->y * yy + pCameraPosition->z * yz);
    pOut->m[2][0] = zx;
    pOut->m[2][1] = zy;
    pOut->m[2][2] = zz;
    pOut->m[2][3] = -(pCameraPosition->x * zx + pCameraPosition->y * zy + pCameraPosition->z * zz);
    return pOut;
}

// 0x0047E180 | nintendogs:bytes [tier A]
nn::math::QUAT* nn::math::ARMv6::MTX34ToQUATC_FAST(nn::math::QUAT* pOut, const nn::math::MTX34* pM)
{
    f32 m00 = pM->m[0][0];
    f32 m11 = pM->m[1][1];
    f32 m22 = pM->m[2][2];
    f32 trace = m00 + m11 + m22;
    if (trace > 0.0f) {
        f32 s = sqrtf(trace + 1.0f);
        pOut->w = s * 0.5f;
        s = 0.5f / s;
        pOut->x = (pM->m[2][1] - pM->m[1][2]) * s;
        pOut->y = (pM->m[0][2] - pM->m[2][0]) * s;
        pOut->z = (pM->m[1][0] - pM->m[0][1]) * s;
        return pOut;
    }
    if (m11 > m00) {
        if (!(m22 > m11)) {
            f32 s = sqrtf(m11 - (m22 + m00) + 1.0f);
            pOut->y = s * 0.5f;
            if (s != 0.0f) {
                s = 0.5f / s;
            }
            pOut->w = (pM->m[0][2] - pM->m[2][0]) * s;
            pOut->z = (pM->m[2][1] + pM->m[1][2]) * s;
            pOut->x = (pM->m[1][0] + pM->m[0][1]) * s;
            return pOut;
        }
    } else if (!(m22 > m00)) {
        f32 s = sqrtf(m00 - (m11 + m22) + 1.0f);
        pOut->x = s * 0.5f;
        if (s != 0.0f) {
            s = 0.5f / s;
        }
        pOut->y = (pM->m[0][1] + pM->m[1][0]) * s;
        pOut->z = (pM->m[0][2] + pM->m[2][0]) * s;
        pOut->w = (pM->m[2][1] - pM->m[1][2]) * s;
        return pOut;
    }
    f32 s = sqrtf(m22 - (m00 + m11) + 1.0f);
    pOut->z = s * 0.5f;
    if (s != 0.0f) {
        s = 0.5f / s;
    }
    pOut->w = (pM->m[1][0] - pM->m[0][1]) * s;
    pOut->x = (pM->m[2][0] + pM->m[0][2]) * s;
    pOut->y = (pM->m[1][2] + pM->m[2][1]) * s;
    return pOut;
}

// 0x0047E324 | fefates:bytes [tier B]
nn::math::MTX34* nn::math::ARMv6::QUATToMTX34C_FAST(nn::math::MTX34* pOut, const nn::math::QUAT* pQ, bool clearTranslation)
{
    f32 x = pQ->x;
    f32 y = pQ->y;
    f32 z = pQ->z;
    f32 w = pQ->w;
    f32 scale = 2.0f / (x * x + y * y + z * z + w * w);
    f32 ys = y * scale;
    f32 xs = x * scale;
    f32 zs = z * scale;
    f32 yy = y * ys;
    f32 xx = x * xs;
    f32 zz = z * zs;
    f32 yz = y * zs;
    f32 wx = w * xs;
    f32 wy = w * ys;
    f32 xy = x * ys;
    f32 wz = w * zs;
    f32 xz = x * zs;
    if (clearTranslation) {
        pOut->m[0][3] = 0.0f;
        pOut->m[1][3] = 0.0f;
        pOut->m[2][3] = 0.0f;
    }
    pOut->m[0][0] = 1.0f - (yy + zz);
    pOut->m[1][0] = xy + wz;
    pOut->m[1][1] = 1.0f - (xx + zz);
    pOut->m[1][2] = yz - wx;
    pOut->m[2][0] = xz - wy;
    pOut->m[2][1] = yz + wx;
    pOut->m[2][2] = 1.0f - (xx + yy);
    pOut->m[0][1] = xy - wz;
    pOut->m[0][2] = xz + wy;
    return pOut;
}

// 0x0047E3F8 | nintendogs:bytes [tier A]
nn::math::MTX44* nn::math::ARMv6::MTX44FrustumC_FAST(nn::math::MTX44* pOut, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f)
{
    pOut->m[3][2] = -1.0f;
    pOut->m[0][1] = 0.0f;
    pOut->m[0][3] = 0.0f;
    pOut->m[1][0] = 0.0f;
    pOut->m[1][3] = 0.0f;
    pOut->m[2][0] = 0.0f;
    pOut->m[2][1] = 0.0f;
    pOut->m[3][0] = 0.0f;
    pOut->m[3][1] = 0.0f;
    pOut->m[3][3] = 0.0f;
    f32 width = 1.0f / (r - l);
    f32 height = 1.0f / (t - b);
    f32 depth = 1.0f / (f - n);
    f32 near2 = n * 2.0f;
    pOut->m[0][0] = near2 * width;
    pOut->m[0][2] = (r + l) * width;
    pOut->m[1][1] = near2 * height;
    pOut->m[1][2] = (t + b) * height;
    pOut->m[2][2] = f * depth;
    pOut->m[2][3] = (f * n) * depth;
    return pOut;
}

} // namespace math
} // namespace nn
