#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_TransformAnimBlendOp.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx28TransformAnimBlendOpAccScaleE @ 0x008D079C
// vtable 0x00902928 (vptr 0x00902930), offset_to_top 0, 7 entries
// scale: accurate, rotation: matrix; the results are the CalculatedTransforms themselves (the evaluator passes them as AnimResult*)
class TransformAnimBlendOpAccScale : public TransformAnimBlendOp
{
public:
    // (inline)
    TransformAnimBlendOpAccScale() {}
    virtual ~TransformAnimBlendOpAccScale();
    virtual bool Blend(anim::AnimResult* pDst, f32* pDstWeights, const anim::AnimResult* pSrc, const f32* pSrcWeights) const;
    virtual bool PostBlend(anim::AnimResult* pResult, const f32* pWeights) const;
    virtual bool Override(anim::AnimResult* pDst, const anim::AnimResult* pSrc) const;
};
} // namespace gfx
} // namespace nw
