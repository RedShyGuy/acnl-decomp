#pragma once

#include "decomp.h"
#include "g3d/dModel.h"

namespace nw {
namespace gfx {
class SkeletalModel;
} // namespace gfx
} // namespace nw

namespace g3d {
class BaseAnim;
class GroupSkeletalAnim;

// a model with a skeleton (names after libgarden except the functions of our own)
// RTTI N3g3d13SkeletalModelE @ 0x008D0CC4
// vtable 0x00903A04 (vptr 0x00903A0C), offset_to_top 0, 4 entries
class SkeletalModel : public ::g3d::Model
{
public:
    SkeletalModel(); // 0x004EE3C4 | libgarden
    virtual ~SkeletalModel(); // 0x004EE40C slot 0x00 | libgarden
    // 0x004EE3E4 (deleting dtor)
    // the world matrix of the node and of the bones
    void Update() override; // 0x004EE3A8 slot 0x08 | libgarden

    // the node as a skeletal model (NULL: none; names are ours)
    nw::gfx::SkeletalModel* GetSkeletalModel() const; // 0x004EDBF8
    // the bones of the skeleton of the resource
    s32 GetBoneCount() const; // 0x004EDAFC
    // the scale of a local bone transform
    void SetBoneScale(s32 boneIdx, const nn::math::VEC3& scale); // 0x004EDB74
    nw::gfx::AnimGroup* GetSkeletalAnimGroup() const; // 0x004EE08C
    // the world transforms and matrices of the bones of another model with as many bones
    void CopyWorldPose(const SkeletalModel& other); // 0x004EDC44
    // the rotation of a local bone transform (radians, in the order x, y, z)
    void SetBoneRotate(s32 boneIdx, nn::math::VEC3 rotate); // 0x004EDE00
    // the animation object of the skeletal animation group (nothing without the group)
    void SetSkeletalAnim(const BaseAnim* anim); // 0x004EE0D8
    void SetSkeletalAnim(const GroupSkeletalAnim* anim); // 0x004EE174
    void ClearSkeletalAnim(); // 0x004EE210
    // the world scales of the bones are not known to be one any more
    void InvalidateWorldScales(); // 0x004EE2A8
};
ASSERT_SIZE(SkeletalModel, 0xC);
} // namespace g3d
