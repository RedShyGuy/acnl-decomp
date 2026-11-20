#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"

// BsNpcMgr::OnPreviewEnd for each NPC
// RTTI 19NpcOnPreviewEndFunc @ 0x008CCA20
// vtable 0x008F50D0 (vptr 0x008F50D8), offset_to_top 0, 1 entries
class NpcOnPreviewEndFunc : public ::BsNpcMgr::NpcForeachFunction
{
public:
    // (inline)
    NpcOnPreviewEndFunc() {}
    void Invoke(AcNpc* npc) override; // 0x002FA634 slot 0x00
};
