#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
class Insert;

// Updates the inserted words of a text: every tag of an inserted word lets the insert fetch its value (the tag handlers
// call the Update slots of Insert). Member names are ours.
// RTTI N6script12InsertEngineE @ 0x008D3834
// vtable 0x0090A0D0 (vptr 0x0090A0D8), offset_to_top 0, 8 entries
class InsertEngine : public ::script::MsgEngine
{
public:
    // (inline)
    InsertEngine(Insert* pInsert, bool isLineFeedUpdate) : m_pInsert(pInsert), m_IsLineFeedUpdate(isLineFeedUpdate) {}
    virtual ~InsertEngine();
    virtual void OnChar(const CharInfo* info); // 0x005E27B4 slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005E36EC slot 0x18

    typedef void (InsertEngine::*TagFunc)(const Tag* tag);
    // the tag handlers (generated)
    void TagYear(const Tag* tag); // 0x005E2DA0
    void TagMonthName(const Tag* tag); // 0x005E2E90
    void TagDay(const Tag* tag); // 0x005E291C
    void TagHour(const Tag* tag); // 0x005E2D90
    void TagMinute(const Tag* tag); // 0x005E2CB8
    void TagSecond(const Tag* tag); // 0x005E2CC8
    void TagNumber1(const Tag* tag); // 0x005E2AA4
    void TagNumber2(const Tag* tag); // 0x005E2AD8
    void TagNumber3(const Tag* tag); // 0x005E2B0C
    void TagNumber4(const Tag* tag); // 0x005E2B40
    void TagNumber5(const Tag* tag); // 0x005E2B74
    void TagNumber6(const Tag* tag); // 0x005E2BA8
    void TagNumber7(const Tag* tag); // 0x005E2BDC
    void TagReceptYear(const Tag* tag); // 0x005E2DD4
    void TagReceptMonth(const Tag* tag); // 0x005E2EA0
    void TagReceptDay(const Tag* tag); // 0x005E2CD8
    void Tag4_16(const Tag* tag); // 0x005E2DB0
    void Tag4_17(const Tag* tag); // 0x005E2CFC
    void Tag4_18(const Tag* tag); // 0x005E2CA8
    void Tag4_19(const Tag* tag); // 0x005E2D6C
    void Tag4_20(const Tag* tag); // 0x005E2F74
    void OnTagGroup4(const Tag* tag); // 0x005E27E0
    static const TagFunc s_Group4Funcs[21];
    void Tag5_0(const Tag* tag); // 0x005E2A24
    void Tag5_1(const Tag* tag); // 0x005E29F0
    void Tag5_2(const Tag* tag); // 0x005E2EFC
    void Tag5_3(const Tag* tag); // 0x005E2EE8
    void Tag5_4(const Tag* tag); // 0x005E3180
    void Tag5_5(const Tag* tag); // 0x005E2940
    void Tag5_6(const Tag* tag); // 0x005E2A34
    void Tag5_7(const Tag* tag); // 0x005E2980
    void Tag5_8(const Tag* tag); // 0x005E2E0C
    void TagWeekday(const Tag* tag); // 0x005E28C8
    void TagImpression(const Tag* tag); // 0x005E315C
    void TagConstellation(const Tag* tag); // 0x005E3494
    void Tag5_12(const Tag* tag); // 0x005E3484
    void TagGenderText(const Tag* tag); // 0x005E2EF8
    void TagDrink(const Tag* tag); // 0x005E2950
    void TagObject(const Tag* tag); // 0x005E2C34
    void TagSweets(const Tag* tag); // 0x005E2A74
    void TagSports(const Tag* tag); // 0x005E2A44
    void TagHobby(const Tag* tag); // 0x005E2990
    void TagFood(const Tag* tag); // 0x005E2898
    void TagMusic(const Tag* tag); // 0x005E29C0
    void Tag5_21(const Tag* tag); // 0x005E2E1C
    void TagAmPm(const Tag* tag); // 0x005E314C
    void TagPartOfDay(const Tag* tag); // 0x005E3504
    void Tag5_24(const Tag* tag); // 0x005E2D48
    void Tag5_25(const Tag* tag); // 0x005E31A4
    void TagTownFruit(const Tag* tag); // 0x005E2FD8
    void Tag5_27(const Tag* tag); // 0x005E34A4
    void Tag5_28(const Tag* tag); // 0x005E3460
    void TagBook(const Tag* tag); // 0x005E2868
    void TagSeason(const Tag* tag); // 0x005E2C64
    void Tag5_31(const Tag* tag); // 0x005E2A00
    void Tag5_32(const Tag* tag); // 0x005E2F0C
    void Tag5_33(const Tag* tag); // 0x005E2C10
    void Tag5_34(const Tag* tag); // 0x005E2EC4
    void Tag5_35(const Tag* tag); // 0x005E32A4
    void Tag5_36(const Tag* tag); // 0x005E3388
    void Tag5_37(const Tag* tag); // 0x005E3200
    void OnTagGroup5(const Tag* tag); // 0x005E2824
    static const TagFunc s_Group5Funcs[38];
    void Tag7_(const Tag* tag); // 0x005E2C88
    void Tag17_0(const Tag* tag); // 0x005E3024
    void Tag17_1(const Tag* tag); // 0x005E2FFC
    void Tag17_2(const Tag* tag); // 0x005E33F8
    void Tag17_3(const Tag* tag); // 0x005E33D0
    void Tag17_4(const Tag* tag); // 0x005E3564
    void Tag17_5(const Tag* tag); // 0x005E3620
    void Tag17_6(const Tag* tag); // 0x005E33A8
    void Tag17_7(const Tag* tag); // 0x005E32BC
    void Tag17_8(const Tag* tag); // 0x005E3398
    void Tag17_9(const Tag* tag); // 0x005E35F8
    void Tag17_10(const Tag* tag); // 0x005E34C8
    void OnTagGroup17(const Tag* tag); // 0x005E2E4C
    static const TagFunc s_Group17Funcs[11];
    void Tag18_0(const Tag* tag); // 0x005E3308
    void Tag18_1(const Tag* tag); // 0x005E344C
    void Tag18_2(const Tag* tag); // 0x005E32E0
    void Tag18_3(const Tag* tag); // 0x005E35B8
    void Tag18_4(const Tag* tag); // 0x005E358C
    void Tag18_5(const Tag* tag); // 0x005E304C
    void Tag18_6(const Tag* tag); // 0x005E3334
    void Tag18_7(const Tag* tag); // 0x005E3420
    void Tag18_8(const Tag* tag); // 0x005E35E4
    void Tag18_9(const Tag* tag); // 0x005E365C
    void Tag18_10(const Tag* tag); // 0x005E3538
    void Tag18_11(const Tag* tag); // 0x005E3630
    void Tag18_12(const Tag* tag); // 0x005E2F98
    void Tag18_13(const Tag* tag); // 0x005E3360
    void Tag18_14(const Tag* tag); // 0x005E34F0
    void Tag18_15(const Tag* tag); // 0x005E2D34
    void Tag18_16(const Tag* tag); // 0x005E292C
    void Tag18_17(const Tag* tag); // 0x005E3374
    void Tag18_18(const Tag* tag); // 0x005E3278
    void Tag18_19(const Tag* tag); // 0x005E3078
    void Tag18_20(const Tag* tag); // 0x005E2DF8
    void Tag18_21(const Tag* tag); // 0x005E3138
    void Tag18_22(const Tag* tag); // 0x005E324C
    void Tag18_23(const Tag* tag); // 0x005E30B8
    void Tag18_24(const Tag* tag); // 0x005E30E4
    void Tag18_25(const Tag* tag); // 0x005E3110
    void Tag18_26(const Tag* tag); // 0x005E3124
    void Tag18_27(const Tag* tag); // 0x005E3224
    void Tag18_28(const Tag* tag); // 0x005E30A4
    void Tag18_29(const Tag* tag); // 0x005E3238
    void Tag18_30(const Tag* tag); // 0x005E32F4
    void Tag18_31(const Tag* tag); // 0x005E2D20
    void Tag18_32(const Tag* tag); // 0x005E2FC4
    void Tag18_33(const Tag* tag); // 0x005E32CC
    void Tag18_34(const Tag* tag); // 0x005E3210
    void OnTagGroup18(const Tag* tag); // 0x005E28D8
    static const TagFunc s_Group18Funcs[35];
    void Tag20_0(const Tag* tag); // 0x005E3694
    void Tag20_1(const Tag* tag); // 0x005E36C8
    void Tag20_2(const Tag* tag); // 0x005E3670
    void Tag20_3(const Tag* tag); // 0x005E36B8
    void OnTagGroup20(const Tag* tag); // 0x005E2F20
    static const TagFunc s_Group20Funcs[5];
    static const TagFunc s_GroupFuncs[21];

    Insert* m_pInsert;       // 0x60
    u8 m_IsLineFeedUpdate;   // 0x64 a line feed updates the word of slot 0x1D8 (MailInsert)
};
ASSERT_SIZE(InsertEngine, 0x68);
} // namespace script
