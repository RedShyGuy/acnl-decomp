#pragma once

#include "decomp.h"
#include "ssys/st/dList.h"
#include "ssys/st/dListNode.h"

// RTTI 13DragIconGroup @ 0x008CB9DC
// vtable 0x008EF270 (vptr 0x008EF278), offset_to_top 0, 2 entries
// vtable 0x008EF280 (vptr 0x008EF288), offset_to_top -12, 2 entries
class DragIconGroup : public ::ssys::st::List, public ::ssys::st::ListNode
{
public:
    DragIconGroup(); // ctor candidate(s) 0x0022CAF8 (unverified)
    virtual ~DragIconGroup(); // 0x0022CB94 slot 0x00 | slot vf_0x00 of ssys::st::List
    // 0x0022CB84 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
};
