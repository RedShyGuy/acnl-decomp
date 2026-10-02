#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_SceneNode.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx13TransformNodeE @ 0x008D0634
// vtable 0x00902598 (vptr 0x009025A0), offset_to_top 0, 11 entries
class TransformNode : public ::nw::gfx::SceneNode
{
public:
    struct Description { u32 _unknown; }; // TODO: real type unknown (placeholder)
    TransformNode(); // ctor candidate(s) 0x0049926C (unverified)
    virtual ~TransformNode(); // 0x004995F8 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x004995EC slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
    virtual void GetRuntimeTypeInfo() const; // 0x00738B80 slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
    virtual void UpdateTransform(nw::gfx::WorldMatrixUpdater*, nw::gfx::SceneContext*); // 0x00498B58 slot 0x0C | nintendogs:callseq
    virtual void Accept(nw::gfx::ISceneVisitor*); // 0x00499048 slot 0x10 | nintendogs:bytes
    virtual void TrackbackWorldMatrix() const; // 0x00738B8C slot 0x14 | slot vf_0x14 of nw::gfx::SceneNode
    virtual void TrackbackWorldTransform() const; // 0x00738B9C slot 0x18 | slot vf_0x18 of nw::gfx::SceneNode
    virtual void TrackbackLocalTransform() const; // 0x00738B94 slot 0x1C | slot vf_0x1C of nw::gfx::SceneNode
    virtual void InheritTraversalResults(); // 0x00498E18 slot 0x20 | nintendogs:bytes
    virtual void Initialize(nw::os::IAllocator*); // 0x004989E4 slot 0x24 | nintendogs:bytes
    virtual void UpdateDirection(); // 0x00498B54 slot 0x28 | slot vf_0x28 of nw::gfx::TransformNode
    void CreateCallbacks(nw::os::IAllocator*); // 0x00498A1C | nintendogs:callgraph [tier A]
    void GetMemorySizeForInitialize(nw::os::MemorySizeCalculator*, nw::gfx::res::ResTransformNode, nw::gfx::TransformNode::Description); // 0x00498E98 | nintendogs:bytes [tier A]
    TransformNode(nw::os::IAllocator*, nw::gfx::res::ResTransformNode, const nw::gfx::TransformNode::Description&); // 0x0049926C | nintendogs:callgraph [tier A]
};
} // namespace gfx
} // namespace nw
