#include "nw/gfx/gfx_SimpleMaterialActivator.h"

#include <new>

namespace nw {
namespace gfx {
// 0x0097F4A0
const TypeInfo nw::gfx::SimpleMaterialActivator::s_TypeInfo(&IMaterialActivator::s_TypeInfo);

// sets s_TypeInfo
// 0x0079FD08 (static initializer)

// 0x004A3840 slot 0x00
// 0x004A383C (deleting dtor)
nw::gfx::SimpleMaterialActivator::~SimpleMaterialActivator()
{
}

// 0x004A2AD8 slot 0x0C (name is ours)
void nw::gfx::SimpleMaterialActivator::Activate(RenderContext* /*pRenderContext*/, const Material* /*pMaterial*/)
{
}

// 0x004A2AA0 | nintendogs:callgraph [tier A]
nw::gfx::SimpleMaterialActivator* nw::gfx::SimpleMaterialActivator::Create(os::IAllocator* pAllocator)
{
    void* pMemory = pAllocator->Alloc(sizeof(SimpleMaterialActivator), 4);
    if (pMemory == nullptr) {
        return nullptr;
    }
    return new (pMemory) SimpleMaterialActivator(pAllocator);
}

} // namespace gfx
} // namespace nw
