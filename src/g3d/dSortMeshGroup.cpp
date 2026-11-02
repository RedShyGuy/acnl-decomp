#include "g3d/dSortMeshGroup.h"
#include "g3d/g3d_System.h"
#include "graphic/dScreenCamera.h"
#include "nw/gfx/gfx_Material.h"
#include "nw/gfx/gfx_MeshRenderer.h"
#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/gfx_SkeletalModel.h"
#include "nw/gfx/gfx_Skeleton.h"
#include "nw/gfx/res/gfx_ResMaterial.h"
#include "nw/gfx/res/gfx_ResSkeleton.h"
#include "sead/seadHeap.h"
#include "sead/seadSafeStringBase.h"

namespace g3d {
namespace {
// the draw mode of the top screen (ssys::ma::lyt::Layout)
const u32 DRAW_MODE_TOP_LEFT = 0x400;
// the heap of the node of a group
const u32 NODE_HEAP_SIZE = 800;

// a buffer of size bytes (rounded up to words) from the heap
inline void* AllocBuffer(sead::Heap* heap, u32 size)
{
    return heap->tryAlloc((size + 3) & ~3, 4);
}
} // namespace

// 0x009CAF00
SortMeshGroup s_SortMeshGroups[SORT_MESH_GROUP_COUNT];

// constructs s_SortMeshGroups (and registers their destruction, a helper of the compiler at 0x4EF450)
// 0x0079BE60 (static initializer)

// 0x004EE410 (name is ours)
void g3d::SortMeshGroup::Clear()
{
    m_IsInScene = false;
    m_OpaqueEntries.Clear();
    m_TranslucentEntries.Clear();
    m_Models.Clear();
}

// 0x004EE45C (name is ours)
bool g3d::SortMeshGroup::Add(nw::gfx::Model* model)
{
    nw::ut::internal::ResArray<nw::gfx::res::ResMesh, nw::ut::internal::ResArrayClassTraits> meshes = model->GetResMeshes();
    m_Models.PushBack(model);
    for (const nw::ut::Offset* offset = meshes.m_pBegin; offset != meshes.m_pEnd; ++offset) {
        nw::gfx::res::ResMesh mesh(offset->ToPtr());
        if (!mesh.ptr()->isVisible) {
            continue;
        }
        const Entry entry = {model, mesh};
        if (static_cast<u8>(model->GetMaterial(mesh.ptr()->materialIndex)
                                ->GetResource(nw::gfx::Material::RESOURCE_SHADING)
                                .ptr()
                                ->translucencyKind) != nw::gfx::res::ResMaterialData::TRANSLUCENCY_KIND_OPAQUE) {
            m_TranslucentEntries.PushBack(entry);
        } else {
            m_OpaqueEntries.PushBack(entry);
        }
    }
    if (!m_IsInScene) {
        m_Func.AddToScene(0);
        m_IsInScene = true;
    }
    return true;
}

// 0x004EE57C (name is ours)
void g3d::SortMeshGroup::DrawOpaqueFunc(nw::gfx::MeshRenderer* renderer, void* userData)
{
    static_cast<SortMeshGroup*>(userData)->DrawOpaque(renderer);
}

// 0x004EE58C (name is ours)
void g3d::SortMeshGroup::DrawOpaque(nw::gfx::MeshRenderer* renderer)
{
    nn::math::MTX34 viewMatrix;
    viewMatrix = graphic::ScreenCamera::s_Instance.GetViewMatrix(DRAW_MODE_TOP_LEFT);
    for (nw::gfx::Model** it = m_Models.m_pBegin; it != m_Models.m_pEnd; ++it) {
        nw::gfx::SkeletalModel* skeletalModel = nw::gfx::DynamicCast<nw::gfx::SkeletalModel>(*it);
        if (skeletalModel != NULL) {
            UpdateSkeletonView(skeletalModel);
            const bool isModelCoordinate =
                (nw::gfx::res::ResSkeleton(skeletalModel->GetSkeleton()->GetResSceneObject().ptr()).ptr()->flags &
                 nw::gfx::res::ResSkeletonData::FLAG_IS_MODEL_COORDINATE) != 0;
            (*it)->UpdateNormalMatrix(viewMatrix, isModelCoordinate);
        } else {
            (*it)->UpdateNormalMatrix(viewMatrix, false);
        }
    }
    Sort(m_OpaqueEntries);
    for (Entry* entry = m_OpaqueEntries.m_pBegin; entry != m_OpaqueEntries.m_pEnd; ++entry) {
        renderer->RenderMesh(entry->mesh, entry->model);
    }
}

// 0x004EE814 (name is ours)
void g3d::SortMeshGroup::DrawTranslucentFunc(nw::gfx::MeshRenderer* renderer, void* userData)
{
    SortMeshGroup* group = static_cast<SortMeshGroup*>(userData);
    group->Sort(group->m_TranslucentEntries);
    for (Entry* entry = group->m_TranslucentEntries.m_pBegin; entry != group->m_TranslucentEntries.m_pEnd; ++entry) {
        renderer->RenderMesh(entry->mesh, entry->model);
    }
}

// 0x004EE8C4 (name is ours)
bool g3d::SortMeshGroup::CompareRenderPriority(const Entry* a, const Entry* b)
{
    return a->mesh.ptr()->renderPriority < b->mesh.ptr()->renderPriority;
}

// 0x004EE8E4 (name is ours)
g3d::SortMeshGroup::SortMeshGroup()
    : m_pCompareFunc(CompareRenderPriority), m_pCompareFunc2(CompareRenderPriority), m_IsInScene(false)
{
    m_OpaqueEntries.Initialize(NULL, 0);
    m_TranslucentEntries.Initialize(NULL, 0);
    m_Models.Initialize(NULL, 0);
}

// 0x004EF31C (name is ours)
void InitializeSortMeshGroups(SortMeshGroup* groups, sead::Heap* heap)
{
    for (s32 i = 0; i < SORT_MESH_GROUP_COUNT; i++) {
        SortMeshGroup* group = &groups[i];
        const u32 opaqueCount = s_SortMeshGroupCounts[i].opaque;
        const u32 translucentCount = s_SortMeshGroupCounts[i].translucent;
        group->m_OpaqueEntries.Initialize(AllocBuffer(heap, opaqueCount * sizeof(SortMeshGroup::Entry)), opaqueCount);
        group->m_TranslucentEntries.Initialize(AllocBuffer(heap, translucentCount * sizeof(SortMeshGroup::Entry)),
                                               translucentCount);
        const u32 modelCount = opaqueCount > translucentCount ? opaqueCount : translucentCount;
        group->m_Models.Initialize(AllocBuffer(heap, modelCount * sizeof(nw::gfx::Model*)), modelCount);
        group->m_HeapAllocator.CreateExpHeap(NODE_HEAP_SIZE, heap, sead::SafeStringBase<char>("SortMeshGroup"),
                                             sead::Heap::HEAP_DIRECTION_FORWARD, false);
        group->m_Func.Initialize(&group->m_HeapAllocator);
        group->m_Func.SetFunctions(SortMeshGroup::DrawOpaqueFunc, SortMeshGroup::DrawTranslucentFunc, group);
        group->m_HeapAllocator.Adjust();
    }
}

// 0x004EF444 (name is ours)
SortMeshGroup* GetSortMeshGroups()
{
    return s_SortMeshGroups;
}

} // namespace g3d
