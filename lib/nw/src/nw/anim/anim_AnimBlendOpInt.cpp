#include "nw/anim/anim_AnimBlendOpInt.h"

namespace nw {
namespace anim {
// 0x004D5184 slot 0x00 (name is ours)
// 0x004D5180 (deleting dtor)
nw::anim::AnimBlendOpInt::~AnimBlendOpInt()
{
    // empty (in the original too)
}

// 0x0074344C slot 0x10 (name is ours)
bool nw::anim::AnimBlendOpInt::Override(AnimResult* pDst, const AnimResult* pSrc) const
{
    pDst->m_EnabledFlags = 1;
    *static_cast<s32*>(pDst->GetValueBuffer()) = *static_cast<const s32*>(pSrc->GetValueBuffer());
    return true;
}

// 0x00743440 slot 0x14 (name is ours)
void nw::anim::AnimBlendOpInt::Apply(void* pTarget, const AnimResult* pResult) const
{
    *static_cast<s32*>(pTarget) = *static_cast<const s32*>(pResult->GetValueBuffer());
}

// 0x0074342C slot 0x18 (name is ours)
void nw::anim::AnimBlendOpInt::ConvertToAnimResult(AnimResult* pResult, const void* pSource) const
{
    pResult->m_EnabledFlags = 1;
    *static_cast<s32*>(pResult->GetValueBuffer()) = *static_cast<const s32*>(pSource);
}

} // namespace anim
} // namespace nw
