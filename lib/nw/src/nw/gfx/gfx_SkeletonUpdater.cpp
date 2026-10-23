#include "nw/gfx/gfx_SkeletonUpdater.h"

#include <cmath>

#include "nn/math/math_ARMv6.h"
#include "nw/gfx/gfx_BillboardUpdater.h"
#include "nw/gfx/gfx_Camera.h"
#include "nw/gfx/gfx_Skeleton.h"
#include "nw/gfx/gfx_TransformNode.h"
#include "nw/gfx/gfx_WorldMatrixUpdater.h"

namespace nw {
namespace gfx {
namespace {
// the smallest squared length of a world scale (a smaller scale keeps the signs with this length)
const f32 WORLD_SCALE_EPSILON = 1.00000011e-6f;

// (inline) the world transform and matrix of one bone
DECOMP_ALWAYS_INLINE void UpdateBoneWorld(const WorldMatrixUpdater& worldMatrixUpdater, Skeleton* pSkeleton, res::ResSkeleton resSkeleton,
                                          bool isModelCoordinate, Skeleton::TransformArray* pLocalTransforms,
                                          Skeleton::TransformArray* pWorldTransforms, nn::math::MTX34* pWorldMatrix, int boneIdx,
                                          u8 scalingRule)
{
    pSkeleton->m_pPreCalculateCallbacks->Invoke(pSkeleton, boneIdx);
    CalculatedTransform& localTransform = (*pLocalTransforms)[boneIdx];
    if (localTransform.IsEnabledFlags(CalculatedTransform::FLAG_UNKNOWN_0x1)) {
        res::ResBone bone = resSkeleton.GetBone(boneIdx);
        const int parentId = bone.ptr()->parentId;
        const CalculatedTransform* pParentWorldTransform;
        const CalculatedTransform* pParentLocalTransform;
        if (parentId != -1) {
            pParentWorldTransform = &(*pWorldTransforms)[parentId];
            pParentLocalTransform = &(*pLocalTransforms)[parentId];
        } else if (isModelCoordinate) {
            pParentWorldTransform = &CalculatedTransform::Identity();
            pParentLocalTransform = &CalculatedTransform::Identity();
        } else {
            pParentWorldTransform = &pSkeleton->m_pOwner->m_WorldTransform;
            pParentLocalTransform = &pSkeleton->m_pOwner->m_Transform;
        }

        CalculatedTransform& worldTransform = (*pWorldTransforms)[boneIdx];
        nn::math::VEC3 scale;
        switch (scalingRule) {
        case res::ResSkeletonData::SCALING_RULE_MAYA:
            if (bone.ptr()->flags & res::ResBoneData::FLAG_SEGMENT_SCALE_COMPENSATE) {
                worldMatrixUpdater.CalculateWorldMayaSsc(&worldTransform.m_TransformMatrix, &scale, localTransform, *pParentWorldTransform,
                                                         *pParentLocalTransform);
            } else {
                worldMatrixUpdater.CalculateWorldBasic(&worldTransform.m_TransformMatrix, &scale, localTransform, *pParentWorldTransform,
                                                       *pParentLocalTransform);
            }
            break;
        case res::ResSkeletonData::SCALING_RULE_STANDARD:
            worldMatrixUpdater.CalculateWorldBasic(&worldTransform.m_TransformMatrix, &scale, localTransform, *pParentWorldTransform,
                                                   *pParentLocalTransform);
            break;
        default:
            worldMatrixUpdater.CalculateWorldXsi(&worldTransform.m_TransformMatrix, &scale, localTransform, *pParentWorldTransform,
                                                 *pParentLocalTransform);
            break;
        }
        if (scale.x * scale.x + scale.y * scale.y + scale.z * scale.z < WORLD_SCALE_EPSILON) {
            scale.x = scale.x < 0.0f ? -WORLD_SCALE_EPSILON : WORLD_SCALE_EPSILON;
            scale.y = scale.y < 0.0f ? -WORLD_SCALE_EPSILON : WORLD_SCALE_EPSILON;
            scale.z = scale.z < 0.0f ? -WORLD_SCALE_EPSILON : WORLD_SCALE_EPSILON;
        }
        worldTransform.m_Scale = scale;
        nn::math::ARMv6::MTX34MultScaleAsm(pWorldMatrix, &worldTransform.m_TransformMatrix, &localTransform.m_Scale);
        worldTransform.DisableFlags(CalculatedTransform::FLAG_IS_IDENTITY | CalculatedTransform::FLAG_IS_ROTATE_TRANSLATE_ZERO |
                                    CalculatedTransform::FLAG_IS_ROTATE_ZERO | CalculatedTransform::FLAG_IS_TRANSLATE_ZERO |
                                    CalculatedTransform::FLAG_IS_SCALE_ONE | CalculatedTransform::FLAG_IS_UNIFORM_SCALE);
        worldTransform.UpdateScaleFlags();
    }
    pSkeleton->m_pPostCalculateCallbacks->Invoke(pSkeleton, boneIdx);
}
} // namespace

// 0x0049BF38 slot 0x00 (name is ours)
// 0x0049BF34 (deleting dtor)
nw::gfx::SkeletonUpdater::~SkeletonUpdater()
{
    // empty (in the original too)
}

// 0x00738BD8 (name is ours)
void nw::gfx::SkeletonUpdater::UpdateView(Skeleton* pSkeleton, const BillboardUpdater& billboardUpdater, const Camera& camera) const
{
    if (pSkeleton->m_IsViewIndependent) {
        return;
    }
    res::ResSkeleton resSkeleton(pSkeleton->GetResSceneObject().ptr());
    bool isViewDependent = false;
    Skeleton::TransformArray* pLocalTransforms = pSkeleton->GetLocalTransforms();
    Skeleton::TransformArray* pWorldTransforms = pSkeleton->GetWorldTransforms();
    const nn::math::MTX34& cameraMatrix = camera.m_WorldMatrix;
    const nn::math::VEC3 cameraPosition(cameraMatrix.m[0][3], cameraMatrix.m[1][3], cameraMatrix.m[2][3]);
    int boneIdx = 0;
    Skeleton::MatrixArray* pWorldMatrices = pSkeleton->GetWorldMatrices();
    for (nn::math::MTX34* pWorldMatrix = pWorldMatrices->begin(); pWorldMatrix != pWorldMatrices->end(); ++pWorldMatrix, ++boneIdx) {
        const CalculatedTransform& localTransform = (*pLocalTransforms)[boneIdx];
        if (!localTransform.IsEnabledFlags(CalculatedTransform::FLAG_UNKNOWN_0x1 | CalculatedTransform::FLAG_UNKNOWN_0x1000)) {
            continue;
        }
        const res::ResBoneData* pBone = resSkeleton.GetBone(boneIdx).ptr();
        const res::ResBone::BillboardMode mode = static_cast<res::ResBone::BillboardMode>(pBone->billboardMode);
        if (mode != res::ResBone::BILLBOARD_MODE_OFF) {
            const CalculatedTransform& worldTransform = (*pWorldTransforms)[boneIdx];
            billboardUpdater.Update(pWorldMatrix, camera.m_ViewMatrix, camera.m_InverseViewMatrix, cameraPosition, worldTransform,
                                    localTransform, mode);
            if (!worldTransform.IsEnabledFlags(CalculatedTransform::FLAG_IS_SCALE_ONE)) {
                // the world scale (the lengths of the columns) applied to the turned axes
                nn::math::MTX34 scaled;
                nn::math::ARMv6::MTX34MultScaleAsm(&scaled, &worldTransform.m_TransformMatrix, &localTransform.m_Scale);
                const f32 x = scaled.m[0][0] * scaled.m[0][0] + scaled.m[1][0] * scaled.m[1][0] + scaled.m[2][0] * scaled.m[2][0];
                const f32 y = scaled.m[0][1] * scaled.m[0][1] + scaled.m[1][1] * scaled.m[1][1] + scaled.m[2][1] * scaled.m[2][1];
                const f32 z = scaled.m[0][2] * scaled.m[0][2] + scaled.m[1][2] * scaled.m[1][2] + scaled.m[2][2] * scaled.m[2][2];
                nn::math::MTX34 scale(std::sqrt(x), 0.0f, 0.0f, 0.0f, 0.0f, std::sqrt(y), 0.0f, 0.0f, 0.0f, 0.0f, std::sqrt(z), 0.0f);
                nn::math::ARMv6::MTX33MultAsm(pWorldMatrix, pWorldMatrix, &scale);
            }
            isViewDependent = true;
        }
        const u32 SKINNING = res::ResBoneData::FLAG_UNKNOWN_0x40 | res::ResBoneData::FLAG_HAS_SKINNING_MATRIX;
        if ((pBone->flags & SKINNING) == SKINNING) {
            nn::math::ARMv6::MTX34MultAsm(&(*pSkeleton->GetSkinningMatrices())[boneIdx], pWorldMatrix, &pBone->inverseBaseMatrix);
            isViewDependent = true;
        }
    }
    if (!isViewDependent) {
        pSkeleton->m_IsViewIndependent = true;
    }
}

// 0x00738E08 | mk7dlp:callseq
void nw::gfx::SkeletonUpdater::UpdateWorld(Skeleton* pSkeleton, const WorldMatrixUpdater& worldMatrixUpdater) const
{
    res::ResSkeleton resSkeleton(pSkeleton->GetResSceneObject().ptr());
    const bool isModelCoordinate = (resSkeleton.ptr()->flags & res::ResSkeletonData::FLAG_IS_MODEL_COORDINATE) != 0;
    Skeleton::TransformArray* pLocalTransforms = pSkeleton->GetLocalTransforms();
    Skeleton::TransformArray* pWorldTransforms = pSkeleton->GetWorldTransforms();
    Skeleton::MatrixArray* pWorldMatrices = pSkeleton->GetWorldMatrices();
    nn::math::MTX34* pBegin = pWorldMatrices->begin();
    nn::math::MTX34* pEnd = pWorldMatrices->end();
    int boneIdx = 0;
    switch (static_cast<u8>(resSkeleton.ptr()->scalingRule)) {
    case res::ResSkeletonData::SCALING_RULE_MAYA:
        for (nn::math::MTX34* pWorldMatrix = pBegin; pWorldMatrix != pEnd; ++pWorldMatrix, ++boneIdx) {
            UpdateBoneWorld(worldMatrixUpdater, pSkeleton, resSkeleton, isModelCoordinate, pLocalTransforms, pWorldTransforms, pWorldMatrix,
                            boneIdx, res::ResSkeletonData::SCALING_RULE_MAYA);
        }
        break;
    case res::ResSkeletonData::SCALING_RULE_STANDARD:
        for (nn::math::MTX34* pWorldMatrix = pBegin; pWorldMatrix != pEnd; ++pWorldMatrix, ++boneIdx) {
            UpdateBoneWorld(worldMatrixUpdater, pSkeleton, resSkeleton, isModelCoordinate, pLocalTransforms, pWorldTransforms, pWorldMatrix,
                            boneIdx, res::ResSkeletonData::SCALING_RULE_STANDARD);
        }
        break;
    default:
        for (nn::math::MTX34* pWorldMatrix = pBegin; pWorldMatrix != pEnd; ++pWorldMatrix, ++boneIdx) {
            UpdateBoneWorld(worldMatrixUpdater, pSkeleton, resSkeleton, isModelCoordinate, pLocalTransforms, pWorldTransforms, pWorldMatrix,
                            boneIdx, res::ResSkeletonData::SCALING_RULE_SOFTIMAGE);
        }
        break;
    }
}

} // namespace gfx
} // namespace nw
