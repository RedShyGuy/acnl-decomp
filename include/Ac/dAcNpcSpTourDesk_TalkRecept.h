#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpTourDesk.h"
#include "Npc/dNpcSpTalkRecept.h"

// RTTI N15AcNpcSpTourDesk10TalkReceptE @ 0x008CDA54
// vtable 0x008FB814 (vptr 0x008FB81C), offset_to_top 0, 91 entries
// vtable 0x008FB988 (vptr 0x008FB990), offset_to_top -124, 14 entries
class AcNpcSpTourDesk::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // 0x00282F18 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x00282F08 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0x98(); // 0x00282ECC slot 0x98 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x104(); // 0x00282E9C slot 0x104 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x11C(); // 0x002825EC slot 0x11C | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x120(); // 0x00282BB8 slot 0x120 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x134(); // 0x00282E58 slot 0x134 | virtual slot, introduced by NpcTalkRecept
};
