#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimBlendOp.h"

namespace nw {
namespace anim {
// RTTI N2nw4anim16AnimBlendOpFloatE @ 0x008D0BA0
// vtable 0x009035BC (vptr 0x009035C4), offset_to_top 0, 7 entries
// the blend operations of a float
class AnimBlendOpFloat : public AnimBlendOp
{
public:
    // (inline)
    AnimBlendOpFloat() : AnimBlendOp(true, false) {}
    virtual ~AnimBlendOpFloat();
    virtual bool Blend(AnimResult* pDst, f32* pDstWeights, const AnimResult* pSrc, const f32* pSrcWeights) const;
    virtual bool Override(AnimResult* pDst, const AnimResult* pSrc) const;
    virtual void Apply(void* pTarget, const AnimResult* pResult) const;
    virtual void ConvertToAnimResult(AnimResult* pResult, const void* pSource) const;
};
} // namespace anim
} // namespace nw
