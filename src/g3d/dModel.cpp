#include "g3d/dModel.h"
#include "g3d/dAnimPack.h"
#include "g3d/dBaseAnim.h"
#include "nw/gfx/gfx_AnimBinding.h"
#include "nw/gfx/gfx_AnimEvaluator.h"
#include "nw/gfx/gfx_Material.h"
#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/gfx_SceneBuilder.h"
#include "nw/gfx/gfx_TransformNode.h"
#include "nw/gfx/res/gfx_ResMaterial.h"
#include "nw/gfx/res/gfx_ResMesh.h"
#include "nw/gfx/res/gfx_ResModel.h"

namespace g3d {
namespace {
// (inline) an animation object of a group of the binding of a model (nothing for a missing group or an index outside;
// name is ours)
inline void SetAnimObject(nw::gfx::Model* model, s32 groupIdx, nw::gfx::AnimObject* animObject, s32 objectIdx)
{
    nw::gfx::AnimBinding* binding = model->m_AnimBinding.Get();
    if (groupIdx < 0 || groupIdx >= static_cast<s32>(binding->m_AnimGroups.size())) {
        return;
    }
    if (objectIdx < 0 || objectIdx >= binding->m_AnimObjectCountPerGroup) {
        return;
    }
    binding->m_AnimObjects[groupIdx * binding->m_AnimObjectCountPerGroup + objectIdx] = animObject;
}

// (inline) the absolute pointers of the meshes back into the offsets of the array (name is ours)
inline void StoreMeshes(nw::gfx::Model* model, nw::gfx::res::ResMeshData** meshes, s32 count)
{
    nw::ut::Offset* offsets = const_cast<nw::ut::Offset*>(model->GetResMeshes().m_pBegin);
    for (s32 i = 0; i < count; i++) {
        offsets[i].Set(meshes[i]);
    }
}
} // namespace

// 0x004F1468 (name is ours)
void g3d::Model::EvaluateAnims()
{
    nw::gfx::AnimBinding* binding = m_pNode->m_AnimBinding.Get();
    if (binding == NULL) {
        return;
    }
    binding->Evaluate(0);
    m_pNode->m_AnimBinding.Get()->Evaluate(1);
}

// 0x004F14DC (name is ours)
void g3d::Model::UpdateAnimFrames()
{
    nw::gfx::AnimBinding* binding = m_pNode->m_AnimBinding.Get();
    if (binding == NULL) {
        return;
    }
    for (s32 i = 0; i < static_cast<s32>(binding->m_AnimObjects.size()); i++) {
        nw::gfx::AnimObject* animObject = binding->m_AnimObjects[i];
        if (animObject != NULL) {
            animObject->UpdateFrame();
        }
    }
}

// 0x004F1540 (name is ours)
void g3d::Model::RestoreMeshOrder()
{
    nw::gfx::res::ResMeshData* meshes[MAX_MESHES];
    s32 count = GetMeshes(meshes);
    for (s32 i = 0; i < count; i++) {
        for (s32 j = i + 1; j < count; j++) {
            if (meshes[i] > meshes[j]) {
                nw::gfx::res::ResMeshData* mesh = meshes[i];
                meshes[i] = meshes[j];
                meshes[j] = mesh;
            }
        }
    }
    StoreMeshes(static_cast<nw::gfx::Model*>(m_pNode), meshes, count);
}

// 0x004F1608 (name is ours)
s32 g3d::Model::GetMeshes(nw::gfx::res::ResMeshData** meshes)
{
    nw::gfx::Model* model = static_cast<nw::gfx::Model*>(m_pNode);
    s32 count = model->GetResMeshes().size();
    nw::ut::internal::ResArray<nw::gfx::res::ResMesh, nw::ut::internal::ResArrayClassTraits> resMeshes = model->GetResMeshes();
    for (s32 i = 0; i < count; i++) {
        meshes[i] = const_cast<nw::gfx::res::ResMeshData*>(resMeshes[i].ptr());
    }
    return count;
}

// 0x004F16B0 (name is ours)
nw::gfx::AnimGroup* g3d::Model::GetMaterialAnimGroup() const
{
    return static_cast<nw::gfx::Model*>(m_pNode)->m_pMaterialAnimGroup;
}

// 0x004F16BC (name is ours)
void g3d::Model::SetMaterialAnim(const BaseAnim* anim, s32 objectIdx)
{
    nw::gfx::Model* model = static_cast<nw::gfx::Model*>(m_pNode);
    nw::gfx::AnimObject* animObject = anim->m_pAnimObject;
    if (model->m_pMaterialAnimGroup == NULL) {
        return;
    }
    SetAnimObject(model, model->m_MaterialAnimGroupIndex, animObject, objectIdx);
}

// 0x004F1718 (name is ours)
void g3d::Model::SetMaterialAnim(const AnimPack* animPack)
{
    nw::gfx::Model* model = static_cast<nw::gfx::Model*>(m_pNode);
    nw::gfx::AnimObject* animObject = animPack->m_pAnimObject;
    if (model->m_pMaterialAnimGroup == NULL) {
        return;
    }
    SetAnimObject(model, model->m_MaterialAnimGroupIndex, animObject, 0);
}

// 0x004F1774 (name is ours)
nw::gfx::AnimGroup* g3d::Model::GetVisibilityAnimGroup() const
{
    return static_cast<nw::gfx::Model*>(m_pNode)->m_pVisibilityAnimGroup;
}

// 0x004F178C (name is ours)
void g3d::Model::ClearMaterialAnim(s32 objectIdx)
{
    nw::gfx::Model* model = static_cast<nw::gfx::Model*>(m_pNode);
    if (model->m_pMaterialAnimGroup == NULL) {
        return;
    }
    SetAnimObject(model, model->m_MaterialAnimGroupIndex, NULL, objectIdx);
}

// 0x004F17EC (name is ours)
void g3d::Model::SetVisibilityAnim(const BaseAnim* anim)
{
    nw::gfx::Model* model = static_cast<nw::gfx::Model*>(m_pNode);
    nw::gfx::AnimObject* animObject = anim->m_pAnimObject;
    if (model->m_pVisibilityAnimGroup == NULL) {
        return;
    }
    SetAnimObject(model, model->m_VisibilityAnimGroupIndex, animObject, 0);
}

// 0x004F1848 (name is ours)
bool g3d::Model::Create(nw::gfx::res::ResSceneObject resource, nw::os::IAllocator* allocator,
                        nw::os::IAllocator* deviceAllocator, u32 bufferOption, bool isAnimationEnabled,
                        s32 maxAnimObjectsPerGroup)
{
    nw::gfx::SceneBuilder builder;
    builder.m_Resource = resource;
    builder.m_IsFixedSizeMemory = true;
    builder.m_MaxChildren = 0;
    builder.m_MaxCallbacks = 0;
    builder.m_BufferOption = bufferOption;
    builder.m_pSharedMaterialModel = NULL;
    builder.m_pSharedMeshNodeVisibilityModel = NULL;
    builder.m_MaxAnimObjectsPerGroup = maxAnimObjectsPerGroup;
    builder.m_ParticleSetMargin = 0;
    builder.m_IsAnimationEnabled = isAnimationEnabled;
    nw::gfx::SceneObject* object = builder.CreateObject(allocator, deviceAllocator);
    if (object == NULL) {
        return false;
    }
    m_pNode = nw::gfx::DynamicCast<nw::gfx::TransformNode>(object);
    m_MaxAnimObjectsPerGroup = maxAnimObjectsPerGroup;
    return true;
}

// 0x004F18F8 slot 0x0C | libgarden
void g3d::Model::Finalize()
{
    if (m_pNode != NULL) {
        RestoreMeshOrder();
    }
    TransformNode::Finalize();
}

// 0x004F191C (name is ours)
void g3d::Model::UpdateAnims()
{
    UpdateAnimFrames();
    EvaluateAnims();
}

// 0x004F19B0 (name is ours)
nw::gfx::Model* g3d::Model::GetModel() const
{
    return nw::gfx::DynamicCast<nw::gfx::Model>(m_pNode);
}

// 0x004F19FC (name is ours)
void g3d::Model::SortMeshes()
{
    nw::gfx::res::ResMeshData* meshes[MAX_MESHES];
    s32 count = GetMeshes(meshes);
    for (s32 i = 0; i < count; i++) {
        for (s32 j = i + 1; j < count; j++) {
            nw::gfx::res::ResMeshData* meshA = meshes[i];
            nw::gfx::res::ResMeshData* meshB = meshes[j];
            nw::gfx::Model* model = static_cast<nw::gfx::Model*>(m_pNode);
            u8 kindA = model->m_Materials[meshA->materialIndex]->GetOriginal().ptr()->translucencyKind;
            u8 kindB = model->m_Materials[meshB->materialIndex]->GetOriginal().ptr()->translucencyKind;
            if (kindA > kindB || (kindA == kindB && meshA->renderPriority > meshB->renderPriority)) {
                meshes[i] = meshB;
                meshes[j] = meshA;
            }
        }
    }
    StoreMeshes(static_cast<nw::gfx::Model*>(m_pNode), meshes, count);
}

// 0x004F1B04 | libgarden
g3d::Model::Model() : m_MaxAnimObjectsPerGroup(0)
{
}

// 0x004F1B34 slot 0x00 | libgarden
// 0x004F1B24 (deleting dtor)
g3d::Model::~Model()
{
    // (only the base is destroyed)
}

} // namespace g3d
