#pragma once

#include "decomp.h"

namespace nn {
namespace math {
class MTX44;
struct VEC3;

// The trigonometry with the table of 256 steps per turn (the index units: 65536 per turn for
// the integer versions, 256 for the float versions)
void SinCosFIdx(f32* pSin, f32* pCos, f32 index); // 0x0047DC98 | nintendogs:callgraph [tier A]
f32 CosIdx(u16 index); // 0x0047E5AC | fefates:bytes [tier B]
f32 SinIdx(u16 index); // 0x0047E5E4 | nintendogs:bytes [tier A]
f32 CosFIdx(f32 index); // 0x0047E660 | nintendogs:callgraph [tier A]
f32 SinFIdx(f32 index); // 0x0047E6BC | fefates:bytes [tier B]

// the identity (name is ours)
nn::math::MTX44* MTX44SetIdentity(nn::math::MTX44* pOut); // 0x0047DD30 (name is ours)
// the normalized vector, or alt for a zero vector
nn::math::VEC3* VEC3SafeNormalize(nn::math::VEC3* pOut, const nn::math::VEC3* pV, const nn::math::VEC3& alt); // 0x0047DDD0 | nintendogs:bytes [tier A]
// the vector transformed by the matrix and divided by w (name is ours)
nn::math::VEC3* VEC3Project(nn::math::VEC3* pOut, const nn::math::MTX44* pM, const nn::math::VEC3* pV); // 0x0047DE38 (name is ours)
// the number of set bits (name is ours)
u32 CountOneBits(u32 value); // 0x0047E61C (name is ours)
nn::math::VEC3* VEC3Cross(nn::math::VEC3* pOut, const nn::math::VEC3* p1, const nn::math::VEC3* p2); // 0x0047E738 | nintendogs:bytes [tier A]
} // namespace math
} // namespace nn
