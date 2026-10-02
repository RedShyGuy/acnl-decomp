#pragma once

#include "decomp.h"
#include "Bs/dBsLightHemiSphereBase.h"

// RTTI 20BsLightHemiSphereFix @ 0x008CCAA0
// vtable 0x008F5398 (vptr 0x008F53A0), offset_to_top 0, 21 entries
class BsLightHemiSphereFix : public ::BsLightHemiSphereBase
{
public:
    BsLightHemiSphereFix(); // ctor candidate(s) 0x007F1BD8 (unverified)
    virtual ~BsLightHemiSphereFix(); // 0x00305D2C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00305D04 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0030571C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Calc(); // 0x00305CDC slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void vf_0x48(); // 0x00305CF0 slot 0x48 | virtual slot, introduced by BsLightBase
    virtual void vf_0x4C(); // 0x00305510 slot 0x4C | virtual slot, introduced by BsLightHemiSphereBase
};
