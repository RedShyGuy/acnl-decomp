#include <cstring>
#include <new>

#include "nw/gfx/gfx_AnimGroup.h"
#include "nw/gfx/gfx_FragmentLight.h"
#include "nw/gfx/res/gfx_ResDynamic.h"

namespace nw {
namespace gfx {
// 0x00976064
const TypeInfo nw::gfx::FragmentLight::s_TypeInfo(&Light::s_TypeInfo);

// sets s_TypeInfo
// 0x0079C194 (static initializer)

// 0x00492A84 slot 0x24 | nintendogs:callseq
u32 nw::gfx::FragmentLight::Initialize(os::IAllocator* pAllocator)
{
    u32 result = RESULT_SUCCESS;
    result |= TransformNode::Initialize(pAllocator);
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    result |= CreateOriginalValue(pAllocator);
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    result |= CreateAnimGroup(pAllocator);
    return result;
}

// 0x00492BA0 slot 0x28 | nintendogs:bytes
void nw::gfx::FragmentLight::UpdateDirection()
{
    const res::ResFragmentLightData* pResource = static_cast<const res::ResFragmentLightData*>(GetResSceneObject().ptr());
    if (!(pResource->flags & res::ResSceneObjectData::FLAG_INHERIT_DIRECTION)) {
        m_Direction = pResource->direction;
        return;
    }
    if (m_pParent == nullptr) {
        m_Direction = pResource->direction;
        return;
    }
    const nn::math::MTX34& parentMatrix = m_pParent->TrackbackWorldMatrix();
    const nn::math::VEC3& direction = pResource->direction;
    m_Direction.x = parentMatrix.m[0][0] * direction.x + parentMatrix.m[0][1] * direction.y + parentMatrix.m[0][2] * direction.z;
    m_Direction.y = parentMatrix.m[1][0] * direction.x + parentMatrix.m[1][1] * direction.y + parentMatrix.m[1][2] * direction.z;
    m_Direction.z = parentMatrix.m[2][0] * direction.x + parentMatrix.m[2][1] * direction.y + parentMatrix.m[2][2] * direction.z;
}

// 0x00492C78 | nintendogs:bytes [tier A]
u32 nw::gfx::FragmentLight::CreateOriginalValue(os::IAllocator* pAllocator)
{
    res::ResFragmentLightData* pCopy = static_cast<res::ResFragmentLightData*>(pAllocator->Alloc(sizeof(res::ResFragmentLightData), 4));
    if (pCopy != nullptr) {
        const res::ResFragmentLightData& resource = *static_cast<const res::ResFragmentLightData*>(GetResSceneObject().ptr());
        res::CopyResTransformNodeData(pCopy, resource);
        pCopy->isLightEnabled = resource.isLightEnabled;
        std::memcpy(pCopy->m_Unknown0xB5, resource.m_Unknown0xB5, sizeof(pCopy->m_Unknown0xB5));
        pCopy->lightKind = resource.lightKind;
        pCopy->ambient = resource.ambient;
        pCopy->diffuse = resource.diffuse;
        pCopy->specular0 = resource.specular0;
        pCopy->specular1 = resource.specular1;
        std::memcpy(pCopy->m_Unknown0xFC, resource.m_Unknown0xFC, sizeof(pCopy->m_Unknown0xFC));
        pCopy->direction = resource.direction;
        pCopy->distanceSampler.m_Offset = resource.distanceSampler.m_Offset;
        pCopy->angleSampler.m_Offset = resource.angleSampler.m_Offset;
        std::memcpy(pCopy->m_Unknown0x120, resource.m_Unknown0x120, sizeof(pCopy->m_Unknown0x120));
        pCopy->distanceAttenuationScale = resource.distanceAttenuationScale;
        pCopy->distanceAttenuationBias = resource.distanceAttenuationBias;
        std::memcpy(pCopy->m_Unknown0x130, resource.m_Unknown0x130, sizeof(pCopy->m_Unknown0x130));
    }
    m_pOriginalValue = pCopy;
    m_OriginalTransform = static_cast<const res::ResTransformNodeData*>(GetResSceneObject().ptr())->transform;
    return RESULT_SUCCESS;
}

// 0x00492E04 | nintendogs:bytes [tier A]
void nw::gfx::FragmentLight::GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResFragmentLight resLight, Description description)
{
    pSize->Add(sizeof(FragmentLight));
    TransformNode::GetMemorySizeForInitialize(pSize, res::ResTransformNode(resLight.ptr()), description);
    // CreateOriginalValue
    pSize->Add(sizeof(res::ResFragmentLightData));
    // CreateAnimGroup
    if (description.isAnimationEnabled && resLight.ptr()->animGroupCount > 0) {
        pSize->Add(sizeof(AnimGroup));
        AnimGroup::GetMemorySizeForInitialize(pSize, res::ResSceneObject(resLight.ptr()).GetAnimGroup(0), true);
    }
}

// 0x00492EFC | nintendogs:bytes-fuzzy [tier A]
nw::gfx::res::ResFragmentLightData* nw::gfx::FragmentLight::CreateResFragmentLight(os::IAllocator* pAllocator, const char* name)
{
    res::ResFragmentLightData* pResource = static_cast<res::ResFragmentLightData*>(pAllocator->Alloc(sizeof(res::ResFragmentLightData), 4));
    std::memset(pResource, 0, sizeof(res::ResFragmentLightData));
    res::SetupDynamicResSceneObject(pResource, res::ResFragmentLightData::TYPE, pAllocator, name);
    res::SetupDynamicResTransformNode(pResource);

    // the lookup tables: references without a name
    res::ResReferenceLookupTableData* pDistanceSampler = res::CreateDynamicReferenceLut(pAllocator);
    res::ResLightingLookupTableData* pAngleSampler =
        static_cast<res::ResLightingLookupTableData*>(pAllocator->Alloc(sizeof(res::ResLightingLookupTableData), 4));
    std::memset(pAngleSampler, 0, sizeof(res::ResLightingLookupTableData));
    pAngleSampler->sampler.Set(res::CreateDynamicReferenceLut(pAllocator));
    pAngleSampler->m_Unknown0x0[0] = 0;
    pAngleSampler->m_Unknown0x0[1] = 0;
    pResource->distanceSampler.Set(pDistanceSampler);
    pResource->angleSampler.Set(pAngleSampler);
    return pResource;
}

// 0x0049319C | nintendogs:bytes [tier A]
void nw::gfx::FragmentLight::DestroyResFragmentLight(os::IAllocator* pAllocator, res::ResFragmentLightData* pResource)
{
    pAllocator->Free(pResource->distanceSampler.ToPtr());
    res::ResLightingLookupTableData* pAngleSampler = pResource->angleSampler.To<res::ResLightingLookupTableData>();
    if (pAngleSampler->sampler.ToPtr() != nullptr) {
        pAllocator->Free(pAngleSampler->sampler.ToPtr());
    }
    pAllocator->Free(pAngleSampler);
    res::DestroyDynamicResSceneObject(pAllocator, pResource);
}

// 0x0049324C slot 0x10 | nintendogs:bytes
void nw::gfx::FragmentLight::Accept(ISceneVisitor* pVisitor)
{
    pVisitor->VisitFragmentLight(this);
    for (SceneNode** ppChild = m_Children.begin(); ppChild != m_Children.end(); ++ppChild) {
        (*ppChild)->Accept(pVisitor);
    }
}

// 0x004932A0 | nintendogs:bytes [tier A]
nw::gfx::FragmentLight* nw::gfx::FragmentLight::Create(SceneNode* pParent, res::ResSceneObject resObject, const Description& description,
                                                       os::IAllocator* pAllocator)
{
    const res::ResFragmentLightData* pResource = res::DynamicCastData<res::ResFragmentLightData>(resObject.ptr(), res::ResFragmentLightData::TYPE);
    void* pMemory = pAllocator->Alloc(sizeof(FragmentLight), 4);
    FragmentLight* pLight = nullptr;
    if (pMemory != nullptr) {
        pLight = new (pMemory) FragmentLight(pAllocator, res::ResTransformNode(pResource), description, nullptr, nullptr);
    }
    pLight->Initialize(pAllocator);
    if (pParent != nullptr) {
        pParent->AttachChild(pLight);
    }
    return pLight;
}

// 0x00493374 slot 0x00 | nintendogs:bytes
// 0x00493368 (deleting dtor)
nw::gfx::FragmentLight::~FragmentLight()
{
    DestroyOriginalValue();
    if (m_pOwnResource != nullptr) {
        DestroyResFragmentLight(m_pResourceAllocator, m_pOwnResource);
    }
}

} // namespace gfx
} // namespace nw
