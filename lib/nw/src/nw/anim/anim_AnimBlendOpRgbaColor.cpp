#include "nw/anim/anim_AnimBlendOpRgbaColor.h"

namespace nw {
namespace anim {
// 0x004D5268 slot 0x00 (name is ours)
// 0x004D5264 (deleting dtor)
nw::anim::AnimBlendOpRgbaColor::~AnimBlendOpRgbaColor()
{
    // empty (in the original too)
}

// 0x007436E4 slot 0x08 | nintendogs:bytes [tier A]
bool nw::anim::AnimBlendOpRgbaColor::Blend(AnimResult* pDst, f32* pDstWeights, const AnimResult* pSrc, const f32* pSrcWeights) const
{
    f32* pDstValue = static_cast<f32*>(pDst->GetValueBuffer());
    const f32* pSrcValue = static_cast<const f32*>(pSrc->GetValueBuffer());
    u32 flag = 1;
    for (int i = 0; i < 4; ++i) {
        if (pSrc->IsEnabledFlags(flag)) {
            const f32 value = *pSrcWeights * pSrcValue[i];
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

// 0x00743768 slot 0x10 | nintendogs:bytes [tier A]
bool nw::anim::AnimBlendOpRgbaColor::Override(AnimResult* pDst, const AnimResult* pSrc) const
{
    return OverrideFloatVector(pDst, pSrc, 4, 0xF);
}

// 0x0074368C slot 0x14 | nintendogs:bytes [tier A]
void nw::anim::AnimBlendOpRgbaColor::Apply(void* pTarget, const AnimResult* pResult) const
{
    ApplyFloatVector(pTarget, pResult, 4);
}

// 0x00743630 slot 0x18 | nintendogs:bytes [tier A]
void nw::anim::AnimBlendOpRgbaColor::ConvertToAnimResult(AnimResult* pResult, const void* pSource) const
{
    ConvertFloatVectorToAnimResult(pResult, pSource, 4);
}

} // namespace anim
} // namespace nw
