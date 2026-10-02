#pragma once

#include "decomp.h"
#include "Other/dDragIconNode.h"

// RTTI 20MyDesignDragIconNode @ 0x008CCBDC
// vtable 0x008F58EC (vptr 0x008F58F4), offset_to_top 0, 29 entries
class MyDesignDragIconNode : public ::DragIconNode
{
public:
    MyDesignDragIconNode(); // ctor candidate(s) 0x0032106C (unverified)
    virtual ~MyDesignDragIconNode(); // 0x003210F8 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x003210B0 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x50(); // 0x00321038 slot 0x50 | virtual slot, introduced by DragIconNode
};
