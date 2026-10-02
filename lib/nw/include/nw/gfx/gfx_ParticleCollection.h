#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx18ParticleCollectionE @ 0x008D06D0
// vtable 0x00902770 (vptr 0x00902778), offset_to_top 0, 3 entries
class ParticleCollection : public ::nw::gfx::GfxObject
{
public:
    ParticleCollection(); // ctor candidate(s) 0x0049EED4 (unverified)
    virtual ~ParticleCollection(); // 0x0049FA00 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x0049F998 slot 0x04 | virtual slot, introduced by nw::gfx::ParticleCollection
    virtual void vf_0x08(); // 0x0073A8A0 slot 0x08 | virtual slot, introduced by nw::gfx::ParticleCollection
    void Clear(); // 0x0049EDE4 | nintendogs:bytes [tier A]
    void Create(nw::gfx::ParticleSet*, nw::gfx::res::ResParticleCollection, nw::os::IAllocator*, nw::os::IAllocator*, nw::gfx::ParticleShape*); // 0x0049EED4 | nintendogs:callseq [tier A]
};
} // namespace gfx
} // namespace nw
