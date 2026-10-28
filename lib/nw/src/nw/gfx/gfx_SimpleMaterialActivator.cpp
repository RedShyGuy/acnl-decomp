#include "nw/gfx/gfx_SimpleMaterialActivator.h"

#include "nw/gfx/internal/gfx_ActivateMaterial.h"

#include <new>

namespace nw {
namespace gfx {
namespace {
// the resource of a part: always the material resource
struct GetOriginal
{
    res::ResMaterial operator()(const Material* pMaterial, int /*part*/) const { return pMaterial->GetOriginal(); }
};
} // namespace

// 0x0097F4A0
const TypeInfo nw::gfx::SimpleMaterialActivator::s_TypeInfo(&IMaterialActivator::s_TypeInfo);

// sets s_TypeInfo
// 0x0079FD08 (static initializer)

// 0x004A3840 slot 0x00
// 0x004A383C (deleting dtor)
nw::gfx::SimpleMaterialActivator::~SimpleMaterialActivator()
{
    // empty (in the original too)
}

// 0x004A2AD8 slot 0x0C (name is ours)
void nw::gfx::SimpleMaterialActivator::Activate(RenderContext* pRenderContext, const Material* pMaterial)
{
    // nothing to do for the material sent last if no light changed
    const SceneEnvironment& environment = pRenderContext->m_SceneEnvironment;
    const bool isLightDirty = environment.m_IsFragmentLightDirty || environment.m_IsAmbientLightDirty;
    const bool isHemiSphereLightDirty = environment.m_IsHemiSphereLightDirty;
    const Material* pActive = pRenderContext->m_pActiveMaterial;
    if (pActive != nullptr && pActive->m_Buffers.size() == 0 && pActive->GetResSceneObject().ptr() == pMaterial->GetResSceneObject().ptr() &&
        !isLightDirty && !isHemiSphereLightDirty) {
        return;
    }
    internal::ActivateMaterial(pRenderContext, pMaterial, GetOriginal());
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
