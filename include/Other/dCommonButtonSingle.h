#pragma once

#include "decomp.h"
#include "Button/dButtonActionNode.h"

// RTTI 18CommonButtonSingle @ 0x008CC820
// vtable 0x008F4684 (vptr 0x008F468C), offset_to_top 0, 16 entries
class CommonButtonSingle : public ::ButtonActionNode
{
public:
    CommonButtonSingle(); // ctor address unknown
    virtual ~CommonButtonSingle(); // 0x002E1108 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x002E1078 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x10(); // 0x002E0E6C slot 0x10 | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x38(); // 0x00721270 slot 0x38 | virtual slot, introduced by ButtonActionNode
};
