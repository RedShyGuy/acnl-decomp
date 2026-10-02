#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"

// RTTI 18BsMenuSantaBagView @ 0x008CC7E8
// vtable 0x008F4550 (vptr 0x008F4558), offset_to_top 0, 25 entries
class BsMenuSantaBagView : public ::MenuBase
{
public:
    BsMenuSantaBagView(); // ctor address unknown
    virtual ~BsMenuSantaBagView(); // 0x002DE9A0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002DE898 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002DE5B4 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002DE854 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002DE794 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002DE5A0 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
