#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 11BsMenuChest @ 0x008CB264
// vtable 0x008EC8CC (vptr 0x008EC8D4), offset_to_top 0, 25 entries
// vtable 0x008EC938 (vptr 0x008EC940), offset_to_top -40, 3 entries
class BsMenuChest : public ::MenuBase, public ::state::Mode<BsMenuChest>
{
public:
    BsMenuChest(); // ctor candidate(s) 0x001C4378 (unverified)
    virtual ~BsMenuChest(); // 0x001C44AC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001C4424 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x001C3C70 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x001C424C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x001C3FF8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x001C3C24 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
