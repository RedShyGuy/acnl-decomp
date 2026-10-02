#pragma once

#include "decomp.h"
#include "Button/dButtonActionControl.h"

// RTTI 15DragIconControl @ 0x008CC020
// vtable 0x008F18D4 (vptr 0x008F18DC), offset_to_top 0, 15 entries
class DragIconControl : public ::ButtonActionControl
{
public:
    DragIconControl(); // ctor candidate(s) 0x0029BD7C (unverified)
    virtual ~DragIconControl(); // 0x0029BDEC slot 0x00 | slot vf_0x00 of ssys::st::List
    // 0x0029BDD0 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
    virtual void Update(); // 0x0029BCB4 slot 0x08 | slot vf_0x08 of ButtonActionControl
    virtual void Reset(); // 0x0029BBB8 slot 0x0C | slot vf_0x0C of ButtonActionControl
    virtual void vf_0x10(); // 0x0029A330 slot 0x10 | virtual slot, introduced by ButtonActionControl
    virtual void vf_0x18(); // 0x0029B5EC slot 0x18 | virtual slot, introduced by ButtonActionControl
    virtual void vf_0x20(); // 0x0029AC30 slot 0x20 | virtual slot, introduced by ButtonActionControl
    virtual void vf_0x24(); // 0x0029AC1C slot 0x24 | virtual slot, introduced by ButtonActionControl
    virtual void vf_0x2C(); // 0x0029B6E8 slot 0x2C | virtual slot, introduced by ButtonActionControl
    virtual void vf_0x34(); // 0x0029B5C0 slot 0x34 | virtual slot, introduced by ButtonActionControl
    virtual void vf_0x38(); // 0x0029B760 slot 0x38 | virtual slot, introduced by ButtonActionControl
};
