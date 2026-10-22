#include "nw/gfx/gfx_AnimBlendOpTransform.h"

namespace nw {
namespace gfx {
namespace {
// (inline) the transform of a result that has none yet starts without parts
inline CalculatedTransform* GetTransform(anim::AnimResult* pResult)
{
    CalculatedTransform* pTransform = static_cast<CalculatedTransform*>(pResult->GetValueBuffer());
    if (!pResult->IsEnabledFlags(1)) {
        pTransform->EnableFlags(CalculatedTransform::FLAG_TRANSLATE_UNSET | CalculatedTransform::FLAG_SCALE_UNSET |
                                CalculatedTransform::FLAG_ROTATE_UNSET);
        pResult->m_EnabledFlags |= 1;
    }
    return pTransform;
}

// (inline) the flags after the parts changed
inline void UpdateFlags(CalculatedTransform* pTransform)
{
    pTransform->UpdateScaleFlags();
    pTransform->UpdateRotateFlags();
    pTransform->UpdateTranslateFlags();
    pTransform->UpdateCompositeFlags();
    pTransform->EnableFlags(CalculatedTransform::FLAG_IS_DIRTY | CalculatedTransform::FLAG_UNKNOWN_0x1);
}
} // namespace

// 0x004A0DF0 slot 0x00 (name is ours)
// 0x004A0DE0 (deleting dtor)
nw::gfx::AnimBlendOpTransform::~AnimBlendOpTransform()
{
    // empty (in the original too)
}

// 0x0073B0D4 slot 0x08 | nintendogs:bytes [tier A]
bool nw::gfx::AnimBlendOpTransform::Blend(anim::AnimResult* pDst, f32* /*pDstWeights*/, const anim::AnimResult* pSrc, const f32* pSrcWeights) const
{
    CalculatedTransform* pDstTransform = GetTransform(pDst);
    const CalculatedTransform* pSrcTransform = static_cast<const CalculatedTransform*>(pSrc->GetValueBuffer());
    const f32 weight = pSrcWeights[0];
    BlendScaleStandard(pDstTransform, pSrcTransform, weight);
    BlendRotateMatrix(pDstTransform, pSrcTransform, weight);
    BlendTranslate(pDstTransform, pSrcTransform, weight);
    return true;
}

// 0x0073B160 slot 0x10 | nintendogs:callseq [tier A]
bool nw::gfx::AnimBlendOpTransform::Override(anim::AnimResult* pDst, const anim::AnimResult* pSrc) const
{
    CalculatedTransform* pDstTransform = GetTransform(pDst);
    const bool isComplete = OverrideTransform(pDstTransform, static_cast<const CalculatedTransform*>(pSrc->GetValueBuffer()), 0);
    UpdateFlags(pDstTransform);
    return isComplete;
}

// 0x0073B234 slot 0x0C | nintendogs:callseq [tier A]
bool nw::gfx::AnimBlendOpTransform::PostBlend(anim::AnimResult* pResult, const f32* /*pWeights*/) const
{
    CalculatedTransform* pTransform = static_cast<CalculatedTransform*>(pResult->GetValueBuffer());
    const bool isNormalized = pTransform->NormalizeRotateMatrix();
    UpdateFlags(pTransform);
    return isNormalized;
}

} // namespace gfx
} // namespace nw
