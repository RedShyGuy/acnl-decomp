#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "script/dITalkRecept.h"
#include "state/dMode.h"

// RTTI 18BsMenuExplainPlate @ 0x008CC74C
// vtable 0x008F40D8 (vptr 0x008F40E0), offset_to_top 0, 26 entries
// vtable 0x008F4148 (vptr 0x008F4150), offset_to_top -40, 63 entries
// vtable 0x008F424C (vptr 0x008F4254), offset_to_top -164, 3 entries
class BsMenuExplainPlate : public ::MenuBase, public ::script::ITalkRecept, public ::state::Mode<BsMenuExplainPlate>
{
public:
    BsMenuExplainPlate(); // ctor address unknown
    virtual ~BsMenuExplainPlate(); // 0x002DA06C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002DA05C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002D96C8 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002D9E30 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002D9D84 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002D9630 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x64(); // 0x002D7824 slot 0x64 | virtual slot, introduced by BsMenuExplainPlate
};
