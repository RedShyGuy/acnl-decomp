#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"

// calls the slot OnMoveOutPitfall of each NPC (BsNpcMgr; members are ours)
// RTTI 23ForeachOnMoveOutPitfall @ 0x008CCF08
// vtable 0x008F7230 (vptr 0x008F7238), offset_to_top 0, 1 entries
class ForeachOnMoveOutPitfall : public ::BsNpcMgr::NpcForeachFunction
{
public:
    // (inline)
    ForeachOnMoveOutPitfall() {}
    void Invoke(AcNpc* npc) override; // 0x003353EC slot 0x00
};
