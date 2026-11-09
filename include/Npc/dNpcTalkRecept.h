#pragma once

#include "decomp.h"
#include "script/dIFlowRecept.h"
#include "script/dITalkRecept.h"

// RTTI 13NpcTalkRecept @ 0x008CBA2C
// vtable 0x008EF3C8 (vptr 0x008EF3D0), offset_to_top 0, 80 entries
// vtable 0x008EF510 (vptr 0x008EF518), offset_to_top -124, 14 entries
class NpcTalkRecept : public ::script::ITalkRecept, public ::script::IFlowRecept
{
public:
    class AsyncSaveHandleHook;
    class ExeHookFuncBase;
    NpcTalkRecept(); // ctor address unknown
    virtual ~NpcTalkRecept(); // 0x0023B2D8 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x0023B2C4 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void GetTag5_2(script::IWord* word); // 0x002373DC slot 0x24
    virtual void GetTag5_3(script::IWord* word); // 0x002373D8 slot 0x28
    virtual void GetWord228(script::WordKey<9, int>* words, u32 index); // 0x002377D4 slot 0x2C
    virtual void GetTag5_7(script::IWord* word); // 0x00231444 slot 0x30
    virtual void GetWord238(script::IWord* word); // 0x00231F58 slot 0x34
    virtual void GetTag5_12(script::IWord* word); // 0x00238A20 slot 0x38
    virtual void GetTag5_31(script::IWord* word, u32 index); // 0x00231448 slot 0x3C
    virtual void GetWord298(script::WordKey<9, int>* words, u32 index); // 0x002373D4 slot 0x40
    virtual s32 GetValue88(u8 gender); // 0x002373CC slot 0x88
    virtual void Slot94(); // 0x0023AD9C slot 0x94
    virtual void OnMessage(script::TalkLabel* message); // 0x00238190 slot 0x98
    virtual void OnMessageStart(script::TalkLabel* message); // 0x00231ADC slot 0x9C
    virtual void OnBegin(u8 value); // 0x00231170 slot 0xA0
    virtual void OnPageEnd(u8 value); // 0x0023158C slot 0xA4
    virtual void OnChoice(const script::ChoiceItem* item, s32 cursor); // 0x00237260 slot 0xA8
    virtual void OnEnd(); // 0x002372D4 slot 0xAC
    virtual bool ProcessTag2(const script::Tag* tag); // 0x0023182C slot 0xB0
    virtual bool ProcessTag3(const script::Tag* tag); // 0x0023105C slot 0xB4
    virtual void OnTag7_18(u8 a, u8 b, u16 c, u16 d); // 0x002384CC slot 0xC0
    virtual void OnTag11(const script::Tag* tag); // 0x0023AE64 slot 0xD8
    virtual void OnTag12(const script::Tag* tag); // 0x006041B4 slot 0xDC
    virtual void OnTag16(const script::Tag* tag); // 0x00231174 slot 0xE0
    virtual void OnTag13_0(const script::Tag* tag); // 0x00231F3C slot 0xE4
    virtual void OnTag13_4(const script::Tag* tag); // 0x002389EC slot 0xEC
    virtual void OnTag13_5(const script::Tag* tag); // 0x00238FA8 slot 0xF0
    virtual s32 GetMood(); // 0x007144E0 slot 0xF4
    virtual void GetVoices(sead::CheckedArray<u8, 4>* voices); // 0x007144F8 slot 0xF8
    virtual void vf_0xFC(); // 0x00230600 slot 0xFC | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x100(); // 0x00231918 slot 0x100 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x104(); // 0x00231450 slot 0x104 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x108(); // 0x00237FD8 slot 0x108 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x10C(); // 0x00237534 slot 0x10C | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x110(); // 0x00231454 slot 0x110 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x114(); // 0x00231988 slot 0x114 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x118(); // 0x007145A4 slot 0x118 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x11C(); // 0x00230708 slot 0x11C | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x120(); // 0x00230DF8 slot 0x120 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x124(); // 0x0023AE78 slot 0x124 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x128(); // 0x00239C30 slot 0x128 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x12C(); // 0x00230DEC slot 0x12C | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x130(); // 0x00230A70 slot 0x130 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x134(); // 0x0023144C slot 0x134 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x138(); // 0x00714590 slot 0x138 | virtual slot, introduced by NpcTalkRecept
    virtual void vf_0x13C(); // 0x00238720 slot 0x13C | virtual slot, introduced by NpcTalkRecept
};
