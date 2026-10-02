#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 16BsMenuItemSelect @ 0x008CC214
// vtable 0x008F2640 (vptr 0x008F2648), offset_to_top 0, 25 entries
// vtable 0x008F26AC (vptr 0x008F26B4), offset_to_top -40, 3 entries
class BsMenuItemSelect : public ::MenuBase, public ::state::Mode<BsMenuItemSelect>
{
public:
    BsMenuItemSelect(); // ctor address unknown
    virtual ~BsMenuItemSelect(); // 0x002B15E8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002B15D8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002B0A74 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002B0E78 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002B0C8C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002B0A38 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
