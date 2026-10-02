#include "nw/gfx/gfx_SceneObject.h"
#include "nw/gfx/gfx_SceneNode.h"

namespace nw {
namespace gfx {
// ctor candidate(s) 0x004B45CC (unverified)
nw::gfx::SceneNode::SceneNode()
{
}

// 0x004B47FC slot 0x00 | nintendogs:bytes
nw::gfx::SceneNode::~SceneNode()
{
}

// 0x004B47F0 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
void nw::gfx::SceneNode::vf_0x04()
{
}

// 0x0073C96C slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
void nw::gfx::SceneNode::GetRuntimeTypeInfo() const
{
}

// 0x004B4150 slot 0x0C | slot vf_0x0C of nw::gfx::SceneNode
void nw::gfx::SceneNode::UpdateTransform(nw::gfx::WorldMatrixUpdater*, nw::gfx::SceneContext*)
{
}

// 0x004B4578 slot 0x10 | nintendogs:bytes
void nw::gfx::SceneNode::Accept(nw::gfx::ISceneVisitor*)
{
}

// 0x0073C998 slot 0x14 | nintendogs:bytes
void nw::gfx::SceneNode::TrackbackWorldMatrix() const
{
}

// 0x0073CA54 slot 0x18 | nintendogs:bytes-fuzzy
void nw::gfx::SceneNode::TrackbackWorldTransform() const
{
}

// 0x0073C9C4 slot 0x1C | nintendogs:bytes-fuzzy
void nw::gfx::SceneNode::TrackbackLocalTransform() const
{
}

// 0x004B4538 slot 0x20 | nintendogs:bytes
void nw::gfx::SceneNode::InheritTraversalResults()
{
}

// 0x004B39D0 slot 0x24 | nintendogs:bytes
void nw::gfx::SceneNode::Initialize(nw::os::IAllocator*)
{
}

// 0x004B3A28 | nintendogs:callgraph [tier A]
void nw::gfx::SceneNode::AttachChild(nw::gfx::SceneNode*)
{
}

// 0x004B3B94 | mk7dlp:bytes [tier B]
void nw::gfx::SceneNode::UpdateFrame()
{
}

// 0x004B3BF4 | nintendogs:callgraph [tier A]
void nw::gfx::SceneNode::CreateChildren(nw::os::IAllocator*)
{
}

// 0x004B4018 | nintendogs:callgraph [tier A]
void nw::gfx::SceneNode::CreateCallbacks(nw::os::IAllocator*)
{
}

// 0x004B4154 | nintendogs:callgraph [tier A]
void nw::gfx::SceneNode::CreateAnimBinding(nw::os::IAllocator*)
{
}

// 0x0073C978 | nintendogs:bytes [tier A]
void nw::gfx::SceneNode::IsCircularReference(const nw::gfx::SceneNode*) const
{
}

} // namespace gfx
} // namespace nw
