#pragma once

#include "decomp.h"
#include "Other/dDragIconNode.h"

// RTTI 16ItemDragIconNode @ 0x008CC32C
// vtable 0x008F29F4 (vptr 0x008F29FC), offset_to_top 0, 30 entries
class ItemDragIconNode : public ::DragIconNode
{
public:
    ItemDragIconNode(); // ctor candidate(s) 0x002B9A0C (unverified)
    virtual ~ItemDragIconNode(); // 0x002B9AD8 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x002B9A80 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x10(); // 0x002B972C slot 0x10 | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x50(); // 0x002B99D8 slot 0x50 | virtual slot, introduced by DragIconNode
    virtual void vf_0x74(); // 0x002B95F0 slot 0x74 | virtual slot, introduced by ItemDragIconNode
};
