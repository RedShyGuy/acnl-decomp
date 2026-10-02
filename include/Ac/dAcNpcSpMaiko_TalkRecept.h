#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpMaiko.h"
#include "Npc/dNpcSpTalkRecept.h"

// RTTI N12AcNpcSpMaiko10TalkReceptE @ 0x008CD90C
// vtable 0x008FB010 (vptr 0x008FB018), offset_to_top 0, 91 entries
// vtable 0x008FB184 (vptr 0x008FB18C), offset_to_top -124, 14 entries
class AcNpcSpMaiko::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // 0x0023B2D4 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x001F24F4 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0x104(); // 0x001F21C0 slot 0x104 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x140(); // 0x001F21EC slot 0x140 | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x144(); // 0x001F2274 slot 0x144 | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x14C(); // 0x001F24CC slot 0x14C | virtual slot, introduced by NpcSpTalkRecept
};
