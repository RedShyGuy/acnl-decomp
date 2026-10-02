#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 15BsMenuBbsEditer @ 0x008CBF08
// vtable 0x008F1338 (vptr 0x008F1340), offset_to_top 0, 25 entries
// vtable 0x008F13A4 (vptr 0x008F13AC), offset_to_top -40, 3 entries
class BsMenuBbsEditer : public ::MenuBase, public ::state::Mode<BsMenuBbsEditer>
{
public:
    BsMenuBbsEditer(); // ctor address unknown
    virtual ~BsMenuBbsEditer(); // 0x00286824 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0028677C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002863B4 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002866F8 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0028665C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0028637C slot 0x30 | slot vf_0x30 of oml::framework::Process
};
