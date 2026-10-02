#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpResetsan.h"
#include "Npc/dNpcSpTalkRecept.h"

// RTTI N15AcNpcSpResetsan10TalkReceptE @ 0x008CDA48
// vtable 0x008FB660 (vptr 0x008FB668), offset_to_top 0, 91 entries
// vtable 0x008FB7D4 (vptr 0x008FB7DC), offset_to_top -124, 14 entries
class AcNpcSpResetsan::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // 0x00280F28 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x00280F18 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0x104(); // 0x00280118 slot 0x104 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x140(); // 0x002801D4 slot 0x140 | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x144(); // 0x002802AC slot 0x144 | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x148(); // 0x00280384 slot 0x148 | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x154(); // 0x002804DC slot 0x154 | virtual slot, introduced by NpcSpTalkRecept
};
