#include "nw/anim/anim_AnimBlendOpVector2.h"

namespace nw {
namespace anim {
// 0x004D5210 slot 0x00 (name is ours)
// 0x004D520C (deleting dtor)
nw::anim::AnimBlendOpVector2::~AnimBlendOpVector2()
{
    // empty (in the original too)
}

// 0x00743260 slot 0x08 | nintendogs:bytes [tier A]
bool nw::anim::AnimBlendOpVector2::Blend(AnimResult* pDst, f32* pDstWeights, const AnimResult* pSrc, const f32* pSrcWeights) const
{
    return BlendFloatVector(pDst, pSrc, *pSrcWeights, 2);
}

// 0x007435DC slot 0x10 | nintendogs:bytes [tier A]
bool nw::anim::AnimBlendOpVector2::Override(AnimResult* pDst, const AnimResult* pSrc) const
{
    return OverrideFloatVector(pDst, pSrc, 2, 0x3);
}

// 0x00743218 slot 0x14 (name after nintendogs)
void nw::anim::AnimBlendOpVector2::Apply(void* pTarget, const AnimResult* pResult) const
{
    ApplyFloatVector(pTarget, pResult, 2);
}

// 0x007433B8 slot 0x18 (name after nintendogs)
void nw::anim::AnimBlendOpVector2::ConvertToAnimResult(AnimResult* pResult, const void* pSource) const
{
    ConvertFloatVectorToAnimResult(pResult, pSource, 2);
}

} // namespace anim
} // namespace nw
