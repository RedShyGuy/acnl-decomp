#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx9AnimGroupE @ 0x008D0820
// vtable 0x00902AE8 (vptr 0x00902AF0), offset_to_top 0, 2 entries
class AnimGroup : public ::nw::gfx::GfxObject
{
public:
    AnimGroup(); // ctor candidate(s) 0x004B3784 (unverified)
    virtual ~AnimGroup(); // 0x004B38EC slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x004B3808 slot 0x04 | virtual slot, introduced by nw::gfx::AnimGroup
    void Initialize(bool); // 0x004B2900 | nintendogs:callgraph [tier A]
    void GetMemorySizeForInitialize(nw::os::MemorySizeCalculator*, nw::anim::res::ResAnimGroup, bool); // 0x004B36E0 | nintendogs:bytes [tier A]
    AnimGroup(nw::anim::res::ResAnimGroup, nw::gfx::SceneNode*, nw::os::IAllocator*); // 0x004B3784 | nintendogs:callgraph [tier A]
};
} // namespace gfx
} // namespace nw
