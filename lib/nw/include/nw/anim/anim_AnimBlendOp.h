#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimResult.h"

namespace nw {
namespace anim {
// RTTI N2nw4anim11AnimBlendOpE @ 0x008D0B80
// The operations of a value type for blending the results of several animations, overriding one
// with another and writing a result to its target. Blend and PostBlend have defaults that the slots
// of several derived classes share (0x0074341C, 0x00743424). Blend: the result of the source
// weighted, added to the destination; returns whether blending is still needed (names after the
// reference symbols, parameter names ours).
class AnimBlendOp
{
public:
    // (inline)
    AnimBlendOp(bool hasBlend, bool hasPostBlend) : m_HasBlend(hasBlend), m_HasPostBlend(hasPostBlend) {}
    virtual ~AnimBlendOp() {}
    virtual bool Blend(AnimResult* pDst, f32* pDstWeights, const AnimResult* pSrc, const f32* pSrcWeights) const;
    virtual bool PostBlend(AnimResult* pResult, const f32* pWeights) const;
    // the components of the source that the destination does not have yet; returns whether the
    // destination is complete
    virtual bool Override(AnimResult* pDst, const AnimResult* pSrc) const = 0; // slot 0x10
    virtual void Apply(void* pTarget, const AnimResult* pResult) const = 0; // slot 0x14
    virtual void ConvertToAnimResult(AnimResult* pResult, const void* pSource) const = 0; // slot 0x18

protected:
    // the float vectors (Vector2, Vector3, RgbaColor)
    void ApplyFloatVector(void* pTarget, const AnimResult* pResult, int count) const;
    bool BlendFloatVector(AnimResult* pDst, const AnimResult* pSrc, f32 weight, int count) const;
    bool OverrideFloatVector(AnimResult* pDst, const AnimResult* pSrc, int count, u32 allFlags) const;
    void ConvertFloatVectorToAnimResult(AnimResult* pResult, const void* pSource, int count) const;

public:
    bool HasBlend() const { return m_HasBlend; }
    bool HasPostBlend() const { return m_HasPostBlend; }

    bool m_HasBlend;     // 0x4 whether the results can be blended (else the first one wins)
    bool m_HasPostBlend; // 0x5 whether the blended result needs PostBlend
};
ASSERT_SIZE(AnimBlendOp, 0x8);
} // namespace anim
} // namespace nw
