#include "script/dMailSeq.h"
#include "script/dIMailRecept.h"
#include "script/dIWord.h"
#include "script/dInsert.h"
#include "script/dInsertEngine.h"
#include "script/dPatchimTargetEngine.h"
#include "script/dRenderTag.h"
#include "script/dSearcher.h"

namespace script {
// 0x005F587C slot 0x14
void MailSeq::OnChar(const CharInfo* info)
{
    if (m_IsCountLines && info->units == '\n') {
        m_LineCount++;
        if (m_LineCount == 1) {
            m_FirstLineLength = m_Length;
            // the word after the addressee gets the patchim of its last character
            const IWord* word = GetInsert()->GetWord2F0();
            m_Frame.pWords[2] = word;
            PatchimTargetEngine target;
            const char16* text = word->GetWord();
            if (text != NULL) {
                target.Run(text);
            }
            if (target.m_LastChar != 0) {
                m_Frame.lastChar = target.m_LastChar;
            }
        }
        return;
    }
    bool isWritten;
    char16 c;
    if (ProcessCapital(&c, info)) {
        if (m_pWrite + 1 <= m_pLast) {
            *m_pWrite = c;
            m_pWrite++;
            isWritten = true;
        } else {
            isWritten = false;
        }
        if (c != ' ' && c != 0x3000 && c != '\n') {
            m_Frame.lastChar = c;
        }
    } else {
        isWritten = PutChar(&m_pWrite, &m_pLast, info);
        if (info->pText != NULL) {
            c = *info->pText;
            if (c != ' ' && c != 0x3000 && c != '\n') {
                m_Frame.lastChar = c;
            }
        }
    }
    if (!isWritten) {
        m_IsSuccess = 0;
        return;
    }
    m_Length++;
}

// 0x005F7338 slot 0x18
void MailSeq::OnTag(const Tag* tag)
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

// 0x005F74EC slot 0x08
void MailSeq::Begin()
{
    m_Frame.pBegin = m_pCur;
}

// 0x005F73D8 (name is ours)
bool MailSeq::RunTo(const char16* const* text)
{
    m_IsCountLines = 0;
    m_IsSuccess = 1;
    m_FirstLineLength = -1;
    m_Length = 0;
    m_pWrite = m_Buffer;
    m_LineCount = 0;
    static_cast<IWord&>(m_Text).Clear();
    Reset();
    m_IsCountLines = 1;
    if (*text == NULL) {
        m_IsSuccess = 0;
    } else {
        InsertEngine insert(GetInsert(), false);
        insert.m_IsLineFeedUpdate = true;
        if (insert.m_pInsert != NULL && *text != NULL) {
            insert.SetText(text, &s_pNoStop);
            insert.SetText2(&s_pNoStop);
        }
        SetText(text, &s_pNoStop);
        SetText2(&s_pNoStop);
        if (m_LineCount > 1) {
            m_IsSuccess = 0;
        }
    }
    return m_IsSuccess;
}

// 0x005F74F8 (name is ours)
bool MailSeq::RunText(const char16* const* text)
{
    m_IsCountLines = 0;
    m_IsSuccess = 1;
    m_FirstLineLength = -1;
    m_Length = 0;
    m_pWrite = m_Buffer;
    m_LineCount = 0;
    static_cast<IWord&>(m_Text).Clear();
    Reset();
    if (*text == NULL) {
        m_IsSuccess = 0;
    } else {
        InsertEngine insert(GetInsert(), false);
        if (insert.m_pInsert != NULL && *text != NULL) {
            insert.SetText(text, &s_pNoStop);
            insert.SetText2(&s_pNoStop);
        }
        SetText(text, &s_pNoStop);
        SetText2(&s_pNoStop);
    }
    return m_IsSuccess;
}

// 0x005F7604 slot 0x00
// 0x005F75F4 (deleting dtor)
MailSeq::~MailSeq()
{
    // (only the members are destroyed)
}

// 0x005F5AC8
void MailSeq::Tag1_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005F6500
void MailSeq::TagYear(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateYear(), tag);
}

// 0x005F66C8
void MailSeq::TagMonthName(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetDateMonthName(), tag);
}

// 0x005F5BF0
void MailSeq::TagDay(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetDateDay(), tag);
}

// 0x005F64C8
void MailSeq::TagHour(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateHour(), tag);
}

// 0x005F62B0
void MailSeq::TagMinute(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateMinute(), tag);
}

// 0x005F62E8
void MailSeq::TagSecond(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateSecond(), tag);
}

// 0x005F5F44
void MailSeq::TagNumber1(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005F5F90
void MailSeq::TagNumber2(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005F5FDC
void MailSeq::TagNumber3(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005F6028
void MailSeq::TagNumber4(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005F6074
void MailSeq::TagNumber5(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005F60C0
void MailSeq::TagNumber6(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005F610C
void MailSeq::TagNumber7(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005F6584
void MailSeq::TagReceptYear(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetYear(index), tag);
    }
}

// 0x005F6700
void MailSeq::TagReceptMonth(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetMonth(index), tag);
    }
}

// 0x005F6320
void MailSeq::TagReceptDay(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetDay(index), tag);
    }
}

// 0x005F6538
void MailSeq::Tag4_16(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_16(index), tag);
    }
}

// 0x005F636C
void MailSeq::Tag4_17(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_17(index), tag);
    }
}

// 0x005F6278
void MailSeq::Tag4_18(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetTag4_18(), tag);
}

// 0x005F647C
void MailSeq::Tag4_19(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_19(index), tag);
    }
}

// 0x005F6834
void MailSeq::Tag4_20(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_20(index), tag);
    }
}

// 0x005F5A20
void MailSeq::OnTagGroup4(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 21) {
        TagFunc func = s_Group4Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005F5E3C
void MailSeq::Tag5_0(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_0(), tag);
}

// 0x005F5DB8
void MailSeq::Tag5_1(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_1(), tag);
}

// 0x005F67A8
void MailSeq::Tag5_2(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_2(), tag);
}

// 0x005F6760
void MailSeq::Tag5_3(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_3(), tag);
}

// 0x005F6B4C
void MailSeq::Tag5_4(const Tag* tag)
{
    InsertWord228(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F5C64
void MailSeq::Tag5_5(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_5(), tag);
}

// 0x005F5E74
void MailSeq::Tag5_6(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_6(), tag);
}

// 0x005F5CE8
void MailSeq::Tag5_7(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_7(), tag);
}

// 0x005F660C
void MailSeq::Tag5_8(const Tag* tag)
{
    InsertWord238(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F5B64
void MailSeq::TagWeekday(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetWeekday(), tag);
}

// 0x005F6B00
void MailSeq::TagImpression(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetImpression(index), tag);
    }
}

// 0x005F7060
void MailSeq::TagConstellation(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetConstellation(), tag);
}

// 0x005F7028
void MailSeq::Tag5_12(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_12(), tag);
}

// 0x005F6798
void MailSeq::TagGenderText(const Tag* tag)
{
    InsertGenderText(m_pOwner->m_pRecept, tag);
}

// 0x005F5C9C
void MailSeq::TagDrink(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetDrink(index), tag);
    }
}

// 0x005F61A4
void MailSeq::TagObject(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetObject(index), tag);
    }
}

// 0x005F5EF8
void MailSeq::TagSweets(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetSweets(index), tag);
    }
}

// 0x005F5EAC
void MailSeq::TagSports(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetSports(index), tag);
    }
}

// 0x005F5D20
void MailSeq::TagHobby(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetHobby(index), tag);
    }
}

// 0x005F5B18
void MailSeq::TagFood(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetFood(index), tag);
    }
}

// 0x005F5D6C
void MailSeq::TagMusic(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetMusic(index), tag);
    }
}

// 0x005F6620
void MailSeq::Tag5_21(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_21(index), tag);
    }
}

// 0x005F6AC8
void MailSeq::TagAmPm(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetAmPm(), tag);
}

// 0x005F7124
void MailSeq::TagPartOfDay(const Tag* tag)
{
    InsertTag5_22OrTag5_23(m_pOwner->m_pPhrase, tag);
}

// 0x005F6430
void MailSeq::Tag5_24(const Tag* tag)
{
    // (the index is tested but not passed)
    if (*reinterpret_cast<const u8*>(tag->args) < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_24(), tag);
    }
}

// 0x005F6B60
void MailSeq::Tag5_25(const Tag* tag)
{
    InsertTag5_24OrWord278(m_pOwner->m_pPhrase, tag);
}

// 0x005F68E0
void MailSeq::TagTownFruit(const Tag* tag)
{
    InsertTownFruit(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F7098
void MailSeq::Tag5_27(const Tag* tag)
{
    InsertWord280(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F7014
void MailSeq::Tag5_28(const Tag* tag)
{
    InsertWord284(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F5ACC
void MailSeq::TagBook(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetBook(index), tag);
    }
}

// 0x005F61F0
void MailSeq::TagSeason(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetSeason(index), tag);
    }
}

// 0x005F5DF0
void MailSeq::Tag5_31(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_31(index), tag);
    }
}

// 0x005F67E0
void MailSeq::Tag5_32(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(0), tag);
}

// 0x005F6158
void MailSeq::Tag5_33(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_33(index), tag);
    }
}

// 0x005F674C
void MailSeq::Tag5_34(const Tag* tag)
{
    InsertWord298(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F6CD0
void MailSeq::Tag5_35(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_35(), tag);
}

// 0x005F6EC8
void MailSeq::Tag5_36(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_36(), tag);
}

// 0x005F6B70
void MailSeq::Tag5_37(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_37(), tag);
}

// 0x005F5A74
void MailSeq::OnTagGroup5(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 38) {
        TagFunc func = s_Group5Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005F6FA4
void MailSeq::Tag14_0(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005F681C
void MailSeq::Tag14_1(const Tag* tag)
{
    InsertArticle(GetInsert(), 2);
}

// 0x005F6BA8
void MailSeq::Tag14_2(const Tag* tag)
{
    InsertArticle(GetInsert(), 1);
}

// 0x005F6F74
void MailSeq::Tag14_3(const Tag* tag)
{
    m_Frame.isTag2 = 1;
}

// 0x005F7134
void MailSeq::Tag14_4(const Tag* tag)
{
    m_Frame.isTag3 = 1;
}

// 0x005F71C8
void MailSeq::TagGenderP(const Tag* tag)
{
    InsertGenderP(tag, GetInsert());
}

// 0x005F72C8
void MailSeq::TagSexText(const Tag* tag)
{
    InsertSexText(m_pOwner->m_pRecept, tag, GetInsert());
}

// 0x005F6F80
void MailSeq::TagGenderN(const Tag* tag)
{
    InsertGenderN(tag, GetInsert());
}

// 0x005F6F90
void MailSeq::TagGender(const Tag* tag)
{
    InsertGender(GetInsert(), tag);
}

// 0x005F6D40
void MailSeq::TagNumberForm(const Tag* tag)
{
    InsertNumberForm(GetInsert(), tag);
}

// 0x005F691C
void MailSeq::TagElision(const Tag* tag)
{
    InsertElision(GetInsert(), tag);
}

// 0x005F66C0
void MailSeq::TagNumberText4(const Tag* tag)
{
    InsertNumberText4(tag);
}

// 0x005F66C4
void MailSeq::TagNumberText5(const Tag* tag)
{
    InsertNumberText5(tag);
}

// 0x005F6830
void MailSeq::TagNumberText6(const Tag* tag)
{
    InsertNumberText6(tag);
}

// 0x005F6930
void MailSeq::TagPatchimForm(const Tag* tag)
{
    InsertPatchimForm(GetInsert(), tag, 0);
}

// 0x005F6BBC
void MailSeq::TagPatchimFormRieulFree(const Tag* tag)
{
    InsertPatchimForm(GetInsert(), tag, 1);
}

// 0x005F70C0
void MailSeq::TagEpenthesisA(const Tag* tag)
{
    InsertEpenthesisA(GetInsert(), tag);
}

// 0x005F70D4
void MailSeq::TagEpenthesisE(const Tag* tag)
{
    InsertEpenthesisE(GetInsert(), tag);
}

// 0x005F7140
void MailSeq::TagGenderEx(const Tag* tag)
{
    InsertGenderEx(GetInsert(), tag);
}

// 0x005F623C
void MailSeq::OnTagGroup14(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 19) {
        TagFunc func = s_Group14Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005F6908
void MailSeq::Tag17_0(const Tag* tag)
{
    InsertWord2B4(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F68F4
void MailSeq::Tag17_1(const Tag* tag)
{
    InsertWord2B8(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F6F60
void MailSeq::Tag17_2(const Tag* tag)
{
    InsertWord2BC(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F6F4C
void MailSeq::Tag17_3(const Tag* tag)
{
    InsertWord2C0(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F7178
void MailSeq::Tag17_4(const Tag* tag)
{
    u32 index = tag->args[0] & 0xFF;
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag17_4(index), tag);
    }
}

// 0x005F7270
void MailSeq::Tag17_5(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag17_5(), tag);
}

// 0x005F6F38
void MailSeq::Tag17_6(const Tag* tag)
{
    InsertWord2CC(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F6D08
void MailSeq::Tag17_7(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag0_4(), tag);
}

// 0x005F6F00
void MailSeq::Tag17_8(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag17_8(), tag);
}

// 0x005F725C
void MailSeq::Tag17_9(const Tag* tag)
{
    InsertWord2D8(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F70AC
void MailSeq::Tag17_10(const Tag* tag)
{
    InsertWord2DC(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F666C
void MailSeq::OnTagGroup17(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 11) {
        TagFunc func = s_Group17Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005F6E08
void MailSeq::Tag18_0(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 0, 7);
}

// 0x005F6FD8
void MailSeq::Tag18_1(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(1), tag);
}

// 0x005F6D90
void MailSeq::Tag18_2(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(1), tag);
}

// 0x005F71FC
void MailSeq::Tag18_3(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 1, 7);
}

// 0x005F71D8
void MailSeq::Tag18_4(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 2, 7);
}

// 0x005F6948
void MailSeq::Tag18_5(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 3, 7);
}

// 0x005F6E2C
void MailSeq::Tag18_6(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 4, 7);
}

// 0x005F6FB4
void MailSeq::Tag18_7(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 5, 7);
}

// 0x005F7220
void MailSeq::Tag18_8(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(3), tag);
}

// 0x005F72F0
void MailSeq::Tag18_9(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(4), tag);
}

// 0x005F7154
void MailSeq::Tag18_10(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 6, 7);
}

// 0x005F72A8
void MailSeq::Tag18_11(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 7, 7);
}

// 0x005F6880
void MailSeq::Tag18_12(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 13, 7);
}

// 0x005F6E50
void MailSeq::Tag18_13(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(6), tag);
}

// 0x005F70E8
void MailSeq::Tag18_14(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(7), tag);
}

// 0x005F63F4
void MailSeq::Tag18_15(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(8), tag);
}

// 0x005F5C28
void MailSeq::Tag18_16(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(9), tag);
}

// 0x005F6E8C
void MailSeq::Tag18_17(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(10), tag);
}

// 0x005F6CAC
void MailSeq::Tag18_18(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 8, 7);
}

// 0x005F696C
void MailSeq::Tag18_19(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 9, 7);
}

// 0x005F65D0
void MailSeq::Tag18_20(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(11), tag);
}

// 0x005F6A8C
void MailSeq::Tag18_21(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(12), tag);
}

// 0x005F6C88
void MailSeq::Tag18_22(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 10, 7);
}

// 0x005F69CC
void MailSeq::Tag18_23(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 11, 7);
}

// 0x005F69F0
void MailSeq::Tag18_24(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 12, 7);
}

// 0x005F6A14
void MailSeq::Tag18_25(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(13), tag);
}

// 0x005F6A50
void MailSeq::Tag18_26(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(14), tag);
}

// 0x005F6C10
void MailSeq::Tag18_27(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(15), tag);
}

// 0x005F6990
void MailSeq::Tag18_28(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(16), tag);
}

// 0x005F6C4C
void MailSeq::Tag18_29(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(17), tag);
}

// 0x005F6DCC
void MailSeq::Tag18_30(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(18), tag);
}

// 0x005F63B8
void MailSeq::Tag18_31(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(19), tag);
}

// 0x005F68A4
void MailSeq::Tag18_32(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(20), tag);
}

// 0x005F6D54
void MailSeq::Tag18_33(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(21), tag);
}

// 0x005F6BD4
void MailSeq::Tag18_34(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(22), tag);
}

// 0x005F5B9C
void MailSeq::OnTagGroup18(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 35) {
        TagFunc func = s_Group18Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005F6FA8
void MailSeq::TagGeneric_2(const Tag* tag)
{
    m_IsCapitalNext = 1;
}

// 0x005F732C
void MailSeq::TagGeneric_3(const Tag* tag)
{
    m_CapitalState = 1;
}

// 0x005F72E4
void MailSeq::TagGeneric_4(const Tag* tag)
{
    m_CapitalState = 0;
}

} // namespace script
