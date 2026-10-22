#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_IMaterialActivator.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx25ParticleMaterialActivatorE @ 0x008D0778
// vtable 0x009028D4 (vptr 0x009028DC), offset_to_top 0, 4 entries
// the activator of the materials of particles (member names are ours)
class ParticleMaterialActivator : public IMaterialActivator
{
public:
    // (inline)
    explicit ParticleMaterialActivator(os::IAllocator* pAllocator) : IMaterialActivator(pAllocator) {}
    virtual ~ParticleMaterialActivator();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073C52C slot 0x08
    virtual void Activate(RenderContext* pRenderContext, const Material* pMaterial); // 0x004A3B58 slot 0x0C | nintendogs:callseq

    // an activator from the allocator (NULL: no memory)
    static ParticleMaterialActivator* Create(os::IAllocator* pAllocator); // 0x004A3B20 | nintendogs [tier C]

    static const TypeInfo s_TypeInfo;
};
ASSERT_SIZE(ParticleMaterialActivator, 0x8);
} // namespace gfx
} // namespace nw
