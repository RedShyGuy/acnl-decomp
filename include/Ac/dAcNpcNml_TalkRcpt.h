#pragma once

#include "decomp.h"
#include "Ac/dAcNpcNml.h"
#include "Npc/dNpcTalkRecept.h"

// RTTI N8AcNpcNml8TalkRcptE @ 0x008D3FAC
// vtable 0x0090BDA8 (vptr 0x0090BDB0), offset_to_top 0, 85 entries
// vtable 0x0090BF04 (vptr 0x0090BF0C), offset_to_top -124, 14 entries
class AcNpcNml::TalkRcpt : public ::NpcTalkRecept
{
public:
    TalkRcpt(); // ctor candidate(s) 0x0064CD50 (unverified)
    virtual ~TalkRcpt(); // 0x0064CE00 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x0064CDA4 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0x08(); // 0x00634F08 slot 0x08 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x24(); // 0x0063A3C4 slot 0x24 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x28(); // 0x0063A2F8 slot 0x28 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x2C(); // 0x0063E7E0 slot 0x2C | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x30(); // 0x00635400 slot 0x30 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x34(); // 0x00637738 slot 0x34 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x38(); // 0x00643C60 slot 0x38 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x3C(); // 0x0063567C slot 0x3C | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x40(); // 0x0063A0DC slot 0x40 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x44(); // 0x006413BC slot 0x44 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x48(); // 0x00636C88 slot 0x48 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x4C(); // 0x0063570C slot 0x4C | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x88(); // 0x00639FE0 slot 0x88 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x9C(); // 0x00636828 slot 0x9C | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xA0(); // 0x00634DB4 slot 0xA0 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xA4(); // 0x00636518 slot 0xA4 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xA8(); // 0x00639E28 slot 0xA8 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xAC(); // 0x00639E9C slot 0xAC | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xB8(); // 0x0063D280 slot 0xB8 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xBC(); // 0x00641180 slot 0xBC | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xC0(); // 0x00642D60 slot 0xC0 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xC4(); // 0x0063E734 slot 0xC4 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xC8(); // 0x00634EA4 slot 0xC8 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xCC(); // 0x0063D424 slot 0xCC | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xD4(); // 0x0063A070 slot 0xD4 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xD8(); // 0x0064CC98 slot 0xD8 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xE8(); // 0x006412E0 slot 0xE8 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0xF4(); // 0x00761668 slot 0xF4 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x100(); // 0x00636584 slot 0x100 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x104(); // 0x00635930 slot 0x104 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x108(); // 0x006407A4 slot 0x108 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x10C(); // 0x0063CE9C slot 0x10C | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x118(); // 0x00762BE8 slot 0x118 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x11C(); // 0x006335F8 slot 0x11C | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x120(); // 0x00633C90 slot 0x120 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x128(); // 0x0064A600 slot 0x128 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x12C(); // 0x00633BC4 slot 0x12C | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x130(); // 0x00230A50 slot 0x130 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x138(); // 0x0076184C slot 0x138 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x140(); // 0x00762090 slot 0x140 | virtual slot, introduced by AcNpcNml::TalkRcpt
    virtual void vf_0x144(); // 0x00761F68 slot 0x144 | virtual slot, introduced by AcNpcNml::TalkRcpt
    virtual void vf_0x148(); // 0x007628F0 slot 0x148 | virtual slot, introduced by AcNpcNml::TalkRcpt
    virtual void vf_0x14C(); // 0x00761FE8 slot 0x14C | virtual slot, introduced by AcNpcNml::TalkRcpt
    virtual void vf_0x150(); // 0x00644878 slot 0x150 | virtual slot, introduced by AcNpcNml::TalkRcpt
};
