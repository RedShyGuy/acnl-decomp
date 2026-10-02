#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"

// RTTI 9BsMenuMgr @ 0x008CD708
// vtable 0x008FA3D0 (vptr 0x008FA3D8), offset_to_top 0, 16 entries
// vtable 0x008FA418 (vptr 0x008FA420), offset_to_top -20, 3 entries
class BsMenuMgr : public ::Base, public ::state::Mode<BsMenuMgr>
{
public:
    struct Menu { u32 _unknown; }; // TODO: real type unknown (placeholder)
    BsMenuMgr(); // ctor candidate(s) 0x006D52B4 (unverified)
    virtual ~BsMenuMgr(); // 0x006D53A4 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x006D5378 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x006D4978 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x006D5158 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x006D4CE0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x006D4970 slot 0x30 | slot vf_0x30 of oml::framework::Process
    void CanOpenMenuThink(); // 0x005C4B04 | libgarden [tier A]
    void CreateMenuProcess(BsMenuMgr::Menu, unsigned char); // 0x006D2B4C | libgarden [tier A]
    void IsPressed(unsigned long); // 0x006D4C24 | libgarden [tier A]
};
