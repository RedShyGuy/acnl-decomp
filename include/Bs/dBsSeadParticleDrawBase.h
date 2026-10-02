#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 22BsSeadParticleDrawBase @ 0x008CCDBC
// vtable 0x008F69B8 (vptr 0x008F69C0), offset_to_top 0, 21 entries
class BsSeadParticleDrawBase : public ::Base
{
public:
    BsSeadParticleDrawBase(); // ctor candidate(s) 0x0032FDF4 (unverified)
    virtual ~BsSeadParticleDrawBase(); // 0x0032FE70 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0032FE40 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0032F958 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0032FD64 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0032FB4C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0032F8E0 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x0032F704 slot 0x40 | virtual slot, introduced by BsSeadParticleDrawBase
    virtual void vf_0x44(); // 0x007261A4 slot 0x44 | virtual slot, introduced by BsSeadParticleDrawBase
    virtual void vf_0x48(); // 0x0072618C slot 0x48 | virtual slot, introduced by BsSeadParticleDrawBase
    virtual void vf_0x4C(); // 0x007261C8 slot 0x4C | virtual slot, introduced by BsSeadParticleDrawBase
    virtual void vf_0x50(); // 0x007261BC slot 0x50 | virtual slot, introduced by BsSeadParticleDrawBase
};
