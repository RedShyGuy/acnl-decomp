#include "nw/gfx/gfx_Model.h"

#include <cstring>
#include <new>

#include "nw/gfx/gfx_AnimBinding.h"
#include "nw/gfx/gfx_AnimGroup.h"
#include "nw/gfx/gfx_ISceneVisitor.h"
#include "nw/gfx/gfx_Material.h"
#include "nw/gfx/gfx_MaterialActivator.h"
#include "nw/gfx/gfx_SimpleMaterialActivator.h"
#include "nw/gfx/res/gfx_ResTextureMapper.h"

namespace nw {
namespace gfx {
namespace {
// the buffer options with which the materials need no hash compare (name is ours)
const u32 BUFFER_OPTION_SIMPLE = 0x1000;
} // namespace

// 0x009760DC
const TypeInfo nw::gfx::Model::s_TypeInfo(&TransformNode::s_TypeInfo);

// sets s_TypeInfo
// 0x00788560 (static initializer)

// 0x004AAD24 slot 0x24 | nintendogs:bytes
u32 nw::gfx::Model::Initialize(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    result |= TransformNode::Initialize(pAllocator);
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    result |= CreateResMeshes(pAllocator);
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    result |= CreateResMeshNodeVisibilities(pAllocator);
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    result |= CreateMaterials(pAllocator);
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    result |= CreateCallbacks(pAllocator);
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    result |= CreateAnimGroups(pAllocator);
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }

    // the materials of a model of its own get an activator
    u32 activatorResult = RESULT_SUCCESS;
    if (!(m_SharingFlags & SHARING_MATERIALS)) {
        IMaterialActivator* pActivator;
        if (m_pDescription->bufferOption == 0 || m_pDescription->bufferOption == BUFFER_OPTION_SIMPLE) {
            pActivator = SimpleMaterialActivator::Create(pAllocator);
        } else {
            pActivator = MaterialActivator::Create(pAllocator);
        }
        if (pActivator == nullptr) {
            activatorResult |= RESULT_FAILURE_MASK;
        } else {
            m_MaterialActivator = GfxPtr<IMaterialActivator>(pActivator);
        }
    }
    return activatorResult | result;
}

// 0x004AAEEC | nintendogs:bytes [tier B]
bool nw::gfx::Model::IsMeshVisible(res::ResMesh mesh)
{
    const res::ResMeshData* pMesh = mesh.ptr();
    if (pMesh->meshNodeVisibilityIndex < 0) {
        return pMesh->isVisible;
    }
    return GetMeshNodeVisibility(pMesh->meshNodeVisibilityIndex)->isVisible && pMesh->isVisible;
}

// 0x004AAF70 | nintendogs:callgraph [tier A]
u32 nw::gfx::Model::CreateCallbacks(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    m_pPreRenderCallbacks =
        RenderCallbackList::Create(pAllocator, m_pDescription->isFixedSizeMemory, m_pDescription->maxCallbacks);
    m_pPostRenderCallbacks =
        RenderCallbackList::Create(pAllocator, m_pDescription->isFixedSizeMemory, m_pDescription->maxCallbacks);
    if (m_pPreRenderCallbacks == nullptr || m_pPostRenderCallbacks == nullptr) {
        result |= RESULT_FAILURE_MASK;
    }
    return result;
}

// 0x004AB1CC | nintendogs:bytes [tier A]
u32 nw::gfx::Model::CreateMaterials(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    if (!(m_SharingFlags & SHARING_MATERIALS)) {
        res::ResModel resource = GetResModel();
        const int count = resource.ptr()->materials.count;
        if (count == 0) {
            return result;
        }
        void* pBuffer = pAllocator->Alloc(count * sizeof(Material*), 4);
        if (pBuffer == nullptr) {
            result |= RESULT_FAILURE_MASK;
        }
        if (result & RESULT_FAILURE_MASK) {
            return result;
        }
        {
            ut::MoveArray<Material*> materials(pBuffer, count, pAllocator);
            m_Materials = materials;
        }
        const int bufferCount = m_BufferOption != 0 ? 1 : 0;
        for (int i = 0; i < count; ++i) {
            Material* pMaterial = Material::Create(resource.GetMaterial(i), bufferCount, this, pAllocator);
            if (pMaterial == nullptr) {
                result |= RESULT_FAILURE_MASK;
            }
            if (result & RESULT_FAILURE_MASK) {
                break;
            }
            m_Materials.PushBackFast(pMaterial);
        }
    } else {
        // the materials of the other model
        m_BufferOption = m_pDescription->pSharedMaterialModel->m_BufferOption;
        const int count = m_pDescription->pSharedMaterialModel->m_Materials.size();
        void* pBuffer = pAllocator->Alloc(count * sizeof(Material*), 4);
        if (pBuffer == nullptr) {
            result |= RESULT_FAILURE_MASK;
        }
        if (result & RESULT_FAILURE_MASK) {
            return result;
        }
        {
            ut::MoveArray<Material*> materials(pBuffer, count, pAllocator);
            m_Materials = materials;
        }
        for (int i = 0; i < count; ++i) {
            m_Materials.PushBackFast(m_pDescription->pSharedMaterialModel->m_Materials[i]);
        }
    }
    return result;
}

// 0x004AB3DC | nintendogs:callgraph [tier A]
u32 nw::gfx::Model::CreateResMeshes(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    const res::ResModelData* pResource = GetResModel().ptr();
    const int count = pResource->meshes.count;
    if (count == 0) {
        return result;
    }

    // the offsets to the copies, and the copies in one block
    ut::Offset* pMeshes = static_cast<ut::Offset*>(pAllocator->Alloc(count * sizeof(ut::Offset), 4));
    if (pMeshes != nullptr) {
        for (int i = 0; i < count; ++i) {
            pMeshes[i].Set(nullptr);
        }
    }
    res::ResMeshData* pCopies = static_cast<res::ResMeshData*>(pAllocator->Alloc(count * sizeof(res::ResMeshData), 4));
    if (pMeshes == nullptr || pCopies == nullptr) {
        if (pMeshes != nullptr) {
            pAllocator->Free(pMeshes);
        } else if (pCopies != nullptr) {
            pAllocator->Free(pCopies);
        }
        result |= RESULT_FAILURE_MASK;
    }
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    for (int i = 0; i < count; ++i) {
        const res::ResMeshData* pOriginal = static_cast<const res::ResMeshData*>(GetResModel().GetMesh(i));
        res::ResMeshData* pCopy = &pCopies[i];
        std::memcpy(pCopy, pOriginal, offsetof(res::ResMeshData, m_Unknown0x84));
        pCopy->owner.Set(GetResModel().ptr());
        pCopy->userDataCount = 0;
        pCopy->userDataDic.Set(nullptr);
        pCopy->name = static_cast<const res::ResMeshData*>(GetResModel().GetMesh(i))->name;
        pCopy->m_Unknown0x70 = static_cast<const res::ResMeshData*>(GetResModel().GetMesh(i))->m_Unknown0x70;
        pMeshes[i].Set(pCopy);
    }
    m_ResMeshes = ut::internal::ResArray<res::ResMesh, ut::internal::ResArrayClassTraits>(pMeshes, count);
    m_IsResourceVisible = pResource->isVisible;

    // the visibility of each mesh
    void* pBuffer = pAllocator->Alloc(count * sizeof(bool), 4);
    if (pBuffer == nullptr) {
        result |= RESULT_FAILURE_MASK;
    }
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    {
        ut::MoveArray<bool> visibilities(pBuffer, count, pAllocator);
        m_MeshVisibilities = visibilities;
    }
    for (int i = 0; i < count; ++i) {
        m_MeshVisibilities.PushBackFast(m_ResMeshes[i].ptr()->isVisible);
    }
    return result;
}

// 0x004AB6E0 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::Model::BindMaterialAnim(AnimGroup* pAnimGroup)
{
    const int memberCount = pAnimGroup->GetMemberCount();
    for (int i = 0; i < memberCount; ++i) {
        anim::res::ResAnimGroupMember member = pAnimGroup->GetResAnimGroupMember(i);
        const anim::res::ResMaterialMemberData* pMember =
            static_cast<const anim::res::ResMaterialMemberData*>(member.ptr());

        // the material of the member, by name
        int materialIndex = 0;
        switch (pMember->memberType) {
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_MATERIAL_COLOR:
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_TEXTURE_SAMPLER:
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_TEXTURE_MAPPER:
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_BLEND_OPERATION:
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_TEXTURE_COORDINATOR:
            materialIndex = GetResModel().ptr()->materials.GetDic().GetIndex(pMember->materialName.To<char>());
            break;
        default:
            break;
        }
        Material* pMaterial = m_Materials[materialIndex];
        pAnimGroup->m_TargetObjects[i] = reinterpret_cast<void*>(materialIndex);

        // the animation writes to the buffer if the part has one, else to the resource
        void* pTarget = pMaterial->GetAnimTargetObject(member, pMaterial->GetResMaterialForAnim(pMember->memberType));
        if (pMember->memberType == anim::res::ResAnimGroupMemberData::MEMBER_TYPE_TEXTURE_SAMPLER) {
            pTarget = static_cast<res::ResPixelBasedTextureMapperData*>(pTarget)->sampler.ToPtr();
        }
        pAnimGroup->m_TargetValues[i] = static_cast<u8*>(pTarget) + pMember->valueOffset;
        pAnimGroup->m_Targets[i] = pMaterial->GetAnimTargetObject(member, pMaterial->GetResMaterialForAnim(pMember->memberType));

        // a buffered part keeps the value of the resource as the original
        if (pMaterial->CanUseBuffer(pMember->memberType)) {
            void* pOriginal = pMaterial->GetAnimTargetObject(member, pMaterial->GetOriginal());
            if (pMember->memberType == anim::res::ResAnimGroupMemberData::MEMBER_TYPE_TEXTURE_SAMPLER) {
                pOriginal = static_cast<res::ResPixelBasedTextureMapperData*>(pOriginal)->sampler.ToPtr();
            }
            pAnimGroup->m_OriginalValues[i] = static_cast<u8*>(pOriginal) + pMember->valueOffset;
        }
    }
}

// 0x004AB964 | nintendogs:bytes [tier A]
u32 nw::gfx::Model::CreateAnimGroups(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    AnimBinding* pBinding = m_AnimBinding.Get();
    if (pBinding == nullptr) {
        return result;
    }
    res::ResModel resource = GetResModel();
    const int groupCount = resource.GetAnimGroupCount();
    for (int i = 0; i < groupCount; ++i) {
        anim::res::ResAnimGroup resAnimGroup = resource.GetAnimGroup(i);
        const u32 targetType = resAnimGroup.GetTargetType();
        if (targetType == anim::res::ResAnimGroupData::TARGET_TYPE_MATERIAL) {
            // the materials of another model are animated there
            if (m_SharingFlags & SHARING_MATERIALS) {
                continue;
            }
        } else if (targetType != anim::res::ResAnimGroupData::TARGET_TYPE_VISIBILITY) {
            continue;
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

        if (targetType == anim::res::ResAnimGroupData::TARGET_TYPE_MATERIAL) {
            BindMaterialAnim(pAnimGroup);
            pBinding->m_AnimGroups[i] = pAnimGroup;
            m_MaterialAnimGroupIndex = i;
            m_pMaterialAnimGroup = pAnimGroup;
        } else if (targetType == anim::res::ResAnimGroupData::TARGET_TYPE_VISIBILITY) {
            BindVisibilityAnim(pAnimGroup);
            pBinding->m_AnimGroups[i] = pAnimGroup;
            m_VisibilityAnimGroupIndex = i;
            m_pVisibilityAnimGroup = pAnimGroup;
        }
    }
    return result;
}

// 0x004ABB04 | nintendogs:bytes [tier A]
void nw::gfx::Model::DestroyResMeshes(os::IAllocator* pAllocator,
                                      ut::internal::ResArray<res::ResMesh, ut::internal::ResArrayClassTraits> meshes)
{
    if (meshes.m_pBegin == meshes.m_pEnd) {
        return;
    }
    // the first offset points to the block of all copies
    pAllocator->Free(const_cast<void*>(meshes.m_pBegin->ToPtr()));
    pAllocator->Free(const_cast<ut::Offset*>(meshes.m_pBegin));
}

// 0x004ABB4C | nintendogs:bytes [tier A]
void nw::gfx::Model::BindVisibilityAnim(AnimGroup* pAnimGroup)
{
    const int memberCount = pAnimGroup->GetMemberCount();
    for (int i = 0; i < memberCount; ++i) {
        anim::res::ResAnimGroupMember member = pAnimGroup->GetResAnimGroupMember(i);
        const anim::res::ResAnimGroupMemberData* pMember = member.ptr();
        switch (pMember->memberType) {
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_MODEL: {
            // field 0: the visibility of the branch, 1: the one of the model
            void* pValue = nullptr;
            if (pMember->field == 1) {
                pValue = &m_IsVisible;
            } else if (pMember->field == 0) {
                pValue = &m_IsBranchVisible;
            }
            pAnimGroup->m_TargetObjects[i] = reinterpret_cast<void*>(-1);
            pAnimGroup->m_TargetValues[i] = pValue;
            pAnimGroup->m_OriginalValues[i] = &m_IsResourceVisible;
            break;
        }
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_MESH: {
            const anim::res::ResMeshMemberData* pMeshMember = res::DynamicCastData<anim::res::ResMeshMemberData>(
                pMember, anim::res::ResMeshMemberData::TYPE);
            const int meshIndex = pMeshMember->meshIndex;
            u8* pMesh = reinterpret_cast<u8*>(GetResMesh(meshIndex).ptr());
            pAnimGroup->m_TargetObjects[i] = reinterpret_cast<void*>(meshIndex);
            pAnimGroup->m_TargetValues[i] = pMesh + pMember->valueOffset;
            pAnimGroup->m_OriginalValues[i] = &m_MeshVisibilities[meshIndex];
            break;
        }
        case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_MESH_NODE_VISIBILITY: {
            const anim::res::ResMeshNodeVisibilityMemberData* pNodeMember =
                res::DynamicCastData<anim::res::ResMeshNodeVisibilityMemberData>(pMember, anim::res::ResMeshNodeVisibilityMemberData::TYPE);
            const int nodeIndex = GetResModel().ptr()->meshNodeVisibilities.GetDic().GetIndex(pNodeMember->nodeName.To<char>());
            u8* pVisibility = reinterpret_cast<u8*>(GetMeshNodeVisibility(nodeIndex));
            pAnimGroup->m_TargetObjects[i] = reinterpret_cast<void*>(nodeIndex);
            pAnimGroup->m_TargetValues[i] = pVisibility + pMember->valueOffset;
            pAnimGroup->m_OriginalValues[i] = &m_MeshNodeVisibilityFlags[nodeIndex];
            break;
        }
        default:
            break;
        }
        pAnimGroup->m_Targets[i] = GetAnimTargetObject(member);
    }
}

// 0x004ABDB0 | nintendogs:bytes [tier A]
void* nw::gfx::Model::GetAnimTargetObject(const anim::res::ResAnimGroupMember& member)
{
    const anim::res::ResAnimGroupMemberData* pMember = member.ptr();
    switch (pMember->memberType) {
    case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_MODEL:
        return this;
    case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_MESH:
        return GetResMesh(static_cast<const anim::res::ResMeshMemberData*>(pMember)->meshIndex).ptr();
    case anim::res::ResAnimGroupMemberData::MEMBER_TYPE_MESH_NODE_VISIBILITY: {
        const char* name = static_cast<const anim::res::ResMeshNodeVisibilityMemberData*>(pMember)->nodeName.To<char>();
        return GetMeshNodeVisibility(GetResModel().ptr()->meshNodeVisibilities.GetDic().GetIndex(name));
    }
    default:
        return nullptr;
    }
}

// 0x004ABED8 | nintendogs:callseq [tier A]
void nw::gfx::Model::GetMemorySizeForInitialize(os::MemorySizeCalculator* pSize, res::ResModel resource, Description description)
{
    TransformNode::GetMemorySizeForInitialize(pSize, resource, description);

    // the copies of the meshes and the mesh node visibilities
    const int meshCount = resource.ptr()->meshes.count;
    pSize->Add(meshCount * sizeof(ut::Offset));
    pSize->Add(meshCount * sizeof(res::ResMeshData));
    pSize->Add(meshCount * sizeof(bool));
    const int meshNodeCount = resource.ptr()->meshNodeVisibilities.count;
    pSize->Add(meshNodeCount * sizeof(res::ResMeshNodeVisibilityData));
    pSize->Add(meshNodeCount * sizeof(bool));

    // the materials
    if (description.pSharedMaterialModel != nullptr) {
        pSize->Add(description.pSharedMaterialModel->m_Materials.size() * sizeof(Material*));
    } else {
        const int bufferCount = description.bufferOption != 0 ? 1 : 0;
        const int materialCount = resource.ptr()->materials.count;
        pSize->Add(materialCount * sizeof(Material*));
        for (int i = 0; i < materialCount; ++i) {
            Material::GetMemorySizeInternal(pSize, resource.GetMaterial(i), bufferCount, description.bufferOption);
        }
    }

    // the callbacks
    pSize->Add(sizeof(RenderCallbackList));
    if (description.maxCallbacks != 0) {
        pSize->Add(description.maxCallbacks * sizeof(NodeCallback<Model>*));
    }
    pSize->Add(sizeof(RenderCallbackList));
    if (description.maxCallbacks != 0) {
        pSize->Add(description.maxCallbacks * sizeof(NodeCallback<Model>*));
    }

    // the material and visibility animation groups
    if (description.isAnimationEnabled) {
        const int groupCount = resource.GetAnimGroupCount();
        for (int i = 0; i < groupCount; ++i) {
            anim::res::ResAnimGroup resAnimGroup = resource.GetAnimGroup(i);
            const u32 targetType = resAnimGroup.GetTargetType();
            if (targetType == anim::res::ResAnimGroupData::TARGET_TYPE_MATERIAL ||
                targetType == anim::res::ResAnimGroupData::TARGET_TYPE_VISIBILITY) {
                pSize->Add(sizeof(AnimGroup));
                AnimGroup::GetMemorySizeForInitialize(pSize, resAnimGroup, true);
            }
        }
    }

    // the material activator
    if (description.pSharedMaterialModel == nullptr) {
        pSize->Add(sizeof(MaterialActivator));
    }
}

// 0x004AC1D8 | nintendogs:callgraph [tier A]
u32 nw::gfx::Model::CreateResMeshNodeVisibilities(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    res::ResModel resource = GetResModel();
    int count;
    if (!(m_SharingFlags & SHARING_MESH_NODE_VISIBILITIES)) {
        count = resource.ptr()->meshNodeVisibilities.count;
        if (count == 0) {
            return result;
        }
        void* pBuffer = pAllocator->Alloc(count * sizeof(res::ResMeshNodeVisibilityData), 4);
        if (pBuffer == nullptr) {
            result |= RESULT_FAILURE_MASK;
        }
        if (result & RESULT_FAILURE_MASK) {
            return result;
        }
        {
            ut::MoveArray<res::ResMeshNodeVisibilityData> visibilities(pBuffer, count, pAllocator);
            m_MeshNodeVisibilities = visibilities;
        }
        for (int i = 0; i < count; ++i) {
            m_MeshNodeVisibilities.PushBackFast(*resource.GetMeshNodeVisibility(i));
            m_MeshNodeVisibilities[i].name = resource.GetMeshNodeVisibility(i)->name;
        }
    } else {
        // the visibilities of the other model (not freed by this one)
        ut::MoveArray<res::ResMeshNodeVisibilityData>& shared = m_pDescription->pSharedMeshNodeVisibilityModel->m_MeshNodeVisibilities;
        res::ResMeshNodeVisibilityData* pShared = shared.begin();
        count = shared.size();
        {
            ut::MoveArray<res::ResMeshNodeVisibilityData> visibilities(pShared, count, nullptr);
            m_MeshNodeVisibilities = visibilities;
        }
        m_MeshNodeVisibilities.m_pEnd = m_MeshNodeVisibilities.m_pBegin + count;
    }

    // the visibility of each mesh node
    if (count == 0) {
        return result;
    }
    void* pBuffer = pAllocator->Alloc(count * sizeof(bool), 4);
    if (pBuffer == nullptr) {
        result |= RESULT_FAILURE_MASK;
    }
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    {
        ut::MoveArray<bool> flags(pBuffer, count, pAllocator);
        m_MeshNodeVisibilityFlags = flags;
    }
    for (int i = 0; i < count; ++i) {
        m_MeshNodeVisibilityFlags.PushBackFast(resource.GetMeshNodeVisibility(i)->isVisible);
    }
    return result;
}

// 0x004AC53C | nintendogs:bytes
void nw::gfx::Model::Accept(ISceneVisitor* pVisitor)
{
    pVisitor->VisitModel(this);
    for (SceneNode** ppChild = m_Children.begin(); ppChild != m_Children.end(); ++ppChild) {
        (*ppChild)->Accept(pVisitor);
    }
}

// 0x004AC590 | nintendogs:callseq [tier A]
nw::gfx::Model* nw::gfx::Model::Create(SceneNode* pParent, res::ResSceneObject resource, const Description& description,
                                       os::IAllocator* pAllocator)
{
    const res::ResModelData* pResource = res::DynamicCastData<res::ResModelData>(resource.ptr(), res::ResModelData::TYPE);
    void* pMemory = pAllocator->Alloc(sizeof(Model), 4);
    Model* pModel = nullptr;
    if (pMemory != nullptr) {
        pModel = new (pMemory) Model(pAllocator, res::ResModel(pResource), description);
    }
    pModel->Initialize(pAllocator);
    if (pParent != nullptr) {
        pParent->AttachChild(pModel);
    }
    return pModel;
}

// 0x004AC958 slot 0x00 | nintendogs:bytes
// 0x004AC94C (deleting dtor)
nw::gfx::Model::~Model()
{
    if (m_SharingFlags & SHARING_MESH_NODE_VISIBILITIES) {
        // the visibilities belong to the other model
        m_MeshNodeVisibilities.m_pBegin = nullptr;
        m_MeshNodeVisibilities.m_pEnd = nullptr;
        m_MeshNodeVisibilities.m_Capacity &= ut::MoveArray<res::ResMeshNodeVisibilityData>::KIND_MASK;
    }
    DestroyResMeshes(GetAllocator(), m_ResMeshes);
    if (m_pPreRenderCallbacks != nullptr) {
        m_pPreRenderCallbacks->Destroy();
        m_pPreRenderCallbacks = nullptr;
    }
    if (m_pPostRenderCallbacks != nullptr) {
        m_pPostRenderCallbacks->Destroy();
        m_pPostRenderCallbacks = nullptr;
    }
    if (!(m_SharingFlags & SHARING_MATERIALS)) {
        for (Material** ppMaterial = m_Materials.begin(); ppMaterial != m_Materials.end(); ++ppMaterial) {
            if (*ppMaterial != nullptr) {
                (*ppMaterial)->~Material();
                (*ppMaterial)->GetAllocator()->Free(*ppMaterial);
                *ppMaterial = nullptr;
            }
        }
        m_Materials.clear();
    }
    if (m_pMaterialAnimGroup != nullptr) {
        m_pMaterialAnimGroup->~AnimGroup();
        m_pMaterialAnimGroup->GetAllocator()->Free(m_pMaterialAnimGroup);
        m_pMaterialAnimGroup = nullptr;
    }
    if (m_pVisibilityAnimGroup != nullptr) {
        m_pVisibilityAnimGroup->~AnimGroup();
        m_pVisibilityAnimGroup->GetAllocator()->Free(m_pVisibilityAnimGroup);
        m_pVisibilityAnimGroup = nullptr;
    }
}

} // namespace gfx
} // namespace nw
