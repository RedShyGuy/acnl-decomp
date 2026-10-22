#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_CalculatedTransform.h"
#include "nw/gfx/gfx_Skeleton.h"
#include "nw/ut/ut_MoveArray.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx16StandardSkeletonE @ 0x008D0688
// vtable 0x009026BC (vptr 0x009026C4), offset_to_top 0, 13 entries
// A skeleton that keeps its arrays itself (Create is from the reference symbols, the rest is ours).
class StandardSkeleton : public ::nw::gfx::Skeleton
{
public:
    // (inline) takes the buffers of the arrays
    StandardSkeleton(os::IAllocator* pAllocator, res::ResSkeleton resSkeleton, int maxCallbacks, bool isFixedSizeMemory,
                     TransformArray& localTransforms, TransformArray& worldTransforms, MatrixArray& worldMatrices,
                     MatrixArray& skinningMatrices)
        : Skeleton(pAllocator, resSkeleton, maxCallbacks, isFixedSizeMemory), m_LocalTransforms(localTransforms),
          m_WorldTransforms(worldTransforms), m_WorldMatrices(worldMatrices), m_SkinningMatrices(skinningMatrices),
          m_OriginalBones(resSkeleton)
    {
    }
    virtual ~StandardSkeleton();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073A840 slot 0x08
    virtual TransformArray* GetLocalTransforms();
    virtual const TransformArray* GetLocalTransforms() const { return &m_LocalTransforms; } // 0x0073A84C slot 0x10
    virtual TransformArray* GetWorldTransforms();
    virtual const TransformArray* GetWorldTransforms() const { return &m_WorldTransforms; } // 0x0073A854 slot 0x18
    virtual MatrixArray* GetWorldMatrices();
    virtual const MatrixArray* GetWorldMatrices() const { return &m_WorldMatrices; } // 0x0073A828 slot 0x20
    virtual MatrixArray* GetSkinningMatrices();
    virtual const MatrixArray* GetSkinningMatrices() const { return &m_SkinningMatrices; } // 0x0073A838 slot 0x28
    virtual res::ResSkeleton* GetOriginalBones();
    virtual const res::ResSkeleton* GetOriginalBones() const { return &m_OriginalBones; } // 0x0073A830 slot 0x30

    // a skeleton for the bones of the resource; the local transforms come from the caller, the
    // others start as identities (NULL: no memory)
    static StandardSkeleton* Create(res::ResSkeleton resSkeleton, int maxCallbacks, bool isFixedSizeMemory,
                                    TransformArray localTransforms, os::IAllocator* pAllocator); // 0x0049C520 | nintendogs:callseq [tier A]

    static const TypeInfo s_TypeInfo;

    TransformArray m_LocalTransforms;  // 0x1C
    TransformArray m_WorldTransforms;  // 0x2C
    MatrixArray m_WorldMatrices;       // 0x3C
    MatrixArray m_SkinningMatrices;    // 0x4C
    res::ResSkeleton m_OriginalBones;  // 0x5C
};
ASSERT_SIZE(StandardSkeleton, 0x60);
} // namespace gfx
} // namespace nw
