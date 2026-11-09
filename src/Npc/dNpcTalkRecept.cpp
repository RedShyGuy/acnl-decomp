#include "script/dITalkRecept.h"
#include "script/dIFlowRecept.h"
#include "Npc/dNpcTalkRecept.h"

// ctor address unknown
NpcTalkRecept::NpcTalkRecept() : IFlowRecept(this)
{
}

// 0x0023B2D8 slot 0x00 | slot vf_0x00 of script::ITalkRecept
NpcTalkRecept::~NpcTalkRecept()
{
}

// 0x002373DC slot 0x24
void NpcTalkRecept::GetTag5_2(script::IWord*)
{
}

// 0x002373D8 slot 0x28
void NpcTalkRecept::GetTag5_3(script::IWord*)
{
}

// 0x002377D4 slot 0x2C
void NpcTalkRecept::GetWord228(script::WordKey<9, int>*, u32)
{
}

// 0x00231444 slot 0x30
void NpcTalkRecept::GetTag5_7(script::IWord*)
{
}

// 0x00231F58 slot 0x34
void NpcTalkRecept::GetWord238(script::IWord*)
{
}

// 0x00238A20 slot 0x38
void NpcTalkRecept::GetTag5_12(script::IWord*)
{
}

// 0x00231448 slot 0x3C
void NpcTalkRecept::GetTag5_31(script::IWord*, u32)
{
}

// 0x002373D4 slot 0x40
void NpcTalkRecept::GetWord298(script::WordKey<9, int>*, u32)
{
}

// 0x002373CC slot 0x88
s32 NpcTalkRecept::GetValue88(u8)
{
}

// 0x0023AD9C slot 0x94
void NpcTalkRecept::Slot94()
{
}

// 0x00238190 slot 0x98
void NpcTalkRecept::OnMessage(script::TalkLabel*)
{
}

// 0x00231ADC slot 0x9C
void NpcTalkRecept::OnMessageStart(script::TalkLabel*)
{
}

// 0x00231170 slot 0xA0
void NpcTalkRecept::OnBegin(u8)
{
}

// 0x0023158C slot 0xA4
void NpcTalkRecept::OnPageEnd(u8)
{
}

// 0x00237260 slot 0xA8
void NpcTalkRecept::OnChoice(const script::ChoiceItem*, s32)
{
}

// 0x002372D4 slot 0xAC
void NpcTalkRecept::OnEnd()
{
}

// 0x0023182C slot 0xB0
bool NpcTalkRecept::ProcessTag2(const script::Tag*)
{
}

// 0x0023105C slot 0xB4
bool NpcTalkRecept::ProcessTag3(const script::Tag*)
{
}

// 0x002384CC slot 0xC0
void NpcTalkRecept::OnTag7_18(u8, u8, u16, u16)
{
}

// 0x0023AE64 slot 0xD8
void NpcTalkRecept::OnTag11(const script::Tag*)
{
}

// 0x006041B4 slot 0xDC
void NpcTalkRecept::OnTag12(const script::Tag*)
{
}

// 0x00231174 slot 0xE0
void NpcTalkRecept::OnTag16(const script::Tag*)
{
}

// 0x00231F3C slot 0xE4
void NpcTalkRecept::OnTag13_0(const script::Tag*)
{
}

// 0x002389EC slot 0xEC
void NpcTalkRecept::OnTag13_4(const script::Tag*)
{
}

// 0x00238FA8 slot 0xF0
void NpcTalkRecept::OnTag13_5(const script::Tag*)
{
}

// 0x007144E0 slot 0xF4
s32 NpcTalkRecept::GetMood()
{
}

// 0x007144F8 slot 0xF8
void NpcTalkRecept::GetVoices(sead::CheckedArray<u8, 4>*)
{
}

// 0x00230600 slot 0xFC | virtual slot, introduced by NpcTalkRecept
void NpcTalkRecept::vf_0xFC()
{
}

// 0x00231918 slot 0x100 | virtual slot, introduced by NpcTalkRecept
void NpcTalkRecept::vf_0x100()
{
}

// 0x00231450 slot 0x104 | virtual slot, introduced by NpcTalkRecept
void NpcTalkRecept::vf_0x104()
{
}

// 0x00237FD8 slot 0x108 | virtual slot, introduced by NpcTalkRecept
void NpcTalkRecept::vf_0x108()
{
}

// 0x00237534 slot 0x10C | virtual slot, introduced by NpcTalkRecept
void NpcTalkRecept::vf_0x10C()
{
}

// 0x00231454 slot 0x110 | virtual slot, introduced by NpcTalkRecept
void NpcTalkRecept::vf_0x110()
{
}

// 0x00231988 slot 0x114 | virtual slot, introduced by NpcTalkRecept
void NpcTalkRecept::vf_0x114()
{
}

// 0x007145A4 slot 0x118 | virtual slot, introduced by NpcTalkRecept
void NpcTalkRecept::vf_0x118()
{
}

// 0x00230708 slot 0x11C | virtual slot, introduced by NpcTalkRecept
void NpcTalkRecept::vf_0x11C()
{
}

// 0x00230DF8 slot 0x120 | virtual slot, introduced by NpcTalkRecept
void NpcTalkRecept::vf_0x120()
{
}

// 0x0023AE78 slot 0x124 | virtual slot, introduced by NpcTalkRecept
void NpcTalkRecept::vf_0x124()
{
}

// 0x00239C30 slot 0x128 | virtual slot, introduced by NpcTalkRecept
void NpcTalkRecept::vf_0x128()
{
}

// 0x00230DEC slot 0x12C | virtual slot, introduced by NpcTalkRecept
void NpcTalkRecept::vf_0x12C()
{
}

// 0x00230A70 slot 0x130 | virtual slot, introduced by NpcTalkRecept
void NpcTalkRecept::vf_0x130()
{
}

// 0x0023144C slot 0x134 | virtual slot, introduced by NpcTalkRecept
void NpcTalkRecept::vf_0x134()
{
}

// 0x00714590 slot 0x138 | virtual slot, introduced by NpcTalkRecept
void NpcTalkRecept::vf_0x138()
{
}

// 0x00238720 slot 0x13C | virtual slot, introduced by NpcTalkRecept
void NpcTalkRecept::vf_0x13C()
{
}

