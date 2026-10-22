#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_TransformAnimBlendOp.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx24TransformAnimBlendOpQuatE @ 0x008D076C
// vtable 0x009028B0 (vptr 0x009028B8), offset_to_top 0, 7 entries
// scale: standard, rotation: quaternion; the results are the CalculatedTransforms themselves (the evaluator passes them as AnimResult*)
class TransformAnimBlendOpQuat : public TransformAnimBlendOp
{
public:
    // (inline)
    TransformAnimBlendOpQuat() {}
    virtual ~TransformAnimBlendOpQuat();
    virtual bool Blend(anim::AnimResult* pDst, f32* pDstWeights, const anim::AnimResult* pSrc, const f32* pSrcWeights) const;
    virtual bool PostBlend(anim::AnimResult* pResult, const f32* pWeights) const;
    virtual bool Override(anim::AnimResult* pDst, const anim::AnimResult* pSrc) const;
};
} // namespace gfx
} // namespace nw
