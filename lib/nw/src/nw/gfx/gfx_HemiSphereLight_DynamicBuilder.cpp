#include <new>

#include "nw/gfx/gfx_HemiSphereLight_DynamicBuilder.h"
#include "nw/gfx/res/gfx_ResDynamic.h"

// 0x0049A9E0 | nintendogs:bytes [tier A]
nw::gfx::HemiSphereLight* nw::gfx::HemiSphereLight::DynamicBuilder::Create(os::IAllocator* pAllocator)
{
    res::ResHemiSphereLightData* pResource = CreateResHemiSphereLight(pAllocator, nullptr);
    void* pMemory = pAllocator->Alloc(sizeof(HemiSphereLight), 4);
    HemiSphereLight* pLight = nullptr;
    if (pMemory != nullptr) {
        // the light frees the resource
        pLight = new (pMemory) HemiSphereLight(pAllocator, res::ResTransformNode(pResource), m_Description, pResource, pAllocator);
        pResource = nullptr;
    }
    pLight->Initialize(pAllocator);
    if (pResource != nullptr) {
        res::DestroyDynamicResSceneObject(pAllocator, pResource);
    }
    return pLight;
}
