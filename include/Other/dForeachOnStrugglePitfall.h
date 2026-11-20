#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"

// calls the slot OnStrugglePitfall of each NPC (BsNpcMgr; members are ours)
// RTTI 24ForeachOnStrugglePitfall @ 0x008CD000
// vtable 0x008F789C (vptr 0x008F78A4), offset_to_top 0, 1 entries
class ForeachOnStrugglePitfall : public ::BsNpcMgr::NpcForeachFunction
{
public:
    // (inline)
    ForeachOnStrugglePitfall() {}
    void Invoke(AcNpc* npc) override; // 0x0033DDCC slot 0x00
};
