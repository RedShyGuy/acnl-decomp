#pragma once

#include "decomp.h"

namespace nw {
namespace anim {
namespace res {
void CalcIntCurve(const nw::anim::res::ResIntCurveData*, float); // 0x004D526C | nintendogs:bytes [tier A]
void CalcBoolCurve(const nw::anim::res::ResBoolCurveData*, float); // 0x004D52AC | nintendogs:bytes-fuzzy [tier A]
void CalcFloatCurve(const nw::anim::res::ResFloatCurveData*, float); // 0x004D5334 | nintendogs:bytes [tier A]
void CalcVector3Curve(nn::math::VEC3*, unsigned*, const nw::anim::res::ResBakedCurveData<nn::math::VEC3_>*, float); // 0x004D5724 | nintendogs:bytes [tier A]
void CalcTransformCurve(nn::math::MTX34*, const nw::anim::res::ResFullBakedCurveData*, float); // 0x004D5CA0 | nintendogs:bytes-fuzzy [tier A]
void CalcTranslateCurve(nn::math::MTX34*, unsigned*, const nw::anim::res::ResBakedCurveData<nn::math::VEC3_>*, float); // 0x004D5CE4 | nintendogs:bytes [tier A]
} // namespace res
} // namespace anim
} // namespace nw
