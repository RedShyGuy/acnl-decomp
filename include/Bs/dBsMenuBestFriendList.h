#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 20BsMenuBestFriendList @ 0x008CCAAC
// vtable 0x008F53F4 (vptr 0x008F53FC), offset_to_top 0, 25 entries
// vtable 0x008F5460 (vptr 0x008F5468), offset_to_top -40, 3 entries
class BsMenuBestFriendList : public ::MenuBase, public ::state::Mode<BsMenuBestFriendList>
{
public:
    BsMenuBestFriendList(); // ctor address unknown
    virtual ~BsMenuBestFriendList(); // 0x00313898 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00313888 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00312D7C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00313524 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x003133E0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00312D4C slot 0x30 | slot vf_0x30 of oml::framework::Process
};
