#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimBlendOp.h"

namespace nw {
namespace anim {
// RTTI N2nw4anim14AnimBlendOpIntE @ 0x008D0B88
// vtable 0x00903574 (vptr 0x0090357C), offset_to_top 0, 7 entries
// the blend operations of an integer (not blended: Blend is the default)
class AnimBlendOpInt : public AnimBlendOp
{
public:
    // (inline)
    AnimBlendOpInt() : AnimBlendOp(false, false) {}
    virtual ~AnimBlendOpInt();
    virtual bool Override(AnimResult* pDst, const AnimResult* pSrc) const;
    virtual void Apply(void* pTarget, const AnimResult* pResult) const;
    virtual void ConvertToAnimResult(AnimResult* pResult, const void* pSource) const;
};
} // namespace anim
} // namespace nw
