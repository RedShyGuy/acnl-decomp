#include "nw/gfx/gfx_TransformAnimBlendOpAccScale.h"

namespace nw {
namespace gfx {
// 0x004A476C slot 0x00 (name is ours)
// 0x004A4768 (deleting dtor)
nw::gfx::TransformAnimBlendOpAccScale::~TransformAnimBlendOpAccScale()
{
    // empty (in the original too)
}

// 0x0073C554 slot 0x08 | nintendogs:callseq [tier A]
bool nw::gfx::TransformAnimBlendOpAccScale::Blend(anim::AnimResult* pDst, f32* /*pDstWeights*/, const anim::AnimResult* pSrc, const f32* pSrcWeights) const
{
    CalculatedTransform* pDstTransform = reinterpret_cast<CalculatedTransform*>(pDst);
    const CalculatedTransform* pSrcTransform = reinterpret_cast<const CalculatedTransform*>(pSrc);
    BlendScaleAccurate(pDstTransform, pSrcTransform, pSrcWeights[0]);
    BlendRotateMatrix(pDstTransform, pSrcTransform, pSrcWeights[1]);
    BlendTranslate(pDstTransform, pSrcTransform, pSrcWeights[2]);
    return true;
}

// 0x0073C5AC slot 0x0C | nintendogs:callseq [tier A]
bool nw::gfx::TransformAnimBlendOpAccScale::PostBlend(anim::AnimResult* pResult, const f32* /*pWeights*/) const
{
    CalculatedTransform* pTransform = reinterpret_cast<CalculatedTransform*>(pResult);
    return PostBlendAccurateScale(pTransform) & pTransform->NormalizeRotateMatrix();
}

// 0x0073C5A4 slot 0x10 (name after nintendogs)
bool nw::gfx::TransformAnimBlendOpAccScale::Override(anim::AnimResult* pDst, const anim::AnimResult* pSrc) const
{
    return OverrideTransform(reinterpret_cast<CalculatedTransform*>(pDst), reinterpret_cast<const CalculatedTransform*>(pSrc), OVERRIDE_ACCURATE_SCALE);
}

} // namespace gfx
} // namespace nw
