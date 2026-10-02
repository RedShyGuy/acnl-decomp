#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "script/dITalkRecept.h"
#include "state/dMode.h"

// RTTI 10BsMenuItem @ 0x008CAFE4
// vtable 0x008EBF3C (vptr 0x008EBF44), offset_to_top 0, 26 entries
// vtable 0x008EBFAC (vptr 0x008EBFB4), offset_to_top -40, 63 entries
// vtable 0x008EC0B0 (vptr 0x008EC0B8), offset_to_top -164, 3 entries
class BsMenuItem : public ::MenuBase, public ::script::ITalkRecept, public ::state::Mode<BsMenuItem>
{
public:
    BsMenuItem(); // ctor candidate(s) 0x001A02B4 (unverified)
    virtual ~BsMenuItem(); // 0x001A0480 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001A03E8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0019F90C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0019FFEC slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0019FCF8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0019F884 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void OnClose(); // 0x0019968C slot 0x44 | libgarden
    virtual void vf_0x64(); // 0x0019EBBC slot 0x64 | virtual slot, introduced by BsMenuItem
};
