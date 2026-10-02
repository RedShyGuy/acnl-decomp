#pragma once

#include "decomp.h"
#include "Bs/dBsSeadParticleDrawBase.h"

// RTTI 20BsSeadParticleDraw2D @ 0x008CCB34
// vtable 0x008F56F8 (vptr 0x008F5700), offset_to_top 0, 21 entries
class BsSeadParticleDraw2D : public ::BsSeadParticleDrawBase
{
public:
    BsSeadParticleDraw2D(); // ctor candidate(s) 0x0031CD98 (unverified)
    virtual ~BsSeadParticleDraw2D(); // 0x0032FE6C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0031CDB8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0031CA9C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0031CD74 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0032FB48 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0031CA94 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x44(); // 0x007261A0 slot 0x44 | virtual slot, introduced by BsSeadParticleDrawBase
    virtual void vf_0x48(); // 0x00726188 slot 0x48 | virtual slot, introduced by BsSeadParticleDrawBase
    virtual void vf_0x4C(); // 0x007261C4 slot 0x4C | virtual slot, introduced by BsSeadParticleDrawBase
    virtual void vf_0x50(); // 0x007261B8 slot 0x50 | virtual slot, introduced by BsSeadParticleDrawBase
};
