#pragma once

#include "decomp.h"
#include "Object/dObjectResource.h"
#include "Other/dActor.h"

// RTTI 12AcObjectBase @ 0x008CB3C0
// vtable 0x0083EF90 (vptr 0x0083EF98), offset_to_top 0, 35 entries
// vtable 0x0083F380 (vptr 0x0083F388), offset_to_top 0, 35 entries
// vtable 0x0083F85C (vptr 0x0083F864), offset_to_top 0, 35 entries
// vtable 0x0083F9FC (vptr 0x0083FA04), offset_to_top 0, 35 entries
// vtable 0x0083FCE4 (vptr 0x0083FCEC), offset_to_top 0, 35 entries
// vtable 0x00840028 (vptr 0x00840030), offset_to_top 0, 35 entries
// vtable 0x0084036C (vptr 0x00840374), offset_to_top 0, 35 entries
// vtable 0x0084079C (vptr 0x008407A4), offset_to_top 0, 35 entries
// vtable 0x0083F04C (vptr 0x0083F054), offset_to_top -396, 11 entries
// vtable 0x0083F918 (vptr 0x0083F920), offset_to_top -396, 11 entries
// vtable 0x0083FAB8 (vptr 0x0083FAC0), offset_to_top -464, 11 entries
// vtable 0x0083FDA0 (vptr 0x0083FDA8), offset_to_top -468, 11 entries
// vtable 0x00840428 (vptr 0x00840430), offset_to_top -476, 11 entries
// vtable 0x00840858 (vptr 0x00840860), offset_to_top -484, 11 entries
// vtable 0x008400E4 (vptr 0x008400EC), offset_to_top -496, 11 entries
// vtable 0x0083F43C (vptr 0x0083F444), offset_to_top -524, 11 entries
class AcObjectBase : public ::Actor, public virtual ::ObjectResource
{
public:
    AcObjectBase(); // ctor address unknown
    virtual ~AcObjectBase(); // 0x001F5664 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001F5654 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x001F5258 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x001F5400 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x001F537C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void CanDraw() const; // 0x001F53EC slot 0x2C | slot vf_0x2C of oml::framework::Process
    virtual void Draw(); // 0x001F50A0 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x44(); // 0x0011C12F slot 0x44 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x48(); // 0x0011C12F slot 0x48 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x4C(); // 0x001F4298 slot 0x4C | virtual slot, introduced by AcObjectBase
    virtual void vf_0x50(); // 0x001F43DC slot 0x50 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x54(); // 0x001F429C slot 0x54 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x58(); // 0x0011C12F slot 0x58 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x5C(); // 0x0011C12F slot 0x5C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x60(); // 0x001F5254 slot 0x60 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x64(); // 0x001F5128 slot 0x64 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x68(); // 0x0011C12F slot 0x68 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x6C(); // 0x001F5020 slot 0x6C | virtual slot, introduced by AcObjectBase
    virtual void vf_0x70(); // 0x0011C12F slot 0x70 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x74(); // 0x001F5450 slot 0x74 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x78(); // 0x001F5584 slot 0x78 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x7C(); // 0x001F4278 slot 0x7C | virtual slot, introduced by AcObjectBase
    virtual void vf_0x80(); // 0x001F5024 slot 0x80 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x84(); // 0x00712738 slot 0x84 | virtual slot, introduced by AcObjectBase
    virtual void vf_0x88(); // 0x001F4C88 slot 0x88 | virtual slot, introduced by AcObjectBase
};
