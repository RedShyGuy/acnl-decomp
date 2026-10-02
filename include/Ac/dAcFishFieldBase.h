#pragma once

#include "decomp.h"
#include "Ac/dAcFishCommon.h"
#include "Resource/dResourceGetSkeletal.h"

// RTTI 15AcFishFieldBase @ 0x008CBE58
// vtable 0x0083F204 (vptr 0x0083F20C), offset_to_top 0, 50 entries
// vtable 0x008F0A08 (vptr 0x008F0A10), offset_to_top 0, 50 entries
// vtable 0x0083F5B8 (vptr 0x0083F5C0), offset_to_top -208, 9 entries
// vtable 0x008F0ADC (vptr 0x008F0AE4), offset_to_top -208, 9 entries
// vtable 0x008F0B30 (vptr 0x008F0B38), offset_to_top -396, 11 entries
// vtable 0x0083F580 (vptr 0x0083F588), offset_to_top -524, 11 entries
class AcFishFieldBase : public ::AcFishCommon, public ::ResourceGetSkeletal
{
public:
    AcFishFieldBase(); // ctor address unknown
    virtual ~AcFishFieldBase(); // 0x0027E2AC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0027E298 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void CanCalc() const; // 0x0027D9BC slot 0x20 | slot vf_0x20 of oml::framework::Process
    virtual void HandleCalcResult(oml::framework::Result); // 0x0027DCB0 slot 0x28 | slot vf_0x28 of oml::framework::Process
    virtual void vf_0x68(); // 0x0071B52C slot 0x68 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x6C(); // 0x0027E0DC slot 0x6C | virtual slot, introduced by AcObjectBase
    virtual void vf_0x7C(); // 0x0027DCF4 slot 0x7C | virtual slot, introduced by AcObjectBase
    virtual void vf_0x94(); // 0x0027DA98 slot 0x94 | virtual slot, introduced by AcFishCommon
    virtual void vf_0x98(); // 0x0027DC30 slot 0x98 | virtual slot, introduced by AcFishCommon
    virtual void vf_0x9C(); // 0x0027DE80 slot 0x9C | virtual slot, introduced by AcFishFieldBase
    virtual void vf_0xA0(); // 0x0027DE54 slot 0xA0 | virtual slot, introduced by AcFishFieldBase
    virtual void vf_0xA4(); // 0x0071B590 slot 0xA4 | virtual slot, introduced by AcFishFieldBase
    virtual void vf_0xA8(); // 0x0027DE4C slot 0xA8 | virtual slot, introduced by AcFishFieldBase
    virtual void vf_0xAC(); // 0x0027E0C0 slot 0xAC | virtual slot, introduced by AcFishFieldBase
    virtual void vf_0xB0(); // 0x0027E0C4 slot 0xB0 | virtual slot, introduced by AcFishFieldBase
    virtual void vf_0xB4(); // 0x0027DE5C slot 0xB4 | virtual slot, introduced by AcFishFieldBase
    virtual void vf_0xB8(); // 0x0027DF1C slot 0xB8 | virtual slot, introduced by AcFishFieldBase
    virtual void vf_0xBC(); // 0x0027DC2C slot 0xBC | virtual slot, introduced by AcFishFieldBase
    virtual void vf_0xC0(); // 0x0027DCAC slot 0xC0 | virtual slot, introduced by AcFishFieldBase
    virtual void vf_0xC4(); // 0x0011C12F slot 0xC4 | slot vf_0x00 of ChangeRentalBase
};
