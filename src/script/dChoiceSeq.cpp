#include "script/dChoiceSeq.h"
#include "script/dChoiceStandard.h"
#include "script/dITalkRecept.h"
#include "script/dInsert.h"

namespace script {
// the buffer the text of a choice is copied to before it is processed back into the choice
// 0x00AD6AE0 (name is ours)
WordFix<65> ChoiceItem::s_ProcessBuffer;

// 0x005D5634 (name is ours)
void ChoiceItem::Process(Phrase* phrase, IRecept* recept, SeqEngine* seq)
{
    if (phrase == NULL) {
        return;
    }
    IWord& word = m_Word;
    const char16* text = word.GetWord();
    if (text == NULL || word.GetSize() == 0 || text[0] == 0) {
        return;
    }
    ChoiceSeq engine(phrase, recept, seq);
    if (engine.m_pPhrase == NULL) {
        return;
    }
    text = static_cast<const IWord&>(word).GetWord();
    if (text == NULL || word.GetSize() == 0 || text[0] == 0) {
        return;
    }
    char16* buffer = word.GetWord();
    engine.m_pWrite = buffer;
    engine.m_pEnd = buffer;
    engine.m_pEnd = buffer + word.GetSize() - 1;
    IWord& copy = s_ProcessBuffer;
    copy.Copy(word);
    word.Clear();
    engine.m_IsOverflow = 0;
    const char16* source = copy.GetWord();
    engine.SetText(&source, &MsgEngine::s_pNoStop);
    if (engine.m_pSeq != NULL) {
        engine.m_Frame.pWords[1] = engine.m_pSeq->m_Frame.pWords[1];
    }
    engine.SetText2(&MsgEngine::s_pNoStop);
}

// 0x006006D4 slot 0x00 (name after the RTTI)
// 0x006006C4 (deleting dtor)
ChoiceSeq::~ChoiceSeq()
{
    // (only the bases are destroyed)
}

// 0x006006A4 slot 0x08 (name is ours)
void ChoiceSeq::Begin()
{
    m_Frame.pBegin = m_pCur;
}

// 0x00600608 slot 0x0C (name is ours)
void ChoiceSeq::End()
{
    // empty (in the original too)
}

// 0x006006B0 slot 0x10 (name is ours)
bool ChoiceSeq::IsContinue()
{
    return !m_IsOverflow;
}

// 0x005FEB84 slot 0x14 (name is ours)
void ChoiceSeq::OnChar(const CharInfo* info)
{
    char16 c;
    if (ProcessCapital(&c, info)) {
        char16* next = m_pWrite + 1;
        if (next <= m_pEnd) {
            *m_pWrite = c;
            m_pWrite = next;
        } else {
            m_IsOverflow = 1;
        }
        if (c != ' ' && c != 0x3000 && c != '\n') {
            m_Frame.lastChar = c;
        }
        return;
    }
    if (!PutChar(&m_pWrite, &m_pEnd, info)) {
        m_IsOverflow = 1;
    }
    if (info->pText != NULL) {
        c = *info->pText;
        if (c != ' ' && c != 0x3000 && c != '\n') {
            m_Frame.lastChar = c;
        }
    }
}

// 0x0060060C slot 0x18 (name is ours)
void ChoiceSeq::OnTag(const Tag* tag)
{
    if (static_cast<u32>(tag->group) < 21) {
        (this->*s_GroupFuncs[tag->group])(tag);
    } else if (tag->group == 0xFF && static_cast<u32>(tag->index) < 7) {
        TagFunc func = s_GenericFuncs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005FF68C
void ChoiceSeq::TagRuby(const Tag* tag)
{
    if (!PutTag(&m_pWrite, &m_pEnd, tag)) {
        m_IsOverflow = 1;
    }
}

// 0x005FF6B4
void ChoiceSeq::TagScale(const Tag* tag)
{
    if (!PutTag(&m_pWrite, &m_pEnd, tag)) {
        m_IsOverflow = 1;
    }
}

// 0x005FF8B4
void ChoiceSeq::TagSetColor(const Tag* tag)
{
    if (!PutTag(&m_pWrite, &m_pEnd, tag)) {
        m_IsOverflow = 1;
    }
}

// 0x005FF128
void ChoiceSeq::OnTagGroup0(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 5) {
        TagFunc func = s_Group0Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005FECD0
void ChoiceSeq::Tag1_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FF930
void ChoiceSeq::Tag2_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FF450
void ChoiceSeq::Tag3_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FF760
void ChoiceSeq::TagYear(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateYear(), tag);
}

// 0x005FF93C
void ChoiceSeq::TagMonthName(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetDateMonthName(), tag);
}

// 0x005FEE00
void ChoiceSeq::TagDay(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetDateDay(), tag);
}

// 0x005FF72C
void ChoiceSeq::TagHour(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateHour(), tag);
}

// 0x005FF4DC
void ChoiceSeq::TagMinute(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateMinute(), tag);
}

// 0x005FF510
void ChoiceSeq::TagSecond(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateSecond(), tag);
}

// 0x005FF17C
void ChoiceSeq::TagNumber1(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005FF1C4
void ChoiceSeq::TagNumber2(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005FF20C
void ChoiceSeq::TagNumber3(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005FF254
void ChoiceSeq::TagNumber4(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005FF29C
void ChoiceSeq::TagNumber5(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005FF2E4
void ChoiceSeq::TagNumber6(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005FF32C
void ChoiceSeq::TagNumber7(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005FF7DC
void ChoiceSeq::TagReceptYear(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetYear(index), tag);
    }
}

// 0x005FF970
void ChoiceSeq::TagReceptMonth(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetMonth(index), tag);
    }
}

// 0x005FF544
void ChoiceSeq::TagReceptDay(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetDay(index), tag);
    }
}

// 0x005FF794
void ChoiceSeq::Tag4_16(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_16(index), tag);
    }
}

// 0x005FF58C
void ChoiceSeq::Tag4_17(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_17(index), tag);
    }
}

// 0x005FF4A8
void ChoiceSeq::Tag4_18(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetTag4_18(), tag);
}

// 0x005FF6E4
void ChoiceSeq::Tag4_19(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_19(index), tag);
    }
}

// 0x005FFAEC
void ChoiceSeq::Tag4_20(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_20(index), tag);
    }
}

// 0x005FEC28
void ChoiceSeq::OnTagGroup4(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 21) {
        TagFunc func = s_Group4Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005FF028
void ChoiceSeq::Tag5_0(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_0(), tag);
}

// 0x005FEFAC
void ChoiceSeq::Tag5_1(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_1(), tag);
}

// 0x005FFA14
void ChoiceSeq::Tag5_2(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_2(), tag);
}

// 0x005FF9C8
void ChoiceSeq::Tag5_3(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_3(), tag);
}

// 0x005FFDBC
void ChoiceSeq::Tag5_4(const Tag* tag)
{
    InsertWord228(m_pPhrase, tag, 0);
}

// 0x005FEE6C
void ChoiceSeq::Tag5_5(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_5(), tag);
}

// 0x005FF05C
void ChoiceSeq::Tag5_6(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_6(), tag);
}

// 0x005FEEE8
void ChoiceSeq::Tag5_7(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_7(), tag);
}

// 0x005FF85C
void ChoiceSeq::Tag5_8(const Tag* tag)
{
    InsertWord238(m_pPhrase, tag, 0);
}

// 0x005FED74
void ChoiceSeq::TagWeekday(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetWeekday(), tag);
}

// 0x005FFD74
void ChoiceSeq::TagImpression(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetImpression(index), tag);
    }
}

// 0x00600254
void ChoiceSeq::TagConstellation(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetConstellation(), tag);
}

// 0x00600220
void ChoiceSeq::Tag5_12(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_12(), tag);
}

// 0x005FF9FC
void ChoiceSeq::TagGenderText(const Tag* tag)
{
    if (m_pRecept != NULL) {
        InsertGenderText(m_pRecept, tag);
    }
}

// 0x005FEEA0
void ChoiceSeq::TagDrink(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetDrink(index), tag);
    }
}

// 0x005FF3BC
void ChoiceSeq::TagObject(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetObject(index), tag);
    }
}

// 0x005FF0D8
void ChoiceSeq::TagSweets(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetSweets(index), tag);
    }
}

// 0x005FF090
void ChoiceSeq::TagSports(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetSports(index), tag);
    }
}

// 0x005FEF1C
void ChoiceSeq::TagHobby(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetHobby(index), tag);
    }
}

// 0x005FED2C
void ChoiceSeq::TagFood(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetFood(index), tag);
    }
}

// 0x005FEF64
void ChoiceSeq::TagMusic(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetMusic(index), tag);
    }
}

// 0x005FF86C
void ChoiceSeq::Tag5_21(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_21(index), tag);
    }
}

// 0x005FFD40
void ChoiceSeq::TagAmPm(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetAmPm(), tag);
}

// 0x00600300
void ChoiceSeq::TagPartOfDay(const Tag* tag)
{
    InsertTag5_22OrTag5_23(m_pPhrase, tag);
}

// 0x005FF644
void ChoiceSeq::Tag5_24(const Tag* tag)
{
    // (the index is tested but not passed)
    if (*reinterpret_cast<const u8*>(tag->args) < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_24(), tag);
    }
}

// 0x005FFDCC
void ChoiceSeq::Tag5_25(const Tag* tag)
{
    InsertTag5_24OrWord278(m_pPhrase, tag);
}

// 0x005FFB8C
void ChoiceSeq::TagTownFruit(const Tag* tag)
{
    InsertTownFruit(m_pPhrase, tag, 0);
}

// 0x00600288
void ChoiceSeq::Tag5_27(const Tag* tag)
{
    InsertWord280(m_pPhrase, tag, 0);
}

// 0x00600210
void ChoiceSeq::Tag5_28(const Tag* tag)
{
    InsertWord284(m_pPhrase, tag, 0);
}

// 0x005FECE4
void ChoiceSeq::TagBook(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetBook(index), tag);
    }
}

// 0x005FF404
void ChoiceSeq::TagSeason(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetSeason(index), tag);
    }
}

// 0x005FEFE0
void ChoiceSeq::Tag5_31(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_31(index), tag);
    }
}

// 0x005FFA48
void ChoiceSeq::Tag5_32(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(0), tag);
}

// 0x005FF374
void ChoiceSeq::Tag5_33(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_33(index), tag);
    }
}

// 0x005FF9B8
void ChoiceSeq::Tag5_34(const Tag* tag)
{
    InsertWord298(m_pPhrase, tag, 0);
}

// 0x005FFF18
void ChoiceSeq::Tag5_35(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_35(), tag);
}

// 0x006000E8
void ChoiceSeq::Tag5_36(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_36(), tag);
}

// 0x005FFDD8
void ChoiceSeq::Tag5_37(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_37(), tag);
}

// 0x005FEC7C
void ChoiceSeq::OnTagGroup5(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 38) {
        TagFunc func = s_Group5Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005FF6E0
void ChoiceSeq::Tag6_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FF44C
void ChoiceSeq::Tag7_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FF120
void ChoiceSeq::Tag8_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FECDC
void ChoiceSeq::Tag9_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FECD4
void ChoiceSeq::Tag10_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FECD8
void ChoiceSeq::Tag11_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FECE0
void ChoiceSeq::Tag12_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FEDA8
void ChoiceSeq::Tag13_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x006001A8
void ChoiceSeq::Tag14_0(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FFAD8
void ChoiceSeq::Tag14_1(const Tag* tag)
{
    InsertArticle(GetInsert(), 2);
}

// 0x005FFE0C
void ChoiceSeq::Tag14_2(const Tag* tag)
{
    InsertArticle(GetInsert(), 1);
}

// 0x00600180
void ChoiceSeq::Tag14_3(const Tag* tag)
{
    m_Frame.isTag2 = 1;
}

// 0x0060030C
void ChoiceSeq::Tag14_4(const Tag* tag)
{
    m_Frame.isTag3 = 1;
}

// 0x00600394
void ChoiceSeq::TagGenderP(const Tag* tag)
{
    InsertGenderP(tag, GetInsert());
}

// 0x0060047C
void ChoiceSeq::TagSexText(const Tag* tag)
{
    if (m_pRecept != NULL) {
        InsertSexText(m_pRecept, tag, GetInsert());
    }
}

// 0x0060018C
void ChoiceSeq::TagGenderN(const Tag* tag)
{
    InsertGenderN(tag, GetInsert());
}

// 0x00600198
void ChoiceSeq::TagGender(const Tag* tag)
{
    InsertGender(GetInsert(), tag);
}

// 0x005FFF80
void ChoiceSeq::TagNumberForm(const Tag* tag)
{
    InsertNumberForm(GetInsert(), tag);
}

// 0x005FFBBC
void ChoiceSeq::TagElision(const Tag* tag)
{
    InsertElision(GetInsert(), tag);
}

// 0x005FF934
void ChoiceSeq::TagNumberText4(const Tag* tag)
{
    InsertNumberText4(tag);
}

// 0x005FF938
void ChoiceSeq::TagNumberText5(const Tag* tag)
{
    InsertNumberText5(tag);
}

// 0x005FFAE8
void ChoiceSeq::TagNumberText6(const Tag* tag)
{
    InsertNumberText6(tag);
}

// 0x005FFBCC
void ChoiceSeq::TagPatchimForm(const Tag* tag)
{
    InsertPatchimForm(GetInsert(), tag, 0);
}

// 0x005FFE1C
void ChoiceSeq::TagPatchimFormRieulFree(const Tag* tag)
{
    InsertPatchimForm(GetInsert(), tag, 1);
}

// 0x006002A8
void ChoiceSeq::TagEpenthesisA(const Tag* tag)
{
    InsertEpenthesisA(GetInsert(), tag);
}

// 0x006002B8
void ChoiceSeq::TagEpenthesisE(const Tag* tag)
{
    InsertEpenthesisE(GetInsert(), tag);
}

// 0x00600318
void ChoiceSeq::TagGenderEx(const Tag* tag)
{
    InsertGenderEx(GetInsert(), tag);
}

// 0x005FF454
void ChoiceSeq::OnTagGroup14(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 19) {
        TagFunc func = s_Group14Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005FFAD4
void ChoiceSeq::Tag15_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FF6DC
void ChoiceSeq::Tag16_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005FFBAC
void ChoiceSeq::Tag17_0(const Tag* tag)
{
    InsertWord2B4(m_pPhrase, tag, 0);
}

// 0x005FFB9C
void ChoiceSeq::Tag17_1(const Tag* tag)
{
    InsertWord2B8(m_pPhrase, tag, 0);
}

// 0x00600170
void ChoiceSeq::Tag17_2(const Tag* tag)
{
    InsertWord2BC(m_pPhrase, tag, 0);
}

// 0x00600160
void ChoiceSeq::Tag17_3(const Tag* tag)
{
    InsertWord2C0(m_pPhrase, tag, 0);
}

// 0x00600348
void ChoiceSeq::Tag17_4(const Tag* tag)
{
    u32 index = tag->args[0] & 0xFF;
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag17_4(index), tag);
    }
}

// 0x00600428
void ChoiceSeq::Tag17_5(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag17_5(), tag);
}

// 0x00600150
void ChoiceSeq::Tag17_6(const Tag* tag)
{
    InsertWord2CC(m_pPhrase, tag, 0);
}

// 0x005FFF4C
void ChoiceSeq::Tag17_7(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag0_4(), tag);
}

// 0x0060011C
void ChoiceSeq::Tag17_8(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag17_8(), tag);
}

// 0x00600418
void ChoiceSeq::Tag17_9(const Tag* tag)
{
    InsertWord2D8(m_pPhrase, tag, 0);
}

// 0x00600298
void ChoiceSeq::Tag17_10(const Tag* tag)
{
    InsertWord2DC(m_pPhrase, tag, 0);
}

// 0x005FF8DC
void ChoiceSeq::OnTagGroup17(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 11) {
        TagFunc func = s_Group17Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x00600038
void ChoiceSeq::Tag18_0(const Tag* tag)
{
    InsertWord2AC(m_pPhrase, tag, 0, 7);
}

// 0x006001D8
void ChoiceSeq::Tag18_1(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(1), tag);
}

// 0x005FFFC8
void ChoiceSeq::Tag18_2(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(2), tag);
}

// 0x006003C0
void ChoiceSeq::Tag18_3(const Tag* tag)
{
    InsertWord2AC(m_pPhrase, tag, 1, 7);
}

// 0x006003A0
void ChoiceSeq::Tag18_4(const Tag* tag)
{
    InsertWord2AC(m_pPhrase, tag, 2, 7);
}

// 0x005FFBE0
void ChoiceSeq::Tag18_5(const Tag* tag)
{
    InsertWord2AC(m_pPhrase, tag, 3, 7);
}

// 0x00600058
void ChoiceSeq::Tag18_6(const Tag* tag)
{
    InsertWord2AC(m_pPhrase, tag, 4, 7);
}

// 0x006001B8
void ChoiceSeq::Tag18_7(const Tag* tag)
{
    InsertWord2AC(m_pPhrase, tag, 5, 7);
}

// 0x006003E0
void ChoiceSeq::Tag18_8(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(3), tag);
}

// 0x006004A8
void ChoiceSeq::Tag18_9(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(4), tag);
}

// 0x00600328
void ChoiceSeq::Tag18_10(const Tag* tag)
{
    InsertWord2AC(m_pPhrase, tag, 6, 7);
}

// 0x0060045C
void ChoiceSeq::Tag18_11(const Tag* tag)
{
    InsertWord2AC(m_pPhrase, tag, 7, 7);
}

// 0x005FFB34
void ChoiceSeq::Tag18_12(const Tag* tag)
{
    InsertWord2AC(m_pPhrase, tag, 13, 7);
}

// 0x00600078
void ChoiceSeq::Tag18_13(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(6), tag);
}

// 0x006002C8
void ChoiceSeq::Tag18_14(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(7), tag);
}

// 0x005FF60C
void ChoiceSeq::Tag18_15(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(8), tag);
}

// 0x005FEE34
void ChoiceSeq::Tag18_16(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(9), tag);
}

// 0x006000B0
void ChoiceSeq::Tag18_17(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(10), tag);
}

// 0x005FFEF8
void ChoiceSeq::Tag18_18(const Tag* tag)
{
    InsertWord2AC(m_pPhrase, tag, 8, 7);
}

// 0x005FFC00
void ChoiceSeq::Tag18_19(const Tag* tag)
{
    InsertWord2AC(m_pPhrase, tag, 9, 7);
}

// 0x005FF824
void ChoiceSeq::Tag18_20(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(11), tag);
}

// 0x005FFD08
void ChoiceSeq::Tag18_21(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(12), tag);
}

// 0x005FFED8
void ChoiceSeq::Tag18_22(const Tag* tag)
{
    InsertWord2AC(m_pPhrase, tag, 10, 7);
}

// 0x005FFC58
void ChoiceSeq::Tag18_23(const Tag* tag)
{
    InsertWord2AC(m_pPhrase, tag, 11, 7);
}

// 0x005FFC78
void ChoiceSeq::Tag18_24(const Tag* tag)
{
    InsertWord2AC(m_pPhrase, tag, 12, 7);
}

// 0x005FFC98
void ChoiceSeq::Tag18_25(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(13), tag);
}

// 0x005FFCD0
void ChoiceSeq::Tag18_26(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(14), tag);
}

// 0x005FFE68
void ChoiceSeq::Tag18_27(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(15), tag);
}

// 0x005FFC20
void ChoiceSeq::Tag18_28(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(16), tag);
}

// 0x005FFEA0
void ChoiceSeq::Tag18_29(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(17), tag);
}

// 0x00600000
void ChoiceSeq::Tag18_30(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(18), tag);
}

// 0x005FF5D4
void ChoiceSeq::Tag18_31(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(19), tag);
}

// 0x005FFB54
void ChoiceSeq::Tag18_32(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(20), tag);
}

// 0x005FFF90
void ChoiceSeq::Tag18_33(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(21), tag);
}

// 0x005FFE30
void ChoiceSeq::Tag18_34(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(22), tag);
}

// 0x005FEDAC
void ChoiceSeq::OnTagGroup18(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 35) {
        TagFunc func = s_Group18Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005FF124
void ChoiceSeq::Tag19_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x00600528
void ChoiceSeq::Tag20_0(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag20_0(index), tag);
    }
}

// 0x006005B0
void ChoiceSeq::Tag20_1(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag20_1(index), tag);
    }
}

// 0x006004E0
void ChoiceSeq::Tag20_2(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag20_2(index), tag);
    }
}

// 0x00600570
void ChoiceSeq::Tag20_3(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag20_3(), tag);
}

// 0x006005F8
void ChoiceSeq::Tag20_4(const Tag* tag)
{
    InsertIslandGenderEnemy(GetInsert(), tag);
}

// 0x005FFA80
void ChoiceSeq::OnTagGroup20(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 5) {
        TagFunc func = s_Group20Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x006001AC
void ChoiceSeq::TagGeneric_2(const Tag* tag)
{
    m_IsCapitalNext = 1;
}

// 0x006005A4
void ChoiceSeq::TagGeneric_3(const Tag* tag)
{
    m_CapitalState = 1;
}

// 0x0060049C
void ChoiceSeq::TagGeneric_4(const Tag* tag)
{
    m_CapitalState = 0;
}
} // namespace script
