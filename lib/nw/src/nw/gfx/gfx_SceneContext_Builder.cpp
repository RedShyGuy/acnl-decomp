#include "nw/gfx/gfx_SceneContext_Builder.h"

#include <new>

namespace nw {
namespace gfx {
namespace {
// (inline) the array replaced by one with a buffer of count elements that does not grow (name is ours)
template <typename T>
inline void SetFixedSize(ut::MoveArray<T>& array, s32 count, os::IAllocator* pAllocator)
{
    void* pBuffer = count != 0 ? pAllocator->Alloc(count * sizeof(T), 32) : nullptr;
    ut::MoveArray<T> fixed(pBuffer, count, pAllocator);
    array = fixed;
}
} // namespace

// 0x0048F6A4 (name is ours)
nw::gfx::SceneContext* nw::gfx::SceneContext::Builder::Create(os::IAllocator* pAllocator)
{
    SceneNodeArray sceneNodes(pAllocator);
    SceneNodeArray unknown0x18Nodes(pAllocator);
    ut::MoveArray<Model*> models(pAllocator);
    ut::MoveArray<SkeletalModel*> skeletalModels(pAllocator);
    ut::MoveArray<Light*> lights(pAllocator);
    ut::MoveArray<FragmentLight*> fragmentLights(pAllocator);
    ut::MoveArray<Light*> vertexLights(pAllocator);
    ut::MoveArray<HemiSphereLight*> hemiSphereLights(pAllocator);
    ut::MoveArray<AmbientLight*> ambientLights(pAllocator);
    ut::MoveArray<Camera*> cameras(pAllocator);
    ut::MoveArray<Fog*> fogs(pAllocator);
    ut::MoveArray<ParticleSet*> particleSets(pAllocator);
    ut::MoveArray<ParticleEmitter*> particleEmitters(pAllocator);
    ut::MoveArray<ParticleModel*> particleModels(pAllocator);
    SceneNodeArray animatableNodes(pAllocator);
    if (m_IsFixedSizeMemory) {
        SetFixedSize(sceneNodes, m_MaxSceneNodes, pAllocator);
        SetFixedSize(unknown0x18Nodes, m_MaxUnknown0x18Nodes, pAllocator);
        SetFixedSize(models, m_MaxModels, pAllocator);
        SetFixedSize(skeletalModels, m_MaxSkeletalModels, pAllocator);
        SetFixedSize(lights, m_MaxLights, pAllocator);
        SetFixedSize(fragmentLights, m_MaxFragmentLights, pAllocator);
        SetFixedSize(vertexLights, m_MaxVertexLights, pAllocator);
        SetFixedSize(hemiSphereLights, m_MaxHemiSphereLights, pAllocator);
        SetFixedSize(ambientLights, m_MaxAmbientLights, pAllocator);
        SetFixedSize(cameras, m_MaxCameras, pAllocator);
        SetFixedSize(fogs, m_MaxFogs, pAllocator);
        SetFixedSize(particleSets, m_MaxParticleSets, pAllocator);
        SetFixedSize(particleEmitters, m_MaxParticleEmitters, pAllocator);
        SetFixedSize(particleModels, m_MaxParticleModels, pAllocator);
        SetFixedSize(animatableNodes, m_MaxAnimatableNodes, pAllocator);
    }
    void* pMemory = pAllocator->Alloc(sizeof(SceneContext), 4);
    if (pMemory == nullptr) {
        return nullptr;
    }
    return new (pMemory) SceneContext(pAllocator, sceneNodes, unknown0x18Nodes, models, skeletalModels, cameras, fogs, lights,
                                      fragmentLights, vertexLights, hemiSphereLights, ambientLights, particleSets, particleEmitters,
                                      particleModels, animatableNodes);
}

} // namespace gfx
} // namespace nw
