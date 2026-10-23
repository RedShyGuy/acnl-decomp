#pragma once

#include <algorithm>

#include "decomp.h"
#include "nn/math/math_ARMv6.h"
#include "nw/gfx/gfx_BillboardUpdater.h"
#include "nw/gfx/gfx_Camera.h"
#include "nw/gfx/gfx_ISceneUpdater.h"
#include "nw/gfx/gfx_ParticleModel.h"
#include "nw/gfx/gfx_SceneContext.h"
#include "nw/gfx/gfx_SceneHelper.h"
#include "nw/gfx/gfx_SkeletalModel.h"
#include "nw/gfx/gfx_Skeleton.h"
#include "nw/gfx/gfx_SkeletonUpdater.h"

namespace nw {
namespace gfx {
// The work of SceneUpdater::SubmitView (the class name and SubmitView are from the reference
// symbols, the functor and the rest are ours).
class SceneUpdateHelper
{
public:
    // how the depths of the meshes are calculated (the sort modes of the scene updater)
    enum SubmitSort
    {
        SUBMIT_SORT_MESH,                  // every mesh at its own depth
        SUBMIT_SORT_TRANSLUCENT_MESH,      // the translucent meshes at their depth, the others at 0
        SUBMIT_SORT_TRANSLUCENT_BY_MODEL,  // the opaque meshes at their depth, the others at the one of the model
    };

    // (inline) the translucency kind of the material of a mesh
    static u8 GetTranslucencyKind(const Model* pModel, res::ResMesh mesh)
    {
        return static_cast<u8>(pModel->GetMaterial(mesh.ptr()->materialIndex)->GetResource(Material::RESOURCE_SHADING).ptr()->translucencyKind);
    }

    // (inline) the depth of a point of the world, like SceneHelper::CalculateDepth
    static f32 CalculateDepthOfPosition(const nn::math::VEC3& position, const Camera& camera)
    {
        nn::math::VEC3 v = position;
        nn::math::ARMv6::VEC3TransformAsm(&v, &camera.m_ViewMatrix, &v);
        const nn::math::MTX44& p = camera.m_ProjectionMatrix;
        const f32 z = p.m[2][1] * v.y + p.m[2][2] * v.z + (p.m[2][3] + p.m[2][0] * v.x);
        const f32 w = p.m[3][1] * v.y + p.m[3][2] * v.z + (p.m[3][3] + p.m[3][0] * v.x);
        const f32 depth = -z / w;
        if (depth > 1.0f) {
            return 1.0f;
        }
        if (depth < 0.0f) {
            return 0.0f;
        }
        return depth;
    }

    // The visible meshes of a model into the queue (for std::for_each over the models of a context):
    // the view-dependent matrices of its skeleton and its normal matrix first.
    template <typename TIsVisibleModelFunctor, int TSubmitSort>
    struct SubmitViewFunctor
    {
        DECOMP_ALWAYS_INLINE void operator()(Model* pModel)
        {
            SkeletalModel* pSkeletalModel = DynamicCast<SkeletalModel>(pModel);
            if (pSkeletalModel != nullptr) {
                pSkeletalModel->GetSkeleton()->m_IsWorldCalculated = false;
            }
            if (!(*m_pIsVisibleModelFunctor)(pModel)) {
                return;
            }
            if (pSkeletalModel != nullptr) {
                Skeleton* pSkeleton = pSkeletalModel->GetSkeleton();
                if (!pSkeletalModel->m_IsSharingSkeleton) {
                    m_pSkeletonUpdater->UpdateView(pSkeleton, *m_pBillboardUpdater, *m_pCamera);
                }
                const bool isModelCoordinate =
                    (res::ResSkeleton(pSkeletalModel->GetSkeleton()->GetResSceneObject().ptr()).ptr()->flags & res::ResSkeletonData::FLAG_IS_MODEL_COORDINATE) != 0;
                pModel->UpdateNormalMatrix(m_pCamera->m_ViewMatrix, !isModelCoordinate);
                const u8 layer = m_Layer | pModel->m_LayerId;
                if (m_pQueue == nullptr) {
                    return;
                }
                ut::internal::ResArray<res::ResMesh, ut::internal::ResArrayClassTraits> meshes = pModel->GetResMeshes();
                typename RenderQueue::CalculateDepthOfSkeletalModelFunctor calculateDepth = {pSkeletalModel, pModel->GetResModel()};
                f32 modelDepth = 0.0f;
                if (TSubmitSort == SUBMIT_SORT_TRANSLUCENT_BY_MODEL) {
                    modelDepth = CalculateDepthOfPosition(
                        nn::math::VEC3(pModel->m_WorldMatrix.m[0][3], pModel->m_WorldMatrix.m[1][3], pModel->m_WorldMatrix.m[2][3]), *m_pCamera);
                }
                for (const ut::Offset* pOffset = meshes.m_pBegin; pOffset != meshes.m_pEnd; ++pOffset) {
                    res::ResMesh mesh(pOffset->ToPtr());
                    if (!pModel->IsMeshVisible(mesh)) {
                        continue;
                    }
                    f32 depth;
                    if (TSubmitSort == SUBMIT_SORT_TRANSLUCENT_BY_MODEL) {
                        if (GetTranslucencyKind(pModel, mesh) != res::ResMaterialData::TRANSLUCENCY_KIND_OPAQUE) {
                            depth = modelDepth;
                        } else {
                            depth = calculateDepth.template operator()<typename RenderQueue::AlwaysCalculatingFunctor>(mesh, *m_pCamera);
                        }
                    } else if (TSubmitSort == SUBMIT_SORT_TRANSLUCENT_MESH) {
                        depth = calculateDepth.template operator()<typename RenderQueue::TranslucentCalculatingFunctor>(mesh, *m_pCamera);
                    } else {
                        depth = calculateDepth.template operator()<typename RenderQueue::AlwaysCalculatingFunctor>(mesh, *m_pCamera);
                    }
                    m_pQueue->Enqueue(RenderElement(mesh, pModel), depth, GetTranslucencyKind(pModel, mesh), layer);
                }
            } else {
                pModel->UpdateNormalMatrix(m_pCamera->m_ViewMatrix, false);
                u8 layer = (pModel != nullptr && pModel->GetRuntimeTypeInfo() == &ParticleModel::s_TypeInfo) ? m_ParticleLayer : m_Layer;
                layer |= pModel->m_LayerId;
                if (m_pQueue == nullptr) {
                    return;
                }
                ut::internal::ResArray<res::ResMesh, ut::internal::ResArrayClassTraits> meshes = pModel->GetResMeshes();
                res::ResModel resModel = pModel->GetResModel();
                f32 modelDepth = 0.0f;
                if (TSubmitSort == SUBMIT_SORT_TRANSLUCENT_BY_MODEL) {
                    modelDepth = CalculateDepthOfPosition(
                        nn::math::VEC3(pModel->m_WorldMatrix.m[0][3], pModel->m_WorldMatrix.m[1][3], pModel->m_WorldMatrix.m[2][3]), *m_pCamera);
                }
                for (const ut::Offset* pOffset = meshes.m_pBegin; pOffset != meshes.m_pEnd; ++pOffset) {
                    res::ResMesh mesh(pOffset->ToPtr());
                    if (!pModel->IsMeshVisible(mesh)) {
                        continue;
                    }
                    const u8 kind = GetTranslucencyKind(pModel, mesh);
                    f32 depth;
                    if (TSubmitSort == SUBMIT_SORT_TRANSLUCENT_BY_MODEL && kind != res::ResMaterialData::TRANSLUCENCY_KIND_OPAQUE) {
                        depth = modelDepth;
                    } else if (TSubmitSort == SUBMIT_SORT_TRANSLUCENT_MESH && kind != res::ResMaterialData::TRANSLUCENCY_KIND_TRANSLUCENT) {
                        depth = 0.0f;
                    } else {
                        const res::ResShapeData* pShape = static_cast<const res::ResShapeData*>(resModel.GetShape(mesh.ptr()->shapeIndex));
                        depth = SceneHelper::CalculateDepth(pShape->boundingBox.To<res::ResOrientedBoundingBoxData>()->center, pModel->m_WorldMatrix,
                                                            *m_pCamera);
                    }
                    m_pQueue->Enqueue(RenderElement(mesh, pModel), depth, kind, layer);
                }
            }
        }

        SkeletonUpdater* m_pSkeletonUpdater;                 // 0x00
        BillboardUpdater* m_pBillboardUpdater;               // 0x04
        RenderQueue* m_pQueue;                               // 0x08
        const Camera* m_pCamera;                             // 0x0C
        u8 m_Layer;                                          // 0x10
        u8 m_ParticleLayer;                                  // 0x11 the layer of particle models
        TIsVisibleModelFunctor* m_pIsVisibleModelFunctor;    // 0x14
    };

    template <typename TIsVisibleModelFunctor>
    static void SubmitView(SkeletonUpdater* pSkeletonUpdater, BillboardUpdater* pBillboardUpdater, RenderQueue* pQueue, SceneContext* pSceneContext,
                           const Camera& camera, u8 layer, u8 particleLayer, ISceneUpdater::RenderSortMode renderSortMode,
                           ISceneUpdater::DepthSortMode depthSortMode, TIsVisibleModelFunctor* pIsVisibleModelFunctor);
};

template <typename TIsVisibleModelFunctor>
void SceneUpdateHelper::SubmitView(SkeletonUpdater* pSkeletonUpdater, BillboardUpdater* pBillboardUpdater, RenderQueue* pQueue,
                                   SceneContext* pSceneContext, const Camera& camera, u8 layer, u8 particleLayer,
                                   ISceneUpdater::RenderSortMode renderSortMode, ISceneUpdater::DepthSortMode depthSortMode,
                                   TIsVisibleModelFunctor* pIsVisibleModelFunctor)
{
    ut::MoveArray<Model*>& models = pSceneContext->m_Models;
    if (renderSortMode == ISceneUpdater::RENDER_SORT_MODE_TRANSLUCENT_BY_MODEL) {
        SubmitViewFunctor<TIsVisibleModelFunctor, SUBMIT_SORT_TRANSLUCENT_BY_MODEL> functor = {
            pSkeletonUpdater, pBillboardUpdater, pQueue, &camera, layer, particleLayer, pIsVisibleModelFunctor};
        std::for_each(models.begin(), models.end(), functor);
    } else if (depthSortMode == ISceneUpdater::DEPTH_SORT_MODE_TRANSLUCENT_ONLY) {
        SubmitViewFunctor<TIsVisibleModelFunctor, SUBMIT_SORT_TRANSLUCENT_MESH> functor = {
            pSkeletonUpdater, pBillboardUpdater, pQueue, &camera, layer, particleLayer, pIsVisibleModelFunctor};
        std::for_each(models.begin(), models.end(), functor);
    } else {
        SubmitViewFunctor<TIsVisibleModelFunctor, SUBMIT_SORT_MESH> functor = {
            pSkeletonUpdater, pBillboardUpdater, pQueue, &camera, layer, particleLayer, pIsVisibleModelFunctor};
        std::for_each(models.begin(), models.end(), functor);
    }
}
} // namespace gfx
} // namespace nw
