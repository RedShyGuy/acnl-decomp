#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"

// BsNpcMgr::OnBuild for each NPC (members are ours)
// RTTI 14NpcOnBuildFunc @ 0x008CBD20
// vtable 0x008F053C (vptr 0x008F0544), offset_to_top 0, 1 entries
class NpcOnBuildFunc : public ::BsNpcMgr::NpcForeachFunction
{
public:
    // (inline)
    NpcOnBuildFunc(s32 x0, s32 z0, s32 x1, s32 z1, u8 flag) : m_X0(x0), m_Z0(z0), m_X1(x1), m_Z1(z1), m_Flag(flag) {}
    void Invoke(AcNpc* npc) override; // 0x00270AC4 slot 0x00

    s32 m_X0;  // 0x04
    s32 m_Z0;  // 0x08
    s32 m_X1;  // 0x0C
    s32 m_Z1;  // 0x10
    u8 m_Flag; // 0x14
};
ASSERT_SIZE(NpcOnBuildFunc, 0x18);
