#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "script/dITalkRecept.h"
#include "state/dMode.h"

// RTTI 16BsMenuItemChange @ 0x008CC1EC
// vtable 0x008F24BC (vptr 0x008F24C4), offset_to_top 0, 25 entries
// vtable 0x008F2528 (vptr 0x008F2530), offset_to_top -40, 63 entries
// vtable 0x008F262C (vptr 0x008F2634), offset_to_top -164, 3 entries
class BsMenuItemChange : public ::MenuBase, public ::script::ITalkRecept, public ::state::Mode<BsMenuItemChange>
{
public:
    BsMenuItemChange(); // ctor address unknown
    virtual ~BsMenuItemChange(); // 0x002B003C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002B002C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002AF794 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002AFC70 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002AFA78 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002AF758 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
