#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nw/gfx/gfx_CalculatedTransform.h"
#include "nw/gfx/gfx_NodeCallback.h"
#include "nw/gfx/gfx_SceneObject.h"
#include "nw/gfx/res/gfx_ResSkeleton.h"
#include "nw/ut/ut_MoveArray.h"

namespace nw {
namespace gfx {
class TransformNode;

// RTTI N2nw3gfx8SkeletonE @ 0x008D0814
// vtable 0x00902AAC (vptr 0x00902AB4), offset_to_top 0, 13 entries
// The bones of a model: per bone the local and world transform, the world and the skinning matrix
// (the arrays are in the derived class), and callbacks per bone (with its index) for the world
// update. The class name is from the RTTI, the rest is ours.
class Skeleton : public ::nw::gfx::SceneObject
{
public:
    typedef NodeCallbackList<Skeleton, int> CallbackList;
    typedef ut::MoveArray<CalculatedTransform> TransformArray;
    typedef ut::MoveArray<nn::math::MTX34> MatrixArray;

    // (inline)
    Skeleton(os::IAllocator* pAllocator, res::ResSkeleton resSkeleton, int maxCallbacks, bool isFixedSizeMemory)
        : SceneObject(pAllocator, res::ResSceneObject(resSkeleton.ptr())), m_pOwner(nullptr), m_pPreCalculateCallbacks(nullptr),
          m_pPostCalculateCallbacks(nullptr), m_Unknown0x18(false), m_Unknown0x19(false)
    {
        CreateCallbacks(pAllocator, maxCallbacks, isFixedSizeMemory);
    }
    virtual ~Skeleton();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073C960 slot 0x08
    virtual TransformArray* GetLocalTransforms() = 0; // slot 0x0C
    virtual const TransformArray* GetLocalTransforms() const = 0; // slot 0x10
    virtual TransformArray* GetWorldTransforms() = 0; // slot 0x14
    virtual const TransformArray* GetWorldTransforms() const = 0; // slot 0x18
    virtual MatrixArray* GetWorldMatrices() = 0; // slot 0x1C
    virtual const MatrixArray* GetWorldMatrices() const = 0; // slot 0x20
    virtual MatrixArray* GetSkinningMatrices() = 0; // slot 0x24
    virtual const MatrixArray* GetSkinningMatrices() const = 0; // slot 0x28
    // the bones of the resource (their original transforms)
    virtual res::ResSkeleton* GetOriginalBones() = 0; // slot 0x2C
    virtual const res::ResSkeleton* GetOriginalBones() const = 0; // slot 0x30

    void CreateCallbacks(os::IAllocator* pAllocator, int maxCallbacks, bool isFixedSizeMemory);

    static const TypeInfo s_TypeInfo;

    TransformNode* m_pOwner;                // 0xC the model (the parent of the root bones)
    CallbackList* m_pPreCalculateCallbacks;  // 0x10 per bone before its world transform is calculated
    CallbackList* m_pPostCalculateCallbacks; // 0x14
    bool m_Unknown0x18;                     // 0x18
    bool m_Unknown0x19;                     // 0x19
};
ASSERT_SIZE(Skeleton, 0x1C);
} // namespace gfx
} // namespace nw
