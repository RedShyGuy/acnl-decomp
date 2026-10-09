#include "nn/math/math_Api.h"
#include <math.h>
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_MTX44.h"
#include "nn/math/math_Vector3.h"

namespace nn {
namespace math {
// an entry of the table: sin and cos of a step and their changes to the next step (names are
// ours)
struct SinCosEntry
{
    f32 sinValue;
    f32 cosValue;
    f32 sinDelta;
    f32 cosDelta;
};

namespace {
// the float index is reduced to one turn of 65536 (the table repeats every 256)
const f32 FLOAT_INDEX_RANGE = 65536.0f;
const f32 FRACTION_UNIT = 1.0f / 256;

inline f32 ReduceIndex(f32 index)
{
    f32 value = fabsf(index);
    if (value >= FLOAT_INDEX_RANGE) {
        value = fmodf(value, FLOAT_INDEX_RANGE);
    }
    return value;
}
} // namespace

// the table of 256 steps per turn (and the first again), in .rodata, not in the source (name
// is ours)
// 0x008A23C0
extern const SinCosEntry s_SinCosTable[];

// 0x0047DC98 | nintendogs:callgraph [tier A]
void SinCosFIdx(f32* pSin, f32* pCos, f32 index)
{
    f32 value = ReduceIndex(index);
    u16 step = static_cast<u32>(value);
    f32 fraction = value - step;
    const SinCosEntry& entry = s_SinCosTable[step & 0xFF];
    f32 sinValue = entry.sinValue + fraction * entry.sinDelta;
    f32 cosValue = entry.cosValue + fraction * entry.cosDelta;
    if (index < 0.0f) {
        sinValue = -sinValue;
    }
    *pSin = sinValue;
    *pCos = cosValue;
}

// 0x0047DD30 (name is ours)
nn::math::MTX44* MTX44SetIdentity(nn::math::MTX44* pOut)
{
    nn::math::ARMv6::MTX44CopyAsm(pOut, &nn::math::MTX44::Identity());
    return pOut;
}

// 0x0047DDD0 | nintendogs:bytes [tier A]
nn::math::VEC3* VEC3SafeNormalize(nn::math::VEC3* pOut, const nn::math::VEC3* pV, const nn::math::VEC3& alt)
{
    f32 lengthSquare = pV->x * pV->x + pV->y * pV->y + pV->z * pV->z;
    if (lengthSquare == 0.0f) {
        *pOut = alt;
        return pOut;
    }
    f32 inverse = 1.0f / sqrtf(lengthSquare);
    pOut->x = pV->x * inverse;
    pOut->y = pV->y * inverse;
    pOut->z = pV->z * inverse;
    return pOut;
}

// 0x0047DE38 (name is ours)
nn::math::VEC3* VEC3Project(nn::math::VEC3* pOut, const nn::math::MTX44* pM, const nn::math::VEC3* pV)
{
    f32 x = pV->x;
    f32 y = pV->y;
    f32 z = pV->z;
    f32 w = (pM->m[3][1] * y + pM->m[3][2] * z) + (pM->m[3][3] + pM->m[3][0] * x);
    f32 outX = (pM->m[0][1] * y + pM->m[0][2] * z) + (pM->m[0][3] + pM->m[0][0] * x);
    f32 outY = (pM->m[1][1] * y + pM->m[1][2] * z) + (pM->m[1][3] + pM->m[1][0] * x);
    f32 outZ = (pM->m[2][1] * y + pM->m[2][2] * z) + (pM->m[2][3] + pM->m[2][0] * x);
    f32 inverse = 1.0f / w;
    pOut->z = outZ * inverse;
    pOut->x = outX * inverse;
    pOut->y = outY * inverse;
    return pOut;
}

// 0x0047E5AC | fefates:bytes [tier B]
f32 CosIdx(u16 index)
{
    const SinCosEntry& entry = s_SinCosTable[index >> 8];
    return entry.cosValue + entry.cosDelta * ((index & 0xFF) * FRACTION_UNIT);
}

// 0x0047E5E4 | nintendogs:bytes [tier A]
f32 SinIdx(u16 index)
{
    const SinCosEntry& entry = s_SinCosTable[index >> 8];
    return entry.sinValue + entry.sinDelta * ((index & 0xFF) * FRACTION_UNIT);
}

// 0x0047E61C (name is ours)
u32 CountOneBits(u32 value)
{
    value = value - ((value >> 1) & 0x55555555);
    value = (value & 0x33333333) + ((value >> 2) & 0x33333333);
    value = (value + (value >> 4)) & 0x0F0F0F0F;
    value = value + (value >> 8);
    value = value + (value >> 16);
    return value & 0x3F;
}

// 0x0047E660 | nintendogs:callgraph [tier A]
f32 CosFIdx(f32 index)
{
    f32 value = ReduceIndex(index);
    u16 step = static_cast<u32>(value);
    f32 fraction = value - step;
    const SinCosEntry& entry = s_SinCosTable[step & 0xFF];
    return entry.cosValue + fraction * entry.cosDelta;
}

// 0x0047E6BC | fefates:bytes [tier B]
f32 SinFIdx(f32 index)
{
    f32 value = ReduceIndex(index);
    u16 step = static_cast<u32>(value);
    f32 fraction = value - step;
    const SinCosEntry& entry = s_SinCosTable[step & 0xFF];
    f32 sinValue = entry.sinValue + fraction * entry.sinDelta;
    if (index < 0.0f) {
        sinValue = -sinValue;
    }
    return sinValue;
}

// 0x0047E738 | nintendogs:bytes [tier A]
nn::math::VEC3* VEC3Cross(nn::math::VEC3* pOut, const nn::math::VEC3* p1, const nn::math::VEC3* p2)
{
    f32 x = p1->y * p2->z - p1->z * p2->y;
    f32 y = p1->z * p2->x - p1->x * p2->z;
    f32 z = p1->x * p2->y - p1->y * p2->x;
    pOut->x = x;
    pOut->y = y;
    pOut->z = z;
    return pOut;
}

} // namespace math
} // namespace nn
