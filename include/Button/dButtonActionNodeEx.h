#pragma once

#include "decomp.h"
#include "Button/dButtonActionNode.h"

// RTTI 18ButtonActionNodeEx @ 0x008CC814
// vtable 0x008F463C (vptr 0x008F4644), offset_to_top 0, 16 entries
class ButtonActionNodeEx : public ::ButtonActionNode
{
public:
    virtual ~ButtonActionNodeEx(); // 0x002E06AC slot 0x00 | libgarden
    // 0x002E05E0 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void ResetSelect(); // 0x002E01BC slot 0x0C | libgarden
    virtual void vf_0x1C(); // 0x002E050C slot 0x1C | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x20(); // 0x002E04AC slot 0x20 | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x24(); // 0x002E0158 slot 0x24 | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x28(); // 0x002E0274 slot 0x28 | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x2C(); // 0x002E01D0 slot 0x2C | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x30(); // 0x002E0364 slot 0x30 | virtual slot, introduced by ButtonActionNode
    ButtonActionNodeEx(); // 0x002E059C | libgarden [tier A]
};
