#pragma once

#include "decomp.h"
#include "Bs/dBsSeadParticleDrawBase.h"

// RTTI 26BsSeadParticleDrawMinigame @ 0x008CD0B0
// vtable 0x008F7E6C (vptr 0x008F7E74), offset_to_top 0, 21 entries
class BsSeadParticleDrawMinigame : public ::BsSeadParticleDrawBase
{
public:
    BsSeadParticleDrawMinigame(); // ctor candidate(s) 0x00342B1C (unverified)
    virtual ~BsSeadParticleDrawMinigame(); // 0x00342B4C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00342B3C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00342AE0 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00342B00 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00342AFC slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00342ADC slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x44(); // 0x007267F8 slot 0x44 | virtual slot, introduced by BsSeadParticleDrawBase
    virtual void vf_0x48(); // 0x007267F4 slot 0x48 | virtual slot, introduced by BsSeadParticleDrawBase
    virtual void vf_0x4C(); // 0x00726800 slot 0x4C | virtual slot, introduced by BsSeadParticleDrawBase
    virtual void vf_0x50(); // 0x007267FC slot 0x50 | virtual slot, introduced by BsSeadParticleDrawBase
};
