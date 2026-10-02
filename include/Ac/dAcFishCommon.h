#pragma once

#include "decomp.h"
#include "Ac/dAcObjectBase.h"

// RTTI 12AcFishCommon @ 0x008CB37C
// vtable 0x0083EEE8 (vptr 0x0083EEF0), offset_to_top 0, 39 entries
// vtable 0x0083F2D8 (vptr 0x0083F2E0), offset_to_top 0, 39 entries
// vtable 0x0083F0A8 (vptr 0x0083F0B0), offset_to_top -396, 11 entries
// vtable 0x0083F498 (vptr 0x0083F4A0), offset_to_top -524, 11 entries
class AcFishCommon : public ::AcObjectBase
{
public:
    AcFishCommon(); // ctor address unknown
    virtual ~AcFishCommon(); // 0x0057C650 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001E96DC slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void vf_0x44(); // 0x00712548 slot 0x44 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x58(); // 0x001E928C slot 0x58 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x5C(); // 0x001E92B0 slot 0x5C | virtual slot, introduced by AcObjectBase
    virtual void vf_0x68(); // 0x00712554 slot 0x68 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x8C(); // 0x001E9340 slot 0x8C | virtual slot, introduced by AcFishCommon
    virtual void vf_0x90(); // 0x001E95DC slot 0x90 | virtual slot, introduced by AcFishCommon
    virtual void vf_0x94(); // 0x0011C12F slot 0x94 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x98(); // 0x0011C12F slot 0x98 | slot vf_0x00 of ChangeRentalBase
};
