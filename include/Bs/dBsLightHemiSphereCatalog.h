#pragma once

#include "decomp.h"
#include "Bs/dBsLightHemiSphereBase.h"

// RTTI 24BsLightHemiSphereCatalog @ 0x008CCF78
// vtable 0x008F7698 (vptr 0x008F76A0), offset_to_top 0, 21 entries
class BsLightHemiSphereCatalog : public ::BsLightHemiSphereBase
{
public:
    BsLightHemiSphereCatalog(); // ctor candidate(s) 0x007F28D0 (unverified)
    virtual ~BsLightHemiSphereCatalog(); // 0x00336488 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00336464 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x003363AC slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Calc(); // 0x0033642C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00336384 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x48(); // 0x00336454 slot 0x48 | virtual slot, introduced by BsLightBase
    virtual void vf_0x4C(); // 0x00336350 slot 0x4C | virtual slot, introduced by BsLightHemiSphereBase
};
