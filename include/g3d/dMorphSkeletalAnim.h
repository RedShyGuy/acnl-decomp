#pragma once

#include "decomp.h"
#include "g3d/dMorphTransformAnimEvaluator.h"
#include "g3d/dSkeletalAnim.h"

namespace g3d {
// a skeletal animation that morphs from the previous pose into the new animation (names are ours)
// RTTI N3g3d17MorphSkeletalAnimE @ 0x008D0D0C
// vtable 0x00903A98 (vptr 0x00903AA0), offset_to_top 0, 8 entries
class MorphSkeletalAnim : public ::g3d::SkeletalAnim
{
public:
    MorphSkeletalAnim(); // 0x004F003C
    virtual ~MorphSkeletalAnim(); // 0x004F0094 slot 0x00
    // 0x004F0074 (deleting dtor)
    // at once, without a morph
    void ChangeAnim(nw::anim::res::ResAnim anim) override; // 0x004F002C slot 0x10
    // a MorphTransformAnimEvaluator
    bool CreateAnimObject(nw::anim::res::ResAnim anim, s32 maxMemberAnims) override; // 0x004EFF40 slot 0x18

    // the animation, morphing from the current pose over duration frames (CalcRatio::Type)
    void ChangeAnim(nw::anim::res::ResAnim anim, u8 type, f32 duration); // 0x004EFE04

    // (inline; copies for the game)
    DECOMP_EMIT_COPY MorphTransformAnimEvaluator* GetMorphEvaluator() const // 0x00746E8C
    {
        return nw::gfx::DynamicCast<MorphTransformAnimEvaluator>(m_pAnimObject);
    }
    DECOMP_EMIT_COPY bool IsMorphing() const { return GetMorphEvaluator()->m_CalcRatio.IsMorphing(); } // 0x00746ED8
};
ASSERT_SIZE(MorphSkeletalAnim, 0x18);
} // namespace g3d
