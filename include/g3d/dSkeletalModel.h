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

    // (inline; copies for the game) the world matrix / the position of a bone without the parent of the model (false
    // for a negative index)
    DECOMP_EMIT_COPY bool GetBoneWorldMatrix(nn::math::MTX34* out, s32 boneIdx) const; // 0x007466FC
    DECOMP_EMIT_COPY bool GetBoneWorldPosition(nn::math::VEC3* out, s32 boneIdx) const; // 0x00746890
    // the index of the bone of the name (-1: none)
    DECOMP_EMIT_COPY s32 GetBoneIndex(const char* name) const; // 0x007469FC
};
ASSERT_SIZE(SkeletalModel, 0xC);
} // namespace g3d

#include "g3d/g3d_System.h"
#include "nw/gfx/gfx_SkeletalModel.h"
#include "nw/gfx/res/gfx_ResModel.h"

inline bool g3d::SkeletalModel::GetBoneWorldMatrix(nn::math::MTX34* out, s32 boneIdx) const
{
    nw::gfx::CalculatedTransform world;
    if (boneIdx < 0) {
        return false;
    }
    const nw::gfx::Skeleton* skeleton = nw::gfx::DynamicCast<nw::gfx::SkeletalModel>(m_pNode)->GetSkeleton();
    const nw::gfx::CalculatedTransform* transform = &skeleton->GetWorldTransforms()->begin()[boneIdx];
    if (transform == NULL) {
        return false;
    }
    UpdateWorldTransform(&world, *transform, GetWorldTransform());
    *out = world.m_TransformMatrix;
    return true;
}

inline bool g3d::SkeletalModel::GetBoneWorldPosition(nn::math::VEC3* out, s32 boneIdx) const
{
    nw::gfx::CalculatedTransform world;
    if (boneIdx < 0) {
        return false;
    }
    const nw::gfx::Skeleton* skeleton = nw::gfx::DynamicCast<nw::gfx::SkeletalModel>(m_pNode)->GetSkeleton();
    const nw::gfx::CalculatedTransform* transform = &skeleton->GetWorldTransforms()->begin()[boneIdx];
    if (transform == NULL) {
        return false;
    }
    UpdateWorldTransform(&world, *transform, GetWorldTransform());
    out->x = world.m_TransformMatrix.m[0][3];
    out->y = world.m_TransformMatrix.m[1][3];
    out->z = world.m_TransformMatrix.m[2][3];
    return true;
}

inline s32 g3d::SkeletalModel::GetBoneIndex(const char* name) const
{
    nw::gfx::SkeletalModel* model = nw::gfx::DynamicCast<nw::gfx::SkeletalModel>(m_pNode);
    const nw::gfx::res::ResSkeletalModelData* resModel = nw::gfx::res::DynamicCastData<nw::gfx::res::ResSkeletalModelData>(
        model->GetResSceneObject().ptr(), nw::gfx::res::ResSkeletalModelData::TYPE);
    const nw::gfx::res::ResSkeletonData* skeleton = resModel->skeleton.To<nw::gfx::res::ResSkeletonData>();
    nw::ut::ResDicPatricia dic = {skeleton->boneDic.To<nw::ut::ResDicPatricia::Data>()};
    return dic.GetIndex(name);
}
