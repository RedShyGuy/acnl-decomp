#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 9BsMenuAtm @ 0x008CD6C8
// vtable 0x008FA2F4 (vptr 0x008FA2FC), offset_to_top 0, 25 entries
// vtable 0x008FA360 (vptr 0x008FA368), offset_to_top -40, 3 entries
class BsMenuAtm : public ::MenuBase, public ::state::Mode<BsMenuAtm>
{
public:
    BsMenuAtm(); // ctor address unknown
    virtual ~BsMenuAtm(); // 0x006D1E20 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x006D1E10 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x006D1B18 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x006D1DD0 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x006D1CE4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x006D1AE8 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
