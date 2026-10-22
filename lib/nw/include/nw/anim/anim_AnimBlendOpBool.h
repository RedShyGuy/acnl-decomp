#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimBlendOp.h"

namespace nw {
namespace anim {
// RTTI N2nw4anim15AnimBlendOpBoolE @ 0x008D0B94
// vtable 0x00903598 (vptr 0x009035A0), offset_to_top 0, 7 entries
// the blend operations of a bool (blended: true if one of the animations is true)
class AnimBlendOpBool : public AnimBlendOp
{
public:
    // (inline)
    AnimBlendOpBool() : AnimBlendOp(true, false) {}
    virtual ~AnimBlendOpBool();
    virtual bool Blend(AnimResult* pDst, f32* pDstWeights, const AnimResult* pSrc, const f32* pSrcWeights) const;
    virtual bool Override(AnimResult* pDst, const AnimResult* pSrc) const;
    virtual void Apply(void* pTarget, const AnimResult* pResult) const;
    virtual void ConvertToAnimResult(AnimResult* pResult, const void* pSource) const;
};
} // namespace anim
} // namespace nw
