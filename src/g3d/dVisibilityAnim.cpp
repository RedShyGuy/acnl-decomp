#include "g3d/dVisibilityAnim.h"
#include "g3d/dModel.h"
#include "nw/gfx/gfx_AnimEvaluator.h"
#include "nw/gfx/gfx_AnimGroup.h"
#include "sead/seadHeap.h"
#include "sead/seadSafeStringBase.h"
#include "ssys/ma/dAllocator.h"

namespace g3d {
namespace {
// the member animations of a visibility animation
const s32 MAX_MEMBER_ANIMS = 64;
} // namespace

// 0x004EECDC slot 0x1C (name is ours)
bool g3d::VisibilityAnim::Bind()
{
    return !(m_pAnimObject->TryBind(m_pModel->GetVisibilityAnimGroup()) & nw::gfx::RESULT_FAILURE_MASK);
}

// 0x004EED10 (name after the heap name)
bool g3d::VisibilityAnim::Build(Model* model, nw::anim::res::ResAnim anim, ssys::ma::Allocator* allocator)
{
    if (!anim.IsValid()) {
        return false;
    }
    m_pModel = model;
    m_HeapAllocator.CreateExpHeap(allocator->m_pHeap->getMaxAllocatableSize(4), allocator->m_pHeap,
                                  sead::SafeStringBase<char>("VisibilityAnim::build"), sead::Heap::HEAP_DIRECTION_FORWARD,
                                  false);
    bool result = CreateAnimObject(anim, 0);
    if (!result) {
        Finalize();
        return false;
    }
    InitPlayMode(anim);
    m_HeapAllocator.Adjust();
    Bind();
    return result;
}

// 0x004EEE00 slot 0x18 (name is ours)
bool g3d::VisibilityAnim::CreateAnimObject(nw::anim::res::ResAnim anim, s32 /*maxMemberAnims*/)
{
    nw::gfx::AnimEvaluator::Builder builder;
    builder.AnimData(anim).MaxMembers(m_pModel->GetVisibilityAnimGroup()->GetMemberCount()).MaxMemberAnims(MAX_MEMBER_ANIMS);
    m_pAnimObject = builder.Create(&m_HeapAllocator);
    return m_pAnimObject != NULL;
}

// 0x004EEEA0 slot 0x14 (name is ours)
void g3d::VisibilityAnim::Finalize()
{
    m_pModel = NULL;
    nw::gfx::SafeDestroy(m_pAnimObject);
    m_HeapAllocator.DestroyHeap();
}

// 0x004EEEF0 (name is ours)
g3d::VisibilityAnim::VisibilityAnim() : m_pModel(NULL)
{
}

// 0x004EEF48 slot 0x00
// 0x004EEF28 (deleting dtor)
g3d::VisibilityAnim::~VisibilityAnim()
{
    // (only the base is destroyed)
}

} // namespace g3d
