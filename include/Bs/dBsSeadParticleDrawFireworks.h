#pragma once

#include "decomp.h"
#include "Bs/dBsSeadParticleDrawBase.h"

// RTTI 27BsSeadParticleDrawFireworks @ 0x008CD0E0
// vtable 0x008F805C (vptr 0x008F8064), offset_to_top 0, 21 entries
class BsSeadParticleDrawFireworks : public ::BsSeadParticleDrawBase
{
public:
    BsSeadParticleDrawFireworks(); // ctor address unknown
    virtual ~BsSeadParticleDrawFireworks(); // 0x00342F9C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00342F8C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00342F2C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00342F68 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00342F64 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00342F28 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x48(); // 0x00726804 slot 0x48 | virtual slot, introduced by BsSeadParticleDrawBase
    virtual void vf_0x4C(); // 0x00726810 slot 0x4C | virtual slot, introduced by BsSeadParticleDrawBase
    virtual void vf_0x50(); // 0x0072680C slot 0x50 | virtual slot, introduced by BsSeadParticleDrawBase
};
