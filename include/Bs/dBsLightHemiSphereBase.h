#pragma once

#include "decomp.h"
#include "Bs/dBsLightBase.h"

// RTTI 21BsLightHemiSphereBase @ 0x008CCC74
// vtable 0x008F5FE8 (vptr 0x008F5FF0), offset_to_top 0, 21 entries
class BsLightHemiSphereBase : public ::BsLightBase
{
public:
    class HemiSphereLightHioNode;
    BsLightHemiSphereBase(); // ctor address unknown
    virtual ~BsLightHemiSphereBase(); // 0x00325C70 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00325C4C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00325A84 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00325B64 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00325B28 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00325A30 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x0072584C slot 0x40 | virtual slot, introduced by BsLightBase
    virtual void vf_0x44(); // 0x00325B84 slot 0x44 | virtual slot, introduced by BsLightBase
    virtual void vf_0x4C(); // 0x0011C12F slot 0x4C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x50(); // 0x004EF0D8 slot 0x50 | virtual slot, introduced by BsLightHemiSphereBase
};
