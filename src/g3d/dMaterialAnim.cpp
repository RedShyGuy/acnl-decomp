#include "g3d/dMaterialAnim.h"
#include "g3d/dModel.h"
#include "nw/gfx/gfx_AnimEvaluator.h"
#include "nw/gfx/gfx_AnimGroup.h"
#include "sead/seadHeap.h"
#include "sead/seadSafeStringBase.h"
#include "ssys/ma/dAllocator.h"

namespace g3d {
namespace {
// the member animations of a material animation
const s32 MAX_MEMBER_ANIMS = 64;
} // namespace

inline nw::gfx::AnimGroup* g3d::MaterialAnim::GetTargetGroup() const
{
    if (m_pModel != NULL) {
        return m_pModel->GetMaterialAnimGroup();
    }
    if (m_pSharedAnimObject != NULL) {
        return m_pSharedAnimObject->GetAnimGroup();
    }
    return NULL;
}

// 0x004ECF88 slot 0x1C (name is ours)
bool g3d::MaterialAnim::Bind()
{
    return !(m_pAnimObject->TryBind(GetTargetGroup()) & nw::gfx::RESULT_FAILURE_MASK);
}

// 0x004ECFD0 (name after the heap name)
bool g3d::MaterialAnim::Build(Model* model, nw::anim::res::ResAnim anim, ssys::ma::Allocator* allocator, bool allocateCache)
{
    if (!anim.IsValid()) {
        return false;
    }
    m_pModel = model;
    m_pSharedAnimObject = NULL;
    m_AllocateCache = allocateCache;
    m_HeapAllocator.CreateExpHeap(allocator->m_pHeap->getMaxAllocatableSize(4), allocator->m_pHeap,
                                  sead::SafeStringBase<char>("MaterialAnim::build"), sead::Heap::HEAP_DIRECTION_FORWARD, false);
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

// 0x004ED0C8 slot 0x18 (name is ours)
bool g3d::MaterialAnim::CreateAnimObject(nw::anim::res::ResAnim anim, s32 /*maxMemberAnims*/)
{
    nw::gfx::AnimEvaluator::Builder builder;
    builder.AnimData(anim).MaxMembers(GetTargetGroup()->GetMemberCount()).MaxMemberAnims(MAX_MEMBER_ANIMS).AllocateCache(m_AllocateCache);
    m_pAnimObject = builder.Create(&m_HeapAllocator);
    return m_pAnimObject != NULL;
}

// 0x004ED1B4 slot 0x14 (name is ours)
void g3d::MaterialAnim::Finalize()
{
    m_pSharedAnimObject = NULL;
    m_pModel = NULL;
    nw::gfx::SafeDestroy(m_pAnimObject);
    m_HeapAllocator.DestroyHeap();
}

// 0x004ED208 (name is ours)
g3d::MaterialAnim::MaterialAnim() : m_pSharedAnimObject(NULL), m_pModel(NULL), m_AllocateCache(false)
{
}

// 0x004ED268 slot 0x00
// 0x004ED248 (deleting dtor)
g3d::MaterialAnim::~MaterialAnim()
{
    // (only the base is destroyed)
}

} // namespace g3d
