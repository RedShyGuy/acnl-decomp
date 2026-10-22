#include "nw/gfx/gfx_TransformAnimBlendOpStandard.h"

namespace nw {
namespace gfx {
// 0x004A4774 slot 0x00 (name is ours)
// 0x004A4770 (deleting dtor)
nw::gfx::TransformAnimBlendOpStandard::~TransformAnimBlendOpStandard()
{
    // empty (in the original too)
}

// 0x0073C5CC slot 0x08 | nintendogs:callseq [tier A]
bool nw::gfx::TransformAnimBlendOpStandard::Blend(anim::AnimResult* pDst, f32* /*pDstWeights*/, const anim::AnimResult* pSrc, const f32* pSrcWeights) const
{
    CalculatedTransform* pDstTransform = reinterpret_cast<CalculatedTransform*>(pDst);
    const CalculatedTransform* pSrcTransform = reinterpret_cast<const CalculatedTransform*>(pSrc);
    BlendScaleStandard(pDstTransform, pSrcTransform, pSrcWeights[0]);
    BlendRotateMatrix(pDstTransform, pSrcTransform, pSrcWeights[1]);
    BlendTranslate(pDstTransform, pSrcTransform, pSrcWeights[2]);
    return true;
}

// 0x0073C624 slot 0x0C (name after nintendogs)
bool nw::gfx::TransformAnimBlendOpStandard::PostBlend(anim::AnimResult* pResult, const f32* /*pWeights*/) const
{
    return reinterpret_cast<CalculatedTransform*>(pResult)->NormalizeRotateMatrix();
}

// 0x0073C61C slot 0x10 (name after nintendogs)
bool nw::gfx::TransformAnimBlendOpStandard::Override(anim::AnimResult* pDst, const anim::AnimResult* pSrc) const
{
    return OverrideTransform(reinterpret_cast<CalculatedTransform*>(pDst), reinterpret_cast<const CalculatedTransform*>(pSrc), 0);
}

} // namespace gfx
} // namespace nw
