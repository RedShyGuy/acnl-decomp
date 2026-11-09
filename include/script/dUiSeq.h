#pragma once

#include "decomp.h"
#include "script/dSeqEngine.h"
#include "script/dUiPhrase.h"
#include "script/dUiSystem.h"

namespace script {
class IWord;
class UiSystem;

// Writes the texts of the user interface with their inserted words (member names are ours).
// RTTI N6script5UiSeqE
// vtable 0x0090A594 (vptr 0x0090A59C), offset_to_top 0, 8 entries
class UiSeq : public ::script::SeqEngine
{
public:
    typedef void (UiSeq::*TagFunc)(const Tag* tag);

    // (inline in UiSystem)
    explicit UiSeq(UiSystem* pOwner)
        : m_pOwner(pOwner), m_IsColored(0), m_IsOk(1), m_pResult(NULL), m_pWrite(NULL), m_pLast(NULL), m_IsSuccess(1)
    {
    }
    virtual ~UiSeq(); // 0x005ED450 slot 0x00
    // 0x005ED440 slot 0x04 (deleting dtor)
    virtual void Begin(); // 0x005ED434 slot 0x08
    virtual void OnChar(const CharInfo* info); // 0x005EB490 slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005ED394 slot 0x18

    // the text into word; true: done
    // writes text with its inserted words into word; true: it fitted
    bool Run(IWord* word, const IWord* text, u32 isColored); // 0x005ED20C

    Insert* GetInsert() { return m_pOwner->m_pPhrase->m_pInsert; }

    void TagRuby(const Tag* tag); // 0x005EC07C
    void TagScale(const Tag* tag); // 0x005EC0B0
    void TagSetColor(const Tag* tag); // 0x005EC2D4
    void TagPageBreak(const Tag* tag); // 0x005ECA28
    void OnTagGroup0(const Tag* tag); // 0x005EBAF0
    void Tag1_(const Tag* tag); // 0x005EB600
    void TagYear(const Tag* tag); // 0x005EC168
    void TagMonthName(const Tag* tag); // 0x005EC364
    void TagDay(const Tag* tag); // 0x005EB728
    void TagHour(const Tag* tag); // 0x005EC130
    void TagMinute(const Tag* tag); // 0x005EBEB0
    void TagSecond(const Tag* tag); // 0x005EBEE8
    void TagNumber1(const Tag* tag); // 0x005EBB44
    void TagNumber2(const Tag* tag); // 0x005EBB90
    void TagNumber3(const Tag* tag); // 0x005EBBDC
    void TagNumber4(const Tag* tag); // 0x005EBC28
    void TagNumber5(const Tag* tag); // 0x005EBC74
    void TagNumber6(const Tag* tag); // 0x005EBCC0
    void TagNumber7(const Tag* tag); // 0x005EBD0C
    void TagReceptYear(const Tag* tag); // 0x005EC1EC
    void TagReceptMonth(const Tag* tag); // 0x005EC39C
    void TagReceptDay(const Tag* tag); // 0x005EBF20
    void Tag4_16(const Tag* tag); // 0x005EC1A0
    void Tag4_17(const Tag* tag); // 0x005EBF6C
    void Tag4_18(const Tag* tag); // 0x005EBE78
    void Tag4_19(const Tag* tag); // 0x005EC0E4
    void Tag4_20(const Tag* tag); // 0x005EC544
    void OnTagGroup4(const Tag* tag); // 0x005EB558
    void Tag5_0(const Tag* tag); // 0x005EB994
    void Tag5_1(const Tag* tag); // 0x005EB900
    void Tag5_2(const Tag* tag); // 0x005EC454
    void Tag5_3(const Tag* tag); // 0x005EC3FC
    void Tag5_4(const Tag* tag); // 0x005EC86C
    void Tag5_5(const Tag* tag); // 0x005EB79C
    void Tag5_6(const Tag* tag); // 0x005EB9DC
    void Tag5_7(const Tag* tag); // 0x005EB830
    void Tag5_8(const Tag* tag); // 0x005EC274
    void TagWeekday(const Tag* tag); // 0x005EB69C
    void TagImpression(const Tag* tag); // 0x005EC820
    void TagConstellation(const Tag* tag); // 0x005ECDF4
    void Tag5_12(const Tag* tag); // 0x005ECDBC
    void TagGenderText(const Tag* tag); // 0x005EC444
    void TagDrink(const Tag* tag); // 0x005EB7E4
    void TagObject(const Tag* tag); // 0x005EBDA4
    void TagSweets(const Tag* tag); // 0x005EBA70
    void TagSports(const Tag* tag); // 0x005EBA24
    void TagHobby(const Tag* tag); // 0x005EB868
    void TagFood(const Tag* tag); // 0x005EB650
    void TagMusic(const Tag* tag); // 0x005EB8B4
    void Tag5_21(const Tag* tag); // 0x005EC288
    void TagAmPm(const Tag* tag); // 0x005EC7E8
    void TagPartOfDay(const Tag* tag); // 0x005ECEB8
    void Tag5_24(const Tag* tag); // 0x005EC030
    void Tag5_25(const Tag* tag); // 0x005EC880
    void TagTownFruit(const Tag* tag); // 0x005EC5F0
    void Tag5_27(const Tag* tag); // 0x005ECE2C
    void Tag5_28(const Tag* tag); // 0x005ECDA8
    void TagBook(const Tag* tag); // 0x005EB604
    void TagSeason(const Tag* tag); // 0x005EBDF0
    void Tag5_31(const Tag* tag); // 0x005EB948
    void Tag5_32(const Tag* tag); // 0x005EC49C
    void Tag5_33(const Tag* tag); // 0x005EBD58
    void Tag5_34(const Tag* tag); // 0x005EC3E8
    void Tag5_35(const Tag* tag); // 0x005EC9F0
    void Tag5_36(const Tag* tag); // 0x005ECC2C
    void Tag5_37(const Tag* tag); // 0x005EC890
    void OnTagGroup5(const Tag* tag); // 0x005EB5AC
    void Tag14_0(const Tag* tag); // 0x005ECD28
    void Tag14_1(const Tag* tag); // 0x005EC52C
    void Tag14_2(const Tag* tag); // 0x005EC8C8
    void Tag14_3(const Tag* tag); // 0x005ECCF8
    void Tag14_4(const Tag* tag); // 0x005ECEC8
    void TagGenderP(const Tag* tag); // 0x005ECF5C
    void TagSexText(const Tag* tag); // 0x005ED06C
    void TagGenderN(const Tag* tag); // 0x005ECD04
    void TagGender(const Tag* tag); // 0x005ECD14
    void TagNumberForm(const Tag* tag); // 0x005ECAA4
    void TagElision(const Tag* tag); // 0x005EC62C
    void TagNumberText4(const Tag* tag); // 0x005EC35C
    void TagNumberText5(const Tag* tag); // 0x005EC360
    void TagNumberText6(const Tag* tag); // 0x005EC540
    void TagPatchimForm(const Tag* tag); // 0x005EC640
    void TagPatchimFormRieulFree(const Tag* tag); // 0x005EC8DC
    void TagEpenthesisA(const Tag* tag); // 0x005ECE54
    void TagEpenthesisE(const Tag* tag); // 0x005ECE68
    void TagGenderEx(const Tag* tag); // 0x005ECED4
    void OnTagGroup14(const Tag* tag); // 0x005EBE3C
    void Tag17_0(const Tag* tag); // 0x005EC618
    void Tag17_1(const Tag* tag); // 0x005EC604
    void Tag17_2(const Tag* tag); // 0x005ECCE4
    void Tag17_3(const Tag* tag); // 0x005ECCD0
    void Tag17_4(const Tag* tag); // 0x005ECF0C
    void Tag17_5(const Tag* tag); // 0x005ED004
    void Tag17_6(const Tag* tag); // 0x005ECCBC
    void Tag17_7(const Tag* tag); // 0x005ECA5C
    void Tag17_8(const Tag* tag); // 0x005ECC74
    void Tag17_9(const Tag* tag); // 0x005ECFF0
    void Tag17_10(const Tag* tag); // 0x005ECE40
    void OnTagGroup17(const Tag* tag); // 0x005EC308
    void Tag18_0(const Tag* tag); // 0x005ECB6C
    void Tag18_1(const Tag* tag); // 0x005ECD5C
    void Tag18_2(const Tag* tag); // 0x005ECAF4
    void Tag18_3(const Tag* tag); // 0x005ECF90
    void Tag18_4(const Tag* tag); // 0x005ECF6C
    void Tag18_5(const Tag* tag); // 0x005EC658
    void Tag18_6(const Tag* tag); // 0x005ECB90
    void Tag18_7(const Tag* tag); // 0x005ECD38
    void Tag18_8(const Tag* tag); // 0x005ECFB4
    void Tag18_9(const Tag* tag); // 0x005ED094
    void Tag18_10(const Tag* tag); // 0x005ECEE8
    void Tag18_11(const Tag* tag); // 0x005ED04C
    void Tag18_12(const Tag* tag); // 0x005EC590
    void Tag18_13(const Tag* tag); // 0x005ECBB4
    void Tag18_14(const Tag* tag); // 0x005ECE7C
    void Tag18_15(const Tag* tag); // 0x005EBFF4
    void Tag18_16(const Tag* tag); // 0x005EB760
    void Tag18_17(const Tag* tag); // 0x005ECBF0
    void Tag18_18(const Tag* tag); // 0x005EC9CC
    void Tag18_19(const Tag* tag); // 0x005EC67C
    void Tag18_20(const Tag* tag); // 0x005EC238
    void Tag18_21(const Tag* tag); // 0x005EC79C
    void Tag18_22(const Tag* tag); // 0x005EC9A8
    void Tag18_23(const Tag* tag); // 0x005EC6DC
    void Tag18_24(const Tag* tag); // 0x005EC700
    void Tag18_25(const Tag* tag); // 0x005EC724
    void Tag18_26(const Tag* tag); // 0x005EC760
    void Tag18_27(const Tag* tag); // 0x005EC930
    void Tag18_28(const Tag* tag); // 0x005EC6A0
    void Tag18_29(const Tag* tag); // 0x005EC96C
    void Tag18_30(const Tag* tag); // 0x005ECB30
    void Tag18_31(const Tag* tag); // 0x005EBFB8
    void Tag18_32(const Tag* tag); // 0x005EC5B4
    void Tag18_33(const Tag* tag); // 0x005ECAB8
    void Tag18_34(const Tag* tag); // 0x005EC8F4
    void OnTagGroup18(const Tag* tag); // 0x005EB6D4
    void Tag19_(const Tag* tag); // 0x005EBABC
    void Tag20_0(const Tag* tag); // 0x005ED11C
    void Tag20_1(const Tag* tag); // 0x005ED1AC
    void Tag20_2(const Tag* tag); // 0x005ED0D0
    void Tag20_3(const Tag* tag); // 0x005ED168
    void Tag20_4(const Tag* tag); // 0x005ED1F8
    void OnTagGroup20(const Tag* tag); // 0x005EC4D8
    void TagGeneric_2(const Tag* tag); // 0x005ECD2C
    void TagGeneric_3(const Tag* tag); // 0x005ED1A0
    void TagGeneric_4(const Tag* tag); // 0x005ED088
    static const TagFunc s_Group0Funcs[5];
    static const TagFunc s_Group4Funcs[21];
    static const TagFunc s_Group5Funcs[38];
    static const TagFunc s_Group14Funcs[19];
    static const TagFunc s_Group17Funcs[11];
    static const TagFunc s_Group18Funcs[35];
    static const TagFunc s_Group20Funcs[5];
    static const TagFunc s_GroupFuncs[21];
    static const TagFunc s_GenericFuncs[7];


    UiSystem* m_pOwner;  // 0x8C
    u8 m_IsColored;      // 0x90 the inserted words get their colors
    u8 m_IsOk;           // 0x91 the result of Run
    IWord* m_pResult;    // 0x94 the word that is written
    char16* m_pWrite;    // 0x98 the end of the written text
    char16* m_pLast;     // 0x9C
    u8 m_IsSuccess;      // 0xA0 the text fitted
};
ASSERT_SIZE(UiSeq, 0xA4);
} // namespace script
