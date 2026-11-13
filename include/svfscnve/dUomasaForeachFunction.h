#pragma once

#include <cstring>

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

namespace svfscnve {
// Marks the NPCs of the list of BsNpcMgr in order (the actor is free and its object passes Func753588) for the Fishing
// Tourney (SvFishingConvention::Func3039D0; the counterpart of KameyamaForeachFunction of the Bug-Off; member names are
// ours).
// RTTI N8svfscnve21UomasaForeachFunctionE @ 0x008D409C
// vtable 0x0090C200 (vptr 0x0090C208), offset_to_top 0, 1 entries
class UomasaForeachFunction : public ::BsNpcMgr::NpcForeachFunction
{
public:
    UomasaForeachFunction() : m_Num(0)
    {
        std::memset(m_IsMarked, 0, sizeof(m_IsMarked));
    }

    void Invoke(AcNpc* npc) override; // 0x006CD70C slot 0x00

    bool m_IsMarked[10]; // 0x04
    s32 m_Num;           // 0x10 the number of NPCs handed in
};
} // namespace svfscnve
