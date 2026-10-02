#pragma once

#include "decomp.h"
#include "Button/dButtonActionNode.h"
#include "state/dMode.h"

// RTTI 13ProButtonNode @ 0x008CBA84
// vtable 0x008EF714 (vptr 0x008EF71C), offset_to_top 0, 17 entries
// vtable 0x008EF760 (vptr 0x008EF768), offset_to_top -224, 3 entries
class ProButtonNode : public ::ButtonActionNode, public ::state::Mode<ProButtonNode>
{
public:
    ProButtonNode(); // ctor candidate(s) 0x0023FFEC (unverified)
    virtual ~ProButtonNode(); // 0x00240118 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00240108 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x40(); // 0x007146A0 slot 0x40 | virtual slot, introduced by ProButtonNode
};
