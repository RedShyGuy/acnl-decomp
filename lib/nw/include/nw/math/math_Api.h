#pragma once

#include "decomp.h"
#include "nn/math/math_Api.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"

namespace nw {
namespace math {
// the view matrix of a camera at pCamPos that looks at pTarget, turned by twist (an angle index:
// 256 is a full turn) around the line of sight
void MTX34LookAtFIdx(nn::math::MTX34* pOut, const nn::math::VEC3* pCamPos, f32 twist, const nn::math::VEC3* pTarget); // 0x004D9948 | nintendogs:bytes-fuzzy [confirmed by mk7dlp] [tier A]

// the factor from radians to the units of the FIdx functions (256 per turn)
const f32 RAD_TO_FIDX = 40.743664f;

// (inline) sin and cos of an angle index (256 per turn): the table of nn::math, the index reduced
// with a loop (nn::math::SinCosFIdx uses fmodf; ARMCC inlines this one in gfx and anim; the name is
// ours)
DECOMP_ALWAYS_INLINE void SinCosFIdx(f32* pSin, f32* pCos, f32 index)
{
    f32 value = (index < 0.0f) ? -index : index;
    while (value >= 65536.0f) {
        value -= 65536.0f;
    }
    const u16 step = static_cast<u32>(value);
    const f32 fraction = value - step;
    const nn::math::SinCosEntry& entry = nn::math::s_SinCosTable[step & 0xFF];
    f32 sinValue = entry.sinValue + fraction * entry.sinDelta;
    const f32 cosValue = entry.cosValue + fraction * entry.cosDelta;
    if (index < 0.0f) {
        sinValue = -sinValue;
    }
    *pSin = sinValue;
    *pCos = cosValue;
}

// (inline) the rotation by the angles in radians around X, then Y, then Z, with a zero translation
// (the name is ours)
DECOMP_ALWAYS_INLINE nn::math::MTX34* MTX34RotateXYZRad(nn::math::MTX34* pOut, f32 x, f32 y, f32 z)
{
    f32 sinX, cosX, sinY, cosY, sinZ, cosZ;
    SinCosFIdx(&sinX, &cosX, x * RAD_TO_FIDX);
    SinCosFIdx(&sinY, &cosY, y * RAD_TO_FIDX);
    SinCosFIdx(&sinZ, &cosZ, z * RAD_TO_FIDX);
    pOut->m[0][0] = cosZ * cosY;
    pOut->m[0][1] = sinX * cosZ * sinY - cosX * sinZ;
    pOut->m[0][2] = cosX * cosZ * sinY + sinX * sinZ;
    pOut->m[1][0] = sinZ * cosY;
    pOut->m[1][1] = sinX * sinZ * sinY + cosX * cosZ;
    pOut->m[1][2] = cosX * sinZ * sinY - sinX * cosZ;
    pOut->m[2][0] = -sinY;
    pOut->m[2][1] = sinX * cosY;
    pOut->m[2][2] = cosX * cosY;
    pOut->m[0][3] = 0.0f;
    pOut->m[1][3] = 0.0f;
    pOut->m[2][3] = 0.0f;
    return pOut;
}
} // namespace math
} // namespace nw
