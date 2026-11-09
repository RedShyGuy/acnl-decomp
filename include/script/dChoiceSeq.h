#pragma once

#include "decomp.h"
#include "script/dSeqEngine.h"
#include "script/dPhrase.h"

namespace script {
class IRecept;
class ITalkRecept;
class Insert;

// Processes the tags of the text of a choice into the text itself (the inserted words are written into the buffer of
// the choice; tags that stay are copied). Member names are ours.
// RTTI N6script9ChoiceSeqE @ 0x008D3E44
// vtable 0x0090BA94 (vptr 0x0090BA9C), offset_to_top 0, 8 entries
class ChoiceSeq : public ::script::SeqEngine
{
public:
    typedef void (ChoiceSeq::*TagFunc)(const Tag* tag);

    // (inline)
    ChoiceSeq(Phrase* pPhrase, IRecept* pRecept, SeqEngine* pSeq)
        : m_pPhrase(pPhrase), m_pRecept(pRecept), m_pSeq(pSeq), m_pWrite(NULL), m_pEnd(NULL), m_IsOverflow(0)
    {
    }
    virtual ~ChoiceSeq(); // 0x006006D4 slot 0x00
    // 0x006006C4 slot 0x04 (deleting dtor)
    virtual void Begin(); // 0x006006A4 slot 0x08
    virtual void End(); // 0x00600608 slot 0x0C
    virtual bool IsContinue(); // 0x006006B0 slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x005FEB84 slot 0x14
    virtual void OnTag(const Tag* tag); // 0x0060060C slot 0x18

    // (inline) the insert of the phrase
    Insert* GetInsert() { return m_pPhrase->m_pInsert; }

    // the tag handlers (generated)
    void TagRuby(const Tag* tag); // 0x005FF68C
    void TagScale(const Tag* tag); // 0x005FF6B4
    void TagSetColor(const Tag* tag); // 0x005FF8B4
    void OnTagGroup0(const Tag* tag); // 0x005FF128
    void Tag1_(const Tag* tag); // 0x005FECD0
    void Tag2_(const Tag* tag); // 0x005FF930
    void Tag3_(const Tag* tag); // 0x005FF450
    void TagYear(const Tag* tag); // 0x005FF760
    void TagMonthName(const Tag* tag); // 0x005FF93C
    void TagDay(const Tag* tag); // 0x005FEE00
    void TagHour(const Tag* tag); // 0x005FF72C
    void TagMinute(const Tag* tag); // 0x005FF4DC
    void TagSecond(const Tag* tag); // 0x005FF510
    void TagNumber1(const Tag* tag); // 0x005FF17C
    void TagNumber2(const Tag* tag); // 0x005FF1C4
    void TagNumber3(const Tag* tag); // 0x005FF20C
    void TagNumber4(const Tag* tag); // 0x005FF254
    void TagNumber5(const Tag* tag); // 0x005FF29C
    void TagNumber6(const Tag* tag); // 0x005FF2E4
    void TagNumber7(const Tag* tag); // 0x005FF32C
    void TagReceptYear(const Tag* tag); // 0x005FF7DC
    void TagReceptMonth(const Tag* tag); // 0x005FF970
    void TagReceptDay(const Tag* tag); // 0x005FF544
    void Tag4_16(const Tag* tag); // 0x005FF794
    void Tag4_17(const Tag* tag); // 0x005FF58C
    void Tag4_18(const Tag* tag); // 0x005FF4A8
    void Tag4_19(const Tag* tag); // 0x005FF6E4
    void Tag4_20(const Tag* tag); // 0x005FFAEC
    void OnTagGroup4(const Tag* tag); // 0x005FEC28
    void Tag5_0(const Tag* tag); // 0x005FF028
    void Tag5_1(const Tag* tag); // 0x005FEFAC
    void Tag5_2(const Tag* tag); // 0x005FFA14
    void Tag5_3(const Tag* tag); // 0x005FF9C8
    void Tag5_4(const Tag* tag); // 0x005FFDBC
    void Tag5_5(const Tag* tag); // 0x005FEE6C
    void Tag5_6(const Tag* tag); // 0x005FF05C
    void Tag5_7(const Tag* tag); // 0x005FEEE8
    void Tag5_8(const Tag* tag); // 0x005FF85C
    void TagWeekday(const Tag* tag); // 0x005FED74
    void TagImpression(const Tag* tag); // 0x005FFD74
    void TagConstellation(const Tag* tag); // 0x00600254
    void Tag5_12(const Tag* tag); // 0x00600220
    void TagGenderText(const Tag* tag); // 0x005FF9FC
    void TagDrink(const Tag* tag); // 0x005FEEA0
    void TagObject(const Tag* tag); // 0x005FF3BC
    void TagSweets(const Tag* tag); // 0x005FF0D8
    void TagSports(const Tag* tag); // 0x005FF090
    void TagHobby(const Tag* tag); // 0x005FEF1C
    void TagFood(const Tag* tag); // 0x005FED2C
    void TagMusic(const Tag* tag); // 0x005FEF64
    void Tag5_21(const Tag* tag); // 0x005FF86C
    void TagAmPm(const Tag* tag); // 0x005FFD40
    void TagPartOfDay(const Tag* tag); // 0x00600300
    void Tag5_24(const Tag* tag); // 0x005FF644
    void Tag5_25(const Tag* tag); // 0x005FFDCC
    void TagTownFruit(const Tag* tag); // 0x005FFB8C
    void Tag5_27(const Tag* tag); // 0x00600288
    void Tag5_28(const Tag* tag); // 0x00600210
    void TagBook(const Tag* tag); // 0x005FECE4
    void TagSeason(const Tag* tag); // 0x005FF404
    void Tag5_31(const Tag* tag); // 0x005FEFE0
    void Tag5_32(const Tag* tag); // 0x005FFA48
    void Tag5_33(const Tag* tag); // 0x005FF374
    void Tag5_34(const Tag* tag); // 0x005FF9B8
    void Tag5_35(const Tag* tag); // 0x005FFF18
    void Tag5_36(const Tag* tag); // 0x006000E8
    void Tag5_37(const Tag* tag); // 0x005FFDD8
    void OnTagGroup5(const Tag* tag); // 0x005FEC7C
    void Tag6_(const Tag* tag); // 0x005FF6E0
    void Tag7_(const Tag* tag); // 0x005FF44C
    void Tag8_(const Tag* tag); // 0x005FF120
    void Tag9_(const Tag* tag); // 0x005FECDC
    void Tag10_(const Tag* tag); // 0x005FECD4
    void Tag11_(const Tag* tag); // 0x005FECD8
    void Tag12_(const Tag* tag); // 0x005FECE0
    void Tag13_(const Tag* tag); // 0x005FEDA8
    void Tag14_0(const Tag* tag); // 0x006001A8
    void Tag14_1(const Tag* tag); // 0x005FFAD8
    void Tag14_2(const Tag* tag); // 0x005FFE0C
    void Tag14_3(const Tag* tag); // 0x00600180
    void Tag14_4(const Tag* tag); // 0x0060030C
    void TagGenderP(const Tag* tag); // 0x00600394
    void TagSexText(const Tag* tag); // 0x0060047C
    void TagGenderN(const Tag* tag); // 0x0060018C
    void TagGender(const Tag* tag); // 0x00600198
    void TagNumberForm(const Tag* tag); // 0x005FFF80
    void TagElision(const Tag* tag); // 0x005FFBBC
    void TagNumberText4(const Tag* tag); // 0x005FF934
    void TagNumberText5(const Tag* tag); // 0x005FF938
    void TagNumberText6(const Tag* tag); // 0x005FFAE8
    void TagPatchimForm(const Tag* tag); // 0x005FFBCC
    void TagPatchimFormRieulFree(const Tag* tag); // 0x005FFE1C
    void TagEpenthesisA(const Tag* tag); // 0x006002A8
    void TagEpenthesisE(const Tag* tag); // 0x006002B8
    void TagGenderEx(const Tag* tag); // 0x00600318
    void OnTagGroup14(const Tag* tag); // 0x005FF454
    void Tag15_(const Tag* tag); // 0x005FFAD4
    void Tag16_(const Tag* tag); // 0x005FF6DC
    void Tag17_0(const Tag* tag); // 0x005FFBAC
    void Tag17_1(const Tag* tag); // 0x005FFB9C
    void Tag17_2(const Tag* tag); // 0x00600170
    void Tag17_3(const Tag* tag); // 0x00600160
    void Tag17_4(const Tag* tag); // 0x00600348
    void Tag17_5(const Tag* tag); // 0x00600428
    void Tag17_6(const Tag* tag); // 0x00600150
    void Tag17_7(const Tag* tag); // 0x005FFF4C
    void Tag17_8(const Tag* tag); // 0x0060011C
    void Tag17_9(const Tag* tag); // 0x00600418
    void Tag17_10(const Tag* tag); // 0x00600298
    void OnTagGroup17(const Tag* tag); // 0x005FF8DC
    void Tag18_0(const Tag* tag); // 0x00600038
    void Tag18_1(const Tag* tag); // 0x006001D8
    void Tag18_2(const Tag* tag); // 0x005FFFC8
    void Tag18_3(const Tag* tag); // 0x006003C0
    void Tag18_4(const Tag* tag); // 0x006003A0
    void Tag18_5(const Tag* tag); // 0x005FFBE0
    void Tag18_6(const Tag* tag); // 0x00600058
    void Tag18_7(const Tag* tag); // 0x006001B8
    void Tag18_8(const Tag* tag); // 0x006003E0
    void Tag18_9(const Tag* tag); // 0x006004A8
    void Tag18_10(const Tag* tag); // 0x00600328
    void Tag18_11(const Tag* tag); // 0x0060045C
    void Tag18_12(const Tag* tag); // 0x005FFB34
    void Tag18_13(const Tag* tag); // 0x00600078
    void Tag18_14(const Tag* tag); // 0x006002C8
    void Tag18_15(const Tag* tag); // 0x005FF60C
    void Tag18_16(const Tag* tag); // 0x005FEE34
    void Tag18_17(const Tag* tag); // 0x006000B0
    void Tag18_18(const Tag* tag); // 0x005FFEF8
    void Tag18_19(const Tag* tag); // 0x005FFC00
    void Tag18_20(const Tag* tag); // 0x005FF824
    void Tag18_21(const Tag* tag); // 0x005FFD08
    void Tag18_22(const Tag* tag); // 0x005FFED8
    void Tag18_23(const Tag* tag); // 0x005FFC58
    void Tag18_24(const Tag* tag); // 0x005FFC78
    void Tag18_25(const Tag* tag); // 0x005FFC98
    void Tag18_26(const Tag* tag); // 0x005FFCD0
    void Tag18_27(const Tag* tag); // 0x005FFE68
    void Tag18_28(const Tag* tag); // 0x005FFC20
    void Tag18_29(const Tag* tag); // 0x005FFEA0
    void Tag18_30(const Tag* tag); // 0x00600000
    void Tag18_31(const Tag* tag); // 0x005FF5D4
    void Tag18_32(const Tag* tag); // 0x005FFB54
    void Tag18_33(const Tag* tag); // 0x005FFF90
    void Tag18_34(const Tag* tag); // 0x005FFE30
    void OnTagGroup18(const Tag* tag); // 0x005FEDAC
    void Tag19_(const Tag* tag); // 0x005FF124
    void Tag20_0(const Tag* tag); // 0x00600528
    void Tag20_1(const Tag* tag); // 0x006005B0
    void Tag20_2(const Tag* tag); // 0x006004E0
    void Tag20_3(const Tag* tag); // 0x00600570
    void Tag20_4(const Tag* tag); // 0x006005F8
    void OnTagGroup20(const Tag* tag); // 0x005FFA80
    void TagGeneric_2(const Tag* tag); // 0x006001AC
    void TagGeneric_3(const Tag* tag); // 0x006005A4
    void TagGeneric_4(const Tag* tag); // 0x0060049C
    static const TagFunc s_Group0Funcs[5];
    static const TagFunc s_Group4Funcs[21];
    static const TagFunc s_Group5Funcs[38];
    static const TagFunc s_Group14Funcs[19];
    static const TagFunc s_Group17Funcs[11];
    static const TagFunc s_Group18Funcs[35];
    static const TagFunc s_Group20Funcs[5];
    static const TagFunc s_GroupFuncs[21];
    static const TagFunc s_GenericFuncs[7];

    Phrase* m_pPhrase;      // 0x8C
    IRecept* m_pRecept;     // 0x90
    SeqEngine* m_pSeq;      // 0x94 the sequence of the message (its last word of attribute 2 is taken over)
    char16* m_pWrite;       // 0x98 the end of the written text
    char16* m_pEnd;         // 0x9C the last character of the buffer
    u8 m_IsOverflow;        // 0xA0 the buffer is full
};
ASSERT_SIZE(ChoiceSeq, 0xA4);
} // namespace script
