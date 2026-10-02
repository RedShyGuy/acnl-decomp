#pragma once

#include "decomp.h"
#include "Item/dItemDragIconNode.h"

// RTTI 21ItemMultiDragIconNode @ 0x008CCCEC
// vtable 0x008F62D8 (vptr 0x008F62E0), offset_to_top 0, 30 entries
class ItemMultiDragIconNode : public ::ItemDragIconNode
{
public:
    ItemMultiDragIconNode(); // ctor candidate(s) 0x003289D0 (unverified)
    virtual ~ItemMultiDragIconNode(); // 0x00328AC4 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00328A58 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x10(); // 0x00328948 slot 0x10 | virtual slot, introduced by ButtonActionNode
    virtual void vf_0x48(); // 0x00328994 slot 0x48 | virtual slot, introduced by DragIconNode
    virtual void vf_0x54(); // 0x003288F8 slot 0x54 | virtual slot, introduced by DragIconNode
    virtual void vf_0x64(); // 0x0032883C slot 0x64 | virtual slot, introduced by DragIconNode
    virtual void vf_0x68(); // 0x00328858 slot 0x68 | virtual slot, introduced by DragIconNode
    virtual void vf_0x74(); // 0x00328874 slot 0x74 | virtual slot, introduced by ItemDragIconNode
};
