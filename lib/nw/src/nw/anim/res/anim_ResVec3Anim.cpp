#include "nw/anim/res/anim_ResVec3Anim.h"

namespace nw {
namespace anim {
namespace res {
// 0x007437F8 | nintendogs:bytes [tier A]
u32 nw::anim::res::ResVec3Anim::EvaluateResult(void* pTarget, u32 result, f32 frame) const
{
    f32* pValue = static_cast<f32*>(pTarget);
    const u32 flags = ptr()->flags;
    for (int i = 0; i < 3; ++i) {
        if (!(flags & (ResVec3AnimData::FLAG_NOT_EXIST_X << i))) {
            const ResVec3AnimData* pData = ptr();
            pValue[i] = (pData->flags & (ResVec3AnimData::FLAG_CONSTANT_X << i)) ? pData->components[i].constantValue
                                                                                 : CalcFloatCurve(pData->components[i].curve.To<ResFloatCurveData>(), frame);
            result |= 1 << i;
        }
    }
    return result;
}

} // namespace res
} // namespace anim
} // namespace nw
