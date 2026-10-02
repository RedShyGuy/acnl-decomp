#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 13BsMenuCoolbox @ 0x008CB8EC
// vtable 0x008EEDC4 (vptr 0x008EEDCC), offset_to_top 0, 25 entries
// vtable 0x008EEE30 (vptr 0x008EEE38), offset_to_top -40, 3 entries
class BsMenuCoolbox : public ::MenuBase, public ::state::Mode<BsMenuCoolbox>
{
public:
    BsMenuCoolbox(); // ctor candidate(s) 0x0021EB68 (unverified)
    virtual ~BsMenuCoolbox(); // 0x0021EC20 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0021EC10 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0021E58C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0021EA68 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0021E878 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0021E544 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
