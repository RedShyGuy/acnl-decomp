#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_IMaterialActivator.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx17MaterialActivatorE @ 0x008D06AC
// vtable 0x00902734 (vptr 0x0090273C), offset_to_top 0, 4 entries
// the activator of materials with buffers: it compares the hashes of each part (member names are ours)
class MaterialActivator : public IMaterialActivator
{
public:
    // (inline)
    explicit MaterialActivator(os::IAllocator* pAllocator) : IMaterialActivator(pAllocator) {}
    virtual ~MaterialActivator();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073A878 slot 0x08
    virtual void Activate(RenderContext* pRenderContext, const Material* pMaterial); // 0x0049D3E4 slot 0x0C (name is ours)

    // an activator from the allocator (NULL: no memory)
    static MaterialActivator* Create(os::IAllocator* pAllocator); // 0x0049D3AC | nintendogs:callgraph [tier A]

    static const TypeInfo s_TypeInfo;
};
ASSERT_SIZE(MaterialActivator, 0x8);
} // namespace gfx
} // namespace nw
