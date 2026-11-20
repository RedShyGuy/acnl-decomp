#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "nn/math/math_Vector3.h"

extern const nn::math::VEC3 VEC3_ZERO;

// collects the positions of the NPCs (at most 10; one NPC can be left out; members are ours)
// RTTI N8BsNpcMgr11NpcPosGetCBE @ 0x008D3FDC
// vtable 0x0090C06C (vptr 0x0090C074), offset_to_top 0, 1 entries
class BsNpcMgr::NpcPosGetCB : public ::BsNpcMgr::NpcForeachFunction
{
public:
    static const s32 POSITION_NUM = 10;

    explicit NpcPosGetCB(AcNpc* exclude); // 0x006960A8
    void Invoke(AcNpc* npc) override; // 0x006960E4 slot 0x00

    // the position at index (zero after the last; inline, the copy lies in the shared code)
    DECOMP_EMIT_COPY DECOMP_NOINLINE nn::math::VEC3 GetPosition(s32 index) const // 0x00766FC4
    {
        if (index < m_Num) {
            return m_Positions[index];
        }
        return VEC3_ZERO;
    }
    s32 GetNum() const { return m_Num; }

    AcNpc* m_pExclude;                          // 0x04
    nn::math::VEC3 m_Positions[POSITION_NUM];   // 0x08
    s32 m_Num;                                  // 0x80
};
ASSERT_SIZE(BsNpcMgr::NpcPosGetCB, 0x84);
