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
    virtual void Initialize(); // 0x006D2144 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x006D264C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x006D240C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x006D213C slot 0x30 | slot vf_0x30 of oml::framework::Process
};
