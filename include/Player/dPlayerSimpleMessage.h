#pragma once

#include "decomp.h"
#include "script/dITalkRecept.h"

// RTTI 19PlayerSimpleMessage @ 0x008CCA38
// vtable 0x008F5110 (vptr 0x008F5118), offset_to_top 0, 63 entries
class PlayerSimpleMessage : public ::script::ITalkRecept
{
public:
    PlayerSimpleMessage(); // ctor address unknown
    virtual ~PlayerSimpleMessage(); // 0x002FCD28 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x002FCD04 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void GetNumber(script::WordNumDigit* number, s32 digits, u32 index); // 0x002FADE4 slot 0x08
    virtual void GetTag4_19(s32* value, u32 index); // 0x002FBE34 slot 0x1C
    virtual void GetTag4_20(s32* value, u32 index); // 0x002FBF6C slot 0x20
    virtual void GetTag5_32(script::IWord* word, u32 index); // 0x002FB2A0 slot 0x48
    virtual void GetWord2C0(script::IWord* word, u32 index); // 0x002FC04C slot 0x5C
    virtual void OnMessage(script::TalkLabel* message); // 0x002FBFB0 slot 0x98
    virtual void OnBegin(u8 value); // 0x002FADD8 slot 0xA0
    virtual void OnPageEnd(u8 value); // 0x002FAE00 slot 0xA4
    virtual void OnChoice(const script::ChoiceItem* item, s32 cursor); // 0x002FBE60 slot 0xA8
    virtual void OnEnd(); // 0x002FBF00 slot 0xAC
};
