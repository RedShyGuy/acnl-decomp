#pragma once

#include "decomp.h"
#include "Other/dDragIconNode.h"

// RTTI 18LetterDragIconNode @ 0x008CC85C
// vtable 0x008F4744 (vptr 0x008F474C), offset_to_top 0, 29 entries
class LetterDragIconNode : public ::DragIconNode
{
public:
    LetterDragIconNode(); // ctor candidate(s) 0x002E3D78 (unverified)
    virtual ~LetterDragIconNode(); // 0x002E3E20 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x002E3DC0 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x50(); // 0x002E3D44 slot 0x50 | virtual slot, introduced by DragIconNode
};
