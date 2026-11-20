#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"

// finds the first NPC with an info (see stage::NpcInfo::Kind; members are ours)
// RTTI N8BsNpcMgr14SearchFunctionE @ 0x008D3FE8
// vtable 0x0090C078 (vptr 0x0090C080), offset_to_top 0, 1 entries
class BsNpcMgr::SearchFunction : public ::BsNpcMgr::NpcForeachFunction
{
public:
    // (inline)
    explicit SearchFunction(const stage::NpcInfo* info) : m_pInfo(info), m_pResult(NULL) {}
    void Invoke(AcNpc* npc) override; // 0x0069670C slot 0x00

    const stage::NpcInfo* m_pInfo; // 0x04
    AcNpc* m_pResult;              // 0x08
};
ASSERT_SIZE(BsNpcMgr::SearchFunction, 0xC);
