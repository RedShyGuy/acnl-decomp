#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 24BsMenuBestFriendRegister @ 0x008CCF90
// vtable 0x008F7750 (vptr 0x008F7758), offset_to_top 0, 25 entries
// vtable 0x008F77BC (vptr 0x008F77C4), offset_to_top -40, 3 entries
class BsMenuBestFriendRegister : public ::MenuBase, public ::state::Mode<BsMenuBestFriendRegister>
{
public:
    BsMenuBestFriendRegister(); // ctor address unknown
    virtual ~BsMenuBestFriendRegister(); // 0x0033AD90 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0033AD80 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0033A8D8 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0033AAEC slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0033AA5C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0033A884 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
