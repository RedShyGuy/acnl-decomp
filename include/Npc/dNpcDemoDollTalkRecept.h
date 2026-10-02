#pragma once

#include "decomp.h"
#include "Npc/dNpcTalkRecept.h"

// RTTI 21NpcDemoDollTalkRecept @ 0x008CCD24
// vtable 0x008F63D4 (vptr 0x008F63DC), offset_to_top 0, 87 entries
// vtable 0x008F6538 (vptr 0x008F6540), offset_to_top -124, 14 entries
class NpcDemoDollTalkRecept : public ::NpcTalkRecept
{
public:
    NpcDemoDollTalkRecept(); // ctor candidate(s) 0x0032A760 (unverified)
    virtual ~NpcDemoDollTalkRecept(); // 0x0032A860 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x0032A7FC slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0x30(); // 0x0071C110 slot 0x30 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x34(); // 0x00329090 slot 0x34 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x44(); // 0x0071C2BC slot 0x44 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x88(); // 0x003291E0 slot 0x88 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x118(); // 0x00725AC0 slot 0x118 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x138(); // 0x007258EC slot 0x138 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x140(); // 0x0032901C slot 0x140 | virtual slot, introduced by NpcDemoDollTalkRecept
    virtual void vf_0x144(); // 0x003299C8 slot 0x144 | virtual slot, introduced by NpcDemoDollTalkRecept
    virtual void vf_0x148(); // 0x003299C0 slot 0x148 | virtual slot, introduced by NpcDemoDollTalkRecept
    virtual void vf_0x14C(); // 0x00329980 slot 0x14C | virtual slot, introduced by NpcDemoDollTalkRecept
    virtual void vf_0x150(); // 0x0032908C slot 0x150 | virtual slot, introduced by NpcDemoDollTalkRecept
    virtual void vf_0x154(); // 0x00329988 slot 0x154 | virtual slot, introduced by NpcDemoDollTalkRecept
    virtual void vf_0x158(); // 0x00329238 slot 0x158 | virtual slot, introduced by NpcDemoDollTalkRecept
};
