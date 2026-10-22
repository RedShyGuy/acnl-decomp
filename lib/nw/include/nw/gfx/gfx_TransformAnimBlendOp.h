#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimBlendOp.h"
#include "nw/gfx/gfx_CalculatedTransform.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx20TransformAnimBlendOpE @ 0x008D0700
// The blend operations of transforms (function names from the reference symbols, the rest ours).
// The derived classes differ in how they blend the scale (standard: weighted sum; accurate: the sum
// of the weighted logarithms, exp afterwards) and the rotation (matrix: weighted sum of the first two
// rows, normalized afterwards; quaternion: slerp). The weights are scale, rotation, translation; a
// weight of -1 leaves the part out.
class TransformAnimBlendOp : public anim::AnimBlendOp
{
public:
    // the parts OverrideTransform converts to the form for blending (names are ours)
    enum OverrideMode
    {
        OVERRIDE_ACCURATE_SCALE = 1 << 0,
        OVERRIDE_QUATERNION = 1 << 1,
    };

    // (inline)
    TransformAnimBlendOp() : AnimBlendOp(true, true) {}

    virtual void Apply(void* pTarget, const anim::AnimResult* pResult) const;
    virtual void ConvertToAnimResult(anim::AnimResult* pResult, const void* pSource) const;

protected:
    void BlendTranslate(CalculatedTransform* pDst, const CalculatedTransform* pSrc, f32 weight) const;
    void BlendRotateMatrix(CalculatedTransform* pDst, const CalculatedTransform* pSrc, f32 weight) const;
    // the parts of the source that the destination does not have yet; true if it has all
    bool OverrideTransform(CalculatedTransform* pDst, const CalculatedTransform* pSrc, u32 mode) const;
    void BlendScaleAccurate(CalculatedTransform* pDst, const CalculatedTransform* pSrc, f32 weight) const;
    void BlendScaleStandard(CalculatedTransform* pDst, const CalculatedTransform* pSrc, f32 weight) const;
    void BlendRotateQuaternion(CalculatedTransform* pDst, const CalculatedTransform* pSrc, f32 weight) const;
    bool PostBlendAccurateScale(CalculatedTransform* pTransform) const;
};
} // namespace gfx
} // namespace nw
