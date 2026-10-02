#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"

// RTTI 19BsMenuRandomHeadsUp @ 0x008CC98C
// vtable 0x008F4E90 (vptr 0x008F4E98), offset_to_top 0, 16 entries
// vtable 0x008F4ED8 (vptr 0x008F4EE0), offset_to_top -20, 3 entries
class BsMenuRandomHeadsUp : public ::Base, public ::state::Mode<BsMenuRandomHeadsUp>
{
public:
    BsMenuRandomHeadsUp(); // ctor candidate(s) 0x007F1884 (unverified)
    virtual ~BsMenuRandomHeadsUp(); // 0x002F3914 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002F38C0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002F3518 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002F3878 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002F37DC slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002F34F8 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
