#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"

// BsNpcMgr::OnPreview for each NPC (members are ours)
// RTTI 16NpcOnPreviewFunc @ 0x008CC368
// vtable 0x008F2B38 (vptr 0x008F2B40), offset_to_top 0, 1 entries
class NpcOnPreviewFunc : public ::BsNpcMgr::NpcForeachFunction
{
public:
    // (inline)
    NpcOnPreviewFunc(s32 x0, s32 z0, s32 x1, s32 z1, u8 flag) : m_X0(x0), m_Z0(z0), m_X1(x1), m_Z1(z1), m_Flag(flag) {}
    void Invoke(AcNpc* npc) override; // 0x002BB598 slot 0x00

    s32 m_X0;  // 0x04
    s32 m_Z0;  // 0x08
    s32 m_X1;  // 0x0C
    s32 m_Z1;  // 0x10
    u8 m_Flag; // 0x14
};
ASSERT_SIZE(NpcOnPreviewFunc, 0x18);
