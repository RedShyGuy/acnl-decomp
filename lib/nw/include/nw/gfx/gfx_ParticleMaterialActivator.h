#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_IMaterialActivator.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx25ParticleMaterialActivatorE @ 0x008D0778
// vtable 0x009028D4 (vptr 0x009028DC), offset_to_top 0, 4 entries
class ParticleMaterialActivator : public ::nw::gfx::IMaterialActivator
{
public:
    ParticleMaterialActivator(); // ctor candidate(s) 0x004A3B20 (unverified)
    virtual void vf_0x00(); // 0x004A4390 slot 0x00 | virtual slot, introduced by nw::gfx::ParticleMaterialActivator
    virtual void vf_0x04(); // 0x004A438C slot 0x04 | virtual slot, introduced by nw::gfx::ParticleMaterialActivator
    virtual void vf_0x08(); // 0x0073C52C slot 0x08 | virtual slot, introduced by nw::gfx::ParticleMaterialActivator
    virtual void Activate(nw::gfx::RenderContext*, const nw::gfx::Material*); // 0x004A3B58 slot 0x0C | nintendogs:callseq
};
} // namespace gfx
} // namespace nw
