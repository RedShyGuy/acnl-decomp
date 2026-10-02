#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"

// RTTI 16BsMenuFriendChat @ 0x008CC1E0
// vtable 0x008F2450 (vptr 0x008F2458), offset_to_top 0, 25 entries
class BsMenuFriendChat : public ::MenuBase
{
public:
    BsMenuFriendChat(); // ctor address unknown
    virtual ~BsMenuFriendChat(); // 0x002AE830 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002AE814 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002AE4C4 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002AE758 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002AE5CC slot 0x24 | slot vf_0x24 of oml::framework::Process
};
