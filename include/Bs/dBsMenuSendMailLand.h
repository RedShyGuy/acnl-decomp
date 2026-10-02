#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 18BsMenuSendMailLand @ 0x008CC7F4
// vtable 0x008F45BC (vptr 0x008F45C4), offset_to_top 0, 25 entries
// vtable 0x008F4628 (vptr 0x008F4630), offset_to_top -40, 3 entries
class BsMenuSendMailLand : public ::MenuBase, public ::state::Mode<BsMenuSendMailLand>
{
public:
    BsMenuSendMailLand(); // ctor candidate(s) 0x002DFF2C (unverified)
    virtual ~BsMenuSendMailLand(); // 0x002DFFEC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002DFFDC slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002DF4DC slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002DFA20 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002DF814 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002DF498 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
