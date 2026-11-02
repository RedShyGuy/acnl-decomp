#include "g3d/dSkeletalAnim.h"
#include "g3d/dSkeletalModel.h"
#include "nw/anim/res/anim_ResBakedTransformAnim.h"
#include "nw/anim/res/anim_ResMemberAnim.h"
#include "nw/anim/res/anim_ResTransformAnim.h"
#include "nw/gfx/gfx_TransformAnimEvaluator.h"
#include "sead/seadHeap.h"
#include "sead/seadSafeStringBase.h"
#include "ssys/ma/dAllocator.h"

namespace g3d {
namespace {
// the member animations of a skeletal animation without a number
const s32 DEFAULT_MAX_MEMBER_ANIMS = 64;

// the "not animated" flags of all three components of a transform member animation
const u32 NOT_EXIST_SCALE = 7 << (nw::anim::res::ResTransformAnimData::NOT_EXIST_SHIFT +
                                  nw::anim::res::ResTransformAnimData::COMPONENT_SCALE_X);
const u32 NOT_EXIST_ROTATE = 7 << (nw::anim::res::ResTransformAnimData::NOT_EXIST_SHIFT +
                                   nw::anim::res::ResTransformAnimData::COMPONENT_ROTATE_X);
const u32 NOT_EXIST_TRANSLATE = 7 << (nw::anim::res::ResTransformAnimData::NOT_EXIST_SHIFT +
                                      nw::anim::res::ResTransformAnimData::COMPONENT_TRANSLATE_X);
} // namespace

// 0x004ED284 (name is ours)
void g3d::SkeletalAnim::SetUnusedComponents(nw::anim::res::ResAnim anim)
{
    bool isScaleUnused = true;
    bool isRotateUnused = true;
    bool isTranslateUnused = true;
    for (s32 i = 0; i < anim.GetMemberAnimCount(); i++) {
        const nw::anim::res::ResMemberAnimData* memberAnim = anim.GetMemberAnim(i).ptr();
        switch (memberAnim->primitiveType) {
        case nw::anim::res::PRIMITIVE_TRANSFORM: {
            u32 flags = memberAnim->flags;
            if (isScaleUnused && (flags & NOT_EXIST_SCALE) != NOT_EXIST_SCALE) {
                isScaleUnused = false;
            }
            if (isRotateUnused && (flags & NOT_EXIST_ROTATE) != NOT_EXIST_ROTATE) {
                isRotateUnused = false;
            }
            if (isTranslateUnused && (flags & NOT_EXIST_TRANSLATE) != NOT_EXIST_TRANSLATE) {
                isTranslateUnused = false;
            }
            break;
        }
        case nw::anim::res::PRIMITIVE_BAKED_TRANSFORM: {
            u32 flags = memberAnim->flags;
            if (!(flags & nw::anim::res::ResBakedTransformAnimData::FLAG_NOT_EXIST_SCALE)) {
                isScaleUnused = false;
            }
            if (!(flags & nw::anim::res::ResBakedTransformAnimData::FLAG_NOT_EXIST_ROTATE)) {
                isRotateUnused = false;
            }
            if (!(flags & nw::anim::res::ResBakedTransformAnimData::FLAG_NOT_EXIST_TRANSLATE)) {
                isTranslateUnused = false;
            }
            break;
        }
        case nw::anim::res::PRIMITIVE_FULL_BAKED_TRANSFORM:
            isScaleUnused = false;
            isRotateUnused = false;
            isTranslateUnused = false;
            break;
        default:
            break;
        }
    }
    nw::gfx::TransformAnimEvaluator* evaluator = static_cast<nw::gfx::TransformAnimEvaluator*>(m_pAnimObject);
    evaluator->m_IsScaleDisabled = isScaleUnused;
    evaluator->m_IsRotateDisabled = isRotateUnused;
    evaluator->m_IsTranslateDisabled = isTranslateUnused;
}

// 0x004ED384 slot 0x1C (name is ours)
bool g3d::SkeletalAnim::Bind()
{
    return !(m_pAnimObject->TryBind(m_pModel->GetSkeletalAnimGroup()) & nw::gfx::RESULT_FAILURE_MASK);
}

// 0x004ED3B8 (name after the heap name)
bool g3d::SkeletalAnim::Build(SkeletalModel* model, nw::anim::res::ResAnim anim, ssys::ma::Allocator* allocator,
                              s32 maxMemberAnims)
{
    if (!anim.IsValid()) {
        return false;
    }
    m_pModel = model;
    m_HeapAllocator.CreateExpHeap(allocator->m_pHeap->getMaxAllocatableSize(4), allocator->m_pHeap,
                                  sead::SafeStringBase<char>("SkeletalAnim::build"), sead::Heap::HEAP_DIRECTION_FORWARD, false);
    if (maxMemberAnims == 0) {
        maxMemberAnims = DEFAULT_MAX_MEMBER_ANIMS;
    }
    bool result = CreateAnimObject(anim, maxMemberAnims);
    if (!result) {
        Finalize();
        return false;
    }
    InitPlayMode(anim);
    m_HeapAllocator.Adjust();
    Bind();
    return result;
}

// 0x004ED4A0 slot 0x18 (name is ours)
bool g3d::SkeletalAnim::CreateAnimObject(nw::anim::res::ResAnim anim, s32 maxMemberAnims)
{
    s32 boneCount = m_pModel->GetBoneCount();
    if (maxMemberAnims == 0) {
        maxMemberAnims = anim.GetMemberAnimCount();
    }
    nw::gfx::TransformAnimEvaluator::Builder builder;
    builder.AnimData(anim).MaxMembers(boneCount).MaxMemberAnims(maxMemberAnims);
    m_pAnimObject = builder.Create(&m_HeapAllocator);
    if (m_pAnimObject == NULL) {
        return false;
    }
    SetUnusedComponents(anim);
    return true;
}

// 0x004ED550 slot 0x14 (name is ours)
void g3d::SkeletalAnim::Finalize()
{
    m_pModel = NULL;
    nw::gfx::SafeDestroy(m_pAnimObject);
    m_HeapAllocator.DestroyHeap();
}

// 0x004ED5A0 (name is ours)
g3d::SkeletalAnim::SkeletalAnim() : m_pModel(NULL)
{
}

// 0x004ED5F8 slot 0x00
// 0x004ED5D8 (deleting dtor)
g3d::SkeletalAnim::~SkeletalAnim()
{
    // (only the base is destroyed)
}

} // namespace g3d
