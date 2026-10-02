#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpRollan.h"
#include "Npc/dNpcSpTalkRecept.h"

// RTTI N13AcNpcSpRollan10TalkReceptE @ 0x008CD95C
// vtable 0x008FB204 (vptr 0x008FB20C), offset_to_top 0, 91 entries
// vtable 0x008FB378 (vptr 0x008FB380), offset_to_top -124, 14 entries
class AcNpcSpRollan::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // 0x00210430 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x00210420 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0x104(); // 0x0020EDF4 slot 0x104 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x108(); // 0x002103CC slot 0x108 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x140(); // 0x0020EF18 slot 0x140 | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x144(); // 0x0020FEF0 slot 0x144 | virtual slot, introduced by NpcSpTalkRecept
};
