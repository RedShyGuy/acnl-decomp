#pragma once

#include "decomp.h"
#include "Npc/dNpcMove.h"

// RTTI 9NpcAction @ 0x008CD7C0
// vtable 0x008FA744 (vptr 0x008FA74C), offset_to_top 0, 2 entries
class NpcAction : public ::NpcMove
{
public:
    NpcAction(); // ctor address unknown
    virtual void vf_0x00(); // 0x006EF008 slot 0x00 | virtual slot, introduced by NpcAction
    virtual void vf_0x04(); // 0x006EEFC4 slot 0x04 | virtual slot, introduced by NpcAction
};
