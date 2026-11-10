#include "script/dTalkMeasure.h"
#include "nw/lyt/lyt_TextBox.h"
#include "script/dChip.h"
#include "script/dITalkRecept.h"
#include "script/dIWord.h"
#include "script/dInsert.h"
#include "script/dRenderTag.h"
#include "script/dSearcher.h"
#include "script/dTalkWin.h"
#include <cmath>
#include <cstring>

namespace script {
// 0x005E00E0 slot 0x14
void TalkMeasure::OnChar(const CharInfo* info)
{
    if (info->units == '\n') {
        m_IsStop = 1;
        return;
    }
    char16 c;
    if (ProcessCapital(&c, info)) {
        if (m_pWrite + 1 <= m_pLast) {
            *m_pWrite = c;
            m_pWrite++;
        }
        if (c != ' ' && c != 0x3000 && c != '\n') {
            m_Frame.lastChar = c;
        }
        return;
    }
    PutChar(&m_pWrite, &m_pLast, info);
    if (info->pText != NULL) {
        c = *info->pText;
        if (c != ' ' && c != 0x3000 && c != '\n') {
            m_Frame.lastChar = c;
        }
    }
}

// 0x005E22B0 slot 0x18
void TalkMeasure::OnTag(const Tag* tag)
{
    if (static_cast<u32>(tag->group) < 21) {
        TagFunc func = s_GroupFuncs[tag->group];
        if (func != NULL) {
            (this->*func)(tag);
        }
    } else if (tag->group == 0xFF && static_cast<u32>(tag->index) < 7) {
        TagFunc func = s_GenericFuncs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005E22AC slot 0x0C
void TalkMeasure::End()
{
    // empty (in the original too)
}

// 0x005E2350 slot 0x08
void TalkMeasure::Begin()
{
    m_Frame.pBegin = m_pCur;
    if (m_Scale != 100) {
        s_ScaleTagText[4] = m_Scale;
        const char16* text = s_ScaleTagText;
        PushText(&text, &s_pNoStop);
    }
}

// 0x005E2388 slot 0x10
bool TalkMeasure::IsContinue()
{
    return !m_IsStop;
}

// 0x005E0D24 slot 0x20
// 0x00774868 (thunk 140)
IMeasure* TalkMeasure::GetChild()
{
    return m_pChild;
}

// 0x005E1344 slot 0x24
// 0x00774878 (thunk 140)
f32 TalkMeasure::Measure(const IWord* word)
{
    for (u32 i = 0; i < BUFFER_SIZE; i++) {
        m_Buffer[i] = 0;
    }
    m_Scale = 100;
    m_IsStop = 0;
    m_pWrite = m_Buffer;
    Reset();
    Run(word->GetWord());
    const char16* begin = m_Buffer;
    const char16* end = m_pWrite;
    return Chip::MeasureWidth(m_pWin->GetTextBox(), &begin, &end);
}

// 0x005E1D78 (name is ours)
f32 TalkMeasure::MeasureCenterOffset(const char16* const* text, u16 scale)
{
    for (u32 i = 0; i < BUFFER_SIZE; i++) {
        m_Buffer[i] = 0;
    }
    m_Scale = scale;
    m_IsStop = 0;
    m_pWrite = m_Buffer;
    Reset();
    m_IsFlag13 = 1;
    SetText(text, &s_pNoStop);
    SetText2(&s_pNoStop);
    m_IsFlag13 = 0;
    const char16* begin = m_Buffer;
    const char16* end = m_pWrite;
    nw::lyt::TextBox* textBox = m_pWin->GetTextBox();
    f32 width = textBox->m_Size.width;
    return std::floor((width - Chip::MeasureWidth(textBox, &begin, &end)) * 0.5f);
}

// 0x005EAB60 slot 0x28
// 0x00774850 (thunk 140)
f32 TalkMeasure::MeasureText(const char16* const* begin, const char16* const* end)
{
    return Chip::MeasureWidth(m_pWin->GetTextBox(), begin, end);
}

// 0x0075BC9C slot 0x2C
// 0x007749D0 (thunk 140)
bool TalkMeasure::IsFlag13()
{
    return m_IsFlag13;
}

// 0x005E23F4 slot 0x00
// 0x005E239C (deleting dtor)
// 0x00774980 (thunk)
// 0x0077492C (deleting thunk)
TalkMeasure::~TalkMeasure()
{
    if (m_pChild != NULL) {
        delete m_pChild;
        m_pChild = NULL;
    }
}

// 0x005E0CE4
void TalkMeasure::TagRuby(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005E0CE8
void TalkMeasure::TagScale(const Tag* tag)
{
    // the whole tag into the text
    u32 num = tag->length / sizeof(char16);
    char16* end = m_pWrite + num;
    if (end <= m_pLast) {
        std::memcpy(m_pWrite, tag->data, num * sizeof(char16));
        m_pWrite = end;
    }
}

// 0x005E18D8
void TalkMeasure::TagPageBreak(const Tag* tag)
{
    m_IsStop = 1;
}

// 0x005E0704
void TalkMeasure::OnTagGroup0(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 5) {
        TagFunc func = s_Group0Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005E0234
void TalkMeasure::Tag1_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005E0E84
void TalkMeasure::TagYear(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateYear(), tag);
}

// 0x005E1070
void TalkMeasure::TagMonthName(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetDateMonthName(), tag);
}

// 0x005E035C
void TalkMeasure::TagDay(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetDateDay(), tag);
}

// 0x005E0E4C
void TalkMeasure::TagHour(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateHour(), tag);
}

// 0x005E0B18
void TalkMeasure::TagMinute(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateMinute(), tag);
}

// 0x005E0B50
void TalkMeasure::TagSecond(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateSecond(), tag);
}

// 0x005E0758
void TalkMeasure::TagNumber1(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005E07A4
void TalkMeasure::TagNumber2(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005E07F0
void TalkMeasure::TagNumber3(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005E083C
void TalkMeasure::TagNumber4(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005E0888
void TalkMeasure::TagNumber5(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005E08D4
void TalkMeasure::TagNumber6(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005E0920
void TalkMeasure::TagNumber7(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005E0F08
void TalkMeasure::TagReceptYear(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetYear(index), tag);
    }
}

// 0x005E10A8
void TalkMeasure::TagReceptMonth(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetMonth(index), tag);
    }
}

// 0x005E0B88
void TalkMeasure::TagReceptDay(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetDay(index), tag);
    }
}

// 0x005E0EBC
void TalkMeasure::Tag4_16(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_16(index), tag);
    }
}

// 0x005E0BD4
void TalkMeasure::Tag4_17(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_17(index), tag);
    }
}

// 0x005E0AE0
void TalkMeasure::Tag4_18(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetTag4_18(), tag);
}

// 0x005E0E00
void TalkMeasure::Tag4_19(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_19(index), tag);
    }
}

// 0x005E1284
void TalkMeasure::Tag4_20(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_20(index), tag);
    }
}

// 0x005E018C
void TalkMeasure::OnTagGroup4(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 21) {
        TagFunc func = s_Group4Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005E05A8
void TalkMeasure::Tag5_0(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_0(), tag);
}

// 0x005E0524
void TalkMeasure::Tag5_1(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_1(), tag);
}

// 0x005E1150
void TalkMeasure::Tag5_2(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_2(), tag);
}

// 0x005E1108
void TalkMeasure::Tag5_3(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_3(), tag);
}

// 0x005E1654
void TalkMeasure::Tag5_4(const Tag* tag)
{
    InsertWord228(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005E03D0
void TalkMeasure::Tag5_5(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_5(), tag);
}

// 0x005E05E0
void TalkMeasure::Tag5_6(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_6(), tag);
}

// 0x005E0454
void TalkMeasure::Tag5_7(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_7(), tag);
}

// 0x005E0F90
void TalkMeasure::Tag5_8(const Tag* tag)
{
    InsertWord238(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005E02D0
void TalkMeasure::TagWeekday(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetWeekday(), tag);
}

// 0x005E1608
void TalkMeasure::TagImpression(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetImpression(index), tag);
    }
}

// 0x005E1D2C
void TalkMeasure::TagConstellation(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetConstellation(), tag);
}

// 0x005E1CF4
void TalkMeasure::Tag5_12(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_12(), tag);
}

// 0x005E1140
void TalkMeasure::TagGenderText(const Tag* tag)
{
    InsertGenderText(m_pOwner->m_pRecept, tag);
}

// 0x005E0408
void TalkMeasure::TagDrink(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetDrink(index), tag);
    }
}

// 0x005E09B8
void TalkMeasure::TagObject(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetObject(index), tag);
    }
}

// 0x005E0664
void TalkMeasure::TagSweets(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetSweets(index), tag);
    }
}

// 0x005E0618
void TalkMeasure::TagSports(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetSports(index), tag);
    }
}

// 0x005E048C
void TalkMeasure::TagHobby(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetHobby(index), tag);
    }
}

// 0x005E0284
void TalkMeasure::TagFood(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetFood(index), tag);
    }
}

// 0x005E04D8
void TalkMeasure::TagMusic(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetMusic(index), tag);
    }
}

// 0x005E0FA4
void TalkMeasure::Tag5_21(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_21(index), tag);
    }
}

// 0x005E15D0
void TalkMeasure::TagAmPm(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetAmPm(), tag);
}

// 0x005E1EB4
void TalkMeasure::TagPartOfDay(const Tag* tag)
{
    InsertTag5_22OrTag5_23(m_pOwner->m_pPhrase, tag);
}

// 0x005E0C98
void TalkMeasure::Tag5_24(const Tag* tag)
{
    // (the index is tested but not passed)
    if (*reinterpret_cast<const u8*>(tag->args) < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_24(), tag);
    }
}

// 0x005E1668
void TalkMeasure::Tag5_25(const Tag* tag)
{
    InsertTag5_24OrWord278(m_pOwner->m_pPhrase, tag);
}

// 0x005E1330
void TalkMeasure::TagTownFruit(const Tag* tag)
{
    InsertTownFruit(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005E1D64
void TalkMeasure::Tag5_27(const Tag* tag)
{
    InsertWord280(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005E1CE0
void TalkMeasure::Tag5_28(const Tag* tag)
{
    InsertWord284(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005E0238
void TalkMeasure::TagBook(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetBook(index), tag);
    }
}

// 0x005E0A04
void TalkMeasure::TagSeason(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetSeason(index), tag);
    }
}

// 0x005E055C
void TalkMeasure::Tag5_31(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_31(index), tag);
    }
}

// 0x005E1188
void TalkMeasure::Tag5_32(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(0), tag);
}

// 0x005E096C
void TalkMeasure::Tag5_33(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_33(index), tag);
    }
}

// 0x005E10F4
void TalkMeasure::Tag5_34(const Tag* tag)
{
    InsertWord298(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005E18A0
void TalkMeasure::Tag5_35(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_35(), tag);
}

// 0x005E1ACC
void TalkMeasure::Tag5_36(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_36(), tag);
}

// 0x005E1678
void TalkMeasure::Tag5_37(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_37(), tag);
}

// 0x005E01E0
void TalkMeasure::OnTagGroup5(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 38) {
        TagFunc func = s_Group5Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005E0D34
void TalkMeasure::Tag6_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005D8FF4
void TalkMeasure::TagWordRangeBegin(const Tag* tag)
{
    InsertWord2B0Range(m_pOwner->m_pPhrase, tag);
}

// 0x005E191C
void TalkMeasure::TagWordRangeEnd(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005E0A50
void TalkMeasure::OnTagGroup7(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 19) {
        TagFunc func = s_Group7Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005E0D38
void TalkMeasure::TagChoiceJump2(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 2);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E0D60
void TalkMeasure::TagChoiceJump3(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 3);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E0D88
void TalkMeasure::TagChoiceJump4(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 4);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E0DB0
void TalkMeasure::TagChoiceJump5(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 5);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E0DD8
void TalkMeasure::TagChoiceJump6(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 6);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E1B04
void TalkMeasure::TagChoiceJump2Cancel(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 2);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E1B2C
void TalkMeasure::TagChoiceJump3Cancel(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 3);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E1B54
void TalkMeasure::TagChoiceJump4Cancel(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 4);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E1B7C
void TalkMeasure::TagChoiceJump5Cancel(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 5);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E1BA4
void TalkMeasure::TagChoiceJump6Cancel(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 6);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E1044
void TalkMeasure::TagChoiceJumpList(const Tag* tag)
{
    s32 length = GetLineLength(tag);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E06B0
void TalkMeasure::OnTagGroup8(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 11) {
        TagFunc func = s_Group8Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005E1C70
void TalkMeasure::Tag14_0(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005E126C
void TalkMeasure::Tag14_1(const Tag* tag)
{
    InsertArticle(GetInsert(), 2);
}

// 0x005E1778
void TalkMeasure::Tag14_2(const Tag* tag)
{
    InsertArticle(GetInsert(), 1);
}

// 0x005E1C40
void TalkMeasure::Tag14_3(const Tag* tag)
{
    m_Frame.isTag2 = 1;
}

// 0x005E1EC4
void TalkMeasure::Tag14_4(const Tag* tag)
{
    m_Frame.isTag3 = 1;
}

// 0x005E1F58
void TalkMeasure::TagGenderP(const Tag* tag)
{
    InsertGenderP(tag, GetInsert());
}

// 0x005E2120
void TalkMeasure::TagSexText(const Tag* tag)
{
    InsertSexText(m_pOwner->m_pRecept, tag, GetInsert());
}

// 0x005E1C4C
void TalkMeasure::TagGenderN(const Tag* tag)
{
    InsertGenderN(tag, GetInsert());
}

// 0x005E1C5C
void TalkMeasure::TagGender(const Tag* tag)
{
    InsertGender(GetInsert(), tag);
}

// 0x005E1944
void TalkMeasure::TagNumberForm(const Tag* tag)
{
    InsertNumberForm(GetInsert(), tag);
}

// 0x005E1424
void TalkMeasure::TagElision(const Tag* tag)
{
    InsertElision(GetInsert(), tag);
}

// 0x005E1068
void TalkMeasure::TagNumberText4(const Tag* tag)
{
    InsertNumberText4(tag);
}

// 0x005E106C
void TalkMeasure::TagNumberText5(const Tag* tag)
{
    InsertNumberText5(tag);
}

// 0x005E1280
void TalkMeasure::TagNumberText6(const Tag* tag)
{
    InsertNumberText6(tag);
}

// 0x005E1438
void TalkMeasure::TagPatchimForm(const Tag* tag)
{
    InsertPatchimForm(GetInsert(), tag, 0);
}

// 0x005E178C
void TalkMeasure::TagPatchimFormRieulFree(const Tag* tag)
{
    InsertPatchimForm(GetInsert(), tag, 1);
}

// 0x005E1E50
void TalkMeasure::TagEpenthesisA(const Tag* tag)
{
    InsertEpenthesisA(GetInsert(), tag);
}

// 0x005E1E64
void TalkMeasure::TagEpenthesisE(const Tag* tag)
{
    InsertEpenthesisE(GetInsert(), tag);
}

// 0x005E1ED0
void TalkMeasure::TagGenderEx(const Tag* tag)
{
    InsertGenderEx(GetInsert(), tag);
}

// 0x005E0AA4
void TalkMeasure::OnTagGroup14(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 19) {
        TagFunc func = s_Group14Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005E16B0
void TalkMeasure::TagChoice2(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 2);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E16D8
void TalkMeasure::TagChoice3(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 3);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E1700
void TalkMeasure::TagChoice4(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 4);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E1728
void TalkMeasure::TagChoice5(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 5);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E1750
void TalkMeasure::TagChoice6(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 6);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E2038
void TalkMeasure::TagChoice2Cancel(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 2);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E2060
void TalkMeasure::TagChoice3Cancel(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 3);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E2088
void TalkMeasure::TagChoice4Cancel(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 4);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E20B0
void TalkMeasure::TagChoice5Cancel(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 5);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E20D8
void TalkMeasure::TagChoice6Cancel(const Tag* tag)
{
    s32 length = GetLinesLength(tag, 6);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E1920
void TalkMeasure::TagChoiceList(const Tag* tag)
{
    s32 length = GetLineLength(tag);
    if (m_pCur != NULL) {
        m_pCur += length;
    }
}

// 0x005E1218
void TalkMeasure::OnTagGroup15(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 11) {
        TagFunc func = s_Group15Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005E1410
void TalkMeasure::Tag17_0(const Tag* tag)
{
    InsertWord2B4(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005E13FC
void TalkMeasure::Tag17_1(const Tag* tag)
{
    InsertWord2B8(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005E1C2C
void TalkMeasure::Tag17_2(const Tag* tag)
{
    InsertWord2BC(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005E1C18
void TalkMeasure::Tag17_3(const Tag* tag)
{
    InsertWord2C0(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005E1F08
void TalkMeasure::Tag17_4(const Tag* tag)
{
    u32 index = tag->args[0] & 0xFF;
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag17_4(index), tag);
    }
}

// 0x005E2000
void TalkMeasure::Tag17_5(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag17_5(), tag);
}

// 0x005E1C04
void TalkMeasure::Tag17_6(const Tag* tag)
{
    InsertWord2CC(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005E18E4
void TalkMeasure::Tag17_7(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag0_4(), tag);
}

// 0x005E1BCC
void TalkMeasure::Tag17_8(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag17_8(), tag);
}

// 0x005E1FEC
void TalkMeasure::Tag17_9(const Tag* tag)
{
    InsertWord2D8(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005E1E3C
void TalkMeasure::Tag17_10(const Tag* tag)
{
    InsertWord2DC(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005E0FF0
void TalkMeasure::OnTagGroup17(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 11) {
        TagFunc func = s_Group17Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005E1A0C
void TalkMeasure::Tag18_0(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 0, 7);
}

// 0x005E1CA4
void TalkMeasure::Tag18_1(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(1), tag);
}

// 0x005E1994
void TalkMeasure::Tag18_2(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(2), tag);
}

// 0x005E1F8C
void TalkMeasure::Tag18_3(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 1, 7);
}

// 0x005E1F68
void TalkMeasure::Tag18_4(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 2, 7);
}

// 0x005E1450
void TalkMeasure::Tag18_5(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 3, 7);
}

// 0x005E1A30
void TalkMeasure::Tag18_6(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 4, 7);
}

// 0x005E1C80
void TalkMeasure::Tag18_7(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 5, 7);
}

// 0x005E1FB0
void TalkMeasure::Tag18_8(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(3), tag);
}

// 0x005E2148
void TalkMeasure::Tag18_9(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(4), tag);
}

// 0x005E1EE4
void TalkMeasure::Tag18_10(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 6, 7);
}

// 0x005E2100
void TalkMeasure::Tag18_11(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 7, 7);
}

// 0x005E12D0
void TalkMeasure::Tag18_12(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 13, 7);
}

// 0x005E1A54
void TalkMeasure::Tag18_13(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(6), tag);
}

// 0x005E1E78
void TalkMeasure::Tag18_14(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(7), tag);
}

// 0x005E0C5C
void TalkMeasure::Tag18_15(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(8), tag);
}

// 0x005E0394
void TalkMeasure::Tag18_16(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(9), tag);
}

// 0x005E1A90
void TalkMeasure::Tag18_17(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(10), tag);
}

// 0x005E187C
void TalkMeasure::Tag18_18(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 8, 7);
}

// 0x005E1474
void TalkMeasure::Tag18_19(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 9, 7);
}

// 0x005E0F54
void TalkMeasure::Tag18_20(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(11), tag);
}

// 0x005E1594
void TalkMeasure::Tag18_21(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(12), tag);
}

// 0x005E1858
void TalkMeasure::Tag18_22(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 10, 7);
}

// 0x005E14D4
void TalkMeasure::Tag18_23(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 11, 7);
}

// 0x005E14F8
void TalkMeasure::Tag18_24(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 12, 7);
}

// 0x005E151C
void TalkMeasure::Tag18_25(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(13), tag);
}

// 0x005E1558
void TalkMeasure::Tag18_26(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(14), tag);
}

// 0x005E17E0
void TalkMeasure::Tag18_27(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(15), tag);
}

// 0x005E1498
void TalkMeasure::Tag18_28(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(16), tag);
}

// 0x005E181C
void TalkMeasure::Tag18_29(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(17), tag);
}

// 0x005E19D0
void TalkMeasure::Tag18_30(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(18), tag);
}

// 0x005E0C20
void TalkMeasure::Tag18_31(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(19), tag);
}

// 0x005E12F4
void TalkMeasure::Tag18_32(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(20), tag);
}

// 0x005E1958
void TalkMeasure::Tag18_33(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(21), tag);
}

// 0x005E17A4
void TalkMeasure::Tag18_34(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(22), tag);
}

// 0x005E0308
void TalkMeasure::OnTagGroup18(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 35) {
        TagFunc func = s_Group18Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005E21D0
void TalkMeasure::Tag20_0(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag20_0(index), tag);
    }
}

// 0x005E2260
void TalkMeasure::Tag20_1(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag20_1(index), tag);
    }
}

// 0x005E2184
void TalkMeasure::Tag20_2(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag20_2(index), tag);
    }
}

// 0x005E221C
void TalkMeasure::Tag20_3(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag20_3(), tag);
}

// 0x005F3B10
void TalkMeasure::Tag20_4(const Tag* tag)
{
    InsertIslandGenderEnemy(GetInsert(), tag);
}

// 0x005E11C4
void TalkMeasure::OnTagGroup20(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 5) {
        TagFunc func = s_Group20Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005E1C74
void TalkMeasure::TagGeneric_2(const Tag* tag)
{
    m_IsCapitalNext = 1;
}

// 0x005E2254
void TalkMeasure::TagGeneric_3(const Tag* tag)
{
    m_CapitalState = 1;
}

// 0x005E213C
void TalkMeasure::TagGeneric_4(const Tag* tag)
{
    m_CapitalState = 0;
}
} // namespace script
