#include "nw/gfx/gfx_SkeletalModel.h"

#include <new>

#include "nw/gfx/gfx_AnimBinding.h"
#include "nw/gfx/gfx_AnimGroup.h"
#include "nw/gfx/gfx_ISceneVisitor.h"

namespace nw {
namespace gfx {
// 0x009760F0
const TypeInfo nw::gfx::SkeletalModel::s_TypeInfo(&Model::s_TypeInfo);

// sets s_TypeInfo
// 0x0079C244 (static initializer)

// 0x00497808 slot 0x24 | nintendogs:bytes
u32 nw::gfx::SkeletalModel::Initialize(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    result |= Model::Initialize(pAllocator);
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    // a shared skeleton is animated by its model
    if (!m_IsSharingSkeleton) {
        result |= CreateSkeletalAnimGroup(pAllocator);
        if (result & RESULT_FAILURE_MASK) {
            return result;
        }
    }
    return result;
}

// 0x0049785C | nintendogs:bytes-fuzzy [tier A]
u32 nw::gfx::SkeletalModel::CreateSkeletalAnimGroup(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    AnimBinding* pBinding = m_AnimBinding.Get();
    if (pBinding == nullptr) {
        return result;
    }
    const res::ResModelData* pResource = GetResModel().ptr();
    const res::ResSkeletonData* pResSkeleton =
        static_cast<const res::ResSkeletalModelData*>(pResource)->skeleton.To<res::ResSkeletonData>();
    m_Skeleton->GetLocalTransforms();
    const res::ResSkeleton* pOriginalBones = m_Skeleton->GetOriginalBones();

    // the group of the bones
    res::ResModel resource = GetResModel();
    const int groupCount = resource.GetAnimGroupCount();
    int groupIndex;
    anim::res::ResAnimGroup resAnimGroup;
    for (groupIndex = 0; groupIndex < groupCount; ++groupIndex) {
        resAnimGroup = resource.GetAnimGroup(groupIndex);
        if ((resAnimGroup.GetFlags() & anim::res::ResAnimGroupData::FLAG_DIRECT_RESULT) &&
            resAnimGroup.GetTargetType() == anim::res::ResAnimGroupData::TARGET_TYPE_SKELETON) {
            break;
        }
    }
    if (groupIndex >= groupCount) {
        return result;
    }

    void* pMemory = pAllocator->Alloc(sizeof(AnimGroup), 4);
    AnimGroup* pAnimGroup = nullptr;
    if (pMemory != nullptr) {
        pAnimGroup = new (pMemory) AnimGroup(resAnimGroup, this, pAllocator);
        if (pAnimGroup->Initialize(true) & RESULT_FAILURE_MASK) {
            if (pAnimGroup != nullptr) {
                pAnimGroup->~AnimGroup();
                pAnimGroup->GetAllocator()->Free(pAnimGroup);
            }
            pAnimGroup = nullptr;
        }
    }
    if (pAnimGroup == nullptr) {
        result |= RESULT_FAILURE_MASK;
    }
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }

    // per member: the bone of the path, its original transform, and the transform it animates
    const int memberCount = pAnimGroup->GetMemberCount();
    for (int i = 0; i < memberCount; ++i) {
        anim::res::ResAnimGroupMember member = pAnimGroup->GetResAnimGroupMember(i);
        ut::ResDicPatricia bones = {pResSkeleton->boneDic.To<ut::ResDicPatricia::Data>()};
        const int boneIndex = static_cast<const res::ResBoneData*>(bones.GetData(member.GetPath()))->jointId;
        pAnimGroup->m_TargetObjects[i] = reinterpret_cast<void*>(boneIndex);
        ut::ResDicPatricia originalBones = {pOriginalBones->ptr()->boneDic.To<ut::ResDicPatricia::Data>()};
        const res::ResBoneData* pOriginalBone = static_cast<const res::ResBoneData*>(originalBones.GetDataByIndex(boneIndex));
        pAnimGroup->m_OriginalValues[i] = const_cast<nn::math::VEC3*>(&pOriginalBone->transform.scale);

        void* pTarget = nullptr;
        if (member.GetMemberType() == anim::res::ResAnimGroupMemberData::MEMBER_TYPE_TRANSFORM) {
            const char* boneName = static_cast<const anim::res::ResBoneMemberData*>(member.ptr())->boneName.To<char>();
            const res::ResSkeletalModelData* pModel =
                res::DynamicCastData<res::ResSkeletalModelData>(GetResModel().ptr(), res::ResSkeletalModelData::TYPE);
            ut::ResDicPatricia modelBones = {pModel->skeleton.To<res::ResSkeletonData>()->boneDic.To<ut::ResDicPatricia::Data>()};
            const int targetIndex = static_cast<const res::ResBoneData*>(modelBones.GetData(boneName))->jointId;
            pTarget = &(*m_Skeleton->GetLocalTransforms())[targetIndex];
        }
        pAnimGroup->m_Targets[i] = pTarget;
    }

    pBinding->m_AnimGroups[groupIndex] = pAnimGroup;
    m_pSkeletalAnimGroup = pAnimGroup;
    m_SkeletalAnimGroupIndex = groupIndex;
    SetupAnimGroup(pAnimGroup, m_IsWorldMatrixAnimation);
    return result;
}

// 0x00497C08 | nintendogs:bytes
void nw::gfx::SkeletalModel::Accept(ISceneVisitor* pVisitor)
{
    pVisitor->VisitSkeletalModel(this);
    for (SceneNode** ppChild = m_Children.begin(); ppChild != m_Children.end(); ++ppChild) {
        (*ppChild)->Accept(pVisitor);
    }
}

// 0x00498360 slot 0x00 | nintendogs:bytes
// 0x00498354 (deleting dtor)
nw::gfx::SkeletalModel::~SkeletalModel()
{
    if (m_pSkeletalAnimGroup != nullptr) {
        m_pSkeletalAnimGroup->~AnimGroup();
        m_pSkeletalAnimGroup->GetAllocator()->Free(m_pSkeletalAnimGroup);
        m_pSkeletalAnimGroup = nullptr;
    }
}

// 0x00738A3C | nintendogs:bytes [tier A]
void nw::gfx::SkeletalModel::SetupAnimGroup(AnimGroup* pAnimGroup, bool isWorldMatrixAnimation) const
{
    const int memberCount = pAnimGroup->GetMemberCount();
    for (int i = 0; i < memberCount; ++i) {
        anim::res::ResAnimGroupMember member = pAnimGroup->GetResAnimGroupMember(i);
        const res::ResSkeletonData* pResSkeleton = reinterpret_cast<const res::ResSkeletonData*>(m_Skeleton->GetResSceneObject().ptr());
        ut::ResDicPatricia bones = {pResSkeleton->boneDic.To<ut::ResDicPatricia::Data>()};
        const int boneIndex = static_cast<const res::ResBoneData*>(bones.GetData(member.GetPath()))->jointId;
        if (isWorldMatrixAnimation) {
            pAnimGroup->m_TargetValues[i] = &(*m_Skeleton->GetWorldMatrices())[boneIndex];
        } else {
            pAnimGroup->m_TargetValues[i] = &(*m_Skeleton->GetLocalTransforms())[boneIndex];
        }
    }
    m_pSkeletalAnimGroup->m_Unknown0x64 = isWorldMatrixAnimation;
}

} // namespace gfx
} // namespace nw
