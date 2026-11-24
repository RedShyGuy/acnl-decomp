#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"

// RTTI 9BsMenuMap @ 0x008CD6E8
// vtable 0x008FA374 (vptr 0x008FA37C), offset_to_top 0, 16 entries
// vtable 0x008FA3BC (vptr 0x008FA3C4), offset_to_top -20, 3 entries
class BsMenuMap : public ::Base, public ::state::Mode<BsMenuMap>
{
public:
    BsMenuMap(); // ctor candidate(s) 0x007F3A50 (unverified)
    virtual ~BsMenuMap(); // 0x006D27E8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x006D27B4 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Initialize(); // 0x006D2144 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual oml::framework::Result Finalize(); // 0x006D264C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual oml::framework::Result Calc(); // 0x006D240C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x006D213C slot 0x30 | slot vf_0x30 of oml::framework::Process

    // (not decompiled yet; names are ours) called by BsMenuMgr: in a building / the map is shown (every frame)
    void Func6D2370(); // 0x006D2370
    void Func6D2590(s8 isShown); // 0x006D2590
    bool Func6D1FAC(u32 a, u32 b); // 0x006D1FAC
};
