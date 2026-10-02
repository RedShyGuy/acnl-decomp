#pragma once

#include "decomp.h"
#include "ssys/st/dListNode.h"

// RTTI 16ButtonActionNode @ 0x008CC2C0
// vtable 0x008F28E4 (vptr 0x008F28EC), offset_to_top 0, 16 entries
class ButtonActionNode : public ::ssys::st::ListNode
{
public:
    struct Info { u32 _unknown; }; // TODO: real type unknown (placeholder)
    virtual ~ButtonActionNode(); // 0x002B74C4 slot 0x00 | libgarden
    // 0x002B7454 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x08(); // 0x0071DB7C slot 0x08 | virtual slot, introduced by ButtonActionNode
    virtual void ResetSelect(); // 0x002B72E4 slot 0x0C | slot vf_0x0C of ButtonActionNode
    virtual void vf_0x10(); // 0x002B72E0 slot 0x10 | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x14(); // 0x0071DAE0 slot 0x14 | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x18(); // 0x0071DB38 slot 0x18 | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x1C(); // 0x002B7350 slot 0x1C | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x20(); // 0x002B7234 slot 0x20 | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x24(); // 0x002B7040 slot 0x24 | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x28(); // 0x002B70F8 slot 0x28 | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x2C(); // 0x002B7070 slot 0x2C | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x30(); // 0x002B7174 slot 0x30 | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x34(); // 0x002B71AC slot 0x34 | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x38(); // 0x0071DB84 slot 0x38 | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x3C(); // 0x0071D9F8 slot 0x3C | virtual slot, introduced by ButtonActionNode
    void Initialize(ButtonActionNode::Info const&); // 0x002B6FCC | libgarden [tier A]
    ButtonActionNode(); // 0x002B73AC | libgarden [tier A]
};
