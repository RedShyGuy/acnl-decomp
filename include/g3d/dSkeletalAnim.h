#pragma once

#include "decomp.h"
#include "g3d/dBaseAnim.h"

namespace g3d {
class SkeletalModel;

// a skeletal animation of a skeletal model (names are ours)
// RTTI N3g3d12SkeletalAnimE @ 0x008D0CAC
// vtable 0x009039B8 (vptr 0x009039C0), offset_to_top 0, 8 entries
class SkeletalAnim : public ::g3d::BaseAnim
{
public:
    SkeletalAnim(); // 0x004ED5A0
    virtual ~SkeletalAnim(); // 0x004ED5F8 slot 0x00
    // 0x004ED5D8 (deleting dtor)
    void Finalize() override; // 0x004ED550 slot 0x14
    bool CreateAnimObject(nw::anim::res::ResAnim anim, s32 maxMemberAnims) override; // 0x004ED4A0 slot 0x18
    bool Bind() override; // 0x004ED384 slot 0x1C

    // the evaluator of the animation in a heap of all that the heap of allocator can give, bound to the skeleton of the
    // model (maxMemberAnims 0: 64; heap name "SkeletalAnim::build"); false if it could not be made
    bool Build(SkeletalModel* model, nw::anim::res::ResAnim anim, ssys::ma::Allocator* allocator, s32 maxMemberAnims); // 0x004ED3B8
    // the components (scale, rotation, translation) that no member animation animates are not written by the evaluator
    void SetUnusedComponents(nw::anim::res::ResAnim anim); // 0x004ED284

    SkeletalModel* m_pModel; // 0x14
};
ASSERT_SIZE(SkeletalAnim, 0x18);
} // namespace g3d
