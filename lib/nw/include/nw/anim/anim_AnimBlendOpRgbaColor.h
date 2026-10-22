#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimBlendOp.h"

namespace nw {
namespace anim {
// RTTI N2nw4anim20AnimBlendOpRgbaColorE @ 0x008D0BD8
// vtable 0x0090365C (vptr 0x00903664), offset_to_top 0, 7 entries
// the blend operations of a float color (r, g, b, a)
class AnimBlendOpRgbaColor : public AnimBlendOp
{
public:
    // (inline)
    AnimBlendOpRgbaColor() : AnimBlendOp(true, false) {}
    virtual ~AnimBlendOpRgbaColor();
    virtual bool Blend(AnimResult* pDst, f32* pDstWeights, const AnimResult* pSrc, const f32* pSrcWeights) const;
    virtual bool Override(AnimResult* pDst, const AnimResult* pSrc) const;
    virtual void Apply(void* pTarget, const AnimResult* pResult) const;
    virtual void ConvertToAnimResult(AnimResult* pResult, const void* pSource) const;
};
} // namespace anim
} // namespace nw
