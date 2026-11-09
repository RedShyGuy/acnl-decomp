#pragma once

#include "decomp.h"
#include "script/dIMeasure.h"
#include "script/dMailPhrase.h"
#include "script/dMailSystem.h"
#include "script/dSeqEngine.h"

namespace script {
class MailSystem;

// Measures the parts of letters with their inserted words (as TalkMeasure, with a buffer of 192 characters; member
// names are ours). The tag handlers (generated from the binary) insert the words of the insert of the letter system.
// RTTI N6script11MailMeasureE @ 0x008D37E8
// vtable 0x00909FFC (vptr 0x0090A004), offset_to_top 0, 11 entries
// vtable 0x0090A030 (vptr 0x0090A038), offset_to_top -140, 6 entries
class MailMeasure : public ::script::SeqEngine, public ::script::IMeasure
{
public:
    typedef void (MailMeasure::*TagFunc)(const Tag* tag);
    static const u32 BUFFER_SIZE = 192;

    // (inline in MailSystem)
    explicit MailMeasure(MailSystem* pOwner)
        : m_pOwner(pOwner), m_pChild(NULL), m_IsStop(0), m_pWrite(m_Buffer), m_pLast(&m_Buffer[BUFFER_SIZE - 1]),
          m_Buffer()
    {
        for (u32 i = 0; i < BUFFER_SIZE; i++) {
            m_Buffer[i] = 0;
        }
        m_IsStop = 0;
        m_pWrite = m_Buffer;
        Reset();
    }
    virtual ~MailMeasure();
    virtual void Begin(); // slot 0x08
    virtual void End(); // slot 0x0C
    virtual bool IsContinue(); // slot 0x10
    virtual void OnChar(const CharInfo* info); // slot 0x14
    virtual void OnTag(const Tag* tag); // slot 0x18
    virtual IMeasure* GetChild(); // slot 0x20
    virtual f32 Measure(const IWord* word); // slot 0x24
    virtual f32 MeasureText(const char16* const* begin, const char16* const* end); // slot 0x28

    Insert* GetInsert() { return m_pOwner->m_pPhrase->m_pInsert; }
    // the width of a text in the text box of the letter menu or of the bulletin board menu (0 without a recept or menu)
    DECOMP_NOINLINE f32 MeasureWithMenuTextBox(const char16* const* begin, const char16* const* end); // 0x0075BBF4

    void Tag1_(const Tag* tag); // 0x005DE324
    void TagYear(const Tag* tag); // 0x005DECFC
    void TagMonthName(const Tag* tag); // 0x005DEEA8
    void TagDay(const Tag* tag); // 0x005DE414
    void TagHour(const Tag* tag); // 0x005DECC4
    void TagMinute(const Tag* tag); // 0x005DEA9C
    void TagSecond(const Tag* tag); // 0x005DEAD4
    void TagNumber1(const Tag* tag); // 0x005DE768
    void TagNumber2(const Tag* tag); // 0x005DE7B4
    void TagNumber3(const Tag* tag); // 0x005DE800
    void TagNumber4(const Tag* tag); // 0x005DE84C
    void TagNumber5(const Tag* tag); // 0x005DE898
    void TagNumber6(const Tag* tag); // 0x005DE8E4
    void TagNumber7(const Tag* tag); // 0x005DE930
    void TagReceptYear(const Tag* tag); // 0x005DED80
    void TagReceptMonth(const Tag* tag); // 0x005DEEE0
    void TagReceptDay(const Tag* tag); // 0x005DEB0C
    void Tag4_16(const Tag* tag); // 0x005DED34
    void Tag4_17(const Tag* tag); // 0x005DEB58
    void Tag4_18(const Tag* tag); // 0x005FE004
    void Tag4_19(const Tag* tag); // 0x005DEC78
    void Tag4_20(const Tag* tag); // 0x005DEFD8
    void OnTagGroup4(const Tag* tag); // 0x005DE27C
    void Tag5_0(const Tag* tag); // 0x005DE660
    void Tag5_1(const Tag* tag); // 0x005DE5DC
    void Tag5_2(const Tag* tag); // 0x005DEF64
    void Tag5_3(const Tag* tag); // 0x005DEF2C
    void Tag5_4(const Tag* tag); // 0x005F2A48
    void Tag5_5(const Tag* tag); // 0x005DE488
    void Tag5_6(const Tag* tag); // 0x005DE698
    void Tag5_7(const Tag* tag); // 0x005DE50C
    void Tag5_8(const Tag* tag); // 0x005F1E6C
    void TagWeekday(const Tag* tag); // 0x005FDF50
    void TagImpression(const Tag* tag); // 0x005DF2DC
    void TagConstellation(const Tag* tag); // 0x005DF790
    void Tag5_12(const Tag* tag); // 0x005DF758
    void TagGenderText(const Tag* tag); // 0x005F22F8
    void TagDrink(const Tag* tag); // 0x005DE4C0
    void TagObject(const Tag* tag); // 0x005DE9C8
    void TagSweets(const Tag* tag); // 0x005DE71C
    void TagSports(const Tag* tag); // 0x005DE6D0
    void TagHobby(const Tag* tag); // 0x005DE544
    void TagFood(const Tag* tag); // 0x005DE374
    void TagMusic(const Tag* tag); // 0x005DE590
    void Tag5_21(const Tag* tag); // 0x005DEE08
    void TagAmPm(const Tag* tag); // 0x005DF2A4
    void TagPartOfDay(const Tag* tag); // 0x005F34BC
    void Tag5_24(const Tag* tag); // 0x005DEC1C
    void Tag5_25(const Tag* tag); // 0x005F2AB4
    void TagTownFruit(const Tag* tag); // 0x005F24B8
    void Tag5_27(const Tag* tag); // 0x005F31B8
    void Tag5_28(const Tag* tag); // 0x005F314C
    void TagBook(const Tag* tag); // 0x005DE328
    void TagSeason(const Tag* tag); // 0x005DEA14
    void Tag5_31(const Tag* tag); // 0x005DE614
    void Tag5_32(const Tag* tag); // 0x005DEF9C
    void Tag5_33(const Tag* tag); // 0x005DE97C
    void Tag5_34(const Tag* tag); // 0x005F228C
    void Tag5_35(const Tag* tag); // 0x005DF488
    void Tag5_36(const Tag* tag); // 0x005DF66C
    void Tag5_37(const Tag* tag); // 0x005DF328
    void OnTagGroup5(const Tag* tag); // 0x005DE2D0
    void Tag14_0(const Tag* tag); // 0x005DF6E8
    void Tag14_1(const Tag* tag); // 0x005F1F80
    void Tag14_2(const Tag* tag); // 0x005DF360
    void Tag14_3(const Tag* tag); // 0x005DF6DC
    void Tag14_4(const Tag* tag); // 0x005DF804
    void TagGenderP(const Tag* tag); // 0x005F37A4
    void TagSexText(const Tag* tag); // 0x005F39A8
    void TagGenderN(const Tag* tag); // 0x005F2DAC
    void TagGender(const Tag* tag); // 0x005F2F44
    void TagNumberForm(const Tag* tag); // 0x005F2B48
    void TagElision(const Tag* tag); // 0x005F25FC
    void TagNumberText4(const Tag* tag); // 0x005F20B8
    void TagNumberText5(const Tag* tag); // 0x005F2194
    void TagNumberText6(const Tag* tag); // 0x005F23C0
    void TagPatchimForm(const Tag* tag); // 0x005F284C
    void TagPatchimFormRieulFree(const Tag* tag); // 0x005DF374
    void TagEpenthesisA(const Tag* tag); // 0x005F3290
    void TagEpenthesisE(const Tag* tag); // 0x005F32FC
    void TagGenderEx(const Tag* tag); // 0x005F3584
    void OnTagGroup14(const Tag* tag); // 0x005DEA60
    void Tag17_0(const Tag* tag); // 0x005F2590
    void Tag17_1(const Tag* tag); // 0x005F2524
    void Tag17_2(const Tag* tag); // 0x005F2D40
    void Tag17_3(const Tag* tag); // 0x005F2CD4
    void Tag17_4(const Tag* tag); // 0x005DF834
    void Tag17_5(const Tag* tag); // 0x005DF908
    void Tag17_6(const Tag* tag); // 0x005F2C68
    void Tag17_7(const Tag* tag); // 0x005DF4C0
    void Tag17_8(const Tag* tag); // 0x005DF6A4
    void Tag17_9(const Tag* tag); // 0x005F393C
    void Tag17_10(const Tag* tag); // 0x005F3224
    void OnTagGroup17(const Tag* tag); // 0x005DEE54
    void Tag18_0(const Tag* tag); // 0x005DF5AC
    void Tag18_1(const Tag* tag); // 0x005DF71C
    void Tag18_2(const Tag* tag); // 0x005DF534
    void Tag18_3(const Tag* tag); // 0x005DF8A8
    void Tag18_4(const Tag* tag); // 0x005DF884
    void Tag18_5(const Tag* tag); // 0x005DF124
    void Tag18_6(const Tag* tag); // 0x005DF5D0
    void Tag18_7(const Tag* tag); // 0x005DF6F8
    void Tag18_8(const Tag* tag); // 0x005DF8CC
    void Tag18_9(const Tag* tag); // 0x005DF96C
    void Tag18_10(const Tag* tag); // 0x005DF810
    void Tag18_11(const Tag* tag); // 0x005DF940
    void Tag18_12(const Tag* tag); // 0x005DF024
    void Tag18_13(const Tag* tag); // 0x005DF5F4
    void Tag18_14(const Tag* tag); // 0x005DF7C8
    void Tag18_15(const Tag* tag); // 0x005DEBE0
    void Tag18_16(const Tag* tag); // 0x005DE44C
    void Tag18_17(const Tag* tag); // 0x005DF630
    void Tag18_18(const Tag* tag); // 0x005DF464
    void Tag18_19(const Tag* tag); // 0x005DF148
    void Tag18_20(const Tag* tag); // 0x005DEDCC
    void Tag18_21(const Tag* tag); // 0x005DF268
    void Tag18_22(const Tag* tag); // 0x005DF440
    void Tag18_23(const Tag* tag); // 0x005DF1A8
    void Tag18_24(const Tag* tag); // 0x005DF1CC
    void Tag18_25(const Tag* tag); // 0x005DF1F0
    void Tag18_26(const Tag* tag); // 0x005DF22C
    void Tag18_27(const Tag* tag); // 0x005DF3C8
    void Tag18_28(const Tag* tag); // 0x005DF16C
    void Tag18_29(const Tag* tag); // 0x005DF404
    void Tag18_30(const Tag* tag); // 0x005DF570
    void Tag18_31(const Tag* tag); // 0x005DEBA4
    void Tag18_32(const Tag* tag); // 0x005DF048
    void Tag18_33(const Tag* tag); // 0x005DF4F8
    void Tag18_34(const Tag* tag); // 0x005DF38C
    void OnTagGroup18(const Tag* tag); // 0x005DE3C0
    void TagGeneric_2(const Tag* tag); // 0x005DF6EC
    void TagGeneric_3(const Tag* tag); // 0x005DF9A8
    void TagGeneric_4(const Tag* tag); // 0x005DF960
    static const TagFunc s_Group4Funcs[21];
    static const TagFunc s_Group5Funcs[38];
    static const TagFunc s_Group14Funcs[19];
    static const TagFunc s_Group17Funcs[11];
    static const TagFunc s_Group18Funcs[35];
    static const TagFunc s_GroupFuncs[21];
    static const TagFunc s_GenericFuncs[7];


    MailSystem* m_pOwner;           // 0x090
    MailMeasure* m_pChild;          // 0x094
    u8 m_IsStop;                    // 0x098
    char16* m_pWrite;               // 0x09C
    char16* m_pLast;                // 0x0A0
    char16 m_Buffer[BUFFER_SIZE];   // 0x0A4
};
ASSERT_SIZE(MailMeasure, 0x224);
} // namespace script
