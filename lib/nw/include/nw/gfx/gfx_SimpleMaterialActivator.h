#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_IMaterialActivator.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx23SimpleMaterialActivatorE @ 0x008D0754
// vtable 0x00902874 (vptr 0x0090287C), offset_to_top 0, 4 entries
// the activator of materials without buffers (member names are ours)
class SimpleMaterialActivator : public IMaterialActivator
{
public:
    // (inline)
    explicit SimpleMaterialActivator(os::IAllocator* pAllocator) : IMaterialActivator(pAllocator) {}
    virtual ~SimpleMaterialActivator();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073C4A4 slot 0x08
    virtual void Activate(RenderContext* pRenderContext, const Material* pMaterial); // 0x004A2AD8 slot 0x0C (name is ours)

    // an activator from the allocator (NULL: no memory)
    static SimpleMaterialActivator* Create(os::IAllocator* pAllocator); // 0x004A2AA0 | nintendogs:callgraph [tier A]

    static const TypeInfo s_TypeInfo;
};
ASSERT_SIZE(SimpleMaterialActivator, 0x8);
} // namespace gfx
} // namespace nw
