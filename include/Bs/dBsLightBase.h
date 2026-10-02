#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 11BsLightBase @ 0x008CB24C
// vtable 0x008EC824 (vptr 0x008EC82C), offset_to_top 0, 19 entries
class BsLightBase : public ::Base
{
public:
    BsLightBase(); // ctor candidate(s) 0x001C2D20 (unverified)
    virtual ~BsLightBase(); // 0x001C2D50 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001C2D40 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x005220B8 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x005220D8 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x005220C4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x005220AC slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void Unk0(); // 0x0070E3BC slot 0x3C | slot vf_0x3C of Base
    virtual void vf_0x40(); // 0x0011C12F slot 0x40 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x44(); // 0x0011C12F slot 0x44 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x48(); // 0x001C2D10 slot 0x48 | virtual slot, introduced by BsLightBase
};
