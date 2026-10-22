#include "nw/anim/anim_AnimBlendOpBool.h"

namespace nw {
namespace anim {
// 0x004D518C slot 0x00 (name is ours)
// 0x004D5188 (deleting dtor)
nw::anim::AnimBlendOpBool::~AnimBlendOpBool()
{
    // empty (in the original too)
}

// 0x00743480 slot 0x08 | nintendogs:bytes [tier A]
bool nw::anim::AnimBlendOpBool::Blend(AnimResult* pDst, f32* pDstWeights, const AnimResult* pSrc, const f32* pSrcWeights) const
{
    bool* pDstValue = static_cast<bool*>(pDst->GetValueBuffer());
    const bool srcValue = *static_cast<const bool*>(pSrc->GetValueBuffer());
    if (!pDst->IsEnabledFlags(1)) {
        *pDstValue = srcValue;
        pDst->m_EnabledFlags |= 1;
    } else {
        *pDstValue = *pDstValue || srcValue;
    }
    // once true, the other animations change nothing
    return !*pDstValue;
}

// 0x007434D0 slot 0x10 | nintendogs:bytes [tier A]
bool nw::anim::AnimBlendOpBool::Override(AnimResult* pDst, const AnimResult* pSrc) const
{
    *static_cast<bool*>(pDst->GetValueBuffer()) = *static_cast<const bool*>(pSrc->GetValueBuffer());
    pDst->m_EnabledFlags = 1;
    return true;
}

// 0x00743474 slot 0x14 (name after nintendogs)
void nw::anim::AnimBlendOpBool::Apply(void* pTarget, const AnimResult* pResult) const
{
    *static_cast<bool*>(pTarget) = *static_cast<const bool*>(pResult->GetValueBuffer());
}

// 0x00743460 slot 0x18 | nintendogs:bytes [tier A]
void nw::anim::AnimBlendOpBool::ConvertToAnimResult(AnimResult* pResult, const void* pSource) const
{
    *static_cast<bool*>(pResult->GetValueBuffer()) = *static_cast<const bool*>(pSource);
    pResult->m_EnabledFlags = 1;
}

} // namespace anim
} // namespace nw
