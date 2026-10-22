#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"

namespace nw {
namespace gfx {
class Material;
class RenderContext;

// RTTI N2nw3gfx18IMaterialActivatorE @ 0x008D06C4
// What sends the state of a material to the GPU before its meshes are drawn (the class name and
// Activate are from the reference symbols, the rest is ours).
class IMaterialActivator : public GfxObject
{
public:
    // (inline)
    explicit IMaterialActivator(os::IAllocator* pAllocator) : GfxObject(pAllocator) {}
    virtual ~IMaterialActivator() {}

    virtual const TypeInfo* GetRuntimeTypeInfo() const = 0; // slot 0x08
    virtual void Activate(RenderContext* pRenderContext, const Material* pMaterial) = 0; // slot 0x0C

    static const TypeInfo s_TypeInfo;
};
ASSERT_SIZE(IMaterialActivator, 0x8);
} // namespace gfx
} // namespace nw
