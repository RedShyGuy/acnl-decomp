#pragma once

#include "decomp.h"
#include "script/dSeqEngine.h"
#include "script/dTagId.h"
#include "script/dTalkRuby.h"
#include "script/dTalkSystem.h"
#include "script/dTalkPhrase.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvPlayer.h"
#include "script/dScriptExtern.h"
#include "sead/seadFixedSafeString.h"
#include "Sound/dSoundLabel.h"

namespace script {
class RenderBase;
class TalkWin;
class TalkMeasure;
class Insert;

// (inline) the u16 parameters of a tag (name is ours)
DECOMP_EMIT_COPY DECOMP_NOINLINE inline void GetTagArgs(const Tag* tag, u16* args, u32 num) // 0x0075B01C
{
    const u16* param = reinterpret_cast<const u16*>(tag->args);
    for (u32 i = 0; i < num; i++) {
        args[i] = param[i];
    }
}

// The engine that writes a talk message: the characters at the speed of the talk into the main renderer, the rubies
// with TalkRuby, the inserted words, and the tags of the talk (branches by the state of the game, choices, sounds,
// the recept of the talk). Generated handlers after the tag tables of the binary, the others by hand; member names
// are ours.
// RTTI N6script7TalkSeqE @ 0x008D3B04
// vtable 0x0090A974 (vptr 0x0090A97C), offset_to_top 0, 8 entries
class TalkSeq : public ::script::SeqEngine
{
public:
    typedef void (TalkSeq::*TagFunc)(const Tag* tag);

    // the states of the writing
    static const u8 STATE_NONE = 0;
    static const u8 STATE_WRITE = 1;   // the characters are written
    static const u8 STATE_FULL = 2;    // the lines of the window are full
    static const u8 STATE_END = 3;     // the text is written
    static const u8 STATE_STOP = 4;    // the tag 0/4: wait
    static const u8 STATE_HOLD = 5;    // a tag holds the text until the next update
    static const u8 ITEM_KIND_SPECIAL = 155;

    // The song of Kapp'n (tags 13/6, 13/7): the speed of the text follows the notes of the song (names are ours).
    struct Song
    {
        // (inline)
        explicit Song(TalkSeq* pSeq) : pSeq(pSeq), lineNum(-1), isActive(0), isDone(0), isStopped(0) {}
        // the tag of a line of the song (isFast: the line at the normal speed)
        void Process(const Tag* tag, bool isFast); // 0x006027D0

        TalkSeq* pSeq;    // 0x0
        s32 lineNum;      // 0x4 (-1)
        u8 isActive;      // 0x8 a line is sung
        u8 isDone;        // 0x9
        u8 isStopped;     // 0xA
    };

    // A random branch that does not repeat the last label for the same message (tag 9; names are ours).
    struct RandomBranch
    {
        // the last label per number of labels
        struct Entry
        {
            sead::FixedSafeString<32> message;  // 0x00 the label of the message
            s32 label;                          // 0x2C (-1)
        };

        // (inline)
        RandomBranch(TalkSystem* pOwner, TalkSeq* pSeq) : pOwner(pOwner), pSeq(pSeq), isNoRepeat(0), entries()
        {
            for (u32 i = 0; i < 7; i++) {
                entries[i].message.clear();
                entries[i].label = -1;
            }
        }

        void Branch(const Tag* tag); // 0x005F7ED8

        TalkSystem* pOwner;   // 0x000
        TalkSeq* pSeq;        // 0x004
        u8 isNoRepeat;        // 0x008
        Entry entries[7];     // 0x00C

        static const u8 s_LabelNums[7];
    };

    TalkSeq(TalkSystem* pOwner, RenderBase* pMain, RenderBase* pRuby, TalkWin* pWin, TalkMeasure* pMeasure); // 0x005FC7F0
    virtual ~TalkSeq();
    virtual void Begin(); // 0x005FC408 slot 0x08
    virtual void End(); // 0x005FC364 slot 0x0C
    virtual bool IsContinue(); // 0x005FC504 slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x005F8030 slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005FC370 slot 0x18

    // the writing is reset (a new page)
    void Reset(); // 0x005FC6F0
    // the timing of the characters and the rubies; true: the next character is due
    bool Advance(); // 0x005F9980
    // the choices of the tags 8 and 15 (num choices; hasCancel: with the cancel choice)
    void SetChoice8(const Tag* tag, u32 num, u32 hasCancel); // 0x005F8674
    void SetChoice15(const Tag* tag, u32 num, u32 hasCancel); // 0x005FAA58

private:
    // (inline) the insert of the talk system
    Insert* GetInsert() { return m_pOwner->m_pPhrase->m_pInsert; }
    // (inline) the flow continues at the label (negative: none); before any text the talk jumps at once
    void Branch(s32 label)
    {
        m_pOwner->Jump(label < 0 ? -1 : label, NULL, NULL, 3);
        if (!m_HasText) {
            m_pOwner->m_IsJumped = 1;
        }
    }
    // (inline) the text stays at the tag until the next update
    void HoldAtTag(const Tag* tag)
    {
        m_pCur -= tag->length / sizeof(char16);
        m_State = STATE_HOLD;
    }
    // (inline) the choices are shown, the text continues behind them
    void StartChoice(s32 length)
    {
        m_pOwner->m_IsChoice = 1;
        m_pOwner->m_Flags |= TalkSystem::FLAG_CHOICE;
        if (m_pCur != NULL) {
            m_pCur += length;
        }
    }
    // (inline) the share of the unlocked items of a kind in steps of ten (0: none or no player, 11: all)
    static u32 PercentToIndex(u32 kind)
    {
        SvPlayer* player = SvGetCurrentPlayer();
        if (player == NULL || !player->playerInfo.IsValid()) {
            return 0;
        }
        u32 percent = Func770F68(player->unlockedItems.bits, kind);
        if (percent == 0) {
            return 0;
        }
        if (percent < 10) {
            return 1;
        }
        if (percent < 20) {
            return 2;
        }
        if (percent < 30) {
            return 3;
        }
        if (percent < 40) {
            return 4;
        }
        if (percent < 50) {
            return 5;
        }
        if (percent < 60) {
            return 6;
        }
        if (percent < 70) {
            return 7;
        }
        if (percent < 80) {
            return 8;
        }
        if (percent < 90) {
            return 9;
        }
        if (percent < 100) {
            return 10;
        }
        return percent == 100 ? 11 : 0;
    }
    // (inline) a count in steps of ten (0: none, 11: 100, more: 0)
    static u32 TensToIndex(s32 count)
    {
        if (count == 0) {
            return 0;
        }
        if (count < 10) {
            return 1;
        }
        if (count < 20) {
            return 2;
        }
        if (count < 30) {
            return 3;
        }
        if (count < 40) {
            return 4;
        }
        if (count < 50) {
            return 5;
        }
        if (count < 60) {
            return 6;
        }
        if (count < 70) {
            return 7;
        }
        if (count < 80) {
            return 8;
        }
        if (count < 90) {
            return 9;
        }
        if (count < 100) {
            return 10;
        }
        return count == 100 ? 11 : 0;
    }

public:
    // the tag handlers (generated, bodies of the special tags by hand)
    void TagRuby(const Tag* tag); // 0x005FA37C
    void TagFont(const Tag* tag); // 0x005FA378
    void TagScale(const Tag* tag); // 0x005FA594
    void TagSetColor(const Tag* tag); // 0x005FA9BC
    void TagPageBreak(const Tag* tag); // 0x005FB8D0
    void OnTagGroup0(const Tag* tag); // 0x005F92A8
    void Tag1(const Tag* tag); // 0x005F8314
    void Tag2(const Tag* tag); // 0x005FAB54
    void Tag3(const Tag* tag); // 0x005F9C98
    void TagYear(const Tag* tag); // 0x005FA7F8
    void TagMonthName(const Tag* tag); // 0x005FACE8
    void TagDay(const Tag* tag); // 0x005F8EEC
    void TagHour(const Tag* tag); // 0x005FA7C0
    void TagMinute(const Tag* tag); // 0x005FA1AC
    void TagSecond(const Tag* tag); // 0x005FA1E4
    void TagNumber1(const Tag* tag); // 0x005F9688
    void TagNumber2(const Tag* tag); // 0x005F96D4
    void TagNumber3(const Tag* tag); // 0x005F9720
    void TagNumber4(const Tag* tag); // 0x005F976C
    void TagNumber5(const Tag* tag); // 0x005F97B8
    void TagNumber6(const Tag* tag); // 0x005F9804
    void TagNumber7(const Tag* tag); // 0x005F9850
    void TagReceptYear(const Tag* tag); // 0x005FA87C
    void TagReceptMonth(const Tag* tag); // 0x005FAD20
    void TagReceptDay(const Tag* tag); // 0x005FA21C
    void Tag4_16(const Tag* tag); // 0x005FA830
    void Tag4_17(const Tag* tag); // 0x005FA268
    void Tag4_18(const Tag* tag); // 0x005FA174
    void Tag4_19(const Tag* tag); // 0x005FA774
    void Tag4_20(const Tag* tag); // 0x005FB1A4
    void OnTagGroup4(const Tag* tag); // 0x005F828C
    void Tag5_0(const Tag* tag); // 0x005F9148
    void Tag5_1(const Tag* tag); // 0x005F90BC
    void Tag5_2(const Tag* tag); // 0x005FAE20
    void Tag5_3(const Tag* tag); // 0x005FADD0
    void Tag5_4(const Tag* tag); // 0x005FB660
    void Tag5_5(const Tag* tag); // 0x005F8F60
    void Tag5_6(const Tag* tag); // 0x005F9188
    void Tag5_7(const Tag* tag); // 0x005F8FEC
    void Tag5_8(const Tag* tag); // 0x005FA95C
    void TagWeekday(const Tag* tag); // 0x005F8630
    void TagImpression(const Tag* tag); // 0x005FB614
    void TagConstellation(const Tag* tag); // 0x005FBE84
    void Tag5_12(const Tag* tag); // 0x005FBE4C
    void TagGenderText(const Tag* tag); // 0x005FAE10
    void TagDrink(const Tag* tag); // 0x005F8FA0
    void TagObject(const Tag* tag); // 0x005F98E8
    void TagSweets(const Tag* tag); // 0x005F9214
    void TagSports(const Tag* tag); // 0x005F91C8
    void TagHobby(const Tag* tag); // 0x005F9024
    void TagFood(const Tag* tag); // 0x005F85E4
    void TagMusic(const Tag* tag); // 0x005F9070
    void Tag5_21(const Tag* tag); // 0x005FA970
    void TagAmPm(const Tag* tag); // 0x005FB5DC
    void TagPartOfDay(const Tag* tag); // 0x005FBF74
    void Tag5_24(const Tag* tag); // 0x005FA32C
    void Tag5_25(const Tag* tag); // 0x005FB674
    void TagTownFruit(const Tag* tag); // 0x005FB250
    void Tag5_27(const Tag* tag); // 0x005FBEBC
    void Tag5_28(const Tag* tag); // 0x005FBE38
    void TagBook(const Tag* tag); // 0x005F8598
    void TagSeason(const Tag* tag); // 0x005F9934
    void Tag5_31(const Tag* tag); // 0x005F90FC
    void Tag5_32(const Tag* tag); // 0x005FAE60
    void Tag5_33(const Tag* tag); // 0x005F989C
    void Tag5_34(const Tag* tag); // 0x005FADBC
    void Tag5_35(const Tag* tag); // 0x005FB898
    void Tag5_36(const Tag* tag); // 0x005FBC40
    void Tag5_37(const Tag* tag); // 0x005FB684
    void OnTagGroup5(const Tag* tag); // 0x005F82D0
    void Tag6_(const Tag* tag); // 0x005FA5F4
    void TagPause(const Tag* tag); // 0x005FB6D8
    void TagWaitButton(const Tag* tag); // 0x005FBD38
    void TagInstantBegin(const Tag* tag); // 0x005FBF84
    void TagInstantEnd(const Tag* tag); // 0x005FBD44
    void TagNoFastBegin(const Tag* tag); // 0x005FBA0C
    void TagNoFastEnd(const Tag* tag); // 0x005FB28C
    void TagWordRangeBegin(const Tag* tag); // 0x005FBF00
    void TagWordRangeEnd(const Tag* tag); // 0x005FBA18
    void TagAutoNext(const Tag* tag); // 0x005FB6BC
    void TagAutoNextOnly(const Tag* tag); // 0x005FBEE4
    void TagShake0(const Tag* tag); // 0x005FB2C8
    void TagShake1(const Tag* tag); // 0x005FBA1C
    void TagShake2(const Tag* tag); // 0x005FB298
    void Tag7_13(const Tag* tag); // 0x005FB91C
    void Tag7_14(const Tag* tag); // 0x005FB940
    void Tag7_15(const Tag* tag); // 0x005FB964
    void Tag7_16(const Tag* tag); // 0x005FB988
    void Tag7_17(const Tag* tag); // 0x005FB9AC
    void Tag7_18(const Tag* tag); // 0x005FB9D0
    void OnTagGroup7(const Tag* tag); // 0x005F9C54
    void TagChoiceJump2(const Tag* tag); // 0x005F8668
    void TagChoiceJump3(const Tag* tag); // 0x005FA5F8
    void TagChoiceJump4(const Tag* tag); // 0x005FA604
    void TagChoiceJump5(const Tag* tag); // 0x005FA610
    void TagChoiceJump6(const Tag* tag); // 0x005FA61C
    void TagChoiceJump2Cancel(const Tag* tag); // 0x005FBC80
    void TagChoiceJump3Cancel(const Tag* tag); // 0x005FBC8C
    void TagChoiceJump4Cancel(const Tag* tag); // 0x005FBC98
    void TagChoiceJump5Cancel(const Tag* tag); // 0x005FBCA4
    void TagChoiceJump6Cancel(const Tag* tag); // 0x005FBCB0
    void TagChoiceJumpList(const Tag* tag); // 0x005FABA0
    void OnTagGroup8(const Tag* tag); // 0x005F9260
    void Tag9(const Tag* tag); // 0x005F8380
    void Tag10_0(const Tag* tag); // 0x005FB320
    void Tag10_1(const Tag* tag); // 0x005F9D2C
    void Tag10_2(const Tag* tag); // 0x005F9370
    void Tag10_3(const Tag* tag); // 0x005F9DD0
    void Tag10_4(const Tag* tag); // 0x005F886C
    void Tag10_5(const Tag* tag); // 0x005FB380
    void TagBranchHour(const Tag* tag); // 0x005F8D80
    void TagBranchPartOfDay(const Tag* tag); // 0x005FAC74
    void Tag10_8(const Tag* tag); // 0x005FA6CC
    void TagBranchWeekday(const Tag* tag); // 0x005F8E30
    void Tag10_10(const Tag* tag); // 0x005FA080
    void Tag10_11(const Tag* tag); // 0x005F8C94
    void Tag10_12(const Tag* tag); // 0x005F94C0
    void Tag10_13(const Tag* tag); // 0x005FA628
    void Tag10_14(const Tag* tag); // 0x005F8A58
    void Tag10_15(const Tag* tag); // 0x005F8C0C
    void TagBranchSeason(const Tag* tag); // 0x005FA100
    void TagBranchMonth(const Tag* tag); // 0x005F95A4
    void Tag10_18(const Tag* tag); // 0x005F8CF4
    void Tag10_19(const Tag* tag); // 0x005F84F4
    void Tag10_20(const Tag* tag); // 0x005F8450
    void TagBranchOutdoor(const Tag* tag); // 0x005F92EC
    void Tag10_22(const Tag* tag); // 0x005FAFAC
    void Tag10_23(const Tag* tag); // 0x005FAF4C
    void Tag10_24(const Tag* tag); // 0x005F8B18
    void Tag10_25(const Tag* tag); // 0x005F87B4
    void Tag10_26(const Tag* tag); // 0x005F93FC
    void Tag10_27(const Tag* tag); // 0x005FB100
    void Tag10_28(const Tag* tag); // 0x005F9628
    void Tag10_29(const Tag* tag); // 0x005F898C
    void Tag10_30(const Tag* tag); // 0x005FAC14
    void Tag10_31(const Tag* tag); // 0x005F9EF0
    void OnTagGroup10(const Tag* tag); // 0x005F8318
    void Tag11(const Tag* tag); // 0x005F835C
    void Tag12(const Tag* tag); // 0x005F842C
    void TagSound(const Tag* tag); // 0x005FA8C8
    void TagIslandGameMusic(const Tag* tag); // 0x005FAD74
    void TagUpaCloseMusic(const Tag* tag); // 0x005FAD98
    void Tag13_3(const Tag* tag); // 0x005FB748
    void Tag13_4(const Tag* tag); // 0x005FBDB0
    void Tag13_5(const Tag* tag); // 0x005FBFC4
    void Tag13_6(const Tag* tag); // 0x006027C4
    void Tag13_7(const Tag* tag); // 0x005FBDA4
    void TagChoiceKind(const Tag* tag); // 0x005FC17C
    void TagFxBegin(const Tag* tag); // 0x005845E8
    void TagFxEnd(const Tag* tag); // 0x005FAD6C
    void TagSpeechFilterBegin(const Tag* tag); // 0x0058468C
    void TagSpeechFilterEnd(const Tag* tag); // 0x005FB410
    void OnTagGroup13(const Tag* tag); // 0x005F872C
    void Tag14_0(const Tag* tag); // 0x005FBD94
    void Tag14_1(const Tag* tag); // 0x005FAF34
    void Tag14_2(const Tag* tag); // 0x005FB71C
    void Tag14_3(const Tag* tag); // 0x005FBD64
    void Tag14_4(const Tag* tag); // 0x005FBFA4
    void TagGenderP(const Tag* tag); // 0x005FC058
    void TagSexText(const Tag* tag); // 0x005FC1B4
    void TagGenderN(const Tag* tag); // 0x005FBD70
    void TagGender(const Tag* tag); // 0x005FBD80
    void TagNumberForm(const Tag* tag); // 0x005FBAB8
    void TagElision(const Tag* tag); // 0x005FB2F4
    void TagNumberText4(const Tag* tag); // 0x005FAC0C
    void TagNumberText5(const Tag* tag); // 0x005FAC10
    void TagNumberText6(const Tag* tag); // 0x005FAF48
    void TagPatchimForm(const Tag* tag); // 0x005FB308
    void TagPatchimFormRieulFree(const Tag* tag); // 0x005FB730
    void TagEpenthesisA(const Tag* tag); // 0x005FBF10
    void TagEpenthesisE(const Tag* tag); // 0x005FBF24
    void TagGenderEx(const Tag* tag); // 0x005FBFB0
    void OnTagGroup14(const Tag* tag); // 0x005F9CF0
    void TagChoice2(const Tag* tag); // 0x005FAA4C
    void TagChoice3(const Tag* tag); // 0x005FB6EC
    void TagChoice4(const Tag* tag); // 0x005FB6F8
    void TagChoice5(const Tag* tag); // 0x005FB704
    void TagChoice6(const Tag* tag); // 0x005FB710
    void TagChoice2Cancel(const Tag* tag); // 0x005FC140
    void TagChoice3Cancel(const Tag* tag); // 0x005FC14C
    void TagChoice4Cancel(const Tag* tag); // 0x005FC158
    void TagChoice5Cancel(const Tag* tag); // 0x005FC164
    void TagChoice6Cancel(const Tag* tag); // 0x005FC170
    void TagChoiceList(const Tag* tag); // 0x005FBA4C
    void OnTagGroup15(const Tag* tag); // 0x005FAEF0
    void Tag16(const Tag* tag); // 0x005FA5D0
    void Tag17_0(const Tag* tag); // 0x005FB278
    void Tag17_1(const Tag* tag); // 0x005FB264
    void Tag17_2(const Tag* tag); // 0x005FBD24
    void Tag17_3(const Tag* tag); // 0x005FBD10
    void Tag17_4(const Tag* tag); // 0x005FC008
    void Tag17_5(const Tag* tag); // 0x005FC100
    void Tag17_6(const Tag* tag); // 0x005FBCFC
    void Tag17_7(const Tag* tag); // 0x005FB8DC
    void Tag17_8(const Tag* tag); // 0x005FBCBC
    void Tag17_9(const Tag* tag); // 0x005FC0EC
    void Tag17_10(const Tag* tag); // 0x005FBED0
    void OnTagGroup17(const Tag* tag); // 0x005FAB10
    void Tag18_0(const Tag* tag); // 0x005FBB80
    void Tag18_1(const Tag* tag); // 0x005FBDF4
    void Tag18_2(const Tag* tag); // 0x005FBB08
    void Tag18_3(const Tag* tag); // 0x005FC08C
    void Tag18_4(const Tag* tag); // 0x005FC068
    void Tag18_5(const Tag* tag); // 0x005FB418
    void Tag18_6(const Tag* tag); // 0x005FBBA4
    void Tag18_7(const Tag* tag); // 0x005FBDD0
    void Tag18_8(const Tag* tag); // 0x005FC0B0
    void Tag18_9(const Tag* tag); // 0x005FC1DC
    void Tag18_10(const Tag* tag); // 0x005FBFE4
    void Tag18_11(const Tag* tag); // 0x005FC194
    void Tag18_12(const Tag* tag); // 0x005FB1F0
    void Tag18_13(const Tag* tag); // 0x005FBBC8
    void Tag18_14(const Tag* tag); // 0x005FBF38
    void Tag18_15(const Tag* tag); // 0x005FA2F0
    void Tag18_16(const Tag* tag); // 0x005F8F24
    void Tag18_17(const Tag* tag); // 0x005FBC04
    void Tag18_18(const Tag* tag); // 0x005FB874
    void Tag18_19(const Tag* tag); // 0x005FB43C
    void Tag18_20(const Tag* tag); // 0x005FA920
    void Tag18_21(const Tag* tag); // 0x005FB598
    void Tag18_22(const Tag* tag); // 0x005FB850
    void Tag18_23(const Tag* tag); // 0x005FB49C
    void Tag18_24(const Tag* tag); // 0x005FB4C0
    void Tag18_25(const Tag* tag); // 0x005FB4E4
    void Tag18_26(const Tag* tag); // 0x005FB55C
    void Tag18_27(const Tag* tag); // 0x005FB7D8
    void Tag18_28(const Tag* tag); // 0x005FB460
    void Tag18_29(const Tag* tag); // 0x005FB814
    void Tag18_30(const Tag* tag); // 0x005FBB44
    void Tag18_31(const Tag* tag); // 0x005FA2B4
    void Tag18_32(const Tag* tag); // 0x005FB214
    void Tag18_33(const Tag* tag); // 0x005FBACC
    void Tag18_34(const Tag* tag); // 0x005FB79C
    void OnTagGroup18(const Tag* tag); // 0x005F8770
    void Tag19_(const Tag* tag); // 0x005F92A4
    void Tag20_0(const Tag* tag); // 0x005FC264
    void Tag20_1(const Tag* tag); // 0x005FC304
    void Tag20_2(const Tag* tag); // 0x005FC218
    void Tag20_3(const Tag* tag); // 0x005FC2B0
    void Tag20_4(const Tag* tag); // 0x005FC350
    void OnTagGroup20(const Tag* tag); // 0x005FAE9C
    void TagGeneric_2(const Tag* tag); // 0x005FBD98
    void TagGeneric_3(const Tag* tag); // 0x005FC2E8
    void TagGeneric_4(const Tag* tag); // 0x005FC1D0
    void TagGeneric_5(const Tag* tag); // 0x005FC2F4
    static const TagFunc s_Group0Funcs[5];
    static const TagFunc s_Group4Funcs[21];
    static const TagFunc s_Group5Funcs[38];
    static const TagFunc s_Group7Funcs[19];
    static const TagFunc s_Group8Funcs[11];
    static const TagFunc s_Group10Funcs[32];
    static const TagFunc s_Group13Funcs[13];
    static const TagFunc s_Group14Funcs[19];
    static const TagFunc s_Group15Funcs[11];
    static const TagFunc s_Group17Funcs[11];
    static const TagFunc s_Group18Funcs[35];
    static const TagFunc s_Group20Funcs[5];
    static const TagFunc s_GroupFuncs[21];
    static const TagFunc s_GenericFuncs[7];

    // the tables of the tags and the speakers
    static const u8 s_RandomBranchNums[7];
    static const u8 s_ChoiceKindsA[5];
    static const u8 s_ChoiceKindsB[5];
    static const u8 s_SpeakerWindows[TalkAttribute::SPEAKER_NUM];
    static const u8 s_SpeakerCenter[TalkAttribute::SPEAKER_NUM];
    static const u8 s_SpeakerInstant[TalkAttribute::SPEAKER_NUM];
    static const u8 s_ItemKinds10_22[14];
    static const SeID s_Sounds13_0[7];
    static const f32 s_Waits18_25[10];
    static const f32 s_WaitsNormal[10];
    static const f32 s_WaitsFast[10];
    static const char16 s_WaParticleEnds[14];

    TalkSystem* m_pOwner;          // 0x08C
    RenderBase* m_pMain;           // 0x090
    RenderBase* m_pRuby;           // 0x094
    TalkWin* m_pWin;               // 0x098
    TalkMeasure* m_pMeasure;       // 0x09C
    s32 m_LineMax;                 // 0x0A0 the lines of the window (2 or 3 by the speaker)
    s32 m_LineNum;                 // 0x0A4
    TalkTimer m_Timer;             // 0x0A8
    s32 m_PauseCount;              // 0x0C0 the pause (tag 7/0), in half steps
    s32 m_Delay;                   // 0x0C4 the updates without a character
    u8 m_IsFastRequested;          // 0x0C8 the fast speed was asked for
    u8 m_IsFastLocked;             // 0x0C9 no fast speed (tags)
    u8 m_IsInstant;                // 0x0CA the whole text at once (tags 7/2, 7/3)
    u8 m_IsCenterPending;          // 0x0CB the line is centred at the next update
    u8 m_IsStylePending;           // 0x0CC the scale and the color are set at the next update
    u8 m_HasText;                  // 0x0CD a character was written
    u16 m_Scale;                   // 0x0CE the font scale in percent
    u16 m_Color;                   // 0x0D0 the color (tag 0/3)
    u8 m_State;                    // 0x0D2
    TalkRuby m_RubyText;           // 0x0D4 the main text under a ruby
    TalkRuby m_Ruby;               // 0x160 the ruby
    Song m_Song;                   // 0x1EC
    RandomBranch m_RandomBranch;   // 0x1F8
};
ASSERT_SIZE(TalkSeq, 0x354);
} // namespace script
