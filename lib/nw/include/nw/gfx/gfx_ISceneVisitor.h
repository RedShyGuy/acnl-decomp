#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"

namespace nw {
namespace gfx {
class SceneNode;
class TransformNode;

// RTTI N2nw3gfx13ISceneVisitorE @ 0x008D05EC
// Visits the nodes of a scene (SceneNode::Accept); one function per node class. The slots are filled
// in as the node classes are written (names after the reference symbols of SceneTraverser).
class ISceneVisitor : public ::nw::gfx::GfxObject
{
public:
    ISceneVisitor(); // ctor address unknown
    virtual ~ISceneVisitor() {}

    virtual const TypeInfo* GetRuntimeTypeInfo() const = 0; // slot 0x08
    virtual void VisitSceneNode(SceneNode* pNode) = 0; // slot 0x0C (name is ours)
    virtual void VisitTransformNode(TransformNode* pNode) = 0; // slot 0x10
};
} // namespace gfx
} // namespace nw
