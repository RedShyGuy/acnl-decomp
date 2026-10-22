#include "nw/gfx/gfx_ParticleMaterialActivator.h"

#include <new>

namespace nw {
namespace gfx {
// 0x0097F484
const TypeInfo nw::gfx::ParticleMaterialActivator::s_TypeInfo(&IMaterialActivator::s_TypeInfo);

// sets s_TypeInfo
// 0x0079FD38 (static initializer)

// 0x004A4390 slot 0x00
// 0x004A438C (deleting dtor)
nw::gfx::ParticleMaterialActivator::~ParticleMaterialActivator()
{
}

// 0x004A3B58 slot 0x0C | nintendogs:callseq
void nw::gfx::ParticleMaterialActivator::Activate(RenderContext* /*pRenderContext*/, const Material* /*pMaterial*/)
{
}

// 0x004A3B20 | nintendogs [tier C]
nw::gfx::ParticleMaterialActivator* nw::gfx::ParticleMaterialActivator::Create(os::IAllocator* pAllocator)
{
    void* pMemory = pAllocator->Alloc(sizeof(ParticleMaterialActivator), 4);
    if (pMemory == nullptr) {
        return nullptr;
    }
    return new (pMemory) ParticleMaterialActivator(pAllocator);
}

} // namespace gfx
} // namespace nw
