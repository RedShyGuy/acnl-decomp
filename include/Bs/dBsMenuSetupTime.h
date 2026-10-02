#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 15BsMenuSetupTime @ 0x008CBF88
// vtable 0x008F1538 (vptr 0x008F1540), offset_to_top 0, 25 entries
// vtable 0x008F15A4 (vptr 0x008F15AC), offset_to_top -40, 3 entries
class BsMenuSetupTime : public ::MenuBase, public ::state::Mode<BsMenuSetupTime>
{
public:
    BsMenuSetupTime(); // ctor address unknown
    virtual ~BsMenuSetupTime(); // 0x0028D6E8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0028D6D8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0028CD94 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0028D30C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0028D1D4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0028CD34 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
