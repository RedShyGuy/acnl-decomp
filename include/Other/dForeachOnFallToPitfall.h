#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"

// calls the slot OnFallToPitfall of each NPC (BsNpcMgr; members are ours)
// RTTI 22ForeachOnFallToPitfall @ 0x008CCDE0
// vtable 0x008F6A48 (vptr 0x008F6A50), offset_to_top 0, 1 entries
class ForeachOnFallToPitfall : public ::BsNpcMgr::NpcForeachFunction
{
public:
    // (inline)
    ForeachOnFallToPitfall() {}
    void Invoke(AcNpc* npc) override; // 0x003300A8 slot 0x00
};
