#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 23BsMenuBestFriendListMgr @ 0x008CCE98
// vtable 0x008F7090 (vptr 0x008F7098), offset_to_top 0, 25 entries
// vtable 0x008F70FC (vptr 0x008F7104), offset_to_top -40, 3 entries
class BsMenuBestFriendListMgr : public ::MenuBase, public ::state::Mode<BsMenuBestFriendListMgr>
{
public:
    BsMenuBestFriendListMgr(); // ctor candidate(s) 0x00333F84 (unverified)
    virtual ~BsMenuBestFriendListMgr(); // 0x006AB504 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00333FD4 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00333E2C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00333F6C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00333EDC slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00333E24 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
