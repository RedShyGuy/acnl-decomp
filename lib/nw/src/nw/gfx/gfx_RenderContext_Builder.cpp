#include "nw/gfx/gfx_RenderContext_Builder.h"

#include <new>

#include "nw/gfx/gfx_ParticleMaterialActivator.h"

namespace nw {
namespace gfx {
// 0x00496E6C | nintendogs:callseq [tier A]
nw::gfx::RenderContext* nw::gfx::RenderContext::Builder::Create(os::IAllocator* pAllocator)
{
    void* pMemory = pAllocator->Alloc(sizeof(RenderContext), 4);
    if (pMemory == nullptr) {
        return nullptr;
    }
    void* pProgramMemory = pAllocator->Alloc(sizeof(ShaderProgram), 4);
    if (pProgramMemory == nullptr) {
        return nullptr;
    }
    ShaderProgram* pProgram = new (pProgramMemory) ShaderProgram(pAllocator);
    if (m_pParticleMaterialActivator == nullptr) {
        m_pParticleMaterialActivator = ParticleMaterialActivator::Create(pAllocator);
    }

    SceneEnvironment::Description description;
    {
        void* pBuffer = m_MaxVertexLights != 0 ? pAllocator->Alloc(m_MaxVertexLights * sizeof(Light*), 32) : nullptr;
        ut::MoveArray<Light*> vertexLights(pBuffer, m_MaxVertexLights, pAllocator);
        description.vertexLights = vertexLights;
    }
    {
        void* pBuffer = m_MaxCameras != 0 ? pAllocator->Alloc(m_MaxCameras * sizeof(Camera*), 32) : nullptr;
        ut::MoveArray<Camera*> cameras(pBuffer, m_MaxCameras, pAllocator);
        description.cameras = cameras;
    }
    {
        void* pBuffer = m_MaxFogs != 0 ? pAllocator->Alloc(m_MaxFogs * sizeof(Fog*), 32) : nullptr;
        ut::MoveArray<Fog*> fogs(pBuffer, m_MaxFogs, pAllocator);
        description.fogs = fogs;
    }
    {
        void* pBuffer = m_MaxLightSets != 0 ? pAllocator->Alloc(m_MaxLightSets * sizeof(LightSet*), 32) : nullptr;
        ut::MoveArray<LightSet*> lightSets(pBuffer, m_MaxLightSets, pAllocator);
        description.lightSets = lightSets;
    }
    return new (pMemory) RenderContext(pAllocator, GfxPtr<ShaderProgram>(pProgram), description,
                                       GfxPtr<IMaterialActivator>(m_pParticleMaterialActivator));
}

} // namespace gfx
} // namespace nw
