#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_SceneObject.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx9SceneNodeE @ 0x008D0834
// vtable 0x00902AF8 (vptr 0x00902B00), offset_to_top 0, 10 entries
class SceneNode : public ::nw::gfx::SceneObject
{
public:
    SceneNode(); // ctor candidate(s) 0x004B45CC (unverified)
    virtual ~SceneNode(); // 0x004B47FC slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x004B47F0 slot 0x04 | virtual slot, introduced by nw::gfx::SceneNode
    virtual void GetRuntimeTypeInfo() const; // 0x0073C96C slot 0x08 | slot vf_0x08 of nw::gfx::SceneNode
    virtual void UpdateTransform(nw::gfx::WorldMatrixUpdater*, nw::gfx::SceneContext*); // 0x004B4150 slot 0x0C | slot vf_0x0C of nw::gfx::SceneNode
    virtual void Accept(nw::gfx::ISceneVisitor*); // 0x004B4578 slot 0x10 | nintendogs:bytes
    virtual void TrackbackWorldMatrix() const; // 0x0073C998 slot 0x14 | nintendogs:bytes
    virtual void TrackbackWorldTransform() const; // 0x0073CA54 slot 0x18 | nintendogs:bytes-fuzzy
    virtual void TrackbackLocalTransform() const; // 0x0073C9C4 slot 0x1C | nintendogs:bytes-fuzzy
    virtual void InheritTraversalResults(); // 0x004B4538 slot 0x20 | nintendogs:bytes
    virtual void Initialize(nw::os::IAllocator*); // 0x004B39D0 slot 0x24 | nintendogs:bytes
    void AttachChild(nw::gfx::SceneNode*); // 0x004B3A28 | nintendogs:callgraph [tier A]
    void UpdateFrame(); // 0x004B3B94 | mk7dlp:bytes [tier B]
    void CreateChildren(nw::os::IAllocator*); // 0x004B3BF4 | nintendogs:callgraph [tier A]
    void CreateCallbacks(nw::os::IAllocator*); // 0x004B4018 | nintendogs:callgraph [tier A]
    void CreateAnimBinding(nw::os::IAllocator*); // 0x004B4154 | nintendogs:callgraph [tier A]
    void IsCircularReference(const nw::gfx::SceneNode*) const; // 0x0073C978 | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
