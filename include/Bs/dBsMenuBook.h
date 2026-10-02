#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 10BsMenuBook @ 0x008CAF84
// vtable 0x008EBDBC (vptr 0x008EBDC4), offset_to_top 0, 25 entries
// vtable 0x008EBE28 (vptr 0x008EBE30), offset_to_top -40, 3 entries
class BsMenuBook : public ::MenuBase, public ::state::Mode<BsMenuBook>
{
public:
    BsMenuBook(); // ctor candidate(s) 0x0019344C (unverified)
    virtual ~BsMenuBook(); // 0x00193644 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00193634 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00192B48 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x001932B8 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00193164 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00192AB4 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
