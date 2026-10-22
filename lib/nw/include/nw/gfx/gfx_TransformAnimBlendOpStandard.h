#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_TransformAnimBlendOp.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx28TransformAnimBlendOpStandardE @ 0x008D07A8
// vtable 0x0090294C (vptr 0x00902954), offset_to_top 0, 7 entries
// scale: standard, rotation: matrix; the results are the CalculatedTransforms themselves (the evaluator passes them as AnimResult*)
class TransformAnimBlendOpStandard : public TransformAnimBlendOp
{
public:
    // (inline)
    TransformAnimBlendOpStandard() {}
    virtual ~TransformAnimBlendOpStandard();
    virtual bool Blend(anim::AnimResult* pDst, f32* pDstWeights, const anim::AnimResult* pSrc, const f32* pSrcWeights) const;
    virtual bool PostBlend(anim::AnimResult* pResult, const f32* pWeights) const;
    virtual bool Override(anim::AnimResult* pDst, const anim::AnimResult* pSrc) const;
};
} // namespace gfx
} // namespace nw
