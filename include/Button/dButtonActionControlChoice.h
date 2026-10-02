#pragma once

#include "decomp.h"
#include "Button/dButtonActionControl.h"

// RTTI 25ButtonActionControlChoice @ 0x008CD054
// vtable 0x008F7C80 (vptr 0x008F7C88), offset_to_top 0, 15 entries
class ButtonActionControlChoice : public ::ButtonActionControl
{
public:
    ButtonActionControlChoice(); // ctor candidate(s) 0x0033F6C4 (unverified)
    virtual ~ButtonActionControlChoice(); // 0x0033F720 slot 0x00 | slot vf_0x00 of ssys::st::List
    // 0x0033F6E8 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
    virtual void vf_0x10(); // 0x0033F3A4 slot 0x10 | virtual slot, introduced by ButtonActionControl
    virtual void vf_0x14(); // 0x0033F61C slot 0x14 | virtual slot, introduced by ButtonActionControl
    virtual void vf_0x1C(); // 0x0033F100 slot 0x1C | virtual slot, introduced by ButtonActionControl
};
