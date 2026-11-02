#pragma once

#include "decomp.h"
#include "g3d/dMorphSkeletalAnim.h"
#include "nw/anim/res/anim_ResAnim.h"
#include "ssys/ma/dHeapAllocator.h"

namespace nw {
namespace gfx {
namespace res {
struct ResBoneData;
} // namespace res
} // namespace gfx
} // namespace nw

namespace ssys {
namespace ma {
class Allocator;
} // namespace ma
} // namespace ssys

namespace g3d {
class GroupTransformAnim;
class SkeletalModel;

// four morphing skeletal animations of one model: each bone takes its pose from one of them (a GroupTransformAnim
// bound to the skeleton; the heap name "GroupSkeletalAnim::build" gives the name of Build; other names are ours)
// RTTI N3g3d17GroupSkeletalAnimE @ 0x008D0D04
// vtable 0x00903A88 (vptr 0x00903A90), offset_to_top 0, 2 entries
class GroupSkeletalAnim
{
public:
    // the animations
    static const s32 ANIM_COUNT = 4;

    GroupSkeletalAnim(); // 0x004EFB14
    virtual ~GroupSkeletalAnim(); // 0x004EFB94 slot 0x00
    // 0x004EFB58 (deleting dtor)

    // the four animations of anim (see SkeletalAnim::Build) and in a heap of their own the group object for the
    // bones of the model (all bones take animation 0); false if an animation could not be made, else the result of
    // making the heap
    bool Build(SkeletalModel* model, nw::anim::res::ResAnim anim, ssys::ma::Allocator* allocator, s32 maxMemberAnims); // 0x004EF668
    // destroys the animations, the group object and the heap
    void Finalize(); // 0x004EFA2C
    // another animation for animation animIdx, morphing from the current pose (duration 0: at once)
    void ChangeAnim(s32 animIdx, nw::anim::res::ResAnim anim, u8 type, f32 duration); // 0x004EF474
    // the bone of that name and the bones below it take their pose from animation animIdx
    void AssignBones(s32 animIdx, const char* boneName); // 0x004EF524
    // the bone and the bones below it (with includeSiblings also its next siblings and theirs) take their pose from
    // animation animIdx
    void AssignBones(s32 animIdx, const nw::gfx::res::ResBoneData* bone, bool includeSiblings); // 0x004EF5E8
    // the bones of animation animIdx morph into it (smoothly; duration 0 or less: at once)
    void StartMorph(s32 animIdx, f32 duration); // 0x004F07AC

    GroupTransformAnim* m_pGroupAnim;            // 0x04 bound to the skeleton
    SkeletalModel* m_pModel;                     // 0x08
    ssys::ma::HeapAllocator m_HeapAllocator;     // 0x0C the heap of the group object
    MorphSkeletalAnim m_Anims[ANIM_COUNT];       // 0x14
};
ASSERT_SIZE(GroupSkeletalAnim, 0x74);
} // namespace g3d
