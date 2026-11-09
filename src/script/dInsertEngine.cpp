#include "script/dInsertEngine.h"
#include "script/dInsert.h"
#include "Other/dRegion.h"

namespace script {
// 0x005E27B4 slot 0x14
void InsertEngine::OnChar(const CharInfo* info)
{
    if (m_IsLineFeedUpdate && info->units == '\n') {
        m_pInsert->UpdateWord1D8();
    }
}

// 0x005E36EC slot 0x18
void InsertEngine::OnTag(const Tag* tag)
{
    if (static_cast<u32>(tag->group) < 21) {
        TagFunc func = s_GroupFuncs[tag->group];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005E3750 slot 0x00
// 0x005E3740 (deleting dtor)
InsertEngine::~InsertEngine()
{
    // empty (in the original too)
}
// 0x005E2DA0
void InsertEngine::TagYear(const Tag* tag)
{
    m_pInsert->UpdateDateYear();
}

// 0x005E2E90
void InsertEngine::TagMonthName(const Tag* tag)
{
    m_pInsert->UpdateDateMonthName();
}

// 0x005E291C
void InsertEngine::TagDay(const Tag* tag)
{
    m_pInsert->UpdateDateDay();
}

// 0x005E2D90
void InsertEngine::TagHour(const Tag* tag)
{
    m_pInsert->UpdateDateHour();
}

// 0x005E2CB8
void InsertEngine::TagMinute(const Tag* tag)
{
    m_pInsert->UpdateDateMinute();
}

// 0x005E2CC8
void InsertEngine::TagSecond(const Tag* tag)
{
    m_pInsert->UpdateDateSecond();
}

// 0x005E2AA4
void InsertEngine::TagNumber1(const Tag* tag)
{
    const u8* args = reinterpret_cast<const u8*>(tag->args);
    u32 index = args[0];
    bool flag = args[1] != 0;
    if (index < 16) {
        m_pInsert->UpdateNumber(index, 1, flag);
    }
}

// 0x005E2AD8
void InsertEngine::TagNumber2(const Tag* tag)
{
    const u8* args = reinterpret_cast<const u8*>(tag->args);
    u32 index = args[0];
    bool flag = args[1] != 0;
    if (index < 16) {
        m_pInsert->UpdateNumber(index, 2, flag);
    }
}

// 0x005E2B0C
void InsertEngine::TagNumber3(const Tag* tag)
{
    const u8* args = reinterpret_cast<const u8*>(tag->args);
    u32 index = args[0];
    bool flag = args[1] != 0;
    if (index < 16) {
        m_pInsert->UpdateNumber(index, 3, flag);
    }
}

// 0x005E2B40
void InsertEngine::TagNumber4(const Tag* tag)
{
    const u8* args = reinterpret_cast<const u8*>(tag->args);
    u32 index = args[0];
    bool flag = args[1] != 0;
    if (index < 16) {
        m_pInsert->UpdateNumber(index, 4, flag);
    }
}

// 0x005E2B74
void InsertEngine::TagNumber5(const Tag* tag)
{
    const u8* args = reinterpret_cast<const u8*>(tag->args);
    u32 index = args[0];
    bool flag = args[1] != 0;
    if (index < 16) {
        m_pInsert->UpdateNumber(index, 5, flag);
    }
}

// 0x005E2BA8
void InsertEngine::TagNumber6(const Tag* tag)
{
    const u8* args = reinterpret_cast<const u8*>(tag->args);
    u32 index = args[0];
    bool flag = args[1] != 0;
    if (index < 16) {
        m_pInsert->UpdateNumber(index, 6, flag);
    }
}

// 0x005E2BDC
void InsertEngine::TagNumber7(const Tag* tag)
{
    const u8* args = reinterpret_cast<const u8*>(tag->args);
    u32 index = args[0];
    bool flag = args[1] != 0;
    if (index < 16) {
        m_pInsert->UpdateNumber(index, 7, flag);
    }
}

// 0x005E2DD4
void InsertEngine::TagReceptYear(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateYear(index);
    }
}

// 0x005E2EA0
void InsertEngine::TagReceptMonth(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateMonth(index);
    }
}

// 0x005E2CD8
void InsertEngine::TagReceptDay(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateDay(index);
    }
}

// 0x005E2DB0
void InsertEngine::Tag4_16(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateTag4_16(index);
    }
}

// 0x005E2CFC
void InsertEngine::Tag4_17(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateTag4_17(index);
    }
}

// 0x005E2CA8
void InsertEngine::Tag4_18(const Tag* tag)
{
    m_pInsert->UpdateTag4_18();
}

// 0x005E2D6C
void InsertEngine::Tag4_19(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateTag4_19(index);
    }
}

// 0x005E2F74
void InsertEngine::Tag4_20(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateTag4_20(index);
    }
}

// 0x005E27E0
void InsertEngine::OnTagGroup4(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 21) {
        TagFunc func = s_Group4Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005E2A24
void InsertEngine::Tag5_0(const Tag* tag)
{
    m_pInsert->UpdateTag5_0();
}

// 0x005E29F0
void InsertEngine::Tag5_1(const Tag* tag)
{
    m_pInsert->UpdateTag5_1();
}

// 0x005E2EFC
void InsertEngine::Tag5_2(const Tag* tag)
{
    m_pInsert->UpdateTag5_2();
}

// 0x005E2EE8
void InsertEngine::Tag5_3(const Tag* tag)
{
    m_pInsert->UpdateTag5_3();
}

// 0x005E3180
void InsertEngine::Tag5_4(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateWord228(index);
    }
}

// 0x005E2940
void InsertEngine::Tag5_5(const Tag* tag)
{
    m_pInsert->UpdateTag5_5();
}

// 0x005E2A34
void InsertEngine::Tag5_6(const Tag* tag)
{
    m_pInsert->UpdateTag5_6();
}

// 0x005E2980
void InsertEngine::Tag5_7(const Tag* tag)
{
    m_pInsert->UpdateTag5_7();
}

// 0x005E2E0C
void InsertEngine::Tag5_8(const Tag* tag)
{
    m_pInsert->UpdateWord238();
}

// 0x005E28C8
void InsertEngine::TagWeekday(const Tag* tag)
{
    m_pInsert->UpdateWeekday();
}

// 0x005E315C
void InsertEngine::TagImpression(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateImpression(index);
    }
}

// 0x005E3494
void InsertEngine::TagConstellation(const Tag* tag)
{
    m_pInsert->UpdateConstellation();
}

// 0x005E3484
void InsertEngine::Tag5_12(const Tag* tag)
{
    m_pInsert->UpdateTag5_12();
}

// 0x005E2EF8
void InsertEngine::TagGenderText(const Tag* tag)
{
    // empty (in the original too)
}

// 0x005E2950
void InsertEngine::TagDrink(const Tag* tag)
{
    const u8* args = reinterpret_cast<const u8*>(tag->args);
    u32 index = args[0];
    bool flag = args[1] != 0;
    if (index < 10) {
        m_pInsert->UpdateDrink(index, flag);
    }
}

// 0x005E2C34
void InsertEngine::TagObject(const Tag* tag)
{
    const u8* args = reinterpret_cast<const u8*>(tag->args);
    u32 index = args[0];
    bool flag = args[1] != 0;
    if (index < 10) {
        m_pInsert->UpdateObject(index, flag);
    }
}

// 0x005E2A74
void InsertEngine::TagSweets(const Tag* tag)
{
    const u8* args = reinterpret_cast<const u8*>(tag->args);
    u32 index = args[0];
    bool flag = args[1] != 0;
    if (index < 10) {
        m_pInsert->UpdateSweets(index, flag);
    }
}

// 0x005E2A44
void InsertEngine::TagSports(const Tag* tag)
{
    const u8* args = reinterpret_cast<const u8*>(tag->args);
    u32 index = args[0];
    bool flag = args[1] != 0;
    if (index < 10) {
        m_pInsert->UpdateSports(index, flag);
    }
}

// 0x005E2990
void InsertEngine::TagHobby(const Tag* tag)
{
    const u8* args = reinterpret_cast<const u8*>(tag->args);
    u32 index = args[0];
    bool flag = args[1] != 0;
    if (index < 10) {
        m_pInsert->UpdateHobby(index, flag);
    }
}

// 0x005E2898
void InsertEngine::TagFood(const Tag* tag)
{
    const u8* args = reinterpret_cast<const u8*>(tag->args);
    u32 index = args[0];
    bool flag = args[1] != 0;
    if (index < 10) {
        m_pInsert->UpdateFood(index, flag);
    }
}

// 0x005E29C0
void InsertEngine::TagMusic(const Tag* tag)
{
    const u8* args = reinterpret_cast<const u8*>(tag->args);
    u32 index = args[0];
    bool flag = args[1] != 0;
    if (index < 10) {
        m_pInsert->UpdateMusic(index, flag);
    }
}

// 0x005E2E1C
void InsertEngine::Tag5_21(const Tag* tag)
{
    const u8* args = reinterpret_cast<const u8*>(tag->args);
    u32 index = args[0];
    bool flag = args[1] != 0;
    if (index < 10) {
        m_pInsert->UpdateTag5_21(index, flag);
    }
}

// 0x005E314C
void InsertEngine::TagAmPm(const Tag* tag)
{
    m_pInsert->UpdateAmPm();
}

// 0x005E3504
void InsertEngine::TagPartOfDay(const Tag* tag)
{
    // (Japan: the tag 5/23, else 5/22)
    if (GetRegion() != 0) {
        m_pInsert->UpdateAmPm();
    } else {
        m_pInsert->UpdatePartOfDayText();
    }
}

// 0x005E2D48
void InsertEngine::Tag5_24(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateTag5_24(index);
    }
}

// 0x005E31A4
void InsertEngine::Tag5_25(const Tag* tag)
{
    u32 index;
    if (GetRegion() != 0) {
        index = *reinterpret_cast<const u8*>(tag->args);
        if (index < 10) {
            m_pInsert->UpdateTag5_24(index);
        }
    } else {
        index = *reinterpret_cast<const u8*>(tag->args);
        if (index < 10) {
            m_pInsert->UpdateWord278(index);
        }
    }
}

// 0x005E2FD8
void InsertEngine::TagTownFruit(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateTownFruit(index);
    }
}

// 0x005E34A4
void InsertEngine::Tag5_27(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateWord280(index);
    }
}

// 0x005E3460
void InsertEngine::Tag5_28(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateWord284(index);
    }
}

// 0x005E2868
void InsertEngine::TagBook(const Tag* tag)
{
    const u8* args = reinterpret_cast<const u8*>(tag->args);
    u32 index = args[0];
    bool flag = args[1] != 0;
    if (index < 10) {
        m_pInsert->UpdateBook(index, flag);
    }
}

// 0x005E2C64
void InsertEngine::TagSeason(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateSeason(index);
    }
}

// 0x005E2A00
void InsertEngine::Tag5_31(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateTag5_31(index);
    }
}

// 0x005E2F0C
void InsertEngine::Tag5_32(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(0);
}

// 0x005E2C10
void InsertEngine::Tag5_33(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateTag5_33(index);
    }
}

// 0x005E2EC4
void InsertEngine::Tag5_34(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateWord298(index);
    }
}

// 0x005E32A4
void InsertEngine::Tag5_35(const Tag* tag)
{
    m_pInsert->UpdateTag5_35(*reinterpret_cast<const u8*>(tag->args));
}

// 0x005E3388
void InsertEngine::Tag5_36(const Tag* tag)
{
    m_pInsert->UpdateTag5_36();
}

// 0x005E3200
void InsertEngine::Tag5_37(const Tag* tag)
{
    m_pInsert->UpdateTag5_37();
}

// 0x005E2824
void InsertEngine::OnTagGroup5(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 38) {
        TagFunc func = s_Group5Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005E2C88
void InsertEngine::Tag7_(const Tag* tag)
{
    if (tag->index == 6) {
        m_pInsert->UpdateWord198();
    }
}

// 0x005E3024
void InsertEngine::Tag17_0(const Tag* tag)
{
    u32 index = tag->args[0] & 0xFF;
    if (index < 10) {
        m_pInsert->UpdateWord2B4(index);
    }
}

// 0x005E2FFC
void InsertEngine::Tag17_1(const Tag* tag)
{
    u32 index = tag->args[0] & 0xFF;
    if (index < 10) {
        m_pInsert->UpdateWord2B8(index);
    }
}

// 0x005E33F8
void InsertEngine::Tag17_2(const Tag* tag)
{
    u32 index = tag->args[0] & 0xFF;
    if (index < 10) {
        m_pInsert->UpdateWord2BC(index);
    }
}

// 0x005E33D0
void InsertEngine::Tag17_3(const Tag* tag)
{
    u32 index = tag->args[0] & 0xFF;
    if (index < 10) {
        m_pInsert->UpdateWord2C0(index);
    }
}

// 0x005E3564
void InsertEngine::Tag17_4(const Tag* tag)
{
    u32 index = tag->args[0] & 0xFF;
    if (index < 10) {
        m_pInsert->UpdateTag17_4(index);
    }
}

// 0x005E3620
void InsertEngine::Tag17_5(const Tag* tag)
{
    m_pInsert->UpdateTag17_5();
}

// 0x005E33A8
void InsertEngine::Tag17_6(const Tag* tag)
{
    u32 index = tag->args[0] & 0xFF;
    if (index < 10) {
        m_pInsert->UpdateWord2CC(index);
    }
}

// 0x005E32BC
void InsertEngine::Tag17_7(const Tag* tag)
{
    m_pInsert->UpdateTag0_4();
}

// 0x005E3398
void InsertEngine::Tag17_8(const Tag* tag)
{
    m_pInsert->UpdateTag17_8();
}

// 0x005E35F8
void InsertEngine::Tag17_9(const Tag* tag)
{
    u32 index = tag->args[0] & 0xFF;
    if (index < 10) {
        m_pInsert->UpdateWord2D8(index);
    }
}

// 0x005E34C8
void InsertEngine::Tag17_10(const Tag* tag)
{
    u32 index = tag->args[0] & 0xFF;
    if (index < 10) {
        m_pInsert->UpdateWord2DC(index);
    }
}

// 0x005E2E4C
void InsertEngine::OnTagGroup17(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 11) {
        TagFunc func = s_Group17Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005E3308
void InsertEngine::Tag18_0(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateWord2AC(0, index);
    }
}

// 0x005E344C
void InsertEngine::Tag18_1(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(1);
}

// 0x005E32E0
void InsertEngine::Tag18_2(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(2);
}

// 0x005E35B8
void InsertEngine::Tag18_3(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateWord2AC(1, index);
    }
}

// 0x005E358C
void InsertEngine::Tag18_4(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateWord2AC(2, index);
    }
}

// 0x005E304C
void InsertEngine::Tag18_5(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateWord2AC(3, index);
    }
}

// 0x005E3334
void InsertEngine::Tag18_6(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateWord2AC(4, index);
    }
}

// 0x005E3420
void InsertEngine::Tag18_7(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateWord2AC(5, index);
    }
}

// 0x005E35E4
void InsertEngine::Tag18_8(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(3);
}

// 0x005E365C
void InsertEngine::Tag18_9(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(4);
}

// 0x005E3538
void InsertEngine::Tag18_10(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateWord2AC(6, index);
    }
}

// 0x005E3630
void InsertEngine::Tag18_11(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateWord2AC(7, index);
    }
}

// 0x005E2F98
void InsertEngine::Tag18_12(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateWord2AC(13, index);
    }
}

// 0x005E3360
void InsertEngine::Tag18_13(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(6);
}

// 0x005E34F0
void InsertEngine::Tag18_14(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(7);
}

// 0x005E2D34
void InsertEngine::Tag18_15(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(8);
}

// 0x005E292C
void InsertEngine::Tag18_16(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(9);
}

// 0x005E3374
void InsertEngine::Tag18_17(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(10);
}

// 0x005E3278
void InsertEngine::Tag18_18(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateWord2AC(8, index);
    }
}

// 0x005E3078
void InsertEngine::Tag18_19(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateWord2AC(9, index);
    }
}

// 0x005E2DF8
void InsertEngine::Tag18_20(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(11);
}

// 0x005E3138
void InsertEngine::Tag18_21(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(12);
}

// 0x005E324C
void InsertEngine::Tag18_22(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateWord2AC(10, index);
    }
}

// 0x005E30B8
void InsertEngine::Tag18_23(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateWord2AC(11, index);
    }
}

// 0x005E30E4
void InsertEngine::Tag18_24(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateWord2AC(12, index);
    }
}

// 0x005E3110
void InsertEngine::Tag18_25(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(13);
}

// 0x005E3124
void InsertEngine::Tag18_26(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(14);
}

// 0x005E3224
void InsertEngine::Tag18_27(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(15);
}

// 0x005E30A4
void InsertEngine::Tag18_28(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(16);
}

// 0x005E3238
void InsertEngine::Tag18_29(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(17);
}

// 0x005E32F4
void InsertEngine::Tag18_30(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(18);
}

// 0x005E2D20
void InsertEngine::Tag18_31(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(19);
}

// 0x005E2FC4
void InsertEngine::Tag18_32(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(20);
}

// 0x005E32CC
void InsertEngine::Tag18_33(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(21);
}

// 0x005E3210
void InsertEngine::Tag18_34(const Tag* tag)
{
    m_pInsert->UpdateTag5_32(22);
}

// 0x005E28D8
void InsertEngine::OnTagGroup18(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 35) {
        TagFunc func = s_Group18Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}

// 0x005E3694
void InsertEngine::Tag20_0(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateTag20_0(index);
    }
}

// 0x005E36C8
void InsertEngine::Tag20_1(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateTag20_1(index);
    }
}

// 0x005E3670
void InsertEngine::Tag20_2(const Tag* tag)
{
    u32 index = *reinterpret_cast<const u8*>(tag->args);
    if (index < 10) {
        m_pInsert->UpdateTag20_2(index);
    }
}

// 0x005E36B8
void InsertEngine::Tag20_3(const Tag* tag)
{
    m_pInsert->UpdateTag20_3();
}

// 0x005E2F20
void InsertEngine::OnTagGroup20(const Tag* tag)
{
    if (static_cast<u32>(tag->index) < 5) {
        TagFunc func = s_Group20Funcs[tag->index];
        if (func != NULL) {
            (this->*func)(tag);
        }
    }
}
} // namespace script
