#include <new>

#include "nw/gfx/gfx_LightSet.h"

namespace nw {
namespace gfx {
namespace {
// (inline) a light set with room for the vertex lights: a fixed array, or one that grows (name is ours)
DECOMP_ALWAYS_INLINE LightSet* CreateLightSet(res::ResLightSet resLightSet, bool isFixedSizeMemory, s32 maxVertexLights, os::IAllocator* pAllocator)
{
    void* pMemory = pAllocator->Alloc(sizeof(LightSet), 4);
    LightSet* pLightSet = pMemory != nullptr ? new (pMemory) LightSet(pAllocator, resLightSet) : nullptr;
    if (isFixedSizeMemory && maxVertexLights != 0) {
        void* pBuffer = pAllocator->Alloc(maxVertexLights * sizeof(Light*), 4);
        ut::MoveArray<Light*> vertexLights(pBuffer, maxVertexLights, pAllocator);
        pLightSet->m_VertexLights = vertexLights;
    } else {
        ut::MoveArray<Light*> vertexLights(pAllocator);
        pLightSet->m_VertexLights = vertexLights;
    }
    return pLightSet;
}
} // namespace

// 0x009760D4
const TypeInfo nw::gfx::LightSet::s_TypeInfo(nullptr);

// sets s_TypeInfo
// 0x0079673C (static initializer)

// 0x004AEA3C (name is ours)
nw::gfx::LightSet* nw::gfx::LightSet::Create(const Description& description, os::IAllocator* pAllocator)
{
    return CreateLightSet(res::ResLightSet(), description.isFixedSizeMemory, description.maxVertexLights, pAllocator);
}

// 0x004AEC1C (name is ours)
nw::gfx::LightSet* nw::gfx::LightSet::Create(res::ResLightSet resLightSet, os::IAllocator* pAllocator)
{
    return CreateLightSet(resLightSet, true, resLightSet.ptr()->lightCount, pAllocator);
}

// 0x004AEE60 slot 0x00 | nintendogs:bytes
// 0x004AEDF0 (deleting dtor)
nw::gfx::LightSet::~LightSet()
{
    // empty (in the original too: only the arrays)
}

} // namespace gfx
} // namespace nw
