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
    virtual void GetNumber(script::WordNumDigit* number, s32 digits, u32 index); // 0x00634F08 slot 0x08
    virtual void GetTag5_2(script::IWord* word); // 0x0063A3C4 slot 0x24
    virtual void GetTag5_3(script::IWord* word); // 0x0063A2F8 slot 0x28
    virtual void GetWord228(script::WordKey<9, int>* words, u32 index); // 0x0063E7E0 slot 0x2C
    virtual void GetTag5_7(script::IWord* word); // 0x00635400 slot 0x30
    virtual void GetWord238(script::IWord* word); // 0x00637738 slot 0x34
    virtual void GetTag5_12(script::IWord* word); // 0x00643C60 slot 0x38
    virtual void GetTag5_31(script::IWord* word, u32 index); // 0x0063567C slot 0x3C
    virtual void GetWord298(script::WordKey<9, int>* words, u32 index); // 0x0063A0DC slot 0x40
    virtual void GetTag5_35(script::IWord* word, u32 index); // 0x006413BC slot 0x44
    virtual void GetTag5_32(script::IWord* word, u32 index); // 0x00636C88 slot 0x48
    virtual void GetWord2AC(script::IWord* word, u32 index, u32 subIndex); // 0x0063570C slot 0x4C
    virtual s32 GetValue88(u8 gender); // 0x00639FE0 slot 0x88
    virtual void OnMessageStart(script::TalkLabel* message); // 0x00636828 slot 0x9C
    virtual void OnBegin(u8 value); // 0x00634DB4 slot 0xA0
    virtual void OnPageEnd(u8 value); // 0x00636518 slot 0xA4
    virtual void OnChoice(const script::ChoiceItem* item, s32 cursor); // 0x00639E28 slot 0xA8
    virtual void OnEnd(); // 0x00639E9C slot 0xAC
    virtual void GetWord198(script::IWord* word); // 0x0063D280 slot 0xB8
    virtual void OnTag7(u32 index); // 0x00641180 slot 0xBC
    virtual void OnTag7_18(u8 a, u8 b, u16 c, u16 d); // 0x00642D60 slot 0xC0
    virtual s32 GetBranch10_0(); // 0x0063E734 slot 0xC4
    virtual s32 GetBranch10_11(); // 0x00634EA4 slot 0xC8
    virtual s32 GetBranch10_23(); // 0x0063D424 slot 0xCC
    virtual s32 GetBranch10_30(); // 0x0063A070 slot 0xD4
    virtual void OnTag11(const script::Tag* tag); // 0x0064CC98 slot 0xD8
    virtual bool ProcessTag13_3(const script::Tag* tag); // 0x006412E0 slot 0xE8
    virtual s32 GetMood(); // 0x00761668 slot 0xF4
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
