#pragma once

#include "decomp.h"
#include "script/dMailPhrase.h"
#include "script/dMailSystem.h"
#include "script/dSeqEngine.h"
#include "script/dWordPtr.h"

namespace script {
// Writes a part of a letter with its inserted words into a text of 256 characters. For the addressee (RunTo) it
// counts the lines: the length of the first line is kept, a line feed is not written, and a second line feed fails.
// The tag handlers are generated from the binary (member names are ours).
// RTTI N6script7MailSeqE @ 0x008D3AD8
// vtable 0x0090A928 (vptr 0x0090A930), offset_to_top 0, 8 entries
class MailSeq : public ::script::SeqEngine
{
public:
    typedef void (MailSeq::*TagFunc)(const Tag* tag);
    static const u32 BUFFER_SIZE = 256;

    // (inline in MailSystem)
    explicit MailSeq(MailSystem* pOwner)
        : m_pOwner(pOwner), m_IsCountLines(0), m_IsSuccess(1), m_FirstLineLength(-1), m_Length(0), m_LineCount(0),
          m_pWrite(m_Buffer), m_pLast(&m_Buffer[BUFFER_SIZE - 1]), m_Text(m_Buffer, BUFFER_SIZE), m_Buffer()
    {
        m_Text.Clear();
    }
    virtual ~MailSeq(); // 0x005F7604 slot 0x00
    // 0x005F75F4 slot 0x04 (deleting dtor)
    virtual void Begin(); // 0x005F74EC slot 0x08
    virtual void OnChar(const CharInfo* info); // 0x005F587C slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005F7338 slot 0x18

    // the addressee from *text into m_Text (at most one line feed); true: it fitted
    bool RunTo(const char16* const* text); // 0x005F73D8
    // a text of the letter from *text into m_Text; true: it fitted
    bool RunText(const char16* const* text); // 0x005F74F8

    Insert* GetInsert() { return m_pOwner->m_pPhrase->m_pInsert; }

    void Tag1_(const Tag* tag); // 0x005F5AC8
    void TagYear(const Tag* tag); // 0x005F6500
    void TagMonthName(const Tag* tag); // 0x005F66C8
    void TagDay(const Tag* tag); // 0x005F5BF0
    void TagHour(const Tag* tag); // 0x005F64C8
    void TagMinute(const Tag* tag); // 0x005F62B0
    void TagSecond(const Tag* tag); // 0x005F62E8
    void TagNumber1(const Tag* tag); // 0x005F5F44
    void TagNumber2(const Tag* tag); // 0x005F5F90
    void TagNumber3(const Tag* tag); // 0x005F5FDC
    void TagNumber4(const Tag* tag); // 0x005F6028
    void TagNumber5(const Tag* tag); // 0x005F6074
    void TagNumber6(const Tag* tag); // 0x005F60C0
    void TagNumber7(const Tag* tag); // 0x005F610C
    void TagReceptYear(const Tag* tag); // 0x005F6584
    void TagReceptMonth(const Tag* tag); // 0x005F6700
    void TagReceptDay(const Tag* tag); // 0x005F6320
    void Tag4_16(const Tag* tag); // 0x005F6538
    void Tag4_17(const Tag* tag); // 0x005F636C
    void Tag4_18(const Tag* tag); // 0x005F6278
    void Tag4_19(const Tag* tag); // 0x005F647C
    void Tag4_20(const Tag* tag); // 0x005F6834
    void OnTagGroup4(const Tag* tag); // 0x005F5A20
    void Tag5_0(const Tag* tag); // 0x005F5E3C
    void Tag5_1(const Tag* tag); // 0x005F5DB8
    void Tag5_2(const Tag* tag); // 0x005F67A8
    void Tag5_3(const Tag* tag); // 0x005F6760
    void Tag5_4(const Tag* tag); // 0x005F6B4C
    void Tag5_5(const Tag* tag); // 0x005F5C64
    void Tag5_6(const Tag* tag); // 0x005F5E74
    void Tag5_7(const Tag* tag); // 0x005F5CE8
    void Tag5_8(const Tag* tag); // 0x005F660C
    void TagWeekday(const Tag* tag); // 0x005F5B64
    void TagImpression(const Tag* tag); // 0x005F6B00
    void TagConstellation(const Tag* tag); // 0x005F7060
    void Tag5_12(const Tag* tag); // 0x005F7028
    void TagGenderText(const Tag* tag); // 0x005F6798
    void TagDrink(const Tag* tag); // 0x005F5C9C
    void TagObject(const Tag* tag); // 0x005F61A4
    void TagSweets(const Tag* tag); // 0x005F5EF8
    void TagSports(const Tag* tag); // 0x005F5EAC
    void TagHobby(const Tag* tag); // 0x005F5D20
    void TagFood(const Tag* tag); // 0x005F5B18
    void TagMusic(const Tag* tag); // 0x005F5D6C
    void Tag5_21(const Tag* tag); // 0x005F6620
    void TagAmPm(const Tag* tag); // 0x005F6AC8
    void TagPartOfDay(const Tag* tag); // 0x005F7124
    void Tag5_24(const Tag* tag); // 0x005F6430
    void Tag5_25(const Tag* tag); // 0x005F6B60
    void TagTownFruit(const Tag* tag); // 0x005F68E0
    void Tag5_27(const Tag* tag); // 0x005F7098
    void Tag5_28(const Tag* tag); // 0x005F7014
    void TagBook(const Tag* tag); // 0x005F5ACC
    void TagSeason(const Tag* tag); // 0x005F61F0
    void Tag5_31(const Tag* tag); // 0x005F5DF0
    void Tag5_32(const Tag* tag); // 0x005F67E0
    void Tag5_33(const Tag* tag); // 0x005F6158
    void Tag5_34(const Tag* tag); // 0x005F674C
    void Tag5_35(const Tag* tag); // 0x005F6CD0
    void Tag5_36(const Tag* tag); // 0x005F6EC8
    void Tag5_37(const Tag* tag); // 0x005F6B70
    void OnTagGroup5(const Tag* tag); // 0x005F5A74
    void Tag14_0(const Tag* tag); // 0x005F6FA4
    void Tag14_1(const Tag* tag); // 0x005F681C
    void Tag14_2(const Tag* tag); // 0x005F6BA8
    void Tag14_3(const Tag* tag); // 0x005F6F74
    void Tag14_4(const Tag* tag); // 0x005F7134
    void TagGenderP(const Tag* tag); // 0x005F71C8
    void TagSexText(const Tag* tag); // 0x005F72C8
    void TagGenderN(const Tag* tag); // 0x005F6F80
    void TagGender(const Tag* tag); // 0x005F6F90
    void TagNumberForm(const Tag* tag); // 0x005F6D40
    void TagElision(const Tag* tag); // 0x005F691C
    void TagNumberText4(const Tag* tag); // 0x005F66C0
    void TagNumberText5(const Tag* tag); // 0x005F66C4
    void TagNumberText6(const Tag* tag); // 0x005F6830
    void TagPatchimForm(const Tag* tag); // 0x005F6930
    void TagPatchimFormRieulFree(const Tag* tag); // 0x005F6BBC
    void TagEpenthesisA(const Tag* tag); // 0x005F70C0
    void TagEpenthesisE(const Tag* tag); // 0x005F70D4
    void TagGenderEx(const Tag* tag); // 0x005F7140
    void OnTagGroup14(const Tag* tag); // 0x005F623C
    void Tag17_0(const Tag* tag); // 0x005F6908
    void Tag17_1(const Tag* tag); // 0x005F68F4
    void Tag17_2(const Tag* tag); // 0x005F6F60
    void Tag17_3(const Tag* tag); // 0x005F6F4C
    void Tag17_4(const Tag* tag); // 0x005F7178
    void Tag17_5(const Tag* tag); // 0x005F7270
    void Tag17_6(const Tag* tag); // 0x005F6F38
    void Tag17_7(const Tag* tag); // 0x005F6D08
    void Tag17_8(const Tag* tag); // 0x005F6F00
    void Tag17_9(const Tag* tag); // 0x005F725C
    void Tag17_10(const Tag* tag); // 0x005F70AC
    void OnTagGroup17(const Tag* tag); // 0x005F666C
    void Tag18_0(const Tag* tag); // 0x005F6E08
    void Tag18_1(const Tag* tag); // 0x005F6FD8
    void Tag18_2(const Tag* tag); // 0x005F6D90
    void Tag18_3(const Tag* tag); // 0x005F71FC
    void Tag18_4(const Tag* tag); // 0x005F71D8
    void Tag18_5(const Tag* tag); // 0x005F6948
    void Tag18_6(const Tag* tag); // 0x005F6E2C
    void Tag18_7(const Tag* tag); // 0x005F6FB4
    void Tag18_8(const Tag* tag); // 0x005F7220
    void Tag18_9(const Tag* tag); // 0x005F72F0
    void Tag18_10(const Tag* tag); // 0x005F7154
    void Tag18_11(const Tag* tag); // 0x005F72A8
    void Tag18_12(const Tag* tag); // 0x005F6880
    void Tag18_13(const Tag* tag); // 0x005F6E50
    void Tag18_14(const Tag* tag); // 0x005F70E8
    void Tag18_15(const Tag* tag); // 0x005F63F4
    void Tag18_16(const Tag* tag); // 0x005F5C28
    void Tag18_17(const Tag* tag); // 0x005F6E8C
    void Tag18_18(const Tag* tag); // 0x005F6CAC
    void Tag18_19(const Tag* tag); // 0x005F696C
    void Tag18_20(const Tag* tag); // 0x005F65D0
    void Tag18_21(const Tag* tag); // 0x005F6A8C
    void Tag18_22(const Tag* tag); // 0x005F6C88
    void Tag18_23(const Tag* tag); // 0x005F69CC
    void Tag18_24(const Tag* tag); // 0x005F69F0
    void Tag18_25(const Tag* tag); // 0x005F6A14
    void Tag18_26(const Tag* tag); // 0x005F6A50
    void Tag18_27(const Tag* tag); // 0x005F6C10
    void Tag18_28(const Tag* tag); // 0x005F6990
    void Tag18_29(const Tag* tag); // 0x005F6C4C
    void Tag18_30(const Tag* tag); // 0x005F6DCC
    void Tag18_31(const Tag* tag); // 0x005F63B8
    void Tag18_32(const Tag* tag); // 0x005F68A4
    void Tag18_33(const Tag* tag); // 0x005F6D54
    void Tag18_34(const Tag* tag); // 0x005F6BD4
    void OnTagGroup18(const Tag* tag); // 0x005F5B9C
    void TagGeneric_2(const Tag* tag); // 0x005F6FA8
    void TagGeneric_3(const Tag* tag); // 0x005F732C
    void TagGeneric_4(const Tag* tag); // 0x005F72E4
    static const TagFunc s_Group4Funcs[21];
    static const TagFunc s_Group5Funcs[38];
    static const TagFunc s_Group14Funcs[19];
    static const TagFunc s_Group17Funcs[11];
    static const TagFunc s_Group18Funcs[35];
    static const TagFunc s_GroupFuncs[21];
    static const TagFunc s_GenericFuncs[7];

    MailSystem* m_pOwner;           // 0x08C
    u8 m_IsCountLines;              // 0x090 the line feeds are counted (RunTo)
    u8 m_IsSuccess;                 // 0x091
    s32 m_FirstLineLength;          // 0x094 the characters before the first line feed (-1: none)
    u32 m_Length;                   // 0x098 the written characters
    u32 m_LineCount;                // 0x09C the line feeds
    char16* m_pWrite;               // 0x0A0
    char16* m_pLast;                // 0x0A4
    WordPtr m_Text;                 // 0x0A8 (m_Buffer)
    char16 m_Buffer[BUFFER_SIZE];   // 0x0B4
};
ASSERT_SIZE(MailSeq, 0x2B4);
} // namespace script
