#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 19BsMenuFriendChatLog @ 0x008CC96C
// vtable 0x008F4E10 (vptr 0x008F4E18), offset_to_top 0, 25 entries
// vtable 0x008F4E7C (vptr 0x008F4E84), offset_to_top -40, 3 entries
class BsMenuFriendChatLog : public ::MenuBase, public ::state::Mode<BsMenuFriendChatLog>
{
public:
    BsMenuFriendChatLog(); // ctor address unknown
    virtual ~BsMenuFriendChatLog(); // 0x002F2F78 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002F2F68 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002F2990 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002F2DC8 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002F2D24 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002F2950 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
