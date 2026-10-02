#pragma once

#include "decomp.h"
#include "Button/dButtonActionNode.h"

// RTTI 28ButtonActionAllGroupBindNode @ 0x008CD100
// vtable 0x008F80D8 (vptr 0x008F80E0), offset_to_top 0, 16 entries
class ButtonActionAllGroupBindNode : public ::ButtonActionNode
{
public:
    ButtonActionAllGroupBindNode(); // ctor candidate(s) 0x003437FC (unverified)
    virtual ~ButtonActionAllGroupBindNode(); // 0x003438D0 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x0034382C slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void ResetSelect(); // 0x00343594 slot 0x0C | slot vf_0x0C of ButtonActionNode
    virtual void vf_0x1C(); // 0x003437A0 slot 0x1C | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x24(); // 0x00343530 slot 0x24 | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x28(); // 0x00343640 slot 0x28 | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x2C(); // 0x0034359C slot 0x2C | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x30(); // 0x00343730 slot 0x30 | virtual slot, introduced by ButtonActionNode
};
