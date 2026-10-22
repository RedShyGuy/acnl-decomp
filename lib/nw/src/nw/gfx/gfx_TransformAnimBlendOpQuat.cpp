#include "nw/gfx/gfx_TransformAnimBlendOpQuat.h"

namespace nw {
namespace gfx {
// 0x004A3B1C slot 0x00 (name is ours)
// 0x004A3B18 (deleting dtor)
nw::gfx::TransformAnimBlendOpQuat::~TransformAnimBlendOpQuat()
{
    // empty (in the original too)
}

// 0x0073C4CC slot 0x08 | nintendogs:callseq [tier A]
bool nw::gfx::TransformAnimBlendOpQuat::Blend(anim::AnimResult* pDst, f32* /*pDstWeights*/, const anim::AnimResult* pSrc, const f32* pSrcWeights) const
{
    CalculatedTransform* pDstTransform = reinterpret_cast<CalculatedTransform*>(pDst);
    const CalculatedTransform* pSrcTransform = reinterpret_cast<const CalculatedTransform*>(pSrc);
    BlendScaleStandard(pDstTransform, pSrcTransform, pSrcWeights[0]);
    BlendRotateQuaternion(pDstTransform, pSrcTransform, pSrcWeights[1]);
    BlendTranslate(pDstTransform, pSrcTransform, pSrcWeights[2]);
    return true;
}

// 0x0073C51C slot 0x0C (name after nintendogs)
bool nw::gfx::TransformAnimBlendOpQuat::PostBlend(anim::AnimResult* pResult, const f32* /*pWeights*/) const
{
    return reinterpret_cast<CalculatedTransform*>(pResult)->NormalizeRotateQuaternion();
}

// 0x0073B440 slot 0x10 (name after nintendogs)
bool nw::gfx::TransformAnimBlendOpQuat::Override(anim::AnimResult* pDst, const anim::AnimResult* pSrc) const
{
    return OverrideTransform(reinterpret_cast<CalculatedTransform*>(pDst), reinterpret_cast<const CalculatedTransform*>(pSrc), OVERRIDE_QUATERNION);
}

} // namespace gfx
} // namespace nw
