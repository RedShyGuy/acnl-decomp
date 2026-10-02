#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 19BsMenuBulletinBoard @ 0x008CC94C
// vtable 0x008F4D90 (vptr 0x008F4D98), offset_to_top 0, 25 entries
// vtable 0x008F4DFC (vptr 0x008F4E04), offset_to_top -40, 3 entries
class BsMenuBulletinBoard : public ::MenuBase, public ::state::Mode<BsMenuBulletinBoard>
{
public:
    BsMenuBulletinBoard(); // ctor address unknown
    virtual ~BsMenuBulletinBoard(); // 0x002F1680 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002F1668 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002F0644 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002F1188 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002F0D08 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002F05EC slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void OnClose(); // 0x002EF0AC slot 0x44 | slot vf_0x44 of MenuBase
};
