#pragma once

#include "decomp.h"
#include "Ac/dAcObjectBase.h"

// RTTI 14AcInsectCommon @ 0x008CBB28
// vtable 0x0083F950 (vptr 0x0083F958), offset_to_top 0, 40 entries
// vtable 0x0083FC38 (vptr 0x0083FC40), offset_to_top 0, 40 entries
// vtable 0x0083FF7C (vptr 0x0083FF84), offset_to_top 0, 40 entries
// vtable 0x008402C0 (vptr 0x008402C8), offset_to_top 0, 40 entries
// vtable 0x008406F0 (vptr 0x008406F8), offset_to_top 0, 40 entries
// vtable 0x008EF93C (vptr 0x008EF944), offset_to_top 0, 40 entries
// vtable 0x008EFA0C (vptr 0x008EFA14), offset_to_top -396, 11 entries
// vtable 0x0083FB14 (vptr 0x0083FB1C), offset_to_top -464, 11 entries
// vtable 0x0083FDFC (vptr 0x0083FE04), offset_to_top -468, 11 entries
// vtable 0x00840484 (vptr 0x0084048C), offset_to_top -476, 11 entries
// vtable 0x008408B4 (vptr 0x008408BC), offset_to_top -484, 11 entries
// vtable 0x00840140 (vptr 0x00840148), offset_to_top -496, 11 entries
class AcInsectCommon : public ::AcObjectBase
{
public:
    AcInsectCommon(); // ctor address unknown
    virtual ~AcInsectCommon(); // 0x0024FA60 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0024FA20 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void CanCalc() const; // 0x0024F7F4 slot 0x20 | slot vf_0x20 of oml::framework::Process
    virtual void vf_0x44(); // 0x007191BC slot 0x44 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x50(); // 0x002D5F2C slot 0x50 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x58(); // 0x0024F8CC slot 0x58 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x5C(); // 0x0024F95C slot 0x5C | virtual slot, introduced by AcObjectBase
    virtual void vf_0x84(); // 0x007191D0 slot 0x84 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x8C(); // 0x0011C12F slot 0x8C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x90(); // 0x007191B4 slot 0x90 | virtual slot, introduced by AcInsectCommon
    virtual void vf_0x94(); // 0x007191C8 slot 0x94 | virtual slot, introduced by AcInsectCommon
    virtual void vf_0x98(); // 0x0011C12F slot 0x98 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x9C(); // 0x0011C12F slot 0x9C | slot vf_0x00 of ChangeRentalBase
};
