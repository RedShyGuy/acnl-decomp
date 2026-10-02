#pragma once

#include "decomp.h"
#include "Bs/dBsLightBase.h"

// RTTI 11BsLightSpot @ 0x008CB258
// vtable 0x008EC878 (vptr 0x008EC880), offset_to_top 0, 19 entries
class BsLightSpot : public ::BsLightBase
{
public:
    BsLightSpot(); // ctor address unknown
    virtual ~BsLightSpot(); // 0x001C2FCC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001C2FA8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x001C2D90 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x001C2F6C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x001C2E1C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x001C2D54 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x0070E3C4 slot 0x40 | virtual slot, introduced by BsLightBase
    virtual void vf_0x44(); // 0x001C2F8C slot 0x44 | virtual slot, introduced by BsLightBase
    virtual void vf_0x48(); // 0x001C2F5C slot 0x48 | virtual slot, introduced by BsLightBase
};
