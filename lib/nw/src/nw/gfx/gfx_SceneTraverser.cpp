#include "nw/gfx/gfx_SceneTraverser.h"

#include "nw/gfx/gfx_AmbientLight.h"
#include "nw/gfx/gfx_Camera.h"
#include "nw/gfx/gfx_Fog.h"
#include "nw/gfx/gfx_FragmentLight.h"
#include "nw/gfx/gfx_HemiSphereLight.h"
#include "nw/gfx/gfx_ParticleEmitter.h"
#include "nw/gfx/gfx_ParticleModel.h"
#include "nw/gfx/gfx_ParticleSet.h"
#include "nw/gfx/gfx_SceneContext.h"
#include "nw/gfx/gfx_SkeletalModel.h"

namespace nw {
namespace gfx {
namespace {
// (inline) the node among the animated ones if it has an animation binding (name is ours)
inline void AddAnimatableNode(SceneContext* pSceneContext, SceneNode* pNode)
{
    if (pNode->m_AnimBinding.Get() != nullptr) {
        pSceneContext->m_AnimatableNodes.push_back(pNode);
    }
}
} // namespace

// 0x009760E8
const TypeInfo nw::gfx::SceneTraverser::s_TypeInfo(&ISceneVisitor::s_TypeInfo);

// sets s_TypeInfo
// 0x0079C994 (static initializer)

// 0x0049A988 slot 0x00 (name is ours)
// 0x0049A984 (deleting dtor)
nw::gfx::SceneTraverser::~SceneTraverser()
{
    // empty (in the original too)
}

// 0x00499CCC slot 0x0C (name is ours)
void nw::gfx::SceneTraverser::VisitSceneNode(SceneNode* pNode)
{
    m_pSceneContext->m_SceneNodes.push_back(pNode);
}

// 0x0049A3A0 slot 0x10 (name is ours)
void nw::gfx::SceneTraverser::VisitTransformNode(TransformNode* pNode)
{
    m_pSceneContext->m_SceneNodes.push_back(pNode);
}

// 0x0049A3BC slot 0x14 (name is ours)
void nw::gfx::SceneTraverser::VisitUnknown0x14(SceneNode* pNode)
{
    m_pSceneContext->m_SceneNodes.push_back(pNode);
    m_pSceneContext->m_Unknown0x18Nodes.push_back(pNode);
}

// 0x00499B20 slot 0x18 | nintendogs:bytes
void nw::gfx::SceneTraverser::VisitModel(Model* pModel)
{
    m_pSceneContext->m_SceneNodes.push_back(pModel);
    m_pSceneContext->m_Models.push_back(pModel);
    AddAnimatableNode(m_pSceneContext, pModel);
}

// 0x0049A240 slot 0x1C (name is ours)
void nw::gfx::SceneTraverser::VisitSkeletalModel(SkeletalModel* pModel)
{
    m_pSceneContext->m_SceneNodes.push_back(pModel);
    m_pSceneContext->m_Models.push_back(pModel);
    m_pSceneContext->m_SkeletalModels.push_back(pModel);
    AddAnimatableNode(m_pSceneContext, pModel);
}

// 0x00499B80 slot 0x20 (name is ours)
void nw::gfx::SceneTraverser::VisitCamera(Camera* pCamera)
{
    m_pSceneContext->m_SceneNodes.push_back(pCamera);
    m_pSceneContext->m_Cameras.push_back(pCamera);
    AddAnimatableNode(m_pSceneContext, pCamera);
}

// 0x0049A838 slot 0x24 (name is ours)
void nw::gfx::SceneTraverser::VisitFog(Fog* pFog)
{
    m_pSceneContext->m_SceneNodes.push_back(pFog);
    m_pSceneContext->m_Fogs.push_back(pFog);
    AddAnimatableNode(m_pSceneContext, pFog);
}

// 0x00499AC0 slot 0x28 | nintendogs:bytes
void nw::gfx::SceneTraverser::VisitLight(Light* pLight)
{
    m_pSceneContext->m_SceneNodes.push_back(pLight);
    m_pSceneContext->m_Lights.push_back(pLight);
    AddAnimatableNode(m_pSceneContext, pLight);
}

// 0x00499F80 slot 0x2C (name is ours)
void nw::gfx::SceneTraverser::VisitFragmentLight(FragmentLight* pLight)
{
    m_pSceneContext->m_SceneNodes.push_back(pLight);
    m_pSceneContext->m_Lights.push_back(pLight);
    m_pSceneContext->m_FragmentLights.push_back(pLight);
    AddAnimatableNode(m_pSceneContext, pLight);
}

// 0x00499E20 slot 0x30 (name is ours)
void nw::gfx::SceneTraverser::VisitAmbientLight(AmbientLight* pLight)
{
    m_pSceneContext->m_SceneNodes.push_back(pLight);
    m_pSceneContext->m_Lights.push_back(pLight);
    m_pSceneContext->m_AmbientLights.push_back(pLight);
    AddAnimatableNode(m_pSceneContext, pLight);
}

// 0x0049A4E4 slot 0x34 (name is ours)
void nw::gfx::SceneTraverser::VisitHemiSphereLight(HemiSphereLight* pLight)
{
    m_pSceneContext->m_SceneNodes.push_back(pLight);
    m_pSceneContext->m_Lights.push_back(pLight);
    m_pSceneContext->m_HemiSphereLights.push_back(pLight);
    AddAnimatableNode(m_pSceneContext, pLight);
}

// 0x00499CE8 slot 0x38 (name is ours)
void nw::gfx::SceneTraverser::VisitParticleSet(ParticleSet* pParticleSet)
{
    // a particle set takes over the traversal results of its parent
    if (pParticleSet->m_pParent != nullptr) {
        pParticleSet->m_TraversalResults = pParticleSet->m_pParent->m_TraversalResults;
    }
    m_pSceneContext->m_SceneNodes.push_back(pParticleSet);
    m_pSceneContext->m_ParticleSets.push_back(pParticleSet);
}

// 0x0049A644 slot 0x3C (name is ours)
void nw::gfx::SceneTraverser::VisitParticleEmitter(ParticleEmitter* pEmitter)
{
    m_pSceneContext->m_SceneNodes.push_back(pEmitter);
    m_pSceneContext->m_ParticleEmitters.push_back(pEmitter);
}

// 0x0049A0E0 slot 0x40 (name is ours)
void nw::gfx::SceneTraverser::VisitParticleModel(ParticleModel* pModel)
{
    m_pSceneContext->m_SceneNodes.push_back(pModel);
    m_pSceneContext->m_Models.push_back(pModel);
    m_pSceneContext->m_ParticleModels.push_back(pModel);
    AddAnimatableNode(m_pSceneContext, pModel);
}

// 0x0049A76C (name is ours)
void nw::gfx::SceneTraverser::End()
{
    m_pSceneContext = nullptr;
}

// 0x0049A778 | nintendogs:bytes [tier B]
void nw::gfx::SceneTraverser::Begin(SceneContext* pSceneContext)
{
    m_pSceneContext = pSceneContext;
    pSceneContext->Clear();
}

} // namespace gfx

namespace ut {
// (ARMCC calls these three out of line and inlines the others: specializations that are not inlined)
// 0x007E6150 | nintendogs:callgraph [tier C]
template <>
bool nw::ut::MoveArray<nw::gfx::Light*>::push_back(nw::gfx::Light* const& value)
{
    return PushBack(value);
}

// 0x007E6278 | nintendogs:callgraph [tier C]
template <>
bool nw::ut::MoveArray<nw::gfx::Model*>::push_back(nw::gfx::Model* const& value)
{
    return PushBack(value);
}

// 0x007E63A0 | nintendogs:callgraph [tier C]
template <>
bool nw::ut::MoveArray<nw::gfx::SceneNode*>::push_back(nw::gfx::SceneNode* const& value)
{
    return PushBack(value);
}
} // namespace ut
} // namespace nw
