#pragma once

#include "decomp.h"
#include "script/dWordFix.h"
#include "script/dWordKey.h"
#include "script/dWordNumDigit.h"
#include "script/dIRecept.h"

namespace script {
class IMeasure;

// The words that the tags of the messages insert: per kind of word an array (mostly 10, for the parameter of the
// tag) with slots to empty, set, update (from the recept of the message, with the inflection) and get them. Kinds
// without a name of the binary are named after the tag (group_index) that gets them, or after the get slot; names are
// ours. The derived classes (TalkInsert, MailInsert, UiInsert) add their own words and the recept.
// RTTI N6script6InsertE @ 0x008D3A90
// vtable 0x0090A5BC (vptr 0x0090A5C4), offset_to_top 0, 192 entries
class Insert
{
public:
    explicit Insert(IMeasure* pMeasure); // 0x005F10B4
    virtual ~Insert(); // 0x005F1694 (the deleting destructor is in the derived classes)

    // all words are emptied
    virtual void Clear(); // 0x005F0990 slot 0x008
    virtual void ClearNumber(u32 index); // 0x005ED58C slot 0x00C
    virtual void ClearYear(u32 index); // 0x005EEC70 slot 0x010
    virtual void ClearMonth(u32 index); // 0x005EEFB0 slot 0x014
    virtual void ClearDay(u32 index); // 0x005EE8E0 slot 0x018
    virtual void ClearTag4_16(u32 index); // 0x005EEC58 slot 0x01C
    virtual void ClearTag4_17(u32 index); // 0x005EE8F8 slot 0x020
    virtual void ClearTag4_18(); // 0x005EE8C4 slot 0x024
    virtual void ClearTag4_19(u32 index); // 0x005EEC38 slot 0x028
    virtual void ClearTag4_20(u32 index); // 0x005EF444 slot 0x02C
    virtual void ClearTag5_0(); // 0x005ED5B4 slot 0x030
    virtual void ClearTag5_6(); // 0x005ED5C8 slot 0x034
    virtual void ClearTag5_7(); // 0x005ED5A0 slot 0x038
    virtual void ClearWord238(); // 0x005EEC84 slot 0x03C
    virtual void ClearConstellation(); // 0x005EFDC4 slot 0x040
    virtual void ClearTag5_12(); // 0x005EFDA8 slot 0x044
    // the words of the derived class are emptied (nothing here)
    virtual void ClearExtra(); // 0x005EF440 slot 0x048
    // a formatted number with its kind and value
    virtual void SetTag4_6(const WordNumDigit& number, u32 index); // 0x005ED4CC slot 0x04C
    // a number (negative: 0)
    virtual void SetTag4_13(s32 value, u32 index); // 0x005EDAC0 slot 0x050
    // the name of the month 1..12
    virtual void SetMonth(s32 month, u32 index); // 0x005EE918 slot 0x054
    // the day 1..31 as text
    virtual void SetDay(s32 day, u32 index); // 0x005ED5DC slot 0x058
    // an hour 0..23 (12-hour format by region)
    virtual void SetTag4_16(u32 hour, u32 index); // 0x005EDA04 slot 0x05C
    // a minute 0..59 (two digits by region)
    virtual void SetTag4_17(u32 minute, u32 index); // 0x005ED620 slot 0x060
    // a measure with one or two decimal places (unit by region)
    virtual void SetTag4_19(u32 value, u32 index); // 0x005ED948 slot 0x064
    // a measure with one decimal place
    virtual void SetTag4_20(u32 value, u32 index); // 0x005EEC98 slot 0x068
    virtual void SetTag5_0(const IWord& word); // 0x005ED51C slot 0x06C
    virtual void SetTag5_5(const IWord& word); // 0x005ED508 slot 0x070
    virtual void SetTag5_6(const IWord& word); // 0x005ED530 slot 0x074
    virtual void SetTag5_35(const IWord& word); // 0x005EF460 slot 0x078
    virtual void SetTag5_37(const IWord& word); // 0x005EF024 slot 0x07C
    virtual void SetTag5_32(const IWord& word, u32 index); // 0x005ED69C slot 0x080
    virtual void SetWord2AC(const IWord& word, u32 index, u32 subIndex); // 0x005ED544 slot 0x084
    virtual void SetWord2B4(const IWord& word, u32 index); // 0x005EEFF8 slot 0x088
    virtual void SetWord2B8(const IWord& word, u32 index); // 0x005EEFCC slot 0x08C
    virtual void SetWord2BC(const IWord& word, u32 index); // 0x005EF94C slot 0x090
    virtual void SetWord2C0(const IWord& word, u32 index); // 0x005EF920 slot 0x094
    virtual void SetTag17_4(const IWord& word, u32 index); // 0x005EFF3C slot 0x098
    virtual void SetTag17_5(const IWord& word); // 0x005F04EC slot 0x09C
    virtual void SetWord2CC(const IWord& word, u32 index); // 0x005EF8F4 slot 0x0A0
    // a 48-bit value as XXXX-XXXX-XXXX
    virtual void SetTag0_4(u64 value); // 0x005EF6E0 slot 0x0A4
    virtual void SetTag17_8(const IWord& word); // 0x005EF8E0 slot 0x0A8
    virtual void SetWord2D8(const IWord& word, u32 index); // 0x005F04C0 slot 0x0AC
    virtual void SetWord2DC(const IWord& word, u32 index); // 0x005EFCF8 slot 0x0B0
    virtual void SetTag20_0(const IWord& word, u32 index); // 0x005F05D0 slot 0x0B4
    virtual void SetTag20_1(const IWord& word, u32 index); // 0x005F0724 slot 0x0B8
    virtual void SetTag20_2(const IWord& word, u32 index); // 0x005F05A4 slot 0x0BC
    virtual void SetTag20_3(const IWord& word); // 0x005F05FC slot 0x0C0
    virtual void UpdateDateYear() = 0; // slot 0x0C4 (the derived classes)
    virtual void UpdateDateMonthName() = 0; // slot 0x0C8 (the derived classes)
    virtual void UpdateDateDay() = 0; // slot 0x0CC (the derived classes)
    virtual void UpdateDateHour() = 0; // slot 0x0D0 (the derived classes)
    virtual void UpdateDateMinute() = 0; // slot 0x0D4 (the derived classes)
    virtual void UpdateDateSecond() = 0; // slot 0x0D8 (the derived classes)
    virtual void UpdateNumber(u32 index, s32 digits, bool isRandom); // 0x005EDB20 slot 0x0DC
    virtual void UpdateYear(u32 index); // 0x005EF17C slot 0x0E0
    virtual void UpdateMonth(u32 index); // 0x005EF474 slot 0x0E4
    virtual void UpdateDay(u32 index); // 0x005EED9C slot 0x0E8
    virtual void UpdateTag4_16(u32 index); // 0x005EF16C slot 0x0EC
    virtual void UpdateTag4_17(u32 index); // 0x005EEE6C slot 0x0F0
    virtual void UpdateTag4_18(); // 0x005EED1C slot 0x0F4
    virtual void UpdateTag4_19(u32 index); // 0x005EF038 slot 0x0F8
    virtual void UpdateTag4_20(u32 index); // 0x005EF720 slot 0x0FC
    virtual void UpdateTag5_0(); // 0x005EE200 slot 0x100
    virtual void UpdateTag5_1() = 0; // slot 0x104 (the derived classes)
    virtual void UpdateTag5_2(); // 0x005EF660 slot 0x108
    virtual void UpdateTag5_3(); // 0x005EF5E0 slot 0x10C
    virtual void UpdateWord228(u32 index); // 0x005EFBA0 slot 0x110
    virtual void UpdateTag5_5(); // 0x005EDC48 slot 0x114
    virtual void UpdateTag5_6(); // 0x005EE2AC slot 0x118
    virtual void UpdateTag5_7(); // 0x005EDE24 slot 0x11C
    virtual void UpdateWord238(); // 0x005EF254 slot 0x120
    virtual void UpdateWeekday() = 0; // slot 0x124 (the derived classes)
    virtual void UpdateImpression(u32 index); // 0x005EFA9C slot 0x128
    virtual void UpdateConstellation(); // 0x005F0310 slot 0x12C
    virtual void UpdateTag5_12(); // 0x005F0290 slot 0x130
    virtual void UpdateDrink(u32 index, bool flag); // 0x005EDCE4 slot 0x134
    virtual void UpdateObject(u32 index, bool flag); // 0x005EEAB0 slot 0x138
    virtual void UpdateSweets(u32 index, bool flag); // 0x005EE5A8 slot 0x13C
    virtual void UpdateSports(u32 index, bool flag); // 0x005EE468 slot 0x140
    virtual void UpdateHobby(u32 index, bool flag); // 0x005EDEA4 slot 0x144
    virtual void UpdateFood(u32 index, bool flag); // 0x005ED808 slot 0x148
    virtual void UpdateMusic(u32 index, bool flag); // 0x005EE02C slot 0x14C
    virtual void UpdateTag5_21(u32 index, bool flag); // 0x005EF2EC slot 0x150
    virtual void UpdateAmPm() = 0; // slot 0x154 (the derived classes)
    virtual void UpdatePartOfDayText() = 0; // slot 0x158 (the derived classes)
    // the am/pm of the hour of Tag4_16
    virtual void UpdateTag5_24(u32 index); // 0x005EEE80 slot 0x15C
    // the part of the day of the hour of Tag4_16
    virtual void UpdateWord278(u32 index); // 0x005EFC38 slot 0x160
    virtual void UpdateTownFruit(u32 index); // 0x005EF818 slot 0x164
    virtual void UpdateWord280(u32 index); // 0x005F03C8 slot 0x168
    virtual void UpdateWord284(u32 index); // 0x005F01A0 slot 0x16C
    virtual void UpdateBook(u32 index, bool flag); // 0x005ED6C8 slot 0x170
    virtual void UpdateSeason(u32 index) = 0; // slot 0x174 (the derived classes)
    virtual void UpdateTag5_31(u32 index); // 0x005EE16C slot 0x178
    virtual void UpdateTag5_33(u32 index); // 0x005EE95C slot 0x17C
    virtual void UpdateWord298(u32 index); // 0x005EF548 slot 0x180
    virtual void UpdateTag5_35(u32 index); // 0x005EFD20 slot 0x184
    virtual void UpdateTag5_36(); // 0x005EFE90 slot 0x188
    virtual void UpdateTag5_37(); // 0x005EFCEC slot 0x18C
    virtual void UpdateTag5_32(u32 index); // 0x005EEF1C slot 0x190
    virtual void UpdateWord2AC(u32 index, u32 subIndex); // 0x005EE3BC slot 0x194
    virtual void UpdateWord198(); // 0x005EF71C slot 0x198
    virtual void UpdateWord2B4(u32 index); // 0x005EFA08 slot 0x19C
    virtual void UpdateWord2B8(u32 index); // 0x005EF974 slot 0x1A0
    virtual void UpdateWord2BC(u32 index); // 0x005F0110 slot 0x1A4
    virtual void UpdateWord2C0(u32 index); // 0x005F007C slot 0x1A8
    virtual void UpdateTag17_4(u32 index); // 0x005F0514 slot 0x1AC
    virtual void UpdateTag17_5(); // 0x005F06A4 slot 0x1B0
    virtual void UpdateWord2CC(u32 index); // 0x005EFFE8 slot 0x1B4
    virtual void UpdateTag0_4(); // 0x005EFDE0 slot 0x1B8
    virtual void UpdateTag17_8(); // 0x005EFF68 slot 0x1BC
    virtual void UpdateWord2D8(u32 index); // 0x005F0610 slot 0x1C0
    virtual void UpdateWord2DC(u32 index); // 0x005F0500 slot 0x1C4
    virtual void UpdateTag20_0(u32 index); // 0x005F07E8 slot 0x1C8
    virtual void UpdateTag20_1(u32 index); // 0x005F08FC slot 0x1CC
    virtual void UpdateTag20_2(u32 index); // 0x005F0754 slot 0x1D0
    virtual void UpdateTag20_3(); // 0x005F087C slot 0x1D4
    virtual void UpdateWord1D8(); // 0x005EED18 slot 0x1D8
    virtual IWord* GetDateYear() = 0; // slot 0x1DC (the derived classes)
    virtual IWord* GetDateMonthName() = 0; // slot 0x1E0 (the derived classes)
    virtual IWord* GetDateDay() = 0; // slot 0x1E4 (the derived classes)
    virtual IWord* GetDateHour() = 0; // slot 0x1E8 (the derived classes)
    virtual IWord* GetDateMinute() = 0; // slot 0x1EC (the derived classes)
    virtual IWord* GetDateSecond() = 0; // slot 0x1F0 (the derived classes)
    virtual IWord* GetNumber(u32 index); // 0x0075C99C slot 0x1F4
    virtual IWord* GetYear(u32 index); // 0x0075CB40 slot 0x1F8
    virtual IWord* GetMonth(u32 index); // 0x0075CB78 slot 0x1FC
    virtual IWord* GetDay(u32 index); // 0x0075CACC slot 0x200
    virtual IWord* GetTag4_16(u32 index); // 0x0075CB30 slot 0x204
    virtual IWord* GetTag4_17(u32 index); // 0x0075CAE4 slot 0x208
    virtual IWord* GetTag4_18(); // 0x0075CAC0 slot 0x20C
    virtual IWord* GetTag4_19(u32 index); // 0x0075CB1C slot 0x210
    virtual IWord* GetTag4_20(u32 index); // 0x0075CBC0 slot 0x214
    virtual IWord* GetTag5_0(); // 0x0075CA24 slot 0x218
    virtual IWord* GetTag5_1() = 0; // slot 0x21C (the derived classes)
    virtual IWord* GetTag5_2(); // 0x0075CBB0 slot 0x220
    virtual IWord* GetTag5_3(); // 0x0075CBA4 slot 0x224
    virtual IWord* GetWord228(u32 index); // 0x0075CC30 slot 0x228
    virtual IWord* GetTag5_5(); // 0x0075C9AC slot 0x22C
    virtual IWord* GetTag5_6(); // 0x0075CA30 slot 0x230
    virtual IWord* GetTag5_7(); // 0x0075C9D0 slot 0x234
    virtual IWord* GetWord238(); // 0x0075CB54 slot 0x238
    virtual IWord* GetWeekday() = 0; // slot 0x23C (the derived classes)
    virtual IWord* GetImpression(u32 index); // 0x0075CC1C slot 0x240
    virtual IWord* GetConstellation(); // 0x0075CCFC slot 0x244
    virtual IWord* GetTag5_12(); // 0x0075CCF0 slot 0x248
    virtual IWord* GetDrink(u32 index); // 0x0075C9B8 slot 0x24C
    virtual IWord* GetObject(u32 index); // 0x0075CAA8 slot 0x250
    virtual IWord* GetSweets(u32 index); // 0x0075CA7C slot 0x254
    virtual IWord* GetSports(u32 index); // 0x0075CA64 slot 0x258
    virtual IWord* GetHobby(u32 index); // 0x0075C9DC slot 0x25C
    virtual IWord* GetFood(u32 index); // 0x0075C984 slot 0x260
    virtual IWord* GetMusic(u32 index); // 0x0075C9F4 slot 0x264
    virtual IWord* GetTag5_21(u32 index); // 0x0075CB60 slot 0x268
    virtual IWord* GetAmPm() = 0; // slot 0x26C (the derived classes)
    virtual IWord* GetPartOfDayText() = 0; // slot 0x270 (the derived classes)
    virtual IWord* GetTag5_24(); // 0x0075CAF8 slot 0x274
    virtual IWord* GetWord278(); // 0x0075CC48 slot 0x278
    virtual IWord* GetTownFruit(u32 index); // 0x0075CBD4 slot 0x27C
    virtual IWord* GetWord280(u32 index); // 0x0075CD08 slot 0x280
    virtual IWord* GetWord284(u32 index); // 0x0075CCD8 slot 0x284
    virtual IWord* GetBook(u32 index); // 0x0075C96C slot 0x288
    virtual IWord* GetSeason(u32 index) = 0; // slot 0x28C (the derived classes)
    virtual IWord* GetTag5_31(u32 index); // 0x0075CA0C slot 0x290
    virtual IWord* GetTag5_33(u32 index); // 0x0075CA94 slot 0x294
    virtual IWord* GetWord298(u32 index); // 0x0075CB8C slot 0x298
    virtual IWord* GetTag5_35(); // 0x0075CC64 slot 0x29C
    virtual IWord* GetTag5_36(); // 0x0075CC7C slot 0x2A0
    virtual IWord* GetTag5_37(); // 0x0075CC54 slot 0x2A4
    virtual IWord* GetTag5_32(u32 index); // 0x0075CB04 slot 0x2A8
    virtual IWord* GetWord2AC(u32 index, u32 subIndex); // 0x0075CA3C slot 0x2AC
    // the empty word (TalkInsert has its own)
    virtual IWord* GetWord2B0(); // 0x0075CBBC slot 0x2B0
    virtual IWord* GetWord2B4(u32 index); // 0x0075CC04 slot 0x2B4
    virtual IWord* GetWord2B8(u32 index); // 0x0075CBEC slot 0x2B8
    virtual IWord* GetWord2BC(u32 index); // 0x0075CCC4 slot 0x2BC
    virtual IWord* GetWord2C0(u32 index); // 0x0075CCAC slot 0x2C0
    virtual IWord* GetTag17_4(u32 index); // 0x0075CD34 slot 0x2C4
    virtual IWord* GetTag17_5(); // 0x0075CD60 slot 0x2C8
    virtual IWord* GetWord2CC(u32 index); // 0x0075CC94 slot 0x2CC
    virtual IWord* GetTag0_4(); // 0x0075CC70 slot 0x2D0
    virtual IWord* GetTag17_8(); // 0x0075CC88 slot 0x2D4
    virtual IWord* GetWord2D8(u32 index); // 0x0075CD48 slot 0x2D8
    virtual IWord* GetWord2DC(u32 index); // 0x0075CD20 slot 0x2DC
    virtual IWord* GetTag20_0(u32 index); // 0x0075CD84 slot 0x2E0
    virtual IWord* GetTag20_1(u32 index); // 0x0075CDA8 slot 0x2E4
    virtual IWord* GetTag20_2(u32 index); // 0x0075CD6C slot 0x2E8
    virtual IWord* GetTag20_3(); // 0x0075CD9C slot 0x2EC
    // the empty word (MailInsert has its own)
    virtual IWord* GetWord2F0(); // 0x0075CC60 slot 0x2F0
    virtual IRecept* GetRecept() = 0; // slot 0x2F4 (the derived classes)
    // (the same as GetRecept)
    virtual const IRecept* GetRecept() const = 0; // slot 0x2F8 (the derived classes)
    // NULL (TalkInsert has its own)
    // whether it is the insert of the talk
    virtual bool IsTalk(); // 0x0075CDC0 slot 0x2FC

protected:
    // the inflection of the word for its tags (InflectEngine)
    DECOMP_NOINLINE void Inflect(IWord* word); // 0x005EE778

public:
    IMeasure* m_pMeasure; // 0x00004 the measure of the inserted texts (the grammar tags pick the widest form while measuring)
    WordNumDigit m_Tag4_6[16]; // 0x00008
    WordNumDigit m_Tag4_13[10]; // 0x00448
    WordFix<33> m_Month[10]; // 0x006F0
    WordFix<33> m_Day[10]; // 0x00A88
    WordNumDigit m_Tag4_16[10]; // 0x00E20
    WordNumDigit m_Tag4_17[10]; // 0x010C8
    WordNumDigit m_Tag4_18; // 0x01370
    WordNumDigit m_Tag4_19[10]; // 0x013B4
    WordNumDigit m_Tag4_20[10]; // 0x0165C
    WordFix<9> m_Tag5_0; // 0x01904
    WordFix<9> m_Tag5_2; // 0x01930
    WordFix<9> m_Tag5_3; // 0x0195C
    WordKey<9, int> m_Word228[10]; // 0x01988
    WordFix<9> m_Tag5_5; // 0x01B90
    WordFix<9> m_Tag5_6; // 0x01BBC
    WordFix<11> m_Tag5_7; // 0x01BE8
    WordFix<9> m_Word238; // 0x01C18
    WordFix<36> m_Impression[10]; // 0x01C44
    WordFix<45> m_Constellation; // 0x02004
    WordFix<45> m_Tag5_12; // 0x02078
    WordKey<29, int> m_Drink[10]; // 0x020EC
    WordKey<29, int> m_Object[10]; // 0x02484
    WordKey<29, int> m_Sweets[10]; // 0x0281C
    WordKey<29, int> m_Sports[10]; // 0x02BB4
    WordKey<57, int> m_Hobby[10]; // 0x02F4C
    WordKey<29, int> m_Food[10]; // 0x03514
    WordKey<29, int> m_Music[10]; // 0x038AC
    WordKey<29, int> m_Tag5_21[10]; // 0x03C44
    WordFix<12> m_Tag5_24; // 0x03FDC
    WordFix<13> m_Word278; // 0x0400C
    WordFix<45> m_TownFruit[10]; // 0x04040
    WordFix<9> m_Word280[10]; // 0x044C8
    WordFix<9> m_Word284[10]; // 0x04680
    WordKey<85, int> m_Book[10]; // 0x04838
    WordFix<45> m_Tag5_31[10]; // 0x05030
    WordKey<45, int> m_Tag5_33[10]; // 0x054B8
    WordKey<9, int> m_Word298[10]; // 0x05990
    WordFix<29> m_Tag5_35; // 0x05B98
    WordFix<9> m_Tag5_36; // 0x05BEC
    WordFix<2> m_Tag5_37; // 0x05C18
    WordFix<109> m_Tag5_32[23]; // 0x05C34
    WordFix<109> m_Word2AC[14][10]; // 0x07220
    WordFix<9> m_Word2B4[10]; // 0x0F790
    WordFix<9> m_Word2B8[10]; // 0x0F948
    WordFix<9> m_Word2BC[10]; // 0x0FB00
    WordFix<33> m_Word2C0[10]; // 0x0FCB8
    WordFix<36> m_Tag17_4[10]; // 0x10050
    WordFix<33> m_Tag17_5; // 0x10410
    WordFix<38> m_Word2CC[10]; // 0x1046C
    WordFix<15> m_Tag0_4; // 0x10854
    WordFix<65> m_Tag17_8; // 0x1088C
    WordFix<33> m_Word2D8[10]; // 0x10928
    WordFix<11> m_Word2DC[10]; // 0x10CC0
    WordFix<45> m_Tag20_0[10]; // 0x10EA0
    WordFix<45> m_Tag20_1[10]; // 0x11328
    WordFix<45> m_Tag20_2[10]; // 0x117B0
    WordFix<64> m_Tag20_3; // 0x11C38
};
ASSERT_SIZE(Insert, 0x11CD0);

// the empty word of the inserts (name is ours)
IWord* GetEmptyWord(); // 0x005EB010
} // namespace script
