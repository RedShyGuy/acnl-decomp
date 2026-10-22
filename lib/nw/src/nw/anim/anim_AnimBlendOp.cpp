#include "nw/anim/anim_AnimBlendOp.h"

namespace nw {
namespace anim {
// 0x0074341C slot 0x08 (name after nintendogs: AnimBlendOpTexture::Blend; the slot of AnimBlendOpInt, too)
bool nw::anim::AnimBlendOp::Blend(AnimResult* /*pDst*/, f32* /*pDstWeights*/, const AnimResult* /*pSrc*/, const f32* /*pSrcWeights*/) const
{
    return true;
}

// 0x00743424 slot 0x0C (name after nintendogs: AnimBlendOpBool::PostBlend; the slot of all derived classes)
bool nw::anim::AnimBlendOp::PostBlend(AnimResult* /*pResult*/, const f32* /*pWeights*/) const
{
    return true;
}

// 0x00743220 | nintendogs:bytes [tier A]
void nw::anim::AnimBlendOp::ApplyFloatVector(void* pTarget, const AnimResult* pResult, int count) const
{
    f32* pDst = static_cast<f32*>(pTarget);
    const f32* pSrc = static_cast<const f32*>(pResult->GetValueBuffer());
    u32 flag = 1;
    for (int i = 0; i < count; ++i) {
        if (pResult->IsEnabledFlags(flag)) {
            pDst[i] = pSrc[i];
        }
        flag <<= 1;
    }
}

// 0x00743274 | nintendogs:bytes [tier A]
bool nw::anim::AnimBlendOp::BlendFloatVector(AnimResult* pDst, const AnimResult* pSrc, f32 weight, int count) const
{
    f32* pDstValue = static_cast<f32*>(pDst->GetValueBuffer());
    const f32* pSrcValue = static_cast<const f32*>(pSrc->GetValueBuffer());
    u32 flag = 1;
    for (int i = 0; i < count; ++i) {
        if (pSrc->IsEnabledFlags(flag)) {
            const f32 value = pSrcValue[i] * weight;
            if (pDst->IsEnabledFlags(flag)) {
                pDstValue[i] += value;
            } else {
                pDstValue[i] = value;
                pDst->m_EnabledFlags |= flag;
            }
        }
        flag <<= 1;
    }
    return true;
}

// 0x007432F8 | nintendogs:bytes [tier B]
bool nw::anim::AnimBlendOp::OverrideFloatVector(AnimResult* pDst, const AnimResult* pSrc, int count, u32 allFlags) const
{
    u32 flags = pDst->m_EnabledFlags;
    const u32 srcFlags = pSrc->m_EnabledFlags;
    f32* pDstValue = static_cast<f32*>(pDst->GetValueBuffer());
    const f32* pSrcValue = static_cast<const f32*>(pSrc->GetValueBuffer());
    u32 flag = 1;
    for (int i = 0; i < count; ++i) {
        if ((srcFlags & ~flags) & flag) {
            pDstValue[i] = pSrcValue[i];
            flags |= flag;
        }
        flag <<= 1;
    }
    pDst->m_EnabledFlags = flags;
    return (allFlags & ~flags) == 0;
}

// 0x007433C0 | nintendogs:bytes [tier A]
void nw::anim::AnimBlendOp::ConvertFloatVectorToAnimResult(AnimResult* pResult, const void* pSource, int count) const
{
    f32* pDst = static_cast<f32*>(pResult->GetValueBuffer());
    const f32* pSrc = static_cast<const f32*>(pSource);
    pResult->m_EnabledFlags &= ~0xF;
    u32 flag = 1;
    for (int i = 0; i < count; ++i) {
        pResult->m_EnabledFlags |= flag;
        pDst[i] = pSrc[i];
        flag <<= 1;
    }
}

} // namespace anim
} // namespace nw
