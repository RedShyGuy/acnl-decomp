#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 14BsMenuSignList @ 0x008CBC18
// vtable 0x008F0008 (vptr 0x008F0010), offset_to_top 0, 25 entries
// vtable 0x008F0074 (vptr 0x008F007C), offset_to_top -40, 3 entries
class BsMenuSignList : public ::MenuBase, public ::state::Mode<BsMenuSignList>
{
public:
    BsMenuSignList(); // ctor candidate(s) 0x00260AD8 (unverified)
    virtual ~BsMenuSignList(); // 0x00260BEC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00260B78 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00260748 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00260A84 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002609F4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00260728 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
