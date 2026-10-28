#include "nw/gfx/gfx_MaterialActivator.h"

#include "nw/gfx/internal/gfx_ActivateMaterial.h"

#include <new>

namespace nw {
namespace gfx {
namespace {
// the resource of a part: the buffer if the part is buffered
struct GetResource
{
    res::ResMaterial operator()(const Material* pMaterial, int part) const { return pMaterial->GetResource(part); }
};
} // namespace

// 0x0097F47C
const TypeInfo nw::gfx::MaterialActivator::s_TypeInfo(&IMaterialActivator::s_TypeInfo);

// sets s_TypeInfo
// 0x0079E8A8 (static initializer)

// 0x0049E12C slot 0x00
// 0x0049E9D8 (deleting dtor)
nw::gfx::MaterialActivator::~MaterialActivator()
{
    // empty (in the original too)
}

// 0x0049D3E4 slot 0x0C (name is ours)
void nw::gfx::MaterialActivator::Activate(RenderContext* pRenderContext, const Material* pMaterial)
{
    internal::ActivateMaterial(pRenderContext, pMaterial, GetResource());
}

// 0x0049D3AC | nintendogs:callgraph [tier A]
nw::gfx::MaterialActivator* nw::gfx::MaterialActivator::Create(os::IAllocator* pAllocator)
{
    void* pMemory = pAllocator->Alloc(sizeof(MaterialActivator), 4);
    if (pMemory == nullptr) {
        return nullptr;
    }
    return new (pMemory) MaterialActivator(pAllocator);
}

} // namespace gfx
} // namespace nw
