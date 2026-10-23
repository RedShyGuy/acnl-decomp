#include "nw/gfx/gfx_SceneUpdater.h"

#include "nw/gfx/gfx_AnimBinding.h"
#include "nw/gfx/gfx_SceneContext.h"
#include "nw/gfx/gfx_SceneUpdateHelper.h"
#include "nw/gfx/gfx_SkeletalModel.h"

namespace nw {
namespace gfx {
namespace {
// the evaluation timings of the animation bindings (AnimBinding::Evaluate; names are ours)
const u8 EVALUATION_TIMING_BEFORE_WORLD_UPDATE = 0;
const u8 EVALUATION_TIMING_AFTER_SCENE_CULLING = 1;
} // namespace

// 0x009760EC
const TypeInfo nw::gfx::SceneUpdater::s_TypeInfo(&ISceneUpdater::s_TypeInfo);

// sets s_TypeInfo
// 0x0079B5AC (static initializer)

// 0x00491DBC slot 0x00 | nintendogs:bytes
// 0x00491CF4 (deleting dtor)
nw::gfx::SceneUpdater::~SceneUpdater()
{
}

// 0x00491B38 slot 0x14 (name is ours)
void nw::gfx::SceneUpdater::UpdateAll(SceneContext* pSceneContext)
{
    SceneContext::SceneNodeArray& animatableNodes = pSceneContext->m_AnimatableNodes;
    for (SceneNode** ppNode = animatableNodes.begin(); ppNode != animatableNodes.end(); ++ppNode) {
        (*ppNode)->m_AnimBinding->Evaluate(EVALUATION_TIMING_BEFORE_WORLD_UPDATE);
    }

    SceneContext::SceneNodeArray& sceneNodes = pSceneContext->m_SceneNodes;
    for (SceneNode** ppNode = sceneNodes.begin(); ppNode != sceneNodes.end(); ++ppNode) {
        SceneNode* pNode = *ppNode;
        pNode->m_pCallbacks->Invoke(pNode, pSceneContext);
        pNode->InheritTraversalResults();
        pNode->UpdateTransform(m_WorldMatrixUpdater.Get(), pSceneContext);
    }

    for (SceneNode** ppNode = animatableNodes.begin(); ppNode != animatableNodes.end(); ++ppNode) {
        (*ppNode)->m_AnimBinding->Evaluate(EVALUATION_TIMING_AFTER_SCENE_CULLING);
    }

    // a skeleton that models share is calculated once
    ut::MoveArray<SkeletalModel*>& skeletalModels = pSceneContext->m_SkeletalModels;
    for (int i = 0; i < skeletalModels.size(); ++i) {
        skeletalModels[i]->GetSkeleton()->m_IsWorldCalculated = false;
    }
    for (SkeletalModel** ppModel = skeletalModels.begin(); ppModel != skeletalModels.end(); ++ppModel) {
        SkeletalModel* pModel = *ppModel;
        if (pModel->m_IsWorldMatrixAnimation) {
            continue;
        }
        Skeleton* pSkeleton = pModel->GetSkeleton();
        if (pSkeleton->m_IsWorldCalculated) {
            continue;
        }
        m_SkeletonUpdater->UpdateWorld(pSkeleton, *m_WorldMatrixUpdater.Get());
        pSkeleton->m_IsWorldCalculated = true;
    }

    for (SceneNode** ppNode = animatableNodes.begin(); ppNode != animatableNodes.end(); ++ppNode) {
        (*ppNode)->UpdateFrame();
    }
}

// 0x00491314 slot 0x18 | nintendogs:bytes
void nw::gfx::SceneUpdater::SubmitView(RenderQueue* pQueue, SceneContext* pSceneContext, const Camera& camera, u8 layer,
                                       RenderSortMode renderSortMode)
{
    Model::IsVisibleModelDefaultFunctor isVisibleModelFunctor;
    SceneUpdateHelper::SubmitView(m_SkeletonUpdater.Get(), m_BillboardUpdater.Get(), pQueue, pSceneContext, camera, layer, layer,
                                  renderSortMode, m_DepthSortMode, &isVisibleModelFunctor);
}

// 0x00491368 slot 0x1C (name is ours)
void nw::gfx::SceneUpdater::SubmitView(RenderQueue* pQueue, SceneContext* pSceneContext, const Camera& camera, u8 layer, u8 particleLayer,
                                       RenderSortMode renderSortMode)
{
    Model::IsVisibleModelDefaultFunctor isVisibleModelFunctor;
    SceneUpdateHelper::SubmitView(m_SkeletonUpdater.Get(), m_BillboardUpdater.Get(), pQueue, pSceneContext, camera, layer, particleLayer,
                                  renderSortMode, m_DepthSortMode, &isVisibleModelFunctor);
}

// 0x004913BC slot 0x20 (name is ours)
void nw::gfx::SceneUpdater::SubmitView(RenderQueue* pQueue, SceneContext* pSceneContext, const Camera& camera, u8 layer, u8 particleLayer,
                                       Model::IsVisibleModelFunctor* pIsVisibleModelFunctor, RenderSortMode renderSortMode)
{
    SceneUpdateHelper::SubmitView(m_SkeletonUpdater.Get(), m_BillboardUpdater.Get(), pQueue, pSceneContext, camera, layer, particleLayer,
                                  renderSortMode, m_DepthSortMode, pIsVisibleModelFunctor);
}

// 0x007E685C | nintendogs (C)
template void SceneUpdateHelper::SubmitView<Model::IsVisibleModelDefaultFunctor>(
    SkeletonUpdater* pSkeletonUpdater, BillboardUpdater* pBillboardUpdater, RenderQueue* pQueue, SceneContext* pSceneContext,
    const Camera& camera, u8 layer, u8 particleLayer, ISceneUpdater::RenderSortMode renderSortMode, ISceneUpdater::DepthSortMode depthSortMode,
    Model::IsVisibleModelDefaultFunctor* pIsVisibleModelFunctor);

} // namespace gfx
} // namespace nw

// the instances of std::for_each that ARMCC calls out of line (with the sort modes other than the
// default; 1: SUBMIT_SORT_TRANSLUCENT_MESH, 2: SUBMIT_SORT_TRANSLUCENT_BY_MODEL)
// 0x008383AC (name is ours)
template nw::gfx::SceneUpdateHelper::SubmitViewFunctor<nw::gfx::Model::IsVisibleModelDefaultFunctor, 1>
std::for_each<nw::gfx::Model**, nw::gfx::SceneUpdateHelper::SubmitViewFunctor<nw::gfx::Model::IsVisibleModelDefaultFunctor, 1>>(nw::gfx::Model** first, nw::gfx::Model** last,
              nw::gfx::SceneUpdateHelper::SubmitViewFunctor<nw::gfx::Model::IsVisibleModelDefaultFunctor, 1> f);
// 0x00838E98 (name is ours)
template nw::gfx::SceneUpdateHelper::SubmitViewFunctor<nw::gfx::Model::IsVisibleModelDefaultFunctor, 2>
std::for_each<nw::gfx::Model**, nw::gfx::SceneUpdateHelper::SubmitViewFunctor<nw::gfx::Model::IsVisibleModelDefaultFunctor, 2>>(nw::gfx::Model** first, nw::gfx::Model** last,
              nw::gfx::SceneUpdateHelper::SubmitViewFunctor<nw::gfx::Model::IsVisibleModelDefaultFunctor, 2> f);
// 0x00837ED4 (name is ours)
template nw::gfx::SceneUpdateHelper::SubmitViewFunctor<nw::gfx::Model::IsVisibleModelFunctor, 1>
std::for_each<nw::gfx::Model**, nw::gfx::SceneUpdateHelper::SubmitViewFunctor<nw::gfx::Model::IsVisibleModelFunctor, 1>>(nw::gfx::Model** first, nw::gfx::Model** last,
              nw::gfx::SceneUpdateHelper::SubmitViewFunctor<nw::gfx::Model::IsVisibleModelFunctor, 1> f);
// 0x00838968 (name is ours)
template nw::gfx::SceneUpdateHelper::SubmitViewFunctor<nw::gfx::Model::IsVisibleModelFunctor, 2>
std::for_each<nw::gfx::Model**, nw::gfx::SceneUpdateHelper::SubmitViewFunctor<nw::gfx::Model::IsVisibleModelFunctor, 2>>(nw::gfx::Model** first, nw::gfx::Model** last,
              nw::gfx::SceneUpdateHelper::SubmitViewFunctor<nw::gfx::Model::IsVisibleModelFunctor, 2> f);
