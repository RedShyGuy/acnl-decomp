#pragma once

#include "decomp.h"
#include "Other/dDragIconNode.h"
#include "Other/dLayout.h"

// RTTI 20ReactionDragIconNode @ 0x008CCBE8
// vtable 0x008F5968 (vptr 0x008F5970), offset_to_top 0, 29 entries
class ReactionDragIconNode : public ::DragIconNode
{
public:
    ReactionDragIconNode(); // ctor candidate(s) 0x0032155C (unverified)
    virtual ~ReactionDragIconNode(); // 0x003215B4 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00321590 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)

    // (names are ours)
    ::Layout m_IconLayout;                       // 0x130
    u32 m_Unknown0x27C;                          // 0x27C
    ssys::ma::lyt::Animation* m_pGroupAnim;      // 0x280 DragIconGroup::m_Extra (DragIconGroup::AddNode)
};
ASSERT_SIZE(ReactionDragIconNode, 0x284);
