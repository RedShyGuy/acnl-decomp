#include <new>

#include "nw/gfx/gfx_Fog_DynamicBuilder.h"

// 0x004A4BE4 | nintendogs:bytes [tier A]
nw::gfx::Fog* nw::gfx::Fog::DynamicBuilder::Create(os::IAllocator* pAllocator)
{
    res::ResFogData* pResource = CreateResFog(pAllocator, nullptr);
    void* pMemory = pAllocator->Alloc(sizeof(Fog), 4);
    Fog* pFog = nullptr;
    if (pMemory != nullptr) {
        // the fog frees the resource
        pFog = new (pMemory) Fog(pAllocator, res::ResTransformNode(pResource), m_Description, pResource, pAllocator);
        pResource = nullptr;
    }
    pFog->Initialize(pAllocator);
    if (pResource != nullptr) {
        DestroyResFog(pAllocator, pResource);
    }
    return pFog;
}
