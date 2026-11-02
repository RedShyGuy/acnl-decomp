#include "g3d/dSkeletalModel.h"
#include <cstring>
#include "g3d/dBaseAnim.h"
#include "g3d/dGroupSkeletalAnim.h"
#include "g3d/dGroupTransformAnim.h"
#include "nw/gfx/gfx_AnimBinding.h"
#include "g3d/g3d_System.h"
#include "nw/gfx/gfx_SkeletalModel.h"
#include "nw/gfx/gfx_Skeleton.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/gfx/res/gfx_ResSkeleton.h"

namespace g3d {
namespace {
// the bytes of a world transform CopyWorldPose copies (the size of a scale, a rotation and a translation)
const u32 WORLD_TRANSFORM_COPY_SIZE = 36;

// (inline) the animation object of the skeletal animation group of the model (nothing without the group; name is
// ours)
inline void SetSkeletalAnimObject(nw::gfx::SkeletalModel* model, nw::gfx::AnimObject* animObject)
{
    if (model->m_pSkeletalAnimGroup == NULL) {
        return;
    }
    nw::gfx::AnimBinding* binding = model->m_AnimBinding.Get();
    const s32 groupIdx = model->m_SkeletalAnimGroupIndex;
    if (groupIdx < 0 || groupIdx >= static_cast<s32>(binding->m_AnimGroups.size())) {
        return;
    }
    if (binding->m_AnimObjectCountPerGroup <= 0) {
        return;
    }
    binding->m_AnimObjects[groupIdx * binding->m_AnimObjectCountPerGroup] = animObject;
}
} // namespace

// 0x004EDBF8 (name is ours)
nw::gfx::SkeletalModel* g3d::SkeletalModel::GetSkeletalModel() const
{
    return nw::gfx::DynamicCast<nw::gfx::SkeletalModel>(m_pNode);
}

// 0x004EDAFC (name is ours)
s32 g3d::SkeletalModel::GetBoneCount() const
{
    nw::gfx::SkeletalModel* model = nw::gfx::DynamicCast<nw::gfx::SkeletalModel>(m_pNode);
    const nw::gfx::res::ResSkeletalModelData* resModel = nw::gfx::res::DynamicCastData<nw::gfx::res::ResSkeletalModelData>(
        model->GetResSceneObject().ptr(), nw::gfx::res::ResSkeletalModelData::TYPE);
    return nw::gfx::res::ResSkeleton(resModel->skeleton.ToPtr()).GetBoneCount();
}

// 0x004EDB74 (name is ours)
void g3d::SkeletalModel::SetBoneScale(s32 boneIdx, const nn::math::VEC3& scale)
{
    nw::gfx::SkeletalModel* model = nw::gfx::DynamicCast<nw::gfx::SkeletalModel>(m_pNode);
    nw::gfx::CalculatedTransform& transform = (*model->m_Skeleton.Get()->GetLocalTransforms())[boneIdx];
    transform.m_Scale = scale;
    transform.DisableFlags(nw::gfx::CalculatedTransform::FLAG_SCALE_UNSET | nw::gfx::CalculatedTransform::FLAG_IS_SCALE_ONE);
    transform.EnableFlags(nw::gfx::CalculatedTransform::FLAG_IS_DIRTY);
}

// 0x004EDC44 (name is ours)
void g3d::SkeletalModel::CopyWorldPose(const SkeletalModel& other)
{
    nw::gfx::Skeleton::TransformArray* transforms = GetSkeletalModel()->GetSkeleton()->GetWorldTransforms();
    const nw::gfx::Skeleton::TransformArray* otherTransforms =
        static_cast<const nw::gfx::Skeleton*>(other.GetSkeletalModel()->GetSkeleton())->GetWorldTransforms();
    if (transforms->size() != otherTransforms->size()) {
        return;
    }
    nw::gfx::Skeleton::MatrixArray* matrices = GetSkeletalModel()->GetSkeleton()->GetWorldMatrices();
    const nw::gfx::Skeleton::MatrixArray* otherMatrices =
        static_cast<const nw::gfx::Skeleton*>(other.GetSkeletalModel()->GetSkeleton())->GetWorldMatrices();
    // (only 36 bytes of each transform: so in the original)
    std::memcpy(static_cast<void*>(transforms->begin()), otherTransforms->begin(), transforms->size() * WORLD_TRANSFORM_COPY_SIZE);
    std::memcpy(static_cast<void*>(matrices->begin()), otherMatrices->begin(), transforms->size() * sizeof(nn::math::MTX34));
}

// 0x004EDE00 (name is ours)
void g3d::SkeletalModel::SetBoneRotate(s32 boneIdx, nn::math::VEC3 rotate)
{
    nw::gfx::CalculatedTransform& transform = (*GetSkeletalModel()->GetSkeleton()->GetLocalTransforms())[boneIdx];
    transform.SetRotateXYZ(rotate.x, rotate.y, rotate.z);
    transform.DisableFlags(nw::gfx::CalculatedTransform::FLAG_ROTATE_UNSET | nw::gfx::CalculatedTransform::FLAG_IS_ROTATE_TRANSLATE_ZERO |
                           nw::gfx::CalculatedTransform::FLAG_IS_ROTATE_ZERO);
}

// 0x004EE08C (name is ours)
nw::gfx::AnimGroup* g3d::SkeletalModel::GetSkeletalAnimGroup() const
{
    return nw::gfx::DynamicCast<nw::gfx::SkeletalModel>(m_pNode)->m_pSkeletalAnimGroup;
}

// 0x004EE0D8 (name is ours)
void g3d::SkeletalModel::SetSkeletalAnim(const BaseAnim* anim)
{
    SetSkeletalAnimObject(GetSkeletalModel(), anim->m_pAnimObject);
}

// 0x004EE174 (name is ours)
void g3d::SkeletalModel::SetSkeletalAnim(const GroupSkeletalAnim* anim)
{
    SetSkeletalAnimObject(GetSkeletalModel(), anim->m_pGroupAnim);
}

// 0x004EE210 (name is ours)
void g3d::SkeletalModel::ClearSkeletalAnim()
{
    SetSkeletalAnimObject(GetSkeletalModel(), NULL);
}

// 0x004EE2A8 (name is ours)
void g3d::SkeletalModel::InvalidateWorldScales()
{
    if (nw::gfx::DynamicCast<nw::gfx::SkeletalModel>(m_pNode) == NULL) {
        return;
    }
    nw::gfx::Skeleton::TransformArray* transforms = GetSkeletalModel()->GetSkeleton()->GetWorldTransforms();
    for (s32 i = 0; i < static_cast<s32>(transforms->size()); i++) {
        (*transforms)[i].DisableFlags(nw::gfx::CalculatedTransform::FLAG_IS_SCALE_ONE);
    }
}

// 0x004EE3A8 slot 0x08 | libgarden
void g3d::SkeletalModel::Update()
{
    UpdateWorldMatrix(m_pNode);
    UpdateSkeleton(m_pNode);
}

// 0x004EE3C4 | libgarden
g3d::SkeletalModel::SkeletalModel()
{
    // (only the base is constructed)
}

// 0x004EE40C slot 0x00 | libgarden
// 0x004EE3E4 (deleting dtor)
g3d::SkeletalModel::~SkeletalModel()
{
    // (only the base is destroyed)
}

} // namespace g3d
