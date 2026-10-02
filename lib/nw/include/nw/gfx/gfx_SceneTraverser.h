#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_ISceneVisitor.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx14SceneTraverserE @ 0x008D0640
// vtable 0x009025CC (vptr 0x009025D4), offset_to_top 0, 17 entries
class SceneTraverser : public ::nw::gfx::ISceneVisitor
{
public:
    class Builder;
    SceneTraverser(); // ctor candidate(s) 0x0049A7F8 (unverified)
    virtual void vf_0x00(); // 0x0049A988 slot 0x00 | virtual slot, introduced by nw::gfx::SceneTraverser
    virtual void vf_0x04(); // 0x0049A984 slot 0x04 | virtual slot, introduced by nw::gfx::SceneTraverser
    virtual void vf_0x08(); // 0x00738BA4 slot 0x08 | virtual slot, introduced by nw::gfx::SceneTraverser
    virtual void vf_0x0C(); // 0x00499CCC slot 0x0C | virtual slot, introduced by nw::gfx::SceneTraverser
    virtual void VisitTransformNode(nw::gfx::TransformNode*); // 0x0049A3A0 slot 0x10 | slot vf_0x10 of nw::gfx::SceneTraverser
    virtual void vf_0x14(); // 0x0049A3BC slot 0x14 | virtual slot, introduced by nw::gfx::SceneTraverser
    virtual void VisitModel(nw::gfx::Model*); // 0x00499B20 slot 0x18 | nintendogs:bytes
    virtual void VisitSkeletalModel(nw::gfx::SkeletalModel*); // 0x0049A240 slot 0x1C | slot vf_0x1C of nw::gfx::SceneTraverser
    virtual void VisitCamera(nw::gfx::Camera*); // 0x00499B80 slot 0x20 | slot vf_0x20 of nw::gfx::SceneTraverser
    virtual void VisitFog(nw::gfx::Fog*); // 0x0049A838 slot 0x24 | slot vf_0x24 of nw::gfx::SceneTraverser
    virtual void VisitLight(nw::gfx::Light*); // 0x00499AC0 slot 0x28 | nintendogs:bytes
    virtual void VisitFragmentLight(nw::gfx::FragmentLight*); // 0x00499F80 slot 0x2C | slot vf_0x2C of nw::gfx::SceneTraverser
    virtual void VisitAmbientLight(nw::gfx::AmbientLight*); // 0x00499E20 slot 0x30 | slot vf_0x30 of nw::gfx::SceneTraverser
    virtual void vf_0x34(); // 0x0049A4E4 slot 0x34 | virtual slot, introduced by nw::gfx::SceneTraverser
    virtual void vf_0x38(); // 0x00499CE8 slot 0x38 | virtual slot, introduced by nw::gfx::SceneTraverser
    virtual void vf_0x3C(); // 0x0049A644 slot 0x3C | virtual slot, introduced by nw::gfx::SceneTraverser
    virtual void vf_0x40(); // 0x0049A0E0 slot 0x40 | virtual slot, introduced by nw::gfx::SceneTraverser
    void Begin(nw::gfx::SceneContext*); // 0x0049A778 | nintendogs:bytes [tier B]
};
} // namespace gfx
} // namespace nw
