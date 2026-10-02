#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "script/dITalkRecept.h"
#include "state/dMode.h"

// RTTI 13BsMenuMailbox @ 0x008CB90C
// vtable 0x008EEE44 (vptr 0x008EEE4C), offset_to_top 0, 25 entries
// vtable 0x008EEEB0 (vptr 0x008EEEB8), offset_to_top -40, 63 entries
// vtable 0x008EEFB4 (vptr 0x008EEFBC), offset_to_top -164, 3 entries
class BsMenuMailbox : public ::MenuBase, public ::script::ITalkRecept, public ::state::Mode<BsMenuMailbox>
{
public:
    BsMenuMailbox(); // ctor candidate(s) 0x00220048 (unverified)
    virtual ~BsMenuMailbox(); // 0x0022011C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0022010C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0021FBBC slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0021FF88 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0021FDA0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0021FB6C slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void OnClose(); // 0x0021EDF8 slot 0x44 | slot vf_0x44 of MenuBase
};
