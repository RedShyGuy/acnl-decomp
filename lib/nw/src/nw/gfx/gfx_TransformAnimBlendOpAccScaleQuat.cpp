#include "nw/gfx/gfx_TransformAnimBlendOpAccScaleQuat.h"

namespace nw {
namespace gfx {
// 0x004A4784 slot 0x00 (name is ours)
// 0x004A4780 (deleting dtor)
nw::gfx::TransformAnimBlendOpAccScaleQuat::~TransformAnimBlendOpAccScaleQuat()
{
    // empty (in the original too)
}

// 0x0073C634 slot 0x08 | nintendogs:callseq [tier A]
bool nw::gfx::TransformAnimBlendOpAccScaleQuat::Blend(anim::AnimResult* pDst, f32* /*pDstWeights*/, const anim::AnimResult* pSrc, const f32* pSrcWeights) const
{
    CalculatedTransform* pDstTransform = reinterpret_cast<CalculatedTransform*>(pDst);
    const CalculatedTransform* pSrcTransform = reinterpret_cast<const CalculatedTransform*>(pSrc);
    BlendScaleAccurate(pDstTransform, pSrcTransform, pSrcWeights[0]);
    BlendRotateQuaternion(pDstTransform, pSrcTransform, pSrcWeights[1]);
    BlendTranslate(pDstTransform, pSrcTransform, pSrcWeights[2]);
    return true;
}

// 0x0073C68C slot 0x0C (name after nintendogs)
bool nw::gfx::TransformAnimBlendOpAccScaleQuat::PostBlend(anim::AnimResult* pResult, const f32* /*pWeights*/) const
{
    CalculatedTransform* pTransform = reinterpret_cast<CalculatedTransform*>(pResult);
    return PostBlendAccurateScale(pTransform) & pTransform->NormalizeRotateQuaternion();
}

// 0x0073C684 slot 0x10 (name after nintendogs)
bool nw::gfx::TransformAnimBlendOpAccScaleQuat::Override(anim::AnimResult* pDst, const anim::AnimResult* pSrc) const
{
    return OverrideTransform(reinterpret_cast<CalculatedTransform*>(pDst), reinterpret_cast<const CalculatedTransform*>(pSrc), OVERRIDE_ACCURATE_SCALE | OVERRIDE_QUATERNION);
}

} // namespace gfx
} // namespace nw
