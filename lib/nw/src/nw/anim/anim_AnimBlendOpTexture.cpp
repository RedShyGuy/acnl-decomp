#include "nw/anim/anim_AnimBlendOpTexture.h"

namespace nw {
namespace anim {
// 0x004D5208 slot 0x00 (name is ours)
// 0x004D5204 (deleting dtor)
nw::anim::AnimBlendOpTexture::~AnimBlendOpTexture()
{
    // empty (in the original too)
}

// 0x007435B0 slot 0x10 | nintendogs:bytes [tier A]
bool nw::anim::AnimBlendOpTexture::Override(AnimResult* pDst, const AnimResult* pSrc) const
{
    *static_cast<ut::Offset*>(pDst->GetValueBuffer()) = *static_cast<const ut::Offset*>(pSrc->GetValueBuffer());
    pDst->m_EnabledFlags = 1;
    return true;
}

// 0x00743590 slot 0x14 | nintendogs:bytes [tier A]
void nw::anim::AnimBlendOpTexture::Apply(void* pTarget, const AnimResult* pResult) const
{
    *static_cast<ut::Offset*>(pTarget) = *static_cast<const ut::Offset*>(pResult->GetValueBuffer());
}

// 0x00743564 slot 0x18 | nintendogs:bytes [tier A]
void nw::anim::AnimBlendOpTexture::ConvertToAnimResult(AnimResult* pResult, const void* pSource) const
{
    *static_cast<ut::Offset*>(pResult->GetValueBuffer()) = *static_cast<const ut::Offset*>(pSource);
    pResult->m_EnabledFlags = 1;
}

} // namespace anim
} // namespace nw
