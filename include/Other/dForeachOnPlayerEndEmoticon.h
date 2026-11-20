#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"

// calls the slot OnPlayerEndEmoticon of each NPC (BsNpcMgr; members are ours)
// RTTI 26ForeachOnPlayerEndEmoticon @ 0x008CD0BC
// vtable 0x008F7EC8 (vptr 0x008F7ED0), offset_to_top 0, 1 entries
class ForeachOnPlayerEndEmoticon : public ::BsNpcMgr::NpcForeachFunction
{
public:
    // (inline)
    explicit ForeachOnPlayerEndEmoticon(u8 emoticon) : m_Emoticon(emoticon) {}
    void Invoke(AcNpc* npc) override; // 0x00342B50 slot 0x00

    u8 m_Emoticon; // 0x4
};
