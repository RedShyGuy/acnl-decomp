#pragma once

#include "decomp.h"
#include "Bs/dBsSeadParticleDrawBase.h"

// RTTI 26BsSeadParticleDrawFootmark @ 0x008CD0A4
// vtable 0x008F7E10 (vptr 0x008F7E18), offset_to_top 0, 21 entries
class BsSeadParticleDrawFootmark : public ::BsSeadParticleDrawBase
{
public:
    BsSeadParticleDrawFootmark(); // ctor address unknown
    virtual ~BsSeadParticleDrawFootmark(); // 0x00342AD8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00342AC8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00342A68 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00342AA4 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00342AA0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00342A64 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x44(); // 0x007267DC slot 0x44 | virtual slot, introduced by BsSeadParticleDrawBase
    virtual void vf_0x48(); // 0x007267D4 slot 0x48 | virtual slot, introduced by BsSeadParticleDrawBase
    virtual void vf_0x4C(); // 0x007267EC slot 0x4C | virtual slot, introduced by BsSeadParticleDrawBase
    virtual void vf_0x50(); // 0x007267E4 slot 0x50 | virtual slot, introduced by BsSeadParticleDrawBase
};
