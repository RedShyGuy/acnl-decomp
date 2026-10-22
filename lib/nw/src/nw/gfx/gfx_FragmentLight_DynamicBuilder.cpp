#include <new>

#include "nw/gfx/gfx_FragmentLight_DynamicBuilder.h"

// 0x00492AD8 | nintendogs:bytes [tier A]
nw::gfx::FragmentLight* nw::gfx::FragmentLight::DynamicBuilder::Create(os::IAllocator* pAllocator)
{
    res::ResFragmentLightData* pResource = CreateResFragmentLight(pAllocator, nullptr);
    void* pMemory = pAllocator->Alloc(sizeof(FragmentLight), 4);
    FragmentLight* pLight = nullptr;
    if (pMemory != nullptr) {
        // the light frees the resource
        pLight = new (pMemory) FragmentLight(pAllocator, res::ResTransformNode(pResource), m_Description, pResource, pAllocator);
        pResource = nullptr;
    }
    pLight->Initialize(pAllocator);
    if (pResource != nullptr) {
        DestroyResFragmentLight(pAllocator, pResource);
    }
    return pLight;
}
