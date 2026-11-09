#include "script/dUiSeq.h"
#include "script/dIUiRecept.h"
#include "script/dIWord.h"
#include "script/dInsert.h"
#include "script/dInsertEngine.h"
#include "script/dMsgEngineModify.h"
#include "script/dRenderTag.h"
#include "script/dSearcher.h"

namespace script {
// 0x005EB490 slot 0x14
void UiSeq::OnChar(const CharInfo* info)
{
    char16 c;
    if (ProcessCapital(&c, info)) {
        bool isWritten;
        if (m_pWrite + 1 <= m_pLast) {
            *m_pWrite = c;
            m_pWrite++;
            isWritten = true;
        } else {
            isWritten = false;
        }
        m_IsSuccess &= isWritten;
        if (c != ' ' && c != 0x3000 && c != '\n') {
            m_Frame.lastChar = c;
        }
    } else {
        m_IsSuccess = PutChar(&m_pWrite, &m_pLast, info) & m_IsSuccess;
        if (info->pText != NULL) {
            c = *info->pText;
            if (c != ' ' && c != 0x3000 && c != '\n') {
                m_Frame.lastChar = c;
            }
        }
    }
    if (!m_IsSuccess) {
        m_IsOk = 0;
    }
}

// 0x005ED394 slot 0x18
void UiSeq::OnTag(const Tag* tag)
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

// 0x005ED434 slot 0x08
void UiSeq::Begin()
{
    m_Frame.pBegin = m_pCur;
}

// 0x005ED20C (name is ours)
bool UiSeq::Run(IWord* word, const IWord* text, u32 isColored)
{
    m_IsColored = isColored;
    m_IsOk = 1;
    m_pResult = NULL;
    m_pWrite = MsgEngineModify::s_pNoStop;
    m_pLast = MsgEngineModify::s_pNoStop;
    m_IsSuccess = 1;
    if (word != NULL) {
        m_pResult = word;
        m_pWrite = word->GetWord();
        m_pLast = m_pWrite;
        m_pLast = m_pLast + word->GetSize() - 1;
        m_pResult->Clear();
    }
    Reset();
    const char16* begin = text->GetWord();
    if (begin != NULL && text->GetSize() != 0 && *begin != 0) {
        begin = text->GetWord();
        InsertEngine insert(GetInsert(), false);
        if (insert.m_pInsert != NULL && begin != NULL) {
            insert.SetText(&begin, &s_pNoStop);
            insert.SetText2(&s_pNoStop);
        }
        SetText(&begin, &s_pNoStop);
        SetText2(&s_pNoStop);
    }
    return m_IsOk;
}

// 0x005ED450 slot 0x00
// 0x005ED440 (deleting dtor)
UiSeq::~UiSeq()
{
    // (only the members are destroyed)
}

// 0x005EC07C
void UiSeq::TagRuby(const Tag* tag)
{
    m_IsSuccess = PutTag(&m_pWrite, &m_pLast, tag) & m_IsSuccess;
    if (!m_IsSuccess) {
        m_IsOk = 0;
    }
}

// 0x005EC0B0
void UiSeq::TagScale(const Tag* tag)
{
    m_IsSuccess = PutTag(&m_pWrite, &m_pLast, tag) & m_IsSuccess;
    if (!m_IsSuccess) {
        m_IsOk = 0;
    }
}

// 0x005EC2D4
void UiSeq::TagSetColor(const Tag* tag)
{
    m_IsSuccess = PutTag(&m_pWrite, &m_pLast, tag) & m_IsSuccess;
    if (!m_IsSuccess) {
        m_IsOk = 0;
    }
}

// 0x005ECA28
void UiSeq::TagPageBreak(const Tag* tag)
{
    IUiRecept* recept = m_pOwner->m_pRecept;
    if (recept != NULL && !recept->ProcessTag94(tag)) {
        m_IsOk = 0;
    }
}

// 0x005EBAF0
void UiSeq::OnTagGroup0(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 5) {
        TagFunc func = s_Group0Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005EB600
void UiSeq::Tag1_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005EC168
void UiSeq::TagYear(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateYear(), tag);
}

// 0x005EC364
void UiSeq::TagMonthName(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetDateMonthName(), tag);
}

// 0x005EB728
void UiSeq::TagDay(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetDateDay(), tag);
}

// 0x005EC130
void UiSeq::TagHour(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateHour(), tag);
}

// 0x005EBEB0
void UiSeq::TagMinute(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateMinute(), tag);
}

// 0x005EBEE8
void UiSeq::TagSecond(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateSecond(), tag);
}

// 0x005EBB44
void UiSeq::TagNumber1(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005EBB90
void UiSeq::TagNumber2(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005EBBDC
void UiSeq::TagNumber3(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005EBC28
void UiSeq::TagNumber4(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005EBC74
void UiSeq::TagNumber5(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005EBCC0
void UiSeq::TagNumber6(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005EBD0C
void UiSeq::TagNumber7(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005EC1EC
void UiSeq::TagReceptYear(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetYear(index), tag);
    }
}

// 0x005EC39C
void UiSeq::TagReceptMonth(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetMonth(index), tag);
    }
}

// 0x005EBF20
void UiSeq::TagReceptDay(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetDay(index), tag);
    }
}

// 0x005EC1A0
void UiSeq::Tag4_16(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_16(index), tag);
    }
}

// 0x005EBF6C
void UiSeq::Tag4_17(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_17(index), tag);
    }
}

// 0x005EBE78
void UiSeq::Tag4_18(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetTag4_18(), tag);
}

// 0x005EC0E4
void UiSeq::Tag4_19(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_19(index), tag);
    }
}

// 0x005EC544
void UiSeq::Tag4_20(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_20(index), tag);
    }
}

// 0x005EB558
void UiSeq::OnTagGroup4(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 21) {
        TagFunc func = s_Group4Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005EB994
void UiSeq::Tag5_0(const Tag* tag)
{
    if (m_IsColored) {
        m_Frame.color = 3;
    }
    m_Frame.InsertWord(GetInsert()->GetTag5_0(), tag);
}

// 0x005EB900
void UiSeq::Tag5_1(const Tag* tag)
{
    if (m_IsColored) {
        m_Frame.color = 4;
    }
    m_Frame.InsertWord(GetInsert()->GetTag5_1(), tag);
}

// 0x005EC454
void UiSeq::Tag5_2(const Tag* tag)
{
    if (m_IsColored) {
        m_Frame.color = 4;
    }
    m_Frame.InsertWord(GetInsert()->GetTag5_2(), tag);
}

// 0x005EC3FC
void UiSeq::Tag5_3(const Tag* tag)
{
    if (m_IsColored) {
        m_Frame.color = 4;
    }
    m_Frame.InsertWord(GetInsert()->GetTag5_3(), tag);
}

// 0x005EC86C
void UiSeq::Tag5_4(const Tag* tag)
{
    InsertWord228(m_pOwner->m_pPhrase, tag, m_IsColored);
}

// 0x005EB79C
void UiSeq::Tag5_5(const Tag* tag)
{
    if (m_IsColored) {
        m_Frame.color = 1;
    }
    m_Frame.InsertWord(GetInsert()->GetTag5_5(), tag);
}

// 0x005EB9DC
void UiSeq::Tag5_6(const Tag* tag)
{
    if (m_IsColored) {
        m_Frame.color = 1;
    }
    m_Frame.InsertWord(GetInsert()->GetTag5_6(), tag);
}

// 0x005EB830
void UiSeq::Tag5_7(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_7(), tag);
}

// 0x005EC274
void UiSeq::Tag5_8(const Tag* tag)
{
    InsertWord238(m_pOwner->m_pPhrase, tag, m_IsColored);
}

// 0x005EB69C
void UiSeq::TagWeekday(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetWeekday(), tag);
}

// 0x005EC820
void UiSeq::TagImpression(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetImpression(index), tag);
    }
}

// 0x005ECDF4
void UiSeq::TagConstellation(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetConstellation(), tag);
}

// 0x005ECDBC
void UiSeq::Tag5_12(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_12(), tag);
}

// 0x005EC444
void UiSeq::TagGenderText(const Tag* tag)
{
    InsertGenderText(m_pOwner->m_pRecept, tag);
}

// 0x005EB7E4
void UiSeq::TagDrink(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetDrink(index), tag);
    }
}

// 0x005EBDA4
void UiSeq::TagObject(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetObject(index), tag);
    }
}

// 0x005EBA70
void UiSeq::TagSweets(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetSweets(index), tag);
    }
}

// 0x005EBA24
void UiSeq::TagSports(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetSports(index), tag);
    }
}

// 0x005EB868
void UiSeq::TagHobby(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetHobby(index), tag);
    }
}

// 0x005EB650
void UiSeq::TagFood(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetFood(index), tag);
    }
}

// 0x005EB8B4
void UiSeq::TagMusic(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetMusic(index), tag);
    }
}

// 0x005EC288
void UiSeq::Tag5_21(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_21(index), tag);
    }
}

// 0x005EC7E8
void UiSeq::TagAmPm(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetAmPm(), tag);
}

// 0x005ECEB8
void UiSeq::TagPartOfDay(const Tag* tag)
{
    InsertTag5_22OrTag5_23(m_pOwner->m_pPhrase, tag);
}

// 0x005EC030
void UiSeq::Tag5_24(const Tag* tag)
{
    // (the index is tested but not passed)
    if (*reinterpret_cast<const u8*>(tag->args) < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_24(), tag);
    }
}

// 0x005EC880
void UiSeq::Tag5_25(const Tag* tag)
{
    InsertTag5_24OrWord278(m_pOwner->m_pPhrase, tag);
}

// 0x005EC5F0
void UiSeq::TagTownFruit(const Tag* tag)
{
    InsertTownFruit(m_pOwner->m_pPhrase, tag, m_IsColored);
}

// 0x005ECE2C
void UiSeq::Tag5_27(const Tag* tag)
{
    InsertWord280(m_pOwner->m_pPhrase, tag, m_IsColored);
}

// 0x005ECDA8
void UiSeq::Tag5_28(const Tag* tag)
{
    InsertWord284(m_pOwner->m_pPhrase, tag, m_IsColored);
}

// 0x005EB604
void UiSeq::TagBook(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetBook(index), tag);
    }
}

// 0x005EBDF0
void UiSeq::TagSeason(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetSeason(index), tag);
    }
}

// 0x005EB948
void UiSeq::Tag5_31(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_31(index), tag);
    }
}

// 0x005EC49C
void UiSeq::Tag5_32(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(0), tag);
}

// 0x005EBD58
void UiSeq::Tag5_33(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_33(index), tag);
    }
}

// 0x005EC3E8
void UiSeq::Tag5_34(const Tag* tag)
{
    InsertWord298(m_pOwner->m_pPhrase, tag, m_IsColored);
}

// 0x005EC9F0
void UiSeq::Tag5_35(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_35(), tag);
}

// 0x005ECC2C
void UiSeq::Tag5_36(const Tag* tag)
{
    if (m_IsColored) {
        m_Frame.color = 3;
    }
    m_Frame.InsertWord(GetInsert()->GetTag5_36(), tag);
}

// 0x005EC890
void UiSeq::Tag5_37(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_37(), tag);
}

// 0x005EB5AC
void UiSeq::OnTagGroup5(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 38) {
        TagFunc func = s_Group5Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005ECD28
void UiSeq::Tag14_0(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005EC52C
void UiSeq::Tag14_1(const Tag* tag)
{
    InsertArticle(GetInsert(), 2);
}

// 0x005EC8C8
void UiSeq::Tag14_2(const Tag* tag)
{
    InsertArticle(GetInsert(), 1);
}

// 0x005ECCF8
void UiSeq::Tag14_3(const Tag* tag)
{
    m_Frame.isTag2 = 1;
}

// 0x005ECEC8
void UiSeq::Tag14_4(const Tag* tag)
{
    m_Frame.isTag3 = 1;
}

// 0x005ECF5C
void UiSeq::TagGenderP(const Tag* tag)
{
    InsertGenderP(tag, GetInsert());
}

// 0x005ED06C
void UiSeq::TagSexText(const Tag* tag)
{
    InsertSexText(m_pOwner->m_pRecept, tag, GetInsert());
}

// 0x005ECD04
void UiSeq::TagGenderN(const Tag* tag)
{
    InsertGenderN(tag, GetInsert());
}

// 0x005ECD14
void UiSeq::TagGender(const Tag* tag)
{
    InsertGender(GetInsert(), tag);
}

// 0x005ECAA4
void UiSeq::TagNumberForm(const Tag* tag)
{
    InsertNumberForm(GetInsert(), tag);
}

// 0x005EC62C
void UiSeq::TagElision(const Tag* tag)
{
    InsertElision(GetInsert(), tag);
}

// 0x005EC35C
void UiSeq::TagNumberText4(const Tag* tag)
{
    InsertNumberText4(tag);
}

// 0x005EC360
void UiSeq::TagNumberText5(const Tag* tag)
{
    InsertNumberText5(tag);
}

// 0x005EC540
void UiSeq::TagNumberText6(const Tag* tag)
{
    InsertNumberText6(tag);
}

// 0x005EC640
void UiSeq::TagPatchimForm(const Tag* tag)
{
    InsertPatchimForm(GetInsert(), tag, 0);
}

// 0x005EC8DC
void UiSeq::TagPatchimFormRieulFree(const Tag* tag)
{
    InsertPatchimForm(GetInsert(), tag, 1);
}

// 0x005ECE54
void UiSeq::TagEpenthesisA(const Tag* tag)
{
    InsertEpenthesisA(GetInsert(), tag);
}

// 0x005ECE68
void UiSeq::TagEpenthesisE(const Tag* tag)
{
    InsertEpenthesisE(GetInsert(), tag);
}

// 0x005ECED4
void UiSeq::TagGenderEx(const Tag* tag)
{
    InsertGenderEx(GetInsert(), tag);
}

// 0x005EBE3C
void UiSeq::OnTagGroup14(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 19) {
        TagFunc func = s_Group14Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005EC618
void UiSeq::Tag17_0(const Tag* tag)
{
    InsertWord2B4(m_pOwner->m_pPhrase, tag, m_IsColored);
}

// 0x005EC604
void UiSeq::Tag17_1(const Tag* tag)
{
    InsertWord2B8(m_pOwner->m_pPhrase, tag, m_IsColored);
}

// 0x005ECCE4
void UiSeq::Tag17_2(const Tag* tag)
{
    InsertWord2BC(m_pOwner->m_pPhrase, tag, m_IsColored);
}

// 0x005ECCD0
void UiSeq::Tag17_3(const Tag* tag)
{
    InsertWord2C0(m_pOwner->m_pPhrase, tag, m_IsColored);
}

// 0x005ECF0C
void UiSeq::Tag17_4(const Tag* tag)
{
    u32 index = tag->args[0] & 0xFF;
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag17_4(index), tag);
    }
}

// 0x005ED004
void UiSeq::Tag17_5(const Tag* tag)
{
    if (m_IsColored) {
        m_Frame.color = 2;
    }
    m_Frame.InsertWord(GetInsert()->GetTag17_5(), tag);
}

// 0x005ECCBC
void UiSeq::Tag17_6(const Tag* tag)
{
    InsertWord2CC(m_pOwner->m_pPhrase, tag, m_IsColored);
}

// 0x005ECA5C
void UiSeq::Tag17_7(const Tag* tag)
{
    if (m_IsColored) {
        m_Frame.color = 2;
    }
    m_Frame.InsertWord(GetInsert()->GetTag0_4(), tag);
}

// 0x005ECC74
void UiSeq::Tag17_8(const Tag* tag)
{
    if (m_IsColored) {
        m_Frame.color = 2;
    }
    m_Frame.InsertWord(GetInsert()->GetTag17_8(), tag);
}

// 0x005ECFF0
void UiSeq::Tag17_9(const Tag* tag)
{
    InsertWord2D8(m_pOwner->m_pPhrase, tag, m_IsColored);
}

// 0x005ECE40
void UiSeq::Tag17_10(const Tag* tag)
{
    InsertWord2DC(m_pOwner->m_pPhrase, tag, m_IsColored);
}

// 0x005EC308
void UiSeq::OnTagGroup17(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 11) {
        TagFunc func = s_Group17Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005ECB6C
void UiSeq::Tag18_0(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 0, 7);
}

// 0x005ECD5C
void UiSeq::Tag18_1(const Tag* tag)
{
    if (m_IsColored) {
        m_Frame.color = 2;
    }
    m_Frame.InsertWord(GetInsert()->GetTag5_32(1), tag);
}

// 0x005ECAF4
void UiSeq::Tag18_2(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(1), tag);
}

// 0x005ECF90
void UiSeq::Tag18_3(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 1, 7);
}

// 0x005ECF6C
void UiSeq::Tag18_4(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 2, 7);
}

// 0x005EC658
void UiSeq::Tag18_5(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 3, 7);
}

// 0x005ECB90
void UiSeq::Tag18_6(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 4, 7);
}

// 0x005ECD38
void UiSeq::Tag18_7(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 5, 7);
}

// 0x005ECFB4
void UiSeq::Tag18_8(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(3), tag);
}

// 0x005ED094
void UiSeq::Tag18_9(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(4), tag);
}

// 0x005ECEE8
void UiSeq::Tag18_10(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 6, 7);
}

// 0x005ED04C
void UiSeq::Tag18_11(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 7, 7);
}

// 0x005EC590
void UiSeq::Tag18_12(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 13, 7);
}

// 0x005ECBB4
void UiSeq::Tag18_13(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(6), tag);
}

// 0x005ECE7C
void UiSeq::Tag18_14(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(7), tag);
}

// 0x005EBFF4
void UiSeq::Tag18_15(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(8), tag);
}

// 0x005EB760
void UiSeq::Tag18_16(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(9), tag);
}

// 0x005ECBF0
void UiSeq::Tag18_17(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(10), tag);
}

// 0x005EC9CC
void UiSeq::Tag18_18(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 8, 7);
}

// 0x005EC67C
void UiSeq::Tag18_19(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 9, 7);
}

// 0x005EC238
void UiSeq::Tag18_20(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(11), tag);
}

// 0x005EC79C
void UiSeq::Tag18_21(const Tag* tag)
{
    if (m_IsColored) {
        m_Frame.color = 2;
    }
    m_Frame.InsertWord(GetInsert()->GetTag5_32(12), tag);
}

// 0x005EC9A8
void UiSeq::Tag18_22(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 10, 7);
}

// 0x005EC6DC
void UiSeq::Tag18_23(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 11, 7);
}

// 0x005EC700
void UiSeq::Tag18_24(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 12, 7);
}

// 0x005EC724
void UiSeq::Tag18_25(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(13), tag);
}

// 0x005EC760
void UiSeq::Tag18_26(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(14), tag);
}

// 0x005EC930
void UiSeq::Tag18_27(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(15), tag);
}

// 0x005EC6A0
void UiSeq::Tag18_28(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(16), tag);
}

// 0x005EC96C
void UiSeq::Tag18_29(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(17), tag);
}

// 0x005ECB30
void UiSeq::Tag18_30(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(18), tag);
}

// 0x005EBFB8
void UiSeq::Tag18_31(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(19), tag);
}

// 0x005EC5B4
void UiSeq::Tag18_32(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(20), tag);
}

// 0x005ECAB8
void UiSeq::Tag18_33(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(21), tag);
}

// 0x005EC8F4
void UiSeq::Tag18_34(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(22), tag);
}

// 0x005EB6D4
void UiSeq::OnTagGroup18(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 35) {
        TagFunc func = s_Group18Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005EBABC
void UiSeq::Tag19_(const Tag* tag)
{
    IUiRecept* recept = m_pOwner->m_pRecept;
    if (recept != NULL && !recept->ProcessTag98(tag)) {
        m_IsOk = 0;
    }
}

// 0x005ED11C
void UiSeq::Tag20_0(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag20_0(index), tag);
    }
}

// 0x005ED1AC
void UiSeq::Tag20_1(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag20_1(index), tag);
    }
}

// 0x005ED0D0
void UiSeq::Tag20_2(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag20_2(index), tag);
    }
}

// 0x005ED168
void UiSeq::Tag20_3(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag20_3(), tag);
}

// 0x005ED1F8
void UiSeq::Tag20_4(const Tag* tag)
{
    InsertIslandGenderEnemy(GetInsert(), tag);
}

// 0x005EC4D8
void UiSeq::OnTagGroup20(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 5) {
        TagFunc func = s_Group20Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005ECD2C
void UiSeq::TagGeneric_2(const Tag* tag)
{
    m_IsCapitalNext = 1;
}

// 0x005ED1A0
void UiSeq::TagGeneric_3(const Tag* tag)
{
    m_CapitalState = 1;
}

// 0x005ED088
void UiSeq::TagGeneric_4(const Tag* tag)
{
    m_CapitalState = 0;
}

} // namespace script
