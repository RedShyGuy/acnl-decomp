#include "nw/gfx/gfx_StandardSkeleton.h"

namespace nw {
namespace gfx {
// 0x0097F4A4
const TypeInfo nw::gfx::StandardSkeleton::s_TypeInfo(&Skeleton::s_TypeInfo);

// sets s_TypeInfo
// 0x0079DAA4 (static initializer)

// 0x0049C4F8 slot 0x1C (name is ours)
nw::gfx::Skeleton::MatrixArray* nw::gfx::StandardSkeleton::GetWorldMatrices()
{
    return &m_WorldMatrices;
}

// 0x0049C500 slot 0x2C (name is ours)
nw::gfx::res::ResSkeleton* nw::gfx::StandardSkeleton::GetOriginalBones()
{
    return &m_OriginalBones;
}

// 0x0049C508 slot 0x24 (name is ours)
nw::gfx::Skeleton::MatrixArray* nw::gfx::StandardSkeleton::GetSkinningMatrices()
{
    return &m_SkinningMatrices;
}

// 0x0049C510 slot 0x0C (name is ours)
nw::gfx::Skeleton::TransformArray* nw::gfx::StandardSkeleton::GetLocalTransforms()
{
    return &m_LocalTransforms;
}

// 0x0049C518 slot 0x14 (name is ours)
nw::gfx::Skeleton::TransformArray* nw::gfx::StandardSkeleton::GetWorldTransforms()
{
    return &m_WorldTransforms;
}

// 0x0049C520 | nintendogs:callseq [tier A]
nw::gfx::StandardSkeleton* nw::gfx::StandardSkeleton::Create(res::ResSkeleton resSkeleton, int maxCallbacks, bool isFixedSizeMemory,
                                                             TransformArray localTransforms, os::IAllocator* pAllocator)
{
    void* pMemory = pAllocator->Alloc(sizeof(StandardSkeleton), 4);

    s32 boneCount = resSkeleton.GetBoneCount();
    void* pBuffer = boneCount != 0 ? pAllocator->Alloc(boneCount * sizeof(CalculatedTransform), 32) : nullptr;
    TransformArray worldTransforms(pBuffer, boneCount, pAllocator);
    boneCount = resSkeleton.GetBoneCount();
    pBuffer = boneCount != 0 ? pAllocator->Alloc(boneCount * sizeof(nn::math::MTX34), 32) : nullptr;
    MatrixArray worldMatrices(pBuffer, boneCount, pAllocator);
    boneCount = resSkeleton.GetBoneCount();
    pBuffer = boneCount != 0 ? pAllocator->Alloc(boneCount * sizeof(nn::math::MTX34), 32) : nullptr;
    MatrixArray skinningMatrices(pBuffer, boneCount, pAllocator);

    for (int i = 0; i < resSkeleton.GetBoneCount(); ++i) {
        worldTransforms.PushBackFast(CalculatedTransform::Identity());
        worldMatrices.PushBackFast(nn::math::MTX34::Identity());
        skinningMatrices.PushBackFast(nn::math::MTX34::Identity());
    }

    if (pMemory == nullptr) {
        return nullptr;
    }
    return new (pMemory) StandardSkeleton(pAllocator, resSkeleton, maxCallbacks, isFixedSizeMemory, localTransforms, worldTransforms,
                                          worldMatrices, skinningMatrices);
}

// 0x0049CCA0 slot 0x00 (name is ours)
// 0x0049CBA4 (deleting dtor)
nw::gfx::StandardSkeleton::~StandardSkeleton()
{
    // empty (in the original too: only the arrays)
}

} // namespace gfx

namespace ut {
// (ARMCC calls it out of line, GCC inlines it)
// 0x007E5E58 | nintendogs:bytes
template MoveArray<nn::math::MTX34>::~MoveArray();
} // namespace ut
} // namespace nw
