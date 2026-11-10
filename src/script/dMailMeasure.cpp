#include "script/dMailMeasure.h"
#include "Other/dProcMgr.h"
#include "oml/framework/dProcessManager.h"
#include "script/dChip.h"
#include "script/dIMailRecept.h"
#include "script/dIWord.h"
#include "script/dInsert.h"
#include "script/dMailPhrase.h"
#include "script/dMailSystem.h"
#include "script/dRenderTag.h"
#include "script/dSearcher.h"

namespace script {
namespace {
// the text box of the letter menu and of the bulletin board menu (the layouts of the menus are not written yet)
const u32 LETTER_TEXT_BOX_OFFSET = 0x620;
const u32 BULLETIN_BOARD_TEXT_BOX_OFFSET = 0x808;

nw::lyt::TextBox* GetTextBox(const oml::framework::Process* process, u32 offset)
{
    return *reinterpret_cast<nw::lyt::TextBox* const*>(reinterpret_cast<const u8*>(process) + offset);
}
} // namespace

// 0x005DE1D0 slot 0x14
void MailMeasure::OnChar(const CharInfo* info)
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

// 0x005DF9B8 slot 0x18
void MailMeasure::OnTag(const Tag* tag)
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

// 0x005DF9B4 slot 0x0C
void MailMeasure::End()
{
    // empty (in the original too)
}

// 0x005DFA58 slot 0x08
void MailMeasure::Begin()
{
    m_Frame.pBegin = m_pCur;
}

// 0x005DFA64 slot 0x10
bool MailMeasure::IsContinue()
{
    return !m_IsStop;
}

// 0x005DEC68 slot 0x20
// 0x00774700 (thunk 140)
IMeasure* MailMeasure::GetChild()
{
    return m_pChild;
}

// 0x005DF084 slot 0x24
// 0x00774710 (thunk 140)
f32 MailMeasure::Measure(const IWord* word)
{
    for (u32 i = 0; i < BUFFER_SIZE; i++) {
        m_Buffer[i] = 0;
    }
    m_IsStop = 0;
    m_pWrite = m_Buffer;
    Reset();
    Run(word->GetWord());
    const char16* begin = m_Buffer;
    const char16* end = m_pWrite;
    return MeasureWithMenuTextBox(&begin, &end);
}

// 0x0075BBF0 slot 0x28
// 0x007746F8 (thunk 140)
f32 MailMeasure::MeasureText(const char16* const* begin, const char16* const* end)
{
    // (a nop in the original that falls into MeasureWithMenuTextBox)
    return MeasureWithMenuTextBox(begin, end);
}

// 0x0075BBF4 (name is ours)
f32 MailMeasure::MeasureWithMenuTextBox(const char16* const* begin, const char16* const* end)
{
    f32 width = 0.0f;
    IMailRecept* recept = m_pOwner->m_pRecept;
    if (recept == NULL) {
        return width;
    }
    nw::lyt::TextBox* textBox;
    if (recept->m_Mode == IMailRecept::MODE_BBS) {
        oml::framework::Process* process = oml::framework::ProcessManager::Get()->FindProcess(
            static_cast<oml::framework::ProcessIndex>(PROC_BS_MENU_BULLETIN_BOARD), NULL);
        if (process == NULL) {
            return width;
        }
        textBox = GetTextBox(process, BULLETIN_BOARD_TEXT_BOX_OFFSET);
    } else {
        oml::framework::Process* process = oml::framework::ProcessManager::Get()->FindProcess(
            static_cast<oml::framework::ProcessIndex>(PROC_BS_MENU_LETTER), NULL);
        if (process == NULL) {
            return width;
        }
        textBox = GetTextBox(process, LETTER_TEXT_BOX_OFFSET);
    }
    if (textBox != NULL) {
        width = Chip::MeasureWidth(textBox, begin, end);
    }
    return width;
}

// 0x005DFAD0 slot 0x00
// 0x005DFA78 (deleting dtor)
// 0x00774800 (thunk)
// 0x007747AC (deleting thunk)
MailMeasure::~MailMeasure()
{
    if (m_pChild != NULL) {
        delete m_pChild;
        m_pChild = NULL;
    }
}

// 0x005DE324
void MailMeasure::Tag1_(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005DECFC
void MailMeasure::TagYear(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateYear(), tag);
}

// 0x005DEEA8
void MailMeasure::TagMonthName(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetDateMonthName(), tag);
}

// 0x005DE414
void MailMeasure::TagDay(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetDateDay(), tag);
}

// 0x005DECC4
void MailMeasure::TagHour(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateHour(), tag);
}

// 0x005DEA9C
void MailMeasure::TagMinute(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateMinute(), tag);
}

// 0x005DEAD4
void MailMeasure::TagSecond(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetDateSecond(), tag);
}

// 0x005DE768
void MailMeasure::TagNumber1(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005DE7B4
void MailMeasure::TagNumber2(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005DE800
void MailMeasure::TagNumber3(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005DE84C
void MailMeasure::TagNumber4(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005DE898
void MailMeasure::TagNumber5(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005DE8E4
void MailMeasure::TagNumber6(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005DE930
void MailMeasure::TagNumber7(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 16) {
        m_Frame.InsertWordEx(GetInsert()->GetNumber(index), tag);
    }
}

// 0x005DED80
void MailMeasure::TagReceptYear(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetYear(index), tag);
    }
}

// 0x005DEEE0
void MailMeasure::TagReceptMonth(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetMonth(index), tag);
    }
}

// 0x005DEB0C
void MailMeasure::TagReceptDay(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetDay(index), tag);
    }
}

// 0x005DED34
void MailMeasure::Tag4_16(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_16(index), tag);
    }
}

// 0x005DEB58
void MailMeasure::Tag4_17(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_17(index), tag);
    }
}

// 0x005FE004
void MailMeasure::Tag4_18(const Tag* tag)
{
    m_Frame.InsertWordEx(GetInsert()->GetTag4_18(), tag);
}

// 0x005DEC78
void MailMeasure::Tag4_19(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_19(index), tag);
    }
}

// 0x005DEFD8
void MailMeasure::Tag4_20(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWordEx(GetInsert()->GetTag4_20(index), tag);
    }
}

// 0x005DE27C
void MailMeasure::OnTagGroup4(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 21) {
        TagFunc func = s_Group4Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005DE660
void MailMeasure::Tag5_0(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_0(), tag);
}

// 0x005DE5DC
void MailMeasure::Tag5_1(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_1(), tag);
}

// 0x005DEF64
void MailMeasure::Tag5_2(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_2(), tag);
}

// 0x005DEF2C
void MailMeasure::Tag5_3(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_3(), tag);
}

// 0x005F2A48
void MailMeasure::Tag5_4(const Tag* tag)
{
    InsertWord228(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005DE488
void MailMeasure::Tag5_5(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_5(), tag);
}

// 0x005DE698
void MailMeasure::Tag5_6(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_6(), tag);
}

// 0x005DE50C
void MailMeasure::Tag5_7(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_7(), tag);
}

// 0x005F1E6C
void MailMeasure::Tag5_8(const Tag* tag)
{
    InsertWord238(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005FDF50
void MailMeasure::TagWeekday(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetWeekday(), tag);
}

// 0x005DF2DC
void MailMeasure::TagImpression(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetImpression(index), tag);
    }
}

// 0x005DF790
void MailMeasure::TagConstellation(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetConstellation(), tag);
}

// 0x005DF758
void MailMeasure::Tag5_12(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_12(), tag);
}

// 0x005F22F8
void MailMeasure::TagGenderText(const Tag* tag)
{
    InsertGenderText(m_pOwner->m_pRecept, tag);
}

// 0x005DE4C0
void MailMeasure::TagDrink(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetDrink(index), tag);
    }
}

// 0x005DE9C8
void MailMeasure::TagObject(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetObject(index), tag);
    }
}

// 0x005DE71C
void MailMeasure::TagSweets(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetSweets(index), tag);
    }
}

// 0x005DE6D0
void MailMeasure::TagSports(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetSports(index), tag);
    }
}

// 0x005DE544
void MailMeasure::TagHobby(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetHobby(index), tag);
    }
}

// 0x005DE374
void MailMeasure::TagFood(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetFood(index), tag);
    }
}

// 0x005DE590
void MailMeasure::TagMusic(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetMusic(index), tag);
    }
}

// 0x005DEE08
void MailMeasure::Tag5_21(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_21(index), tag);
    }
}

// 0x005DF2A4
void MailMeasure::TagAmPm(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetAmPm(), tag);
}

// 0x005F34BC
void MailMeasure::TagPartOfDay(const Tag* tag)
{
    InsertTag5_22OrTag5_23(m_pOwner->m_pPhrase, tag);
}

// 0x005DEC1C
void MailMeasure::Tag5_24(const Tag* tag)
{
    // (the index is tested but not passed)
    if (*reinterpret_cast<const u8*>(tag->args) < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_24(), tag);
    }
}

// 0x005F2AB4
void MailMeasure::Tag5_25(const Tag* tag)
{
    InsertTag5_24OrWord278(m_pOwner->m_pPhrase, tag);
}

// 0x005F24B8
void MailMeasure::TagTownFruit(const Tag* tag)
{
    InsertTownFruit(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F31B8
void MailMeasure::Tag5_27(const Tag* tag)
{
    InsertWord280(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F314C
void MailMeasure::Tag5_28(const Tag* tag)
{
    InsertWord284(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005DE328
void MailMeasure::TagBook(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetBook(index), tag);
    }
}

// 0x005DEA14
void MailMeasure::TagSeason(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetSeason(index), tag);
    }
}

// 0x005DE614
void MailMeasure::Tag5_31(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_31(index), tag);
    }
}

// 0x005DEF9C
void MailMeasure::Tag5_32(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(0), tag);
}

// 0x005DE97C
void MailMeasure::Tag5_33(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag5_33(index), tag);
    }
}

// 0x005F228C
void MailMeasure::Tag5_34(const Tag* tag)
{
    InsertWord298(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005DF488
void MailMeasure::Tag5_35(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_35(), tag);
}

// 0x005DF66C
void MailMeasure::Tag5_36(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_36(), tag);
}

// 0x005DF328
void MailMeasure::Tag5_37(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_37(), tag);
}

// 0x005DE2D0
void MailMeasure::OnTagGroup5(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 38) {
        TagFunc func = s_Group5Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005DF6E8
void MailMeasure::Tag14_0(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005F1F80
void MailMeasure::Tag14_1(const Tag* tag)
{
    InsertArticle(GetInsert(), 2);
}

// 0x005DF360
void MailMeasure::Tag14_2(const Tag* tag)
{
    InsertArticle(GetInsert(), 1);
}

// 0x005DF6DC
void MailMeasure::Tag14_3(const Tag* tag)
{
    m_Frame.isTag2 = 1;
}

// 0x005DF804
void MailMeasure::Tag14_4(const Tag* tag)
{
    m_Frame.isTag3 = 1;
}

// 0x005F37A4
void MailMeasure::TagGenderP(const Tag* tag)
{
    InsertGenderP(tag, GetInsert());
}

// 0x005F39A8
void MailMeasure::TagSexText(const Tag* tag)
{
    InsertSexText(m_pOwner->m_pRecept, tag, GetInsert());
}

// 0x005F2DAC
void MailMeasure::TagGenderN(const Tag* tag)
{
    InsertGenderN(tag, GetInsert());
}

// 0x005F2F44
void MailMeasure::TagGender(const Tag* tag)
{
    InsertGender(GetInsert(), tag);
}

// 0x005F2B48
void MailMeasure::TagNumberForm(const Tag* tag)
{
    InsertNumberForm(GetInsert(), tag);
}

// 0x005F25FC
void MailMeasure::TagElision(const Tag* tag)
{
    InsertElision(GetInsert(), tag);
}

// 0x005F20B8
void MailMeasure::TagNumberText4(const Tag* tag)
{
    InsertNumberText4(tag);
}

// 0x005F2194
void MailMeasure::TagNumberText5(const Tag* tag)
{
    InsertNumberText5(tag);
}

// 0x005F23C0
void MailMeasure::TagNumberText6(const Tag* tag)
{
    InsertNumberText6(tag);
}

// 0x005F284C
void MailMeasure::TagPatchimForm(const Tag* tag)
{
    InsertPatchimForm(GetInsert(), tag, 0);
}

// 0x005DF374
void MailMeasure::TagPatchimFormRieulFree(const Tag* tag)
{
    InsertPatchimForm(GetInsert(), tag, 1);
}

// 0x005F3290
void MailMeasure::TagEpenthesisA(const Tag* tag)
{
    InsertEpenthesisA(GetInsert(), tag);
}

// 0x005F32FC
void MailMeasure::TagEpenthesisE(const Tag* tag)
{
    InsertEpenthesisE(GetInsert(), tag);
}

// 0x005F3584
void MailMeasure::TagGenderEx(const Tag* tag)
{
    InsertGenderEx(GetInsert(), tag);
}

// 0x005DEA60
void MailMeasure::OnTagGroup14(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 19) {
        TagFunc func = s_Group14Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005F2590
void MailMeasure::Tag17_0(const Tag* tag)
{
    InsertWord2B4(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F2524
void MailMeasure::Tag17_1(const Tag* tag)
{
    InsertWord2B8(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F2D40
void MailMeasure::Tag17_2(const Tag* tag)
{
    InsertWord2BC(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F2CD4
void MailMeasure::Tag17_3(const Tag* tag)
{
    InsertWord2C0(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005DF834
void MailMeasure::Tag17_4(const Tag* tag)
{
    u32 index = tag->args[0] & 0xFF;
    if (index < 10) {
        m_Frame.InsertWord(GetInsert()->GetTag17_4(index), tag);
    }
}

// 0x005DF908
void MailMeasure::Tag17_5(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag17_5(), tag);
}

// 0x005F2C68
void MailMeasure::Tag17_6(const Tag* tag)
{
    InsertWord2CC(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005DF4C0
void MailMeasure::Tag17_7(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag0_4(), tag);
}

// 0x005DF6A4
void MailMeasure::Tag17_8(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag17_8(), tag);
}

// 0x005F393C
void MailMeasure::Tag17_9(const Tag* tag)
{
    InsertWord2D8(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005F3224
void MailMeasure::Tag17_10(const Tag* tag)
{
    InsertWord2DC(m_pOwner->m_pPhrase, tag, 0);
}

// 0x005DEE54
void MailMeasure::OnTagGroup17(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 11) {
        TagFunc func = s_Group17Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005DF5AC
void MailMeasure::Tag18_0(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 0, 7);
}

// 0x005DF71C
void MailMeasure::Tag18_1(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(1), tag);
}

// 0x005DF534
void MailMeasure::Tag18_2(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(1), tag);
}

// 0x005DF8A8
void MailMeasure::Tag18_3(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 1, 7);
}

// 0x005DF884
void MailMeasure::Tag18_4(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 2, 7);
}

// 0x005DF124
void MailMeasure::Tag18_5(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 3, 7);
}

// 0x005DF5D0
void MailMeasure::Tag18_6(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 4, 7);
}

// 0x005DF6F8
void MailMeasure::Tag18_7(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 5, 7);
}

// 0x005DF8CC
void MailMeasure::Tag18_8(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(3), tag);
}

// 0x005DF96C
void MailMeasure::Tag18_9(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(4), tag);
}

// 0x005DF810
void MailMeasure::Tag18_10(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 6, 7);
}

// 0x005DF940
void MailMeasure::Tag18_11(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 7, 7);
}

// 0x005DF024
void MailMeasure::Tag18_12(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 13, 7);
}

// 0x005DF5F4
void MailMeasure::Tag18_13(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(6), tag);
}

// 0x005DF7C8
void MailMeasure::Tag18_14(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(7), tag);
}

// 0x005DEBE0
void MailMeasure::Tag18_15(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(8), tag);
}

// 0x005DE44C
void MailMeasure::Tag18_16(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(9), tag);
}

// 0x005DF630
void MailMeasure::Tag18_17(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(10), tag);
}

// 0x005DF464
void MailMeasure::Tag18_18(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 8, 7);
}

// 0x005DF148
void MailMeasure::Tag18_19(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 9, 7);
}

// 0x005DEDCC
void MailMeasure::Tag18_20(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(11), tag);
}

// 0x005DF268
void MailMeasure::Tag18_21(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(12), tag);
}

// 0x005DF440
void MailMeasure::Tag18_22(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 10, 7);
}

// 0x005DF1A8
void MailMeasure::Tag18_23(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 11, 7);
}

// 0x005DF1CC
void MailMeasure::Tag18_24(const Tag* tag)
{
    InsertWord2AC(m_pOwner->m_pPhrase, tag, 12, 7);
}

// 0x005DF1F0
void MailMeasure::Tag18_25(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(13), tag);
}

// 0x005DF22C
void MailMeasure::Tag18_26(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(14), tag);
}

// 0x005DF3C8
void MailMeasure::Tag18_27(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(15), tag);
}

// 0x005DF16C
void MailMeasure::Tag18_28(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(16), tag);
}

// 0x005DF404
void MailMeasure::Tag18_29(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(17), tag);
}

// 0x005DF570
void MailMeasure::Tag18_30(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(18), tag);
}

// 0x005DEBA4
void MailMeasure::Tag18_31(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(19), tag);
}

// 0x005DF048
void MailMeasure::Tag18_32(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(20), tag);
}

// 0x005DF4F8
void MailMeasure::Tag18_33(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(21), tag);
}

// 0x005DF38C
void MailMeasure::Tag18_34(const Tag* tag)
{
    m_Frame.InsertWord(GetInsert()->GetTag5_32(22), tag);
}

// 0x005DE3C0
void MailMeasure::OnTagGroup18(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 35) {
        TagFunc func = s_Group18Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005DF6EC
void MailMeasure::TagGeneric_2(const Tag* tag)
{
    m_IsCapitalNext = 1;
}

// 0x005DF9A8
void MailMeasure::TagGeneric_3(const Tag* tag)
{
    m_CapitalState = 1;
}

// 0x005DF960
void MailMeasure::TagGeneric_4(const Tag* tag)
{
    m_CapitalState = 0;
}

} // namespace script
