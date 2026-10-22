#include <cstring>
#include <new>

#include "nw/gfx/gfx_AnimGroup.h"
#include "nw/gfx/gfx_HemiSphereLight.h"
#include "nw/gfx/res/gfx_ResDynamic.h"

namespace nw {
namespace gfx {
// 0x009760D0
const TypeInfo nw::gfx::HemiSphereLight::s_TypeInfo(&Light::s_TypeInfo);

// sets s_TypeInfo
// 0x0079D0CC (static initializer)

// 0x0049A98C slot 0x24 | nintendogs:bytes
u32 nw::gfx::HemiSphereLight::Initialize(os::IAllocator* pAllocator)
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

// 0x0049AAB8 | nintendogs:bytes [tier A]
u32 nw::gfx::HemiSphereLight::CreateOriginalValue(os::IAllocator* pAllocator)
{
    res::ResHemiSphereLightData* pCopy = static_cast<res::ResHemiSphereLightData*>(pAllocator->Alloc(sizeof(res::ResHemiSphereLightData), 4));
    if (pCopy != nullptr) {
        const res::ResHemiSphereLightData& resource = *static_cast<const res::ResHemiSphereLightData*>(GetResSceneObject().ptr());
        res::CopyResTransformNodeData(pCopy, resource);
        pCopy->isLightEnabled = resource.isLightEnabled;
        std::memcpy(pCopy->m_Unknown0xB5, resource.m_Unknown0xB5, sizeof(pCopy->m_Unknown0xB5));
        pCopy->groundColor = resource.groundColor;
        pCopy->skyColor = resource.skyColor;
        pCopy->direction = resource.direction;
        pCopy->lerpFactor = resource.lerpFactor;
    }
    m_pOriginalValue = pCopy;
    m_OriginalTransform = static_cast<const res::ResTransformNodeData*>(GetResSceneObject().ptr())->transform;
    return RESULT_SUCCESS;
}

// 0x0049ACA0 | nintendogs:bytes [tier A]
nw::gfx::res::ResHemiSphereLightData* nw::gfx::HemiSphereLight::CreateResHemiSphereLight(os::IAllocator* pAllocator, const char* name)
{
    res::ResHemiSphereLightData* pResource = static_cast<res::ResHemiSphereLightData*>(pAllocator->Alloc(sizeof(res::ResHemiSphereLightData), 4));
    std::memset(pResource, 0, sizeof(res::ResHemiSphereLightData));
    res::SetupDynamicResSceneObject(pResource, res::ResHemiSphereLightData::TYPE, pAllocator, name);
    res::SetupDynamicResTransformNode(pResource);
    return pResource;
}

// 0x0049ABA8 | nintendogs:bytes [tier A]
void nw::gfx::HemiSphereLight::GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResHemiSphereLight resLight, Description description)
{
    pSize->Add(sizeof(HemiSphereLight));
    TransformNode::GetMemorySizeForInitialize(pSize, res::ResTransformNode(resLight.ptr()), description);
    // CreateOriginalValue
    pSize->Add(sizeof(res::ResHemiSphereLightData));
    // CreateAnimGroup
    if (description.isAnimationEnabled && resLight.ptr()->animGroupCount > 0) {
        pSize->Add(sizeof(AnimGroup));
        AnimGroup::GetMemorySizeForInitialize(pSize, res::ResSceneObject(resLight.ptr()).GetAnimGroup(0), true);
    }
}

// 0x0049AE60 slot 0x10 | nintendogs:bytes
void nw::gfx::HemiSphereLight::Accept(ISceneVisitor* pVisitor)
{
    pVisitor->VisitHemiSphereLight(this);
    for (SceneNode** ppChild = m_Children.begin(); ppChild != m_Children.end(); ++ppChild) {
        (*ppChild)->Accept(pVisitor);
    }
}

// 0x0049AEB4 (name is ours)
nw::gfx::HemiSphereLight* nw::gfx::HemiSphereLight::Create(SceneNode* pParent, res::ResSceneObject resObject, const Description& description,
                                       os::IAllocator* pAllocator)
{
    const res::ResHemiSphereLightData* pResource = res::DynamicCastData<res::ResHemiSphereLightData>(resObject.ptr(), res::ResHemiSphereLightData::TYPE);
    void* pMemory = pAllocator->Alloc(sizeof(HemiSphereLight), 4);
    HemiSphereLight* pLight = nullptr;
    if (pMemory != nullptr) {
        pLight = new (pMemory) HemiSphereLight(pAllocator, res::ResTransformNode(pResource), description, nullptr, nullptr);
    }
    pLight->Initialize(pAllocator);
    if (pParent != nullptr) {
        pParent->AttachChild(pLight);
    }
    return pLight;
}

// 0x0049B0CC slot 0x00 | nintendogs:bytes
// 0x0049B0C0 (deleting dtor)
nw::gfx::HemiSphereLight::~HemiSphereLight()
{
    DestroyOriginalValue();
    if (m_pOwnResource != nullptr) {
        res::DestroyDynamicResSceneObject(m_pResourceAllocator, m_pOwnResource);
    }
}

} // namespace gfx
} // namespace nw
