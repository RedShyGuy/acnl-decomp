#include "nw/anim/res/anim_ResRgbaColorAnim.h"

namespace nw {
namespace anim {
namespace res {
// 0x00743C6C | nintendogs:bytes [tier A]
u32 nw::anim::res::ResRgbaColorAnim::EvaluateResult(void* pTarget, u32 result, f32 frame) const
{
    f32* pValue = static_cast<f32*>(pTarget);
    const u32 flags = ptr()->flags;
    for (int i = 0; i < 4; ++i) {
        if (!(flags & (ResRgbaColorAnimData::FLAG_NOT_EXIST_R << i))) {
            const ResRgbaColorAnimData* pData = ptr();
            pValue[i] = (pData->flags & (ResRgbaColorAnimData::FLAG_CONSTANT_R << i)) ? pData->components[i].constantValue
                                                                                 : CalcFloatCurve(pData->components[i].curve.To<ResFloatCurveData>(), frame);
            result |= 1 << i;
        }
    }
    return result;
}

} // namespace res
} // namespace anim
} // namespace nw
