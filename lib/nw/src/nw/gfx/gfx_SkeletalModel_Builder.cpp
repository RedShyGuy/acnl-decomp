#include "nw/gfx/gfx_SkeletalModel_Builder.h"

#include <new>

#include "nw/gfx/gfx_StandardSkeleton.h"

// 0x00497C5C | nintendogs:callseq [tier A]
nw::gfx::SkeletalModel* nw::gfx::SkeletalModel::Builder::Create(SceneNode* pParent, res::ResSceneObject resource,
                                                                os::IAllocator* pAllocator)
{
    GfxPtr<Skeleton> skeleton;
    bool isSharingSkeleton = true;
    if (m_pSharedSkeleton != nullptr) {
        skeleton = GfxPtr<Skeleton>(m_pSharedSkeleton, false);
    } else {
        // a skeleton of its own, the local transforms from the bones
        const res::ResSkeletonData* pResSkeleton =
            static_cast<const res::ResSkeletalModelData*>(resource.ptr())->skeleton.To<res::ResSkeletonData>();
        const int boneCount = pResSkeleton->boneCount;
        void* pBuffer = boneCount != 0 ? pAllocator->Alloc(boneCount * sizeof(CalculatedTransform), 32) : nullptr;
        Skeleton::TransformArray localTransforms(pBuffer, boneCount, pAllocator);
        ut::ResDicPatricia bones = {pResSkeleton->boneDic.To<ut::ResDicPatricia::Data>()};
        for (int i = 0; i < pResSkeleton->boneCount; ++i) {
            localTransforms.PushBackFast(CalculatedTransform(res::ResBone(bones.GetDataByIndex(i))));
        }
        skeleton = GfxPtr<Skeleton>(StandardSkeleton::Create(res::ResSkeleton(pResSkeleton), m_Description.maxCallbacks,
                                                             m_Description.isFixedSizeMemory, localTransforms, pAllocator));
        isSharingSkeleton = false;
    }

    void* pMemory = pAllocator->Alloc(sizeof(SkeletalModel), 4);
    SkeletalModel* pModel = nullptr;
    if (pMemory != nullptr) {
        pModel = new (pMemory) SkeletalModel(pAllocator, res::ResModel(resource.ptr()), m_Description, skeleton, isSharingSkeleton);
    }
    pModel->Initialize(pAllocator);
    if (pParent != nullptr) {
        pParent->AttachChild(pModel);
    }
    return pModel;
}
