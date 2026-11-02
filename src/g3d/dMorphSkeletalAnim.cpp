#include "g3d/dMorphSkeletalAnim.h"
#include <new>
#include "g3d/dSkeletalModel.h"

namespace g3d {
// 0x004EFE04 (name is ours)
void g3d::MorphSkeletalAnim::ChangeAnim(nw::anim::res::ResAnim anim, u8 type, f32 duration)
{
    nw::gfx::SafeDestroy(m_pAnimObject);
    CreateAnimObject(anim, 0);
    InitPlayMode(anim);
    Bind();
    GetMorphEvaluator()->m_CalcRatio.Start(type, duration);
}

// 0x004EFF40 slot 0x18 (name is ours)
bool g3d::MorphSkeletalAnim::CreateAnimObject(nw::anim::res::ResAnim anim, s32 maxMemberAnims)
{
    s32 boneCount = m_pModel->GetBoneCount();
    if (maxMemberAnims == 0) {
        maxMemberAnims = anim.GetMemberAnimCount();
    }
    void* memory = m_HeapAllocator.Alloc(sizeof(MorphTransformAnimEvaluator), 4);
    MorphTransformAnimEvaluator* evaluator = NULL;
    if (memory != NULL) {
        evaluator = new (memory) MorphTransformAnimEvaluator(&m_HeapAllocator);
        evaluator->Initialize(anim, boneCount, maxMemberAnims, false);
        evaluator->m_AnimType = ANIM_TYPE_MORPH;
    }
    m_pAnimObject = evaluator;
    if (evaluator == NULL) {
        return false;
    }
    SetUnusedComponents(anim);
    return true;
}

// 0x004F002C slot 0x10 (name is ours)
void g3d::MorphSkeletalAnim::ChangeAnim(nw::anim::res::ResAnim anim)
{
    ChangeAnim(anim, CalcRatio::TYPE_SMOOTH, 0.0f);
}

// 0x004F003C (name is ours)
g3d::MorphSkeletalAnim::MorphSkeletalAnim()
{
    // (only the base is constructed)
}

// 0x004F0094 slot 0x00
// 0x004F0074 (deleting dtor)
g3d::MorphSkeletalAnim::~MorphSkeletalAnim()
{
    // (only the base is destroyed)
}

} // namespace g3d
