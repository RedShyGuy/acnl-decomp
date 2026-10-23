#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/ut/ut_MoveArray.h"

namespace nw {
namespace gfx {
class SceneNode;
class Model;
class SkeletalModel;
class Camera;
class Fog;
class Light;
class FragmentLight;
class HemiSphereLight;
class AmbientLight;
class ParticleSet;
class ParticleEmitter;
class ParticleModel;

// RTTI N2nw3gfx12SceneContextE @ 0x008D05A4
// vtable 0x0090241C (vptr 0x00902424), offset_to_top 0, 2 entries
// The nodes of a scene by class, collected by the SceneTraverser for the updaters and renderers
// (member names are ours).
class SceneContext : public ::nw::gfx::GfxObject
{
public:
    class Builder;

    typedef ut::MoveArray<SceneNode*> SceneNodeArray;

    // (inline) the context with the arrays (they are moved into it)
    SceneContext(os::IAllocator* pAllocator, SceneNodeArray& sceneNodes, SceneNodeArray& unknown0x18Nodes, ut::MoveArray<Model*>& models,
                 ut::MoveArray<SkeletalModel*>& skeletalModels, ut::MoveArray<Camera*>& cameras, ut::MoveArray<Fog*>& fogs,
                 ut::MoveArray<Light*>& lights, ut::MoveArray<FragmentLight*>& fragmentLights, ut::MoveArray<Light*>& vertexLights,
                 ut::MoveArray<HemiSphereLight*>& hemiSphereLights, ut::MoveArray<AmbientLight*>& ambientLights,
                 ut::MoveArray<ParticleSet*>& particleSets, ut::MoveArray<ParticleEmitter*>& particleEmitters,
                 ut::MoveArray<ParticleModel*>& particleModels, SceneNodeArray& animatableNodes)
        : GfxObject(pAllocator), m_SceneNodes(sceneNodes), m_Unknown0x18Nodes(unknown0x18Nodes), m_Models(models),
          m_SkeletalModels(skeletalModels), m_Cameras(cameras), m_Fogs(fogs), m_Lights(lights), m_FragmentLights(fragmentLights),
          m_VertexLights(vertexLights), m_HemiSphereLights(hemiSphereLights), m_AmbientLights(ambientLights),
          m_ParticleSets(particleSets), m_ParticleEmitters(particleEmitters), m_ParticleModels(particleModels),
          m_AnimatableNodes(animatableNodes)
    {
    }
    virtual ~SceneContext();

    // (inline) all arrays emptied
    void Clear()
    {
        m_SceneNodes.clear();
        m_Unknown0x18Nodes.clear();
        m_Models.clear();
        m_SkeletalModels.clear();
        m_Lights.clear();
        m_FragmentLights.clear();
        m_VertexLights.clear();
        m_HemiSphereLights.clear();
        m_AmbientLights.clear();
        m_Cameras.clear();
        m_Fogs.clear();
        m_ParticleSets.clear();
        m_ParticleEmitters.clear();
        m_ParticleModels.clear();
        m_AnimatableNodes.clear();
    }

    SceneNodeArray m_SceneNodes;                         // 0x08 every node
    SceneNodeArray m_Unknown0x18Nodes;                   // 0x18 the nodes of ISceneVisitor slot 0x14
    ut::MoveArray<Model*> m_Models;                      // 0x28 also the skeletal and particle models
    ut::MoveArray<SkeletalModel*> m_SkeletalModels;      // 0x38
    ut::MoveArray<Camera*> m_Cameras;                    // 0x48
    ut::MoveArray<Fog*> m_Fogs;                          // 0x58
    ut::MoveArray<Light*> m_Lights;                      // 0x68 every light
    ut::MoveArray<FragmentLight*> m_FragmentLights;      // 0x78
    ut::MoveArray<Light*> m_VertexLights;                // 0x88
    ut::MoveArray<HemiSphereLight*> m_HemiSphereLights;  // 0x98
    ut::MoveArray<AmbientLight*> m_AmbientLights;        // 0xA8
    ut::MoveArray<ParticleSet*> m_ParticleSets;          // 0xB8
    ut::MoveArray<ParticleEmitter*> m_ParticleEmitters;  // 0xC8
    ut::MoveArray<ParticleModel*> m_ParticleModels;      // 0xD8
    SceneNodeArray m_AnimatableNodes;                    // 0xE8 the nodes with an animation binding
};
ASSERT_OFFSET(SceneContext, m_Cameras, 0x48);
ASSERT_OFFSET(SceneContext, m_AnimatableNodes, 0xE8);
ASSERT_SIZE(SceneContext, 0xF8);
} // namespace gfx
} // namespace nw
