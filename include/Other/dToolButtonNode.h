#pragma once

#include "decomp.h"
#include "Button/dButtonActionNode.h"
#include "state/dMode.h"

// RTTI 14ToolButtonNode @ 0x008CBE2C
// vtable 0x008F0980 (vptr 0x008F0988), offset_to_top 0, 16 entries
// vtable 0x008F09C8 (vptr 0x008F09D0), offset_to_top -224, 3 entries
class ToolButtonNode : public ::ButtonActionNode, public ::state::Mode<ToolButtonNode>
{
public:
    ToolButtonNode(); // ctor candidate(s) 0x0027C61C (unverified)
    virtual ~ToolButtonNode(); // 0x0027C760 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x0027C6FC slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
};
