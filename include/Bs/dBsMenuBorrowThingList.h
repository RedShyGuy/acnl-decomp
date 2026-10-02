#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 21BsMenuBorrowThingList @ 0x008CCC80
// vtable 0x008F6044 (vptr 0x008F604C), offset_to_top 0, 25 entries
// vtable 0x008F60B0 (vptr 0x008F60B8), offset_to_top -40, 3 entries
class BsMenuBorrowThingList : public ::MenuBase, public ::state::Mode<BsMenuBorrowThingList>
{
public:
    BsMenuBorrowThingList(); // ctor candidate(s) 0x003267B8 (unverified)
    virtual ~BsMenuBorrowThingList(); // 0x003268D4 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00326858 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x003262DC slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x003265AC slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0032651C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x003262BC slot 0x30 | slot vf_0x30 of oml::framework::Process
};
