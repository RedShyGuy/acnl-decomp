#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 18BsMenuRentalHaniwa @ 0x008CC7C8
// vtable 0x008F44D0 (vptr 0x008F44D8), offset_to_top 0, 25 entries
// vtable 0x008F453C (vptr 0x008F4544), offset_to_top -40, 3 entries
class BsMenuRentalHaniwa : public ::MenuBase, public ::state::Mode<BsMenuRentalHaniwa>
{
public:
    BsMenuRentalHaniwa(); // ctor candidate(s) 0x002DE0C8 (unverified)
    virtual ~BsMenuRentalHaniwa(); // 0x002DE178 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002DE168 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002DDA4C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002DDF1C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002DDD94 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002DDA10 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
