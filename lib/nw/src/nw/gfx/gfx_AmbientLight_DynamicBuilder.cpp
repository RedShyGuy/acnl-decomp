#include <new>

#include "nw/gfx/gfx_AmbientLight_DynamicBuilder.h"
#include "nw/gfx/res/gfx_ResDynamic.h"

// 0x0048DB5C | nintendogs:bytes [tier A]
nw::gfx::AmbientLight* nw::gfx::AmbientLight::DynamicBuilder::Create(os::IAllocator* pAllocator)
{
    res::ResAmbientLightData* pResource = CreateResAmbientLight(pAllocator, nullptr);
    void* pMemory = pAllocator->Alloc(sizeof(AmbientLight), 4);
    AmbientLight* pLight = nullptr;
    if (pMemory != nullptr) {
        // the light frees the resource
        pLight = new (pMemory) AmbientLight(pAllocator, res::ResTransformNode(pResource), m_Description, pResource, pAllocator);
        pResource = nullptr;
    }
    pLight->Initialize(pAllocator);
    if (pResource != nullptr) {
        res::DestroyDynamicResSceneObject(pAllocator, pResource);
    }
    return pLight;
}
