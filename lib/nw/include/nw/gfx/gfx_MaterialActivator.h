#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_IMaterialActivator.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx17MaterialActivatorE @ 0x008D06AC
// vtable 0x00902734 (vptr 0x0090273C), offset_to_top 0, 4 entries
class MaterialActivator : public ::nw::gfx::IMaterialActivator
{
public:
    MaterialActivator(); // ctor candidate(s) 0x0049D3AC (unverified)
    virtual void vf_0x00(); // 0x0049E12C slot 0x00 | virtual slot, introduced by nw::gfx::MaterialActivator
    virtual void vf_0x04(); // 0x0049E9D8 slot 0x04 | virtual slot, introduced by nw::gfx::MaterialActivator
    virtual void vf_0x08(); // 0x0073A878 slot 0x08 | virtual slot, introduced by nw::gfx::MaterialActivator
    virtual void vf_0x0C(); // 0x0049D3E4 slot 0x0C | virtual slot, introduced by nw::gfx::MaterialActivator
    void Create(nw::os::IAllocator*); // 0x0049D3AC | nintendogs:callgraph [tier A]
};
} // namespace gfx
} // namespace nw
