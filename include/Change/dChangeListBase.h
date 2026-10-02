#pragma once

#include "decomp.h"
#include "Change/dChangeRentalBase.h"
#include "state/dMode.h"

// RTTI 14ChangeListBase @ 0x008CBC74
// vtable 0x008F0208 (vptr 0x008F0210), offset_to_top 0, 9 entries
// vtable 0x008F0234 (vptr 0x008F023C), offset_to_top -2624, 3 entries
class ChangeListBase : public ::ChangeRentalBase, public ::state::Mode<ChangeListBase>
{
public:
    ChangeListBase(); // ctor candidate(s) 0x00268868, 0x00268A7C (unverified)
    virtual void vf_0x0C(); // 0x00267D88 slot 0x0C | virtual slot, introduced by ChangeRentalBase
    virtual void vf_0x10(); // 0x0026879C slot 0x10 | virtual slot, introduced by ChangeRentalBase
    virtual void vf_0x14(); // 0x00719520 slot 0x14 | virtual slot, introduced by ChangeRentalBase
    virtual void vf_0x18(); // 0x00267DC4 slot 0x18 | virtual slot, introduced by ChangeRentalBase
    virtual void vf_0x1C(); // 0x00267D14 slot 0x1C | virtual slot, introduced by ChangeListBase
    virtual void vf_0x20(); // 0x0026882C slot 0x20 | virtual slot, introduced by ChangeListBase
};
