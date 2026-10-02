#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 15BsMenuMiiSelect @ 0x008CBF68
// vtable 0x008F14B8 (vptr 0x008F14C0), offset_to_top 0, 25 entries
// vtable 0x008F1524 (vptr 0x008F152C), offset_to_top -40, 3 entries
class BsMenuMiiSelect : public ::MenuBase, public ::state::Mode<BsMenuMiiSelect>
{
public:
    BsMenuMiiSelect(); // ctor address unknown
    virtual ~BsMenuMiiSelect(); // 0x0028A118 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0028A108 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002895D0 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00289890 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002897AC slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00289598 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
