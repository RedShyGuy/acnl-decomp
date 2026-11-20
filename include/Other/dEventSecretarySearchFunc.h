#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

// finds an event secretary (AcNpcSpSecretaryEvent, or AcNpcSpSecretaryGpEvent without an event of Func308E74) that is not
// in the state 27 (BsNpcMgr::IsEventSecretaryPresent; members are ours)
// RTTI 24EventSecretarySearchFunc @ 0x008CCFE8
// vtable 0x008F7868 (vptr 0x008F7870), offset_to_top 0, 1 entries
class EventSecretarySearchFunc : public ::BsNpcMgr::NpcForeachFunction
{
public:
    // (inline)
    EventSecretarySearchFunc() : m_IsFound(false) {}
    void Invoke(AcNpc* npc) override; // 0x0033DB6C slot 0x00

    bool m_IsFound; // 0x04
};
ASSERT_SIZE(EventSecretarySearchFunc, 0x8);
