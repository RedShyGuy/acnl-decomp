#include "nw/gfx/gfx_TransformNode.h"
#include "g3d/dFuncNode.h"

namespace g3d {
// (inline in the creation function 0x004F1370; the members at 0x148-0x150 are set to 0 there)
g3d::FuncNode::FuncNode(nw::os::IAllocator* pAllocator, const Description& description)
    : nw::gfx::TransformNode(pAllocator, nw::gfx::res::ResTransformNode(), description)
{
}

// 0x004F2DF4 slot 0x00 | slot vf_0x00 of nw::gfx::SceneNode
g3d::FuncNode::~FuncNode()
{
}

// 0x004F2DE8 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
void g3d::FuncNode::vf_0x04()
{
}

// 0x00747500 slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
const nw::gfx::TypeInfo* g3d::FuncNode::GetRuntimeTypeInfo() const
{
}

} // namespace g3d
