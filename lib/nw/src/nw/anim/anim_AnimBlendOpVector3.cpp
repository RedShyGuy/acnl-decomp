#include "nw/anim/anim_AnimBlendOpVector3.h"

namespace nw {
namespace anim {
// 0x004D5218 slot 0x00 (name is ours)
// 0x004D5214 (deleting dtor)
nw::anim::AnimBlendOpVector3::~AnimBlendOpVector3()
{
    // empty (in the original too)
}

// 0x00743604 slot 0x08 | nintendogs:bytes [tier A]
bool nw::anim::AnimBlendOpVector3::Blend(AnimResult* pDst, f32* pDstWeights, const AnimResult* pSrc, const f32* pSrcWeights) const
{
    return BlendFloatVector(pDst, pSrc, *pSrcWeights, 3);
}

// 0x00743618 slot 0x10 | nintendogs:bytes [tier A]
bool nw::anim::AnimBlendOpVector3::Override(AnimResult* pDst, const AnimResult* pSrc) const
{
    return OverrideFloatVector(pDst, pSrc, 3, 0x7);
}

// 0x007435FC slot 0x14 (name after nintendogs)
void nw::anim::AnimBlendOpVector3::Apply(void* pTarget, const AnimResult* pResult) const
{
    ApplyFloatVector(pTarget, pResult, 3);
}

// 0x007435F4 slot 0x18 (name after nintendogs)
void nw::anim::AnimBlendOpVector3::ConvertToAnimResult(AnimResult* pResult, const void* pSource) const
{
    ConvertFloatVectorToAnimResult(pResult, pSource, 3);
}

} // namespace anim
} // namespace nw
