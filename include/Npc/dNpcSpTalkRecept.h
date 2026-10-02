#pragma once

#include "decomp.h"
#include "Npc/dNpcTalkRecept.h"

// RTTI 15NpcSpTalkRecept @ 0x008CC0F8
// vtable 0x008F1C28 (vptr 0x008F1C30), offset_to_top 0, 91 entries
// vtable 0x008F1D9C (vptr 0x008F1DA4), offset_to_top -124, 14 entries
class NpcSpTalkRecept : public ::NpcTalkRecept
{
public:
    NpcSpTalkRecept(); // ctor candidate(s) 0x0029E710 (unverified)
    virtual ~NpcSpTalkRecept(); // 0x0029E750 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x0029E740 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0x118(); // 0x0071C8E0 slot 0x118 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x11C(); // 0x0029DF1C slot 0x11C | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x120(); // 0x0029DFEC slot 0x120 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x138(); // 0x0071C78C slot 0x138 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x140(); // 0x0029E6B8 slot 0x140 | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x144(); // 0x0029E6C0 slot 0x144 | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x148(); // 0x0029E6C8 slot 0x148 | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x14C(); // 0x0029E6D0 slot 0x14C | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x150(); // 0x0029E6D8 slot 0x150 | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x154(); // 0x0029E6E0 slot 0x154 | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x158(); // 0x0029E6E8 slot 0x158 | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x15C(); // 0x0029E6F0 slot 0x15C | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x160(); // 0x0029E6F8 slot 0x160 | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x164(); // 0x0029E700 slot 0x164 | virtual slot, introduced by NpcSpTalkRecept
    virtual void vf_0x168(); // 0x0029E708 slot 0x168 | virtual slot, introduced by NpcSpTalkRecept
};
