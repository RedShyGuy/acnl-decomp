#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimBlendOp.h"

namespace nw {
namespace anim {
// RTTI N2nw4anim18AnimBlendOpVector2E @ 0x008D0BB8
// vtable 0x00903604 (vptr 0x0090360C), offset_to_top 0, 7 entries
// the blend operations of a VEC2
class AnimBlendOpVector2 : public AnimBlendOp
{
public:
    // (inline)
    AnimBlendOpVector2() : AnimBlendOp(true, false) {}
    virtual ~AnimBlendOpVector2();
    virtual bool Blend(AnimResult* pDst, f32* pDstWeights, const AnimResult* pSrc, const f32* pSrcWeights) const;
    virtual bool Override(AnimResult* pDst, const AnimResult* pSrc) const;
    virtual void Apply(void* pTarget, const AnimResult* pResult) const;
    virtual void ConvertToAnimResult(AnimResult* pResult, const void* pSource) const;
};
} // namespace anim
} // namespace nw
