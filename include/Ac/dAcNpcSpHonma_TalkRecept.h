#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpHonma.h"
#include "Npc/dNpcSpTalkRecept.h"

// RTTI N12AcNpcSpHonma10TalkReceptE @ 0x008CD900
// vtable 0x008FAE5C (vptr 0x008FAE64), offset_to_top 0, 91 entries
// vtable 0x008FAFD0 (vptr 0x008FAFD8), offset_to_top -124, 14 entries
class AcNpcSpHonma::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // 0x001F1CA0 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x001F1C74 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0x104(); // 0x001EDE2C slot 0x104 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x120(); // 0x0029DFE8 slot 0x120 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x140(); // 0x001EDF14 slot 0x140 | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x144(); // 0x001EE338 slot 0x144 | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x14C(); // 0x001EE504 slot 0x14C | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x15C(); // 0x001EE590 slot 0x15C | virtual slot, introduced by NpcSpTalkRecept
};
