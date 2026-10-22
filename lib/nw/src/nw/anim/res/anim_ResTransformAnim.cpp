#include "nw/anim/res/anim_ResTransformAnim.h"

#include <new>

#include "nn/math/math_Transform.h"
#include "nw/gfx/gfx_CalculatedTransform.h"

namespace nw {
namespace anim {
namespace res {
namespace {
// (inline) a component: the original value, the constant or the curve at the frame
inline f32 EvaluateComponent(const ResTransformAnimData* pData, int index, f32 frame, f32 originalValue)
{
    if (pData->flags & (1 << (ResTransformAnimData::NOT_EXIST_SHIFT + index))) {
        return originalValue;
    }
    if (pData->flags & (1 << (ResTransformAnimData::CONSTANT_SHIFT + index))) {
        return pData->components[index].constantValue;
    }
    return CalcFloatCurve(pData->components[index].curve.To<ResFloatCurveData>(), frame);
}
} // namespace

// 0x00743D84 | nintendogs:bytes-fuzzy [tier A]
u32 nw::anim::res::ResTransformAnim::EvaluateResult(void* pTarget, u32 result, f32 frame, const void* pOriginalValue) const
{
    const nn::math::Transform3* pOriginal = static_cast<const nn::math::Transform3*>(pOriginalValue);
    const ResTransformAnimData* pData = ptr();
    nn::math::VEC3 scale;
    nn::math::VEC3 rotate;
    nn::math::VEC3 translate;
    scale.x = EvaluateComponent(pData, ResTransformAnimData::COMPONENT_SCALE_X + 0, frame, pOriginal->scale.x);
    scale.y = EvaluateComponent(ptr(), ResTransformAnimData::COMPONENT_SCALE_X + 1, frame, pOriginal->scale.y);
    scale.z = EvaluateComponent(ptr(), ResTransformAnimData::COMPONENT_SCALE_X + 2, frame, pOriginal->scale.z);
    rotate.x = EvaluateComponent(ptr(), ResTransformAnimData::COMPONENT_ROTATE_X + 0, frame, pOriginal->rotate.x);
    rotate.y = EvaluateComponent(ptr(), ResTransformAnimData::COMPONENT_ROTATE_X + 1, frame, pOriginal->rotate.y);
    rotate.z = EvaluateComponent(ptr(), ResTransformAnimData::COMPONENT_ROTATE_X + 2, frame, pOriginal->rotate.z);
    translate.x = EvaluateComponent(ptr(), ResTransformAnimData::COMPONENT_TRANSLATE_X + 0, frame, pOriginal->translate.x);
    translate.y = EvaluateComponent(ptr(), ResTransformAnimData::COMPONENT_TRANSLATE_X + 1, frame, pOriginal->translate.y);
    translate.z = EvaluateComponent(ptr(), ResTransformAnimData::COMPONENT_TRANSLATE_X + 2, frame, pOriginal->translate.z);

    gfx::CalculatedTransform* pTransform =
        new (pTarget) gfx::CalculatedTransform(nn::math::MTX34::Identity(), nn::math::VEC3(1.0f, 1.0f, 1.0f));
    pTransform->SetScale(scale);
    pTransform->SetRotateAndTranslate(rotate, translate);
    pTransform->UpdateScaleFlags();
    pTransform->UpdateRotateFlags();
    pTransform->UpdateTranslateFlags();
    pTransform->UpdateCompositeFlags();
    return result;
}

} // namespace res
} // namespace anim
} // namespace nw
