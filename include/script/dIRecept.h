#pragma once

#include "decomp.h"
#include "script/dWordKey.h"

namespace script {
class IWord;
class WordNumDigit;

// The recept of a message: the game object that provides the values of the inserted words (Insert asks it when a word
// is empty). The default implementations do nothing; slot names after the words of Insert they fill (names are ours).
// RTTI N6script7IReceptE @ 0x008D3AC4
class IRecept
{
public:
    // (inline)
    IRecept() {}
    virtual ~IRecept() {}
    virtual void GetNumber(WordNumDigit* number, s32 digits, u32 index); // 0x005F55D4 slot 0x08
    virtual void GetYear(s32* value, u32 index); // 0x005F55F4 slot 0x0C
    virtual void GetMonth(s32* month, u32 index); // 0x005F5604 slot 0x10
    virtual void GetDay(s32* day, u32 index); // 0x005F55E8 slot 0x14
    virtual void GetTag4_18(IWord* word); // 0x005F55E4 slot 0x18
    virtual void GetTag4_19(s32* value, u32 index); // 0x005F55F0 slot 0x1C
    virtual void GetTag4_20(s32* value, u32 index); // 0x005F5614 slot 0x20
    virtual void GetTag5_2(IWord* word); // 0x005F5610 slot 0x24
    virtual void GetTag5_3(IWord* word); // 0x005F560C slot 0x28
    virtual void GetWord228(WordKey<9, int>* words, u32 index); // 0x005F5620 slot 0x2C
    virtual void GetTag5_7(IWord* word); // 0x005F55D8 slot 0x30
    virtual void GetWord238(IWord* word); // 0x005F55F8 slot 0x34
    virtual void GetTag5_12(IWord* word); // 0x005F563C slot 0x38
    virtual void GetTag5_31(IWord* word, u32 index); // 0x005F55DC slot 0x3C
    virtual void GetWord298(WordKey<9, int>* words, u32 index); // 0x005F5608 slot 0x40
    virtual void GetTag5_35(IWord* word, u32 index); // 0x005F5624 slot 0x44
    virtual void GetTag5_32(IWord* word, u32 index); // 0x005F55EC slot 0x48
    virtual void GetWord2AC(IWord* word, u32 index, u32 subIndex); // 0x005F55E0 slot 0x4C
    virtual void GetWord2B4(IWord* word, u32 index); // 0x005F561C slot 0x50
    virtual void GetWord2B8(IWord* word, u32 index); // 0x005F5618 slot 0x54
    virtual void GetWord2BC(IWord* word, u32 index); // 0x005F5638 slot 0x58
    virtual void GetWord2C0(IWord* word, u32 index); // 0x005F5634 slot 0x5C
    virtual void GetTag17_4(IWord* word, u32 index); // 0x005F5640 slot 0x60
    virtual void GetTag17_5(IWord* word); // 0x005F5648 slot 0x64
    virtual void GetWord2CC(IWord* word, u32 index); // 0x005F5630 slot 0x68
    virtual void GetTag0_4(u64* value); // 0x005F5628 slot 0x6C
    virtual void GetTag17_8(IWord* word); // 0x005F562C slot 0x70
    virtual void GetWord2D8(IWord* word, u32 index); // 0x005F5644 slot 0x74
    virtual void GetTag20_0(IWord* word, u32 index); // 0x005F5650 slot 0x78
    virtual void GetTag20_1(IWord* word, u32 index); // 0x005F5658 slot 0x7C
    virtual void GetTag20_2(IWord* word, u32 index); // 0x005F564C slot 0x80
    virtual void GetTag20_3(IWord* word); // 0x005F5654 slot 0x84
    // the index of a STR_Common string for the gender of the word of attribute 8 (Japan only; none: -1)
    virtual s32 GetValue88(u8 gender); // 0x005F55FC slot 0x88
    // the sex of the speaker for the words by sex (1: male, 2: female)
    virtual s32 GetSex() = 0; // slot 0x8C
    // the sex of the speaker for the sex tags (1: male, 2: female)
    virtual s32 GetSpeakerSex() = 0; // slot 0x90
};
} // namespace script
