#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_TransformAnimBlendOpStandard.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx20AnimBlendOpTransformE @ 0x008D06F4
// vtable 0x009027B8 (vptr 0x009027C0), offset_to_top 0, 7 entries
// the blend operations of transforms in AnimResults (scale: standard, rotation: matrix; one weight
// for all parts)
class AnimBlendOpTransform : public TransformAnimBlendOp
{
public:
    // (inline)
    AnimBlendOpTransform() {}
    virtual ~AnimBlendOpTransform();
    virtual bool Blend(anim::AnimResult* pDst, f32* pDstWeights, const anim::AnimResult* pSrc, const f32* pSrcWeights) const;
    virtual bool PostBlend(anim::AnimResult* pResult, const f32* pWeights) const;
    virtual bool Override(anim::AnimResult* pDst, const anim::AnimResult* pSrc) const;

    TransformAnimBlendOpStandard m_BlendOpStandard; // 0x8 (not used by the functions here)
};
ASSERT_SIZE(AnimBlendOpTransform, 0x10);
} // namespace gfx
} // namespace nw
