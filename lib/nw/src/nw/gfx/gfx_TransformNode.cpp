#include "nw/gfx/gfx_SceneNode.h"
#include "nw/gfx/gfx_TransformNode.h"

namespace nw {
namespace gfx {
// ctor candidate(s) 0x0049926C (unverified)
nw::gfx::TransformNode::TransformNode()
{
}

// 0x004995F8 slot 0x00 | nintendogs:bytes
nw::gfx::TransformNode::~TransformNode()
{
}

// 0x004995EC slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
void nw::gfx::TransformNode::vf_0x04()
{
}

// 0x00738B80 slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
void nw::gfx::TransformNode::GetRuntimeTypeInfo() const
{
}

// 0x00498B58 slot 0x0C | nintendogs:callseq
void nw::gfx::TransformNode::UpdateTransform(nw::gfx::WorldMatrixUpdater*, nw::gfx::SceneContext*)
{
}

// 0x00499048 slot 0x10 | nintendogs:bytes
void nw::gfx::TransformNode::Accept(nw::gfx::ISceneVisitor*)
{
}

// 0x00738B8C slot 0x14 | slot vf_0x14 of nw::gfx::SceneNode
void nw::gfx::TransformNode::TrackbackWorldMatrix() const
{
}

// 0x00738B9C slot 0x18 | slot vf_0x18 of nw::gfx::SceneNode
void nw::gfx::TransformNode::TrackbackWorldTransform() const
{
}

// 0x00738B94 slot 0x1C | slot vf_0x1C of nw::gfx::SceneNode
void nw::gfx::TransformNode::TrackbackLocalTransform() const
{
}

// 0x00498E18 slot 0x20 | nintendogs:bytes
void nw::gfx::TransformNode::InheritTraversalResults()
{
}

// 0x004989E4 slot 0x24 | nintendogs:bytes
void nw::gfx::TransformNode::Initialize(nw::os::IAllocator*)
{
}

// 0x00498B54 slot 0x28 | slot vf_0x28 of nw::gfx::TransformNode
void nw::gfx::TransformNode::UpdateDirection()
{
}

// 0x00498A1C | nintendogs:callgraph [tier A]
void nw::gfx::TransformNode::CreateCallbacks(nw::os::IAllocator*)
{
}

// 0x00498E98 | nintendogs:bytes [tier A]
void nw::gfx::TransformNode::GetMemorySizeForInitialize(nw::os::MemorySizeCalculator*, nw::gfx::res::ResTransformNode, nw::gfx::TransformNode::Description)
{
}

// 0x0049926C | nintendogs:callgraph [tier A]
nw::gfx::TransformNode::TransformNode(nw::os::IAllocator*, nw::gfx::res::ResTransformNode, const nw::gfx::TransformNode::Description&)
{
}

} // namespace gfx
} // namespace nw
