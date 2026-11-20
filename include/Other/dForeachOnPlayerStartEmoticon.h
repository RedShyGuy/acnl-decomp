#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"

// calls the slot OnPlayerStartEmoticon of each NPC (BsNpcMgr; members are ours)
// RTTI 28ForeachOnPlayerStartEmoticon @ 0x008CD10C
// vtable 0x008F8120 (vptr 0x008F8128), offset_to_top 0, 1 entries
class ForeachOnPlayerStartEmoticon : public ::BsNpcMgr::NpcForeachFunction
{
public:
    // (inline)
    explicit ForeachOnPlayerStartEmoticon(u8 emoticon) : m_Emoticon(emoticon) {}
    void Invoke(AcNpc* npc) override; // 0x00343974 slot 0x00

    u8 m_Emoticon; // 0x4
};
