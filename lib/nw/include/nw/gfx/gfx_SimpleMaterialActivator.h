#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_IMaterialActivator.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx23SimpleMaterialActivatorE @ 0x008D0754
// vtable 0x00902874 (vptr 0x0090287C), offset_to_top 0, 4 entries
class SimpleMaterialActivator : public ::nw::gfx::IMaterialActivator
{
public:
    SimpleMaterialActivator(); // ctor candidate(s) 0x004A2AA0 (unverified)
    virtual void vf_0x00(); // 0x004A3840 slot 0x00 | virtual slot, introduced by nw::gfx::SimpleMaterialActivator
    virtual void vf_0x04(); // 0x004A383C slot 0x04 | virtual slot, introduced by nw::gfx::SimpleMaterialActivator
    virtual void vf_0x08(); // 0x0073C4A4 slot 0x08 | virtual slot, introduced by nw::gfx::SimpleMaterialActivator
    virtual void vf_0x0C(); // 0x004A2AD8 slot 0x0C | virtual slot, introduced by nw::gfx::SimpleMaterialActivator
    void Create(nw::os::IAllocator*); // 0x004A2AA0 | nintendogs:callgraph [tier A]
};
} // namespace gfx
} // namespace nw
