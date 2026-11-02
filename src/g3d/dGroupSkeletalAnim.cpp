#include "g3d/dGroupSkeletalAnim.h"
#include <new>
#include "g3d/dGroupTransformAnim.h"
#include "g3d/dSkeletalModel.h"
#include "nw/gfx/gfx_SkeletalModel.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/gfx/res/gfx_ResSkeleton.h"
#include "nw/ut/ut_ResDicPatricia.h"
#include "sead/seadHeap.h"
#include "sead/seadSafeStringBase.h"
#include "ssys/ma/dAllocator.h"

namespace g3d {
// 0x004EF474 (name is ours)
void g3d::GroupSkeletalAnim::ChangeAnim(s32 animIdx, nw::anim::res::ResAnim anim, u8 type, f32 duration)
{
    m_Anims[animIdx].ChangeAnim(anim, type, duration);
    if (animIdx == 0) {
        return;
    }
    if (duration == 0.0f) {
        m_pGroupAnim->EndMorph(animIdx);
    }
    m_pGroupAnim->EnableEvaluator(animIdx);
}

// 0x004EF524 (name is ours)
void g3d::GroupSkeletalAnim::AssignBones(s32 animIdx, const char* boneName)
{
    const nw::gfx::res::ResSkeletalModelData* resModel = nw::gfx::res::DynamicCastData<nw::gfx::res::ResSkeletalModelData>(
        m_pModel->GetSkeletalModel()->GetResSceneObject().ptr(), nw::gfx::res::ResSkeletalModelData::TYPE);
    const nw::gfx::res::ResSkeletonData* skeleton = static_cast<const nw::gfx::res::ResSkeletonData*>(resModel->skeleton.ToPtr());
    nw::ut::ResDicPatricia bones = {skeleton->boneDic.To<nw::ut::ResDicPatricia::Data>()};
    AssignBones(animIdx, static_cast<const nw::gfx::res::ResBoneData*>(bones.GetData(boneName)), false);
}

// 0x004EF5E8 (name is ours)
void g3d::GroupSkeletalAnim::AssignBones(s32 animIdx, const nw::gfx::res::ResBoneData* bone, bool includeSiblings)
{
    if (bone == NULL) {
        return;
    }
    m_pGroupAnim->m_pMemberEvaluators[bone->jointId] = animIdx;
    AssignBones(animIdx, static_cast<const nw::gfx::res::ResBoneData*>(bone->firstChild.ToPtr()), true);
    if (includeSiblings) {
        AssignBones(animIdx, static_cast<const nw::gfx::res::ResBoneData*>(bone->nextSibling.ToPtr()), true);
    }
}

// 0x004EF668 (name after the heap name)
bool g3d::GroupSkeletalAnim::Build(SkeletalModel* model, nw::anim::res::ResAnim anim, ssys::ma::Allocator* allocator,
                                   s32 maxMemberAnims)
{
    if (!anim.IsValid()) {
        return false;
    }
    m_pModel = model;
    for (s32 i = 0; i < ANIM_COUNT; i++) {
        if (!m_Anims[i].Build(model, anim, allocator, maxMemberAnims)) {
            Finalize();
            return false;
        }
    }
    bool result = m_HeapAllocator.CreateExpHeap(allocator->m_pHeap->getMaxAllocatableSize(4), allocator->m_pHeap,
                                                sead::SafeStringBase<char>("GroupSkeletalAnim::build"),
                                                sead::Heap::HEAP_DIRECTION_FORWARD, false);
    s32 boneCount = model->GetBoneCount();
    void* memory = m_HeapAllocator.Alloc(sizeof(GroupTransformAnim), 4);
    GroupTransformAnim* groupAnim = NULL;
    if (memory != NULL) {
        groupAnim = new (memory) GroupTransformAnim(&m_HeapAllocator, boneCount);
    }
    m_pGroupAnim = groupAnim;
    for (s32 i = 0; i < ANIM_COUNT; i++) {
        m_pGroupAnim->SetEvaluator(i, m_Anims[i].GetMorphEvaluator());
    }
    m_HeapAllocator.Adjust();
    return result;
}

// 0x004EFA2C (name is ours)
void g3d::GroupSkeletalAnim::Finalize()
{
    for (s32 i = 0; i < ANIM_COUNT; i++) {
        m_Anims[i].Finalize();
    }
    if (m_pGroupAnim != NULL) {
        m_pGroupAnim->Clear();
    }
    nw::gfx::SafeDestroy(m_pGroupAnim);
    m_HeapAllocator.DestroyHeap();
}

// 0x004EFB14 (name is ours)
g3d::GroupSkeletalAnim::GroupSkeletalAnim() : m_pGroupAnim(NULL), m_pModel(NULL)
{
}

// 0x004EFB94 slot 0x00
// 0x004EFB58 (deleting dtor)
g3d::GroupSkeletalAnim::~GroupSkeletalAnim()
{
    // (only the members are destroyed)
}

// 0x004F07AC (name is ours)
void g3d::GroupSkeletalAnim::StartMorph(s32 animIdx, f32 duration)
{
    m_pGroupAnim->StartMorph(animIdx, CalcRatio::TYPE_SMOOTH, duration);
}

} // namespace g3d
