#pragma once

#include "decomp.h"
#include "Bs/dBsLightHemiSphereFix.h"

// RTTI 24BsLightHemiSphereFixDemo @ 0x008CCF84
// vtable 0x008F76F4 (vptr 0x008F76FC), offset_to_top 0, 21 entries
class BsLightHemiSphereFixDemo : public ::BsLightHemiSphereFix
{
public:
    BsLightHemiSphereFixDemo(); // ctor candidate(s) 0x007F28F8 (unverified)
    virtual ~BsLightHemiSphereFixDemo(); // 0x00336AA4 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00336A80 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00336564 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00336A6C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00336908 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void vf_0x48(); // 0x00336A5C slot 0x48 | virtual slot, introduced by BsLightBase
};
