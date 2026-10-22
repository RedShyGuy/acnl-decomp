#include "nw/anim/anim_AnimBlendOpFloat.h"

namespace nw {
namespace anim {
// 0x004D5200 slot 0x00 (name is ours)
// 0x004D51FC (deleting dtor)
nw::anim::AnimBlendOpFloat::~AnimBlendOpFloat()
{
    // empty (in the original too)
}

// 0x00743504 slot 0x08 | nintendogs:bytes [tier A]
bool nw::anim::AnimBlendOpFloat::Blend(AnimResult* pDst, f32* pDstWeights, const AnimResult* pSrc, const f32* pSrcWeights) const
{
    f32* pDstValue = static_cast<f32*>(pDst->GetValueBuffer());
    const f32 value = *pSrcWeights * *static_cast<const f32*>(pSrc->GetValueBuffer());
    if (!pDst->IsEnabledFlags(1)) {
        *pDstValue = value;
        pDst->m_EnabledFlags |= 1;
    } else {
        *pDstValue += value;
    }
    return true;
}

// 0x00743550 slot 0x10 (name is ours)
bool nw::anim::AnimBlendOpFloat::Override(AnimResult* pDst, const AnimResult* pSrc) const
{
    pDst->m_EnabledFlags = 1;
    *static_cast<f32*>(pDst->GetValueBuffer()) = *static_cast<const f32*>(pSrc->GetValueBuffer());
    return true;
}

// 0x007434F8 slot 0x14 (name is ours)
void nw::anim::AnimBlendOpFloat::Apply(void* pTarget, const AnimResult* pResult) const
{
    *static_cast<f32*>(pTarget) = *static_cast<const f32*>(pResult->GetValueBuffer());
}

// 0x007434E4 slot 0x18 (name is ours)
void nw::anim::AnimBlendOpFloat::ConvertToAnimResult(AnimResult* pResult, const void* pSource) const
{
    pResult->m_EnabledFlags = 1;
    *static_cast<f32*>(pResult->GetValueBuffer()) = *static_cast<const f32*>(pSource);
}

} // namespace anim
} // namespace nw
