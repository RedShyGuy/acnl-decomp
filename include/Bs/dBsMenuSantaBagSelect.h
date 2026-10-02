#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 20BsMenuSantaBagSelect @ 0x008CCB14
// vtable 0x008F5678 (vptr 0x008F5680), offset_to_top 0, 25 entries
// vtable 0x008F56E4 (vptr 0x008F56EC), offset_to_top -40, 3 entries
class BsMenuSantaBagSelect : public ::MenuBase, public ::state::Mode<BsMenuSantaBagSelect>
{
public:
    BsMenuSantaBagSelect(); // ctor address unknown
    virtual ~BsMenuSantaBagSelect(); // 0x0031C7F0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0031C7E0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0031C56C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0031C7B4 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0031C714 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0031C4B4 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
