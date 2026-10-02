#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"

// RTTI 15BsTimeBelWindow @ 0x008CBFE0
// vtable 0x008F1794 (vptr 0x008F179C), offset_to_top 0, 16 entries
// vtable 0x008F17DC (vptr 0x008F17E4), offset_to_top -20, 3 entries
class BsTimeBelWindow : public ::Base, public ::state::Mode<BsTimeBelWindow>
{
public:
    class MoneyProc;
    BsTimeBelWindow(); // ctor address unknown
    virtual ~BsTimeBelWindow(); // 0x00295E68 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00295E58 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00293ACC slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00294DC8 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00294918 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00293A30 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
