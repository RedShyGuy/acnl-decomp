#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Transform.h"
#include "nw/gfx/gfx_CalculatedTransform.h"
#include "nw/gfx/gfx_SceneNode.h"
#include "nw/gfx/res/gfx_ResSceneObject.h"
#include "nw/os/os_MemorySizeCalculator.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx13TransformNodeE @ 0x008D0634
// vtable 0x00902598 (vptr 0x009025A0), offset_to_top 0, 11 entries
// A scene node with a local transform and the world transform and matrix calculated from it and the
// parents; callbacks run after the calculation. The function names are from the reference symbols,
// the rest is ours.
class TransformNode : public ::nw::gfx::SceneNode
{
public:
    // (the same fields as the one of SceneNode; name from the reference symbols)
    struct Description : public SceneNode::Description
    {
    };

    TransformNode(os::IAllocator* pAllocator, res::ResTransformNode resNode, const Description& description); // 0x0049926C | nintendogs:callgraph [tier A]
    TransformNode(); // ctor address unknown (for the derived classes not written yet)
    virtual ~TransformNode();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x00738B80 slot 0x08
    virtual void UpdateTransform(WorldMatrixUpdater* pWorldMatrixUpdater, SceneContext* pSceneContext);
    virtual void Accept(ISceneVisitor* pVisitor);
    virtual const nn::math::MTX34& TrackbackWorldMatrix() const { return m_WorldMatrix; } // 0x00738B8C slot 0x14
    virtual const CalculatedTransform& TrackbackWorldTransform() const { return m_WorldTransform; } // 0x00738B9C slot 0x18
    virtual const CalculatedTransform& TrackbackLocalTransform() const { return m_Transform; } // 0x00738B94 slot 0x1C
    virtual void InheritTraversalResults();
    virtual u32 Initialize(os::IAllocator* pAllocator);
    virtual void UpdateDirection();

    // a node made from the resource, attached to pParent (name is ours)
    static TransformNode* Create(SceneNode* pParent, res::ResTransformNode resNode, const Description& description, os::IAllocator* pAllocator);
    u32 CreateCallbacks(os::IAllocator* pAllocator);
    // the memory Initialize allocates (not the node itself)
    static void GetMemorySizeForInitialize(os::MemorySizeCalculator* pSize, res::ResTransformNode resNode, Description description);
    void ClearUnknown0x12C(); // (name is ours)

    static const TypeInfo s_TypeInfo;

    CalculatedTransform m_Transform;                            // 0x4C the local transform
    nn::math::MTX34 m_WorldMatrix;                              // 0x8C
    CalculatedTransform m_WorldTransform;                       // 0xBC
    nn::math::MTX34 m_Unknown0xFC;                              // 0xFC (the identity)
    bool m_Unknown0x12C;                                        // 0x12C cleared when the world matrix is calculated
    NodeCallbackList<TransformNode>* m_pCalculatedCallbacks;    // 0x130 after the world matrix is calculated
    bool m_Unknown0x134;                                        // 0x134
    Description m_TransformDescription;                         // 0x138
};
ASSERT_SIZE(TransformNode, 0x148);
} // namespace gfx
} // namespace nw
