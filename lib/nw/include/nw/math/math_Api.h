#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"

namespace nw {
namespace math {
// the view matrix of a camera at pCamPos that looks at pTarget, turned by twist (an angle index:
// 256 is a full turn) around the line of sight
void MTX34LookAtFIdx(nn::math::MTX34* pOut, const nn::math::VEC3* pCamPos, f32 twist, const nn::math::VEC3* pTarget); // 0x004D9948 | nintendogs:bytes-fuzzy [confirmed by mk7dlp] [tier A]
} // namespace math
} // namespace nw
