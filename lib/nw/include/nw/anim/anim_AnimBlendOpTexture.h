#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimBlendOp.h"
#include "nw/ut/ut_Offset.h"

namespace nw {
namespace anim {
// RTTI N2nw4anim18AnimBlendOpTextureE @ 0x008D0BAC
// vtable 0x009035E0 (vptr 0x009035E8), offset_to_top 0, 7 entries
// the blend operations of a texture: an Offset to its reference (not blended)
class AnimBlendOpTexture : public AnimBlendOp
{
public:
    // (inline)
    AnimBlendOpTexture() : AnimBlendOp(false, false) {}
    virtual ~AnimBlendOpTexture();
    virtual bool Override(AnimResult* pDst, const AnimResult* pSrc) const;
    virtual void Apply(void* pTarget, const AnimResult* pResult) const;
    virtual void ConvertToAnimResult(AnimResult* pResult, const void* pSource) const;
};
} // namespace anim
} // namespace nw
