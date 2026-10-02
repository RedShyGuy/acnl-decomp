#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "script/dITalkRecept.h"
#include "state/dMode.h"

// RTTI 5BsSkb @ 0x008CD2B0
// vtable 0x008F8A44 (vptr 0x008F8A4C), offset_to_top 0, 25 entries
// vtable 0x008F8AB0 (vptr 0x008F8AB8), offset_to_top -40, 63 entries
// vtable 0x008F8BB4 (vptr 0x008F8BBC), offset_to_top -164, 3 entries
class BsSkb : public ::MenuBase, public ::script::ITalkRecept, public ::state::Mode<BsSkb>
{
public:
    BsSkb(); // ctor candidate(s) 0x00581870 (unverified)
    virtual ~BsSkb(); // 0x00581AB8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00581AA0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0057FDF4 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0058100C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00580E0C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0057FDB8 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void OnClose(); // 0x0057C900 slot 0x44 | slot vf_0x44 of MenuBase
};
