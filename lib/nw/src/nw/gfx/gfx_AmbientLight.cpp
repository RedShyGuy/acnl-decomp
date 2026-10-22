#include <cstring>
#include <new>

#include "nw/gfx/gfx_AnimGroup.h"
#include "nw/gfx/gfx_AmbientLight.h"
#include "nw/gfx/res/gfx_ResDynamic.h"

namespace nw {
namespace gfx {
// 0x00976044
const TypeInfo nw::gfx::AmbientLight::s_TypeInfo(&Light::s_TypeInfo);

// sets s_TypeInfo
// 0x0079B594 (static initializer)

// 0x0048DB08 slot 0x24 | nintendogs:bytes
u32 nw::gfx::AmbientLight::Initialize(os::IAllocator* pAllocator)
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

// 0x0048DC34 | nintendogs:bytes [tier A]
u32 nw::gfx::AmbientLight::CreateOriginalValue(os::IAllocator* pAllocator)
{
    res::ResAmbientLightData* pCopy = static_cast<res::ResAmbientLightData*>(pAllocator->Alloc(sizeof(res::ResAmbientLightData), 4));
    if (pCopy != nullptr) {
        const res::ResAmbientLightData& resource = *static_cast<const res::ResAmbientLightData*>(GetResSceneObject().ptr());
        res::CopyResTransformNodeData(pCopy, resource);
        pCopy->isLightEnabled = resource.isLightEnabled;
        std::memcpy(pCopy->m_Unknown0xB5, resource.m_Unknown0xB5, sizeof(pCopy->m_Unknown0xB5));
        pCopy->color = resource.color;
        pCopy->colorU32 = resource.colorU32;
        std::memcpy(pCopy->m_Unknown0xCC, resource.m_Unknown0xCC, sizeof(pCopy->m_Unknown0xCC));
    }
    m_pOriginalValue = pCopy;
    m_OriginalTransform = static_cast<const res::ResTransformNodeData*>(GetResSceneObject().ptr())->transform;
    return RESULT_SUCCESS;
}

// 0x0048DD1C | nintendogs:bytes [tier A]
nw::gfx::res::ResAmbientLightData* nw::gfx::AmbientLight::CreateResAmbientLight(os::IAllocator* pAllocator, const char* name)
{
    res::ResAmbientLightData* pResource = static_cast<res::ResAmbientLightData*>(pAllocator->Alloc(sizeof(res::ResAmbientLightData), 4));
    std::memset(pResource, 0, sizeof(res::ResAmbientLightData));
    res::SetupDynamicResSceneObject(pResource, res::ResAmbientLightData::TYPE, pAllocator, name);
    res::SetupDynamicResTransformNode(pResource);
    return pResource;
}

// 0x0048DEDC | nintendogs:bytes [tier A]
void nw::gfx::AmbientLight::GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResAmbientLight resLight, Description description)
{
    pSize->Add(sizeof(AmbientLight));
    TransformNode::GetMemorySizeForInitialize(pSize, res::ResTransformNode(resLight.ptr()), description);
    // CreateOriginalValue
    pSize->Add(sizeof(res::ResAmbientLightData));
    // CreateAnimGroup
    if (description.isAnimationEnabled && resLight.ptr()->animGroupCount > 0) {
        pSize->Add(sizeof(AnimGroup));
        AnimGroup::GetMemorySizeForInitialize(pSize, res::ResSceneObject(resLight.ptr()).GetAnimGroup(0), true);
    }
}

// 0x0048DFD4 slot 0x10 | nintendogs:bytes
void nw::gfx::AmbientLight::Accept(ISceneVisitor* pVisitor)
{
    pVisitor->VisitAmbientLight(this);
    for (SceneNode** ppChild = m_Children.begin(); ppChild != m_Children.end(); ++ppChild) {
        (*ppChild)->Accept(pVisitor);
    }
}

// 0x0048E028 (name is ours)
nw::gfx::AmbientLight* nw::gfx::AmbientLight::Create(SceneNode* pParent, res::ResSceneObject resObject, const Description& description,
                                       os::IAllocator* pAllocator)
{
    const res::ResAmbientLightData* pResource = res::DynamicCastData<res::ResAmbientLightData>(resObject.ptr(), res::ResAmbientLightData::TYPE);
    void* pMemory = pAllocator->Alloc(sizeof(AmbientLight), 4);
    AmbientLight* pLight = nullptr;
    if (pMemory != nullptr) {
        pLight = new (pMemory) AmbientLight(pAllocator, res::ResTransformNode(pResource), description, nullptr, nullptr);
    }
    pLight->Initialize(pAllocator);
    if (pParent != nullptr) {
        pParent->AttachChild(pLight);
    }
    return pLight;
}

// 0x0048E240 slot 0x00 | nintendogs:bytes
// 0x0048E234 (deleting dtor)
nw::gfx::AmbientLight::~AmbientLight()
{
    DestroyOriginalValue();
    if (m_pOwnResource != nullptr) {
        res::DestroyDynamicResSceneObject(m_pResourceAllocator, m_pOwnResource);
    }
}

} // namespace gfx
} // namespace nw
