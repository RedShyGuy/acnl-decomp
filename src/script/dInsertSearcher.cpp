#include "script/dInsertSearcher.h"
#include "script/dInsert.h"
#include "script/dSeqEngine.h"

namespace script {
// 0x005E6E6C slot 0x00
// 0x005E6E5C (deleting dtor)
InsertSearcher::~InsertSearcher()
{
    // empty (in the original too)
}

// 0x005E6D3C
IWord* InsertSearcher::Search(const char16* const* text, u8 kind, u8 argIndex, s32 group, s32 index)
{
    m_Kind = kind;
    m_pResult = NULL;
    m_Group = group;
    m_Index = index;
    m_ArgIndex = argIndex;
    if (m_pInsert != NULL && kind <= 10 && *text != NULL) {
        SetText(text, &s_pNoStop);
        SetText2(&s_pNoStop);
    }
    return m_pResult;
}

// 0x005E6E48 slot 0x10
bool InsertSearcher::IsContinue()
{
    return m_pResult == NULL;
}

// 0x005E6DA8 slot 0x18
void InsertSearcher::OnTag(const Tag* tag)
{
    if (static_cast<u32>(tag->group) >= 21) {
        return;
    }
    TagFunc func = s_GroupFuncs[tag->group];
    if (func == NULL) {
        return;
    }
    if (m_Group != GROUP_ANY && m_Index != INDEX_ANY && (m_Group != tag->group || m_Index != tag->index)) {
        return;
    }
    if (HasTagAttribute(tag->group, tag->index, m_Kind)) {
        (this->*func)(tag);
    }
}

// 0x005E60B4
void InsertSearcher::TagYear(const Tag* tag)
{
    m_pResult = m_pInsert->GetDateYear();
}

// 0x005E6234
void InsertSearcher::TagMonthName(const Tag* tag)
{
    m_pResult = m_pInsert->GetDateMonthName();
}

// 0x005E5B5C
void InsertSearcher::TagDay(const Tag* tag)
{
    m_pResult = m_pInsert->GetDateDay();
}

// 0x005E6094
void InsertSearcher::TagHour(const Tag* tag)
{
    m_pResult = m_pInsert->GetDateHour();
}

// 0x005E5F14
void InsertSearcher::TagMinute(const Tag* tag)
{
    m_pResult = m_pInsert->GetDateMinute();
}

// 0x005E5F34
void InsertSearcher::TagSecond(const Tag* tag)
{
    m_pResult = m_pInsert->GetDateSecond();
}

// 0x005E5B14
void InsertSearcher::TagNumber1(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 16)) {
        m_pResult = m_pInsert->GetNumber(index);
    }
}

// 0x005E611C
void InsertSearcher::TagReceptYear(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetYear(index);
    }
}

// 0x005E6254
void InsertSearcher::TagReceptMonth(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetMonth(index);
    }
}

// 0x005E5F54
void InsertSearcher::TagReceptDay(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetDay(index);
    }
}

// 0x005E60D4
void InsertSearcher::Tag4_16(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetTag4_16(index);
    }
}

// 0x005E5F9C
void InsertSearcher::Tag4_17(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetTag4_17(index);
    }
}

// 0x005E5EF4
void InsertSearcher::Tag4_18(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag4_18();
}

// 0x005E604C
void InsertSearcher::Tag4_19(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetTag4_19(index);
    }
}

// 0x005E63A0
void InsertSearcher::Tag4_20(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetTag4_20(index);
    }
}

// 0x005E5948
void InsertSearcher::OnTagGroup4(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 21) {
        TagFunc func = s_Group4Funcs[tag->index];
        (this->*func)(tag);
    }
}

// 0x005E5D20
void InsertSearcher::Tag5_0(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_0();
}

// 0x005E5CB8
void InsertSearcher::Tag5_1(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_1();
}

// 0x005E6308
void InsertSearcher::Tag5_2(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_2();
}

// 0x005E62E4
void InsertSearcher::Tag5_3(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_3();
}

// 0x005E6624
void InsertSearcher::Tag5_4(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetWord228(index);
    }
}

// 0x005E5BA0
void InsertSearcher::Tag5_5(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_5();
}

// 0x005E5D40
void InsertSearcher::Tag5_6(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_6();
}

// 0x005E5C08
void InsertSearcher::Tag5_7(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_7();
}

// 0x005E6188
void InsertSearcher::Tag5_8(const Tag* tag)
{
    m_pResult = m_pInsert->GetWord238();
}

// 0x005E5AB0
void InsertSearcher::TagWeekday(const Tag* tag)
{
    m_pResult = m_pInsert->GetWeekday();
}

// 0x005E65DC
void InsertSearcher::TagImpression(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetImpression(index);
    }
}

// 0x005E6A0C
void InsertSearcher::TagConstellation(const Tag* tag)
{
    m_pResult = m_pInsert->GetConstellation();
}

// 0x005E69EC
void InsertSearcher::Tag5_12(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_12();
}

// 0x005E6304
void InsertSearcher::TagGenderText(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005E5BC0
void InsertSearcher::TagDrink(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetDrink(index);
    }
}

// 0x005E5E38
void InsertSearcher::TagObject(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetObject(index);
    }
}

// 0x005E5DA8
void InsertSearcher::TagSweets(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetSweets(index);
    }
}

// 0x005E5D60
void InsertSearcher::TagSports(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetSports(index);
    }
}

// 0x005E5C28
void InsertSearcher::TagHobby(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetHobby(index);
    }
}

// 0x005E5A68
void InsertSearcher::TagFood(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetFood(index);
    }
}

// 0x005E5C70
void InsertSearcher::TagMusic(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetMusic(index);
    }
}

// 0x005E61A8
void InsertSearcher::Tag5_21(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetTag5_21(index);
    }
}

// 0x005E65BC
void InsertSearcher::TagAmPm(const Tag* tag)
{
    m_pResult = m_pInsert->GetAmPm();
}

// 0x005E6AE4
void InsertSearcher::TagPartOfDay(const Tag* tag)
{
    m_pResult = m_pInsert->GetPartOfDayText();
}

// 0x005E602C
void InsertSearcher::Tag5_24(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_24();
}

// 0x005E666C
void InsertSearcher::Tag5_25(const Tag* tag)
{
    m_pResult = m_pInsert->GetWord278();
}

// 0x005E640C
void InsertSearcher::TagTownFruit(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetTownFruit(index);
    }
}

// 0x005E6A2C
void InsertSearcher::Tag5_27(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetWord280(index);
    }
}

// 0x005E69A4
void InsertSearcher::Tag5_28(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetWord284(index);
    }
}

// 0x005E5A20
void InsertSearcher::TagBook(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetBook(index);
    }
}

// 0x005E5E80
void InsertSearcher::TagSeason(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetSeason(index);
    }
}

// 0x005E5CD8
void InsertSearcher::Tag5_31(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetTag5_31(index);
    }
}

// 0x005E6328
void InsertSearcher::Tag5_32(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(0);
}

// 0x005E5DF0
void InsertSearcher::Tag5_33(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetTag5_33(index);
    }
}

// 0x005E629C
void InsertSearcher::Tag5_34(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetWord298(index);
    }
}

// 0x005E6738
void InsertSearcher::Tag5_35(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_35();
}

// 0x005E684C
void InsertSearcher::Tag5_36(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_36();
}

// 0x005E668C
void InsertSearcher::Tag5_37(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_37();
}

// 0x005E598C
void InsertSearcher::OnTagGroup5(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 38) {
        TagFunc func = s_Group5Funcs[tag->index];
        (this->*func)(tag);
    }
}

// 0x005E5EC8
void InsertSearcher::Tag7(const Tag* tag)
{
    if (tag->index == 2) {
        m_pResult = m_pInsert->GetWord2B0();
    }
}

// 0x005E64A0
void InsertSearcher::Tag17_0(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u16*>(tag->args) & 0xFF;
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetWord2B4(index);
    }
}

// 0x005E6454
void InsertSearcher::Tag17_1(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u16*>(tag->args) & 0xFF;
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetWord2B8(index);
    }
}

// 0x005E6924
void InsertSearcher::Tag17_2(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u16*>(tag->args) & 0xFF;
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetWord2BC(index);
    }
}

// 0x005E68D8
void InsertSearcher::Tag17_3(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u16*>(tag->args) & 0xFF;
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetWord2C0(index);
    }
}

// 0x005E6B14
void InsertSearcher::Tag17_4(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u16*>(tag->args) & 0xFF;
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetTag17_4(index);
    }
}

// 0x005E6BF0
void InsertSearcher::Tag17_5(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag17_5();
}

// 0x005E688C
void InsertSearcher::Tag17_6(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u16*>(tag->args) & 0xFF;
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetWord2CC(index);
    }
}

// 0x005E6758
void InsertSearcher::Tag17_7(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag0_4();
}

// 0x005E686C
void InsertSearcher::Tag17_8(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag17_8();
}

// 0x005E6BA4
void InsertSearcher::Tag17_9(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u16*>(tag->args) & 0xFF;
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetWord2D8(index);
    }
}

// 0x005E6A74
void InsertSearcher::Tag17_10(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u16*>(tag->args) & 0xFF;
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetWord2DC(index);
    }
}

// 0x005E61F0
void InsertSearcher::OnTagGroup17(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 11) {
        TagFunc func = s_Group17Funcs[tag->index];
        (this->*func)(tag);
    }
}

// 0x005E67E4
void InsertSearcher::Tag18_0(const Tag* tag)
{
    SearchWord2AC(0, *reinterpret_cast<const u8*>(tag->args));
}

// 0x005E6980
void InsertSearcher::Tag18_1(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(1);
}

// 0x005E679C
void InsertSearcher::Tag18_2(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(2);
}

// 0x005E6B70
void InsertSearcher::Tag18_3(const Tag* tag)
{
    SearchWord2AC(1, *reinterpret_cast<const u8*>(tag->args));
}

// 0x005E6B60
void InsertSearcher::Tag18_4(const Tag* tag)
{
    SearchWord2AC(2, *reinterpret_cast<const u8*>(tag->args));
}

// 0x005E64EC
void InsertSearcher::Tag18_5(const Tag* tag)
{
    SearchWord2AC(3, *reinterpret_cast<const u8*>(tag->args));
}

// 0x005E67F4
void InsertSearcher::Tag18_6(const Tag* tag)
{
    SearchWord2AC(4, *reinterpret_cast<const u8*>(tag->args));
}

// 0x005E6970
void InsertSearcher::Tag18_7(const Tag* tag)
{
    SearchWord2AC(5, *reinterpret_cast<const u8*>(tag->args));
}

// 0x005E6B80
void InsertSearcher::Tag18_8(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(3);
}

// 0x005E6C20
void InsertSearcher::Tag18_9(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(4);
}

// 0x005E6B04
void InsertSearcher::Tag18_10(const Tag* tag)
{
    SearchWord2AC(6, *reinterpret_cast<const u8*>(tag->args));
}

// 0x005E6C10
void InsertSearcher::Tag18_11(const Tag* tag)
{
    SearchWord2AC(7, *reinterpret_cast<const u8*>(tag->args));
}

// 0x005E59D0
void InsertSearcher::Tag18_12(const Tag* tag)
{
    SearchWord2AC(13, *reinterpret_cast<const u8*>(tag->args));
}

// 0x005E6804
void InsertSearcher::Tag18_13(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(6);
}

// 0x005E6AC0
void InsertSearcher::Tag18_14(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(7);
}

// 0x005E6008
void InsertSearcher::Tag18_15(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(8);
}

// 0x005E5B7C
void InsertSearcher::Tag18_16(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(9);
}

// 0x005E6828
void InsertSearcher::Tag18_17(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(10);
}

// 0x005E6728
void InsertSearcher::Tag18_18(const Tag* tag)
{
    SearchWord2AC(8, *reinterpret_cast<const u8*>(tag->args));
}

// 0x005E64FC
void InsertSearcher::Tag18_19(const Tag* tag)
{
    SearchWord2AC(9, *reinterpret_cast<const u8*>(tag->args));
}

// 0x005E6164
void InsertSearcher::Tag18_20(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(11);
}

// 0x005E6598
void InsertSearcher::Tag18_21(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(12);
}

// 0x005E6718
void InsertSearcher::Tag18_22(const Tag* tag)
{
    SearchWord2AC(10, *reinterpret_cast<const u8*>(tag->args));
}

// 0x005E6530
void InsertSearcher::Tag18_23(const Tag* tag)
{
    SearchWord2AC(11, *reinterpret_cast<const u8*>(tag->args));
}

// 0x005E6540
void InsertSearcher::Tag18_24(const Tag* tag)
{
    SearchWord2AC(12, *reinterpret_cast<const u8*>(tag->args));
}

// 0x005E6550
void InsertSearcher::Tag18_25(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(13);
}

// 0x005E6574
void InsertSearcher::Tag18_26(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(14);
}

// 0x005E66D0
void InsertSearcher::Tag18_27(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(15);
}

// 0x005E650C
void InsertSearcher::Tag18_28(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(16);
}

// 0x005E66F4
void InsertSearcher::Tag18_29(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(17);
}

// 0x005E67C0
void InsertSearcher::Tag18_30(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(18);
}

// 0x005E5FE4
void InsertSearcher::Tag18_31(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(19);
}

// 0x005E63E8
void InsertSearcher::Tag18_32(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(20);
}

// 0x005E6778
void InsertSearcher::Tag18_33(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(21);
}

// 0x005E66AC
void InsertSearcher::Tag18_34(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag5_32(22);
}

// 0x005E5AD0
void InsertSearcher::OnTagGroup18(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 35) {
        TagFunc func = s_Group18Funcs[tag->index];
        (this->*func)(tag);
    }
}

// 0x005E6C8C
void InsertSearcher::Tag20_0(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetTag20_0(index);
    }
}

// 0x005E6CF4
void InsertSearcher::Tag20_1(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetTag20_1(index);
    }
}

// 0x005E6C44
void InsertSearcher::Tag20_2(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetTag20_2(index);
    }
}

// 0x005E6CD4
void InsertSearcher::Tag20_3(const Tag* tag)
{
    m_pResult = m_pInsert->GetTag20_3();
}

// 0x005E634C
void InsertSearcher::OnTagGroup20(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 5) {
        TagFunc func = s_Group20Funcs[tag->index];
        (this->*func)(tag);
    }
}

// 0x005E59E0
void InsertSearcher::SearchWord2AC(u32 kind, u32 index)
{
    if (IsIndex(index, 10)) {
        m_pResult = m_pInsert->GetWord2AC(kind, index);
    }
}
} // namespace script
