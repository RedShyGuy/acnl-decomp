#pragma once

#include "decomp.h"
#include "script/dIMeasure.h"
#include "script/dSeqEngine.h"
#include "script/dTalkPhrase.h"
#include "script/dTalkSystem.h"

namespace script {
class TalkWin;

// Measures the width of talk messages with their inserted words for the text box of the talk window: the text with
// the inserted words goes into a buffer of 192 characters, which is measured with the font of the text box. The tag
// handlers (generated from the binary) insert the words of the insert of the talk system. Names are ours.
// RTTI N6script11TalkMeasureE @ 0x008D3814
// vtable 0x0090A078 (vptr 0x0090A080), offset_to_top 0, 12 entries
// vtable 0x0090A0B0 (vptr 0x0090A0B8), offset_to_top -140, 6 entries
class TalkMeasure : public ::script::SeqEngine, public ::script::IMeasure
{
public:
    typedef void (TalkMeasure::*TagFunc)(const Tag* tag);
    static const u32 BUFFER_SIZE = 192;

    // (inline)
    TalkMeasure(TalkSystem* pOwner, TalkWin* pWin)
        : m_pOwner(pOwner), m_pWin(pWin), m_pChild(NULL), m_Scale(100), m_IsStop(0), m_IsFlag13(0), m_pWrite(m_Buffer),
          m_pLast(&m_Buffer[BUFFER_SIZE - 1]), m_Buffer()
    {
        for (u32 i = 0; i < BUFFER_SIZE; i++) {
            m_Buffer[i] = 0;
        }
        m_Scale = 100;
        m_IsStop = 0;
        m_pWrite = m_Buffer;
        Reset();
    }
    virtual ~TalkMeasure();
    virtual void Begin(); // 0x005E2350 slot 0x08
    virtual void End(); // 0x005E22AC slot 0x0C
    virtual bool IsContinue(); // 0x005E2388 slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x005E00E0 slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005E22B0 slot 0x18
    virtual IMeasure* GetChild(); // 0x005E0D24 slot 0x20
    virtual f32 Measure(const IWord* word); // 0x005E1344 slot 0x24
    virtual f32 MeasureText(const char16* const* begin, const char16* const* end); // 0x005EAB60 slot 0x28
    virtual bool IsFlag13(); // 0x0075BC9C slot 0x2C

    // the offset of a centred line from its start: half the width of the text box left over by the line (the cursor
    // of TalkSeq; name is ours)
    f32 MeasureCenterOffset(const char16* const* text, u16 scale); // 0x005E1D78

    // (inline) the insert of the talk system
    Insert* GetInsert() { return m_pOwner->m_pPhrase->m_pInsert; }

    // the tag handlers (generated)
    void TagRuby(const Tag* tag); // 0x005E0CE4
    void TagScale(const Tag* tag); // 0x005E0CE8
    void TagPageBreak(const Tag* tag); // 0x005E18D8
    void OnTagGroup0(const Tag* tag); // 0x005E0704
    void Tag1_(const Tag* tag); // 0x005E0234
    void TagYear(const Tag* tag); // 0x005E0E84
    void TagMonthName(const Tag* tag); // 0x005E1070
    void TagDay(const Tag* tag); // 0x005E035C
    void TagHour(const Tag* tag); // 0x005E0E4C
    void TagMinute(const Tag* tag); // 0x005E0B18
    void TagSecond(const Tag* tag); // 0x005E0B50
    void TagNumber1(const Tag* tag); // 0x005E0758
    void TagNumber2(const Tag* tag); // 0x005E07A4
    void TagNumber3(const Tag* tag); // 0x005E07F0
    void TagNumber4(const Tag* tag); // 0x005E083C
    void TagNumber5(const Tag* tag); // 0x005E0888
    void TagNumber6(const Tag* tag); // 0x005E08D4
    void TagNumber7(const Tag* tag); // 0x005E0920
    void TagReceptYear(const Tag* tag); // 0x005E0F08
    void TagReceptMonth(const Tag* tag); // 0x005E10A8
    void TagReceptDay(const Tag* tag); // 0x005E0B88
    void Tag4_16(const Tag* tag); // 0x005E0EBC
    void Tag4_17(const Tag* tag); // 0x005E0BD4
    void Tag4_18(const Tag* tag); // 0x005E0AE0
    void Tag4_19(const Tag* tag); // 0x005E0E00
    void Tag4_20(const Tag* tag); // 0x005E1284
    void OnTagGroup4(const Tag* tag); // 0x005E018C
    void Tag5_0(const Tag* tag); // 0x005E05A8
    void Tag5_1(const Tag* tag); // 0x005E0524
    void Tag5_2(const Tag* tag); // 0x005E1150
    void Tag5_3(const Tag* tag); // 0x005E1108
    void Tag5_4(const Tag* tag); // 0x005E1654
    void Tag5_5(const Tag* tag); // 0x005E03D0
    void Tag5_6(const Tag* tag); // 0x005E05E0
    void Tag5_7(const Tag* tag); // 0x005E0454
    void Tag5_8(const Tag* tag); // 0x005E0F90
    void TagWeekday(const Tag* tag); // 0x005E02D0
    void TagImpression(const Tag* tag); // 0x005E1608
    void TagConstellation(const Tag* tag); // 0x005E1D2C
    void Tag5_12(const Tag* tag); // 0x005E1CF4
    void TagGenderText(const Tag* tag); // 0x005E1140
    void TagDrink(const Tag* tag); // 0x005E0408
    void TagObject(const Tag* tag); // 0x005E09B8
    void TagSweets(const Tag* tag); // 0x005E0664
    void TagSports(const Tag* tag); // 0x005E0618
    void TagHobby(const Tag* tag); // 0x005E048C
    void TagFood(const Tag* tag); // 0x005E0284
    void TagMusic(const Tag* tag); // 0x005E04D8
    void Tag5_21(const Tag* tag); // 0x005E0FA4
    void TagAmPm(const Tag* tag); // 0x005E15D0
    void TagPartOfDay(const Tag* tag); // 0x005E1EB4
    void Tag5_24(const Tag* tag); // 0x005E0C98
    void Tag5_25(const Tag* tag); // 0x005E1668
    void TagTownFruit(const Tag* tag); // 0x005E1330
    void Tag5_27(const Tag* tag); // 0x005E1D64
    void Tag5_28(const Tag* tag); // 0x005E1CE0
    void TagBook(const Tag* tag); // 0x005E0238
    void TagSeason(const Tag* tag); // 0x005E0A04
    void Tag5_31(const Tag* tag); // 0x005E055C
    void Tag5_32(const Tag* tag); // 0x005E1188
    void Tag5_33(const Tag* tag); // 0x005E096C
    void Tag5_34(const Tag* tag); // 0x005E10F4
    void Tag5_35(const Tag* tag); // 0x005E18A0
    void Tag5_36(const Tag* tag); // 0x005E1ACC
    void Tag5_37(const Tag* tag); // 0x005E1678
    void OnTagGroup5(const Tag* tag); // 0x005E01E0
    void Tag6_(const Tag* tag); // 0x005E0D34
    void TagWordRangeBegin(const Tag* tag); // 0x005D8FF4
    void TagWordRangeEnd(const Tag* tag); // 0x005E191C
    void OnTagGroup7(const Tag* tag); // 0x005E0A50
    void TagChoiceJump2(const Tag* tag); // 0x005E0D38
    void TagChoiceJump3(const Tag* tag); // 0x005E0D60
    void TagChoiceJump4(const Tag* tag); // 0x005E0D88
    void TagChoiceJump5(const Tag* tag); // 0x005E0DB0
    void TagChoiceJump6(const Tag* tag); // 0x005E0DD8
    void TagChoiceJump2Cancel(const Tag* tag); // 0x005E1B04
    void TagChoiceJump3Cancel(const Tag* tag); // 0x005E1B2C
    void TagChoiceJump4Cancel(const Tag* tag); // 0x005E1B54
    void TagChoiceJump5Cancel(const Tag* tag); // 0x005E1B7C
    void TagChoiceJump6Cancel(const Tag* tag); // 0x005E1BA4
    void TagChoiceJumpList(const Tag* tag); // 0x005E1044
    void OnTagGroup8(const Tag* tag); // 0x005E06B0
    void Tag14_0(const Tag* tag); // 0x005E1C70
    void Tag14_1(const Tag* tag); // 0x005E126C
    void Tag14_2(const Tag* tag); // 0x005E1778
    void Tag14_3(const Tag* tag); // 0x005E1C40
    void Tag14_4(const Tag* tag); // 0x005E1EC4
    void TagGenderP(const Tag* tag); // 0x005E1F58
    void TagSexText(const Tag* tag); // 0x005E2120
    void TagGenderN(const Tag* tag); // 0x005E1C4C
    void TagGender(const Tag* tag); // 0x005E1C5C
    void TagNumberForm(const Tag* tag); // 0x005E1944
    void TagElision(const Tag* tag); // 0x005E1424
    void TagNumberText4(const Tag* tag); // 0x005E1068
    void TagNumberText5(const Tag* tag); // 0x005E106C
    void TagNumberText6(const Tag* tag); // 0x005E1280
    void TagPatchimForm(const Tag* tag); // 0x005E1438
    void TagPatchimFormRieulFree(const Tag* tag); // 0x005E178C
    void TagEpenthesisA(const Tag* tag); // 0x005E1E50
    void TagEpenthesisE(const Tag* tag); // 0x005E1E64
    void TagGenderEx(const Tag* tag); // 0x005E1ED0
    void OnTagGroup14(const Tag* tag); // 0x005E0AA4
    void TagChoice2(const Tag* tag); // 0x005E16B0
    void TagChoice3(const Tag* tag); // 0x005E16D8
    void TagChoice4(const Tag* tag); // 0x005E1700
    void TagChoice5(const Tag* tag); // 0x005E1728
    void TagChoice6(const Tag* tag); // 0x005E1750
    void TagChoice2Cancel(const Tag* tag); // 0x005E2038
    void TagChoice3Cancel(const Tag* tag); // 0x005E2060
    void TagChoice4Cancel(const Tag* tag); // 0x005E2088
    void TagChoice5Cancel(const Tag* tag); // 0x005E20B0
    void TagChoice6Cancel(const Tag* tag); // 0x005E20D8
    void TagChoiceList(const Tag* tag); // 0x005E1920
    void OnTagGroup15(const Tag* tag); // 0x005E1218
    void Tag17_0(const Tag* tag); // 0x005E1410
    void Tag17_1(const Tag* tag); // 0x005E13FC
    void Tag17_2(const Tag* tag); // 0x005E1C2C
    void Tag17_3(const Tag* tag); // 0x005E1C18
    void Tag17_4(const Tag* tag); // 0x005E1F08
    void Tag17_5(const Tag* tag); // 0x005E2000
    void Tag17_6(const Tag* tag); // 0x005E1C04
    void Tag17_7(const Tag* tag); // 0x005E18E4
    void Tag17_8(const Tag* tag); // 0x005E1BCC
    void Tag17_9(const Tag* tag); // 0x005E1FEC
    void Tag17_10(const Tag* tag); // 0x005E1E3C
    void OnTagGroup17(const Tag* tag); // 0x005E0FF0
    void Tag18_0(const Tag* tag); // 0x005E1A0C
    void Tag18_1(const Tag* tag); // 0x005E1CA4
    void Tag18_2(const Tag* tag); // 0x005E1994
    void Tag18_3(const Tag* tag); // 0x005E1F8C
    void Tag18_4(const Tag* tag); // 0x005E1F68
    void Tag18_5(const Tag* tag); // 0x005E1450
    void Tag18_6(const Tag* tag); // 0x005E1A30
    void Tag18_7(const Tag* tag); // 0x005E1C80
    void Tag18_8(const Tag* tag); // 0x005E1FB0
    void Tag18_9(const Tag* tag); // 0x005E2148
    void Tag18_10(const Tag* tag); // 0x005E1EE4
    void Tag18_11(const Tag* tag); // 0x005E2100
    void Tag18_12(const Tag* tag); // 0x005E12D0
    void Tag18_13(const Tag* tag); // 0x005E1A54
    void Tag18_14(const Tag* tag); // 0x005E1E78
    void Tag18_15(const Tag* tag); // 0x005E0C5C
    void Tag18_16(const Tag* tag); // 0x005E0394
    void Tag18_17(const Tag* tag); // 0x005E1A90
    void Tag18_18(const Tag* tag); // 0x005E187C
    void Tag18_19(const Tag* tag); // 0x005E1474
    void Tag18_20(const Tag* tag); // 0x005E0F54
    void Tag18_21(const Tag* tag); // 0x005E1594
    void Tag18_22(const Tag* tag); // 0x005E1858
    void Tag18_23(const Tag* tag); // 0x005E14D4
    void Tag18_24(const Tag* tag); // 0x005E14F8
    void Tag18_25(const Tag* tag); // 0x005E151C
    void Tag18_26(const Tag* tag); // 0x005E1558
    void Tag18_27(const Tag* tag); // 0x005E17E0
    void Tag18_28(const Tag* tag); // 0x005E1498
    void Tag18_29(const Tag* tag); // 0x005E181C
    void Tag18_30(const Tag* tag); // 0x005E19D0
    void Tag18_31(const Tag* tag); // 0x005E0C20
    void Tag18_32(const Tag* tag); // 0x005E12F4
    void Tag18_33(const Tag* tag); // 0x005E1958
    void Tag18_34(const Tag* tag); // 0x005E17A4
    void OnTagGroup18(const Tag* tag); // 0x005E0308
    void Tag20_0(const Tag* tag); // 0x005E21D0
    void Tag20_1(const Tag* tag); // 0x005E2260
    void Tag20_2(const Tag* tag); // 0x005E2184
    void Tag20_3(const Tag* tag); // 0x005E221C
    void Tag20_4(const Tag* tag); // 0x005F3B10
    void OnTagGroup20(const Tag* tag); // 0x005E11C4
    void TagGeneric_2(const Tag* tag); // 0x005E1C74
    void TagGeneric_3(const Tag* tag); // 0x005E2254
    void TagGeneric_4(const Tag* tag); // 0x005E213C
    static const TagFunc s_Group0Funcs[5];
    static const TagFunc s_Group4Funcs[21];
    static const TagFunc s_Group5Funcs[38];
    static const TagFunc s_Group7Funcs[19];
    static const TagFunc s_Group8Funcs[11];
    static const TagFunc s_Group14Funcs[19];
    static const TagFunc s_Group15Funcs[11];
    static const TagFunc s_Group17Funcs[11];
    static const TagFunc s_Group18Funcs[35];
    static const TagFunc s_Group20Funcs[5];
    static const TagFunc s_GroupFuncs[21];
    static const TagFunc s_GenericFuncs[7];

    TalkSystem* m_pOwner;       // 0x090
    TalkWin* m_pWin;            // 0x094
    TalkMeasure* m_pChild;      // 0x098 the measure of the inserted texts (deleted)
    u16 m_Scale;                // 0x09C the font scale in percent
    u8 m_IsStop;                // 0x09E a line feed came
    u8 m_IsFlag13;              // 0x09F
    char16* m_pWrite;           // 0x0A0 the end of the written text
    char16* m_pLast;            // 0x0A4
    char16 m_Buffer[BUFFER_SIZE]; // 0x0A8
};
ASSERT_SIZE(TalkMeasure, 0x228);
} // namespace script
