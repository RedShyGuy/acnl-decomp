#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpPreSisyo.h"
#include "Npc/dNpcSpTalkRecept.h"

// RTTI N15AcNpcSpPreSisyo10TalkReceptE @ 0x008CDA3C
// vtable 0x008FB4AC (vptr 0x008FB4B4), offset_to_top 0, 91 entries
// vtable 0x008FB620 (vptr 0x008FB628), offset_to_top -124, 14 entries
class AcNpcSpPreSisyo::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // 0x0027EEE8 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x0027EED8 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0x104(); // 0x0027EC88 slot 0x104 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x108(); // 0x0027EDF4 slot 0x108 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x140(); // 0x0027ECF8 slot 0x140 | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x144(); // 0x0027ED00 slot 0x144 | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x148(); // 0x0027ED94 slot 0x148 | virtual slot, introduced by NpcSpTalkRecept
};
