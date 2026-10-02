#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 23BsMenuPassportForFriend @ 0x008CCEB8
// vtable 0x008F7110 (vptr 0x008F7118), offset_to_top 0, 25 entries
// vtable 0x008F717C (vptr 0x008F7184), offset_to_top -40, 3 entries
class BsMenuPassportForFriend : public ::MenuBase, public ::state::Mode<BsMenuPassportForFriend>
{
public:
    BsMenuPassportForFriend(); // ctor address unknown
    virtual ~BsMenuPassportForFriend(); // 0x00334F3C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00334E94 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00334B94 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00334D70 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00334CD0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00334B68 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
