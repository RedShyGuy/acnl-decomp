#include "script/dTalkSeq.h"

// The tables of TalkSeq (a file of their own, else GCC folds them into the code).

namespace script {
// 0x00891724
const u8 TalkSeq::s_RandomBranchNums[7] = {2, 3, 4, 5, 6, 7, 8};
// 0x0089172B
const u8 TalkSeq::RandomBranch::s_LabelNums[7] = {2, 3, 4, 5, 6, 7, 8};
// 0x00891714
const u8 TalkSeq::s_ChoiceKindsA[5] = {3, 0, 5, 6, 10};
// 0x00891719
const u8 TalkSeq::s_ChoiceKindsB[5] = {4, 1, 1, 1, 1};
// the window per kind of speaker (TalkWin::m_WindowKind; the windows 9, 10 have two lines)
// 0x00891F1A
const u8 TalkSeq::s_SpeakerWindows[TalkAttribute::SPEAKER_NUM] = {0, 0, 0, 0, 0, 0, 1, 2, 2, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 8, 8, 8, 9, 10, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0};
// the speakers whose lines are centred
// 0x00891F8F
const u8 TalkSeq::s_SpeakerCenter[TalkAttribute::SPEAKER_NUM] = {0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// the speakers whose text comes at once (1) or never fast (2)
// 0x00891F68
const u8 TalkSeq::s_SpeakerInstant[TalkAttribute::SPEAKER_NUM] = {0, 0, 1, 1, 2, 2, 1, 0, 2, 0, 2, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 2, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// the item kinds of the tag 10/22 (ITEM_KIND_SPECIAL: checked by the item itself)
// 0x008931CC
const u8 TalkSeq::s_ItemKinds10_22[14] = {17, 15, 16, 19, 155, 14, 18, 26, 27, 29, 23, 28, 155, 20};
// 0x008931DC
const SeID TalkSeq::s_Sounds13_0[7] = {SE_ACT_RESET_VIBRASLAP, SE_ACT_RESET_THUNDER, SE_ACT_KAPPEI_ONARA, SE_SYS_WIN_TEXT_ACCENT, SE_ACT_BEV_BELCH, SE_SYS_COPY, SE_SYS_PUBLIC_GET_NEW_CHOICE};
// the waiting times per character by region
// 0x008931F8
const f32 TalkSeq::s_Waits18_25[10] = {0.5916667f, 1.2f, 1.2f, 1.2f, 1.2f, 1.2f, 1.2f, 1.2f, 1.2f, 0.5916667f};
// 0x00892A64
const f32 TalkSeq::s_WaitsNormal[10] = {0.6666667f, 1.2f, 1.2f, 1.2f, 1.2f, 1.2f, 1.2f, 1.2f, 1.2f, 0.6666667f};
// 0x00892A8C
const f32 TalkSeq::s_WaitsFast[10] = {2.0f, 2.4f, 2.4f, 2.4f, 2.4f, 2.4f, 2.4f, 2.4f, 2.4f, 2.0f};
// the characters after which は sounds as わ
// 0x00892948
const char16 TalkSeq::s_WaParticleEnds[14] = {0x002C, 0x00B7, 0x30FB, 0x003F, 0xFF1F, 0x0021, 0xFF01, 0x3001, 0x3002, 0x0020, 0x3000, 0x002E, 0x000A, 0x0000};

// 0x00892AB4
const TalkSeq::TagFunc TalkSeq::s_Group0Funcs[5] = {
    &TalkSeq::TagRuby,
    &TalkSeq::TagFont,
    &TalkSeq::TagScale,
    &TalkSeq::TagSetColor,
    &TalkSeq::TagPageBreak,
};

// 0x00892ADC
const TalkSeq::TagFunc TalkSeq::s_Group4Funcs[21] = {
    &TalkSeq::TagYear,
    &TalkSeq::TagMonthName,
    &TalkSeq::TagDay,
    &TalkSeq::TagHour,
    &TalkSeq::TagMinute,
    &TalkSeq::TagSecond,
    &TalkSeq::TagNumber1,
    &TalkSeq::TagNumber2,
    &TalkSeq::TagNumber3,
    &TalkSeq::TagNumber4,
    &TalkSeq::TagNumber5,
    &TalkSeq::TagNumber6,
    &TalkSeq::TagNumber7,
    &TalkSeq::TagReceptYear,
    &TalkSeq::TagReceptMonth,
    &TalkSeq::TagReceptDay,
    &TalkSeq::Tag4_16,
    &TalkSeq::Tag4_17,
    &TalkSeq::Tag4_18,
    &TalkSeq::Tag4_19,
    &TalkSeq::Tag4_20,
};

// 0x00892B84
const TalkSeq::TagFunc TalkSeq::s_Group5Funcs[38] = {
    &TalkSeq::Tag5_0,
    &TalkSeq::Tag5_1,
    &TalkSeq::Tag5_2,
    &TalkSeq::Tag5_3,
    &TalkSeq::Tag5_4,
    &TalkSeq::Tag5_5,
    &TalkSeq::Tag5_6,
    &TalkSeq::Tag5_7,
    &TalkSeq::Tag5_8,
    &TalkSeq::TagWeekday,
    &TalkSeq::TagImpression,
    &TalkSeq::TagConstellation,
    &TalkSeq::Tag5_12,
    &TalkSeq::TagGenderText,
    &TalkSeq::TagDrink,
    &TalkSeq::TagObject,
    &TalkSeq::TagSweets,
    &TalkSeq::TagSports,
    &TalkSeq::TagHobby,
    &TalkSeq::TagFood,
    &TalkSeq::TagMusic,
    &TalkSeq::Tag5_21,
    &TalkSeq::TagAmPm,
    &TalkSeq::TagPartOfDay,
    &TalkSeq::Tag5_24,
    &TalkSeq::Tag5_25,
    &TalkSeq::TagTownFruit,
    &TalkSeq::Tag5_27,
    &TalkSeq::Tag5_28,
    &TalkSeq::TagBook,
    &TalkSeq::TagSeason,
    &TalkSeq::Tag5_31,
    &TalkSeq::Tag5_32,
    &TalkSeq::Tag5_33,
    &TalkSeq::Tag5_34,
    &TalkSeq::Tag5_35,
    &TalkSeq::Tag5_36,
    &TalkSeq::Tag5_37,
};

// 0x00892CB4
const TalkSeq::TagFunc TalkSeq::s_Group7Funcs[19] = {
    &TalkSeq::TagPause,
    &TalkSeq::TagWaitButton,
    &TalkSeq::TagInstantBegin,
    &TalkSeq::TagInstantEnd,
    &TalkSeq::TagNoFastBegin,
    &TalkSeq::TagNoFastEnd,
    &TalkSeq::TagWordRangeBegin,
    &TalkSeq::TagWordRangeEnd,
    &TalkSeq::TagAutoNext,
    &TalkSeq::TagAutoNextOnly,
    &TalkSeq::TagShake0,
    &TalkSeq::TagShake1,
    &TalkSeq::TagShake2,
    &TalkSeq::Tag7_13,
    &TalkSeq::Tag7_14,
    &TalkSeq::Tag7_15,
    &TalkSeq::Tag7_16,
    &TalkSeq::Tag7_17,
    &TalkSeq::Tag7_18,
};

// 0x00892D4C
const TalkSeq::TagFunc TalkSeq::s_Group8Funcs[11] = {
    &TalkSeq::TagChoiceJump2,
    &TalkSeq::TagChoiceJump3,
    &TalkSeq::TagChoiceJump4,
    &TalkSeq::TagChoiceJump5,
    &TalkSeq::TagChoiceJump6,
    &TalkSeq::TagChoiceJump2Cancel,
    &TalkSeq::TagChoiceJump3Cancel,
    &TalkSeq::TagChoiceJump4Cancel,
    &TalkSeq::TagChoiceJump5Cancel,
    &TalkSeq::TagChoiceJump6Cancel,
    &TalkSeq::TagChoiceJumpList,
};

// 0x00892DA4
const TalkSeq::TagFunc TalkSeq::s_Group10Funcs[32] = {
    &TalkSeq::Tag10_0,
    &TalkSeq::Tag10_1,
    &TalkSeq::Tag10_2,
    &TalkSeq::Tag10_3,
    &TalkSeq::Tag10_4,
    &TalkSeq::Tag10_5,
    &TalkSeq::TagBranchHour,
    &TalkSeq::TagBranchPartOfDay,
    &TalkSeq::Tag10_8,
    &TalkSeq::TagBranchWeekday,
    &TalkSeq::Tag10_10,
    &TalkSeq::Tag10_11,
    &TalkSeq::Tag10_12,
    &TalkSeq::Tag10_13,
    &TalkSeq::Tag10_14,
    &TalkSeq::Tag10_15,
    &TalkSeq::TagBranchSeason,
    &TalkSeq::TagBranchMonth,
    &TalkSeq::Tag10_18,
    &TalkSeq::Tag10_19,
    &TalkSeq::Tag10_20,
    &TalkSeq::TagBranchOutdoor,
    &TalkSeq::Tag10_22,
    &TalkSeq::Tag10_23,
    &TalkSeq::Tag10_24,
    &TalkSeq::Tag10_25,
    &TalkSeq::Tag10_26,
    &TalkSeq::Tag10_27,
    &TalkSeq::Tag10_28,
    &TalkSeq::Tag10_29,
    &TalkSeq::Tag10_30,
    &TalkSeq::Tag10_31,
};

// 0x00892EA4
const TalkSeq::TagFunc TalkSeq::s_Group13Funcs[13] = {
    &TalkSeq::TagSound,
    &TalkSeq::TagIslandGameMusic,
    &TalkSeq::TagUpaCloseMusic,
    &TalkSeq::Tag13_3,
    &TalkSeq::Tag13_4,
    &TalkSeq::Tag13_5,
    &TalkSeq::Tag13_6,
    &TalkSeq::Tag13_7,
    &TalkSeq::TagChoiceKind,
    &TalkSeq::TagFxBegin,
    &TalkSeq::TagFxEnd,
    &TalkSeq::TagSpeechFilterBegin,
    &TalkSeq::TagSpeechFilterEnd,
};

// 0x00892F0C
const TalkSeq::TagFunc TalkSeq::s_Group14Funcs[19] = {
    &TalkSeq::Tag14_0,
    &TalkSeq::Tag14_1,
    &TalkSeq::Tag14_2,
    &TalkSeq::Tag14_3,
    &TalkSeq::Tag14_4,
    &TalkSeq::TagGenderP,
    &TalkSeq::TagSexText,
    &TalkSeq::TagGenderN,
    &TalkSeq::TagGender,
    &TalkSeq::TagNumberForm,
    &TalkSeq::TagElision,
    &TalkSeq::TagNumberText4,
    &TalkSeq::TagNumberText5,
    &TalkSeq::TagNumberText6,
    &TalkSeq::TagPatchimForm,
    &TalkSeq::TagPatchimFormRieulFree,
    &TalkSeq::TagEpenthesisA,
    &TalkSeq::TagEpenthesisE,
    &TalkSeq::TagGenderEx,
};

// 0x00892FA4
const TalkSeq::TagFunc TalkSeq::s_Group15Funcs[11] = {
    &TalkSeq::TagChoice2,
    &TalkSeq::TagChoice3,
    &TalkSeq::TagChoice4,
    &TalkSeq::TagChoice5,
    &TalkSeq::TagChoice6,
    &TalkSeq::TagChoice2Cancel,
    &TalkSeq::TagChoice3Cancel,
    &TalkSeq::TagChoice4Cancel,
    &TalkSeq::TagChoice5Cancel,
    &TalkSeq::TagChoice6Cancel,
    &TalkSeq::TagChoiceList,
};

// 0x00892FFC
const TalkSeq::TagFunc TalkSeq::s_Group17Funcs[11] = {
    &TalkSeq::Tag17_0,
    &TalkSeq::Tag17_1,
    &TalkSeq::Tag17_2,
    &TalkSeq::Tag17_3,
    &TalkSeq::Tag17_4,
    &TalkSeq::Tag17_5,
    &TalkSeq::Tag17_6,
    &TalkSeq::Tag17_7,
    &TalkSeq::Tag17_8,
    &TalkSeq::Tag17_9,
    &TalkSeq::Tag17_10,
};

// 0x00893054
const TalkSeq::TagFunc TalkSeq::s_Group18Funcs[35] = {
    &TalkSeq::Tag18_0,
    &TalkSeq::Tag18_1,
    &TalkSeq::Tag18_2,
    &TalkSeq::Tag18_3,
    &TalkSeq::Tag18_4,
    &TalkSeq::Tag18_5,
    &TalkSeq::Tag18_6,
    &TalkSeq::Tag18_7,
    &TalkSeq::Tag18_8,
    &TalkSeq::Tag18_9,
    &TalkSeq::Tag18_10,
    &TalkSeq::Tag18_11,
    &TalkSeq::Tag18_12,
    &TalkSeq::Tag18_13,
    &TalkSeq::Tag18_14,
    &TalkSeq::Tag18_15,
    &TalkSeq::Tag18_16,
    &TalkSeq::Tag18_17,
    &TalkSeq::Tag18_18,
    &TalkSeq::Tag18_19,
    &TalkSeq::Tag18_20,
    &TalkSeq::Tag18_21,
    &TalkSeq::Tag18_22,
    &TalkSeq::Tag18_23,
    &TalkSeq::Tag18_24,
    &TalkSeq::Tag18_25,
    &TalkSeq::Tag18_26,
    &TalkSeq::Tag18_27,
    &TalkSeq::Tag18_28,
    &TalkSeq::Tag18_29,
    &TalkSeq::Tag18_30,
    &TalkSeq::Tag18_31,
    &TalkSeq::Tag18_32,
    &TalkSeq::Tag18_33,
    &TalkSeq::Tag18_34,
};

// 0x0089316C
const TalkSeq::TagFunc TalkSeq::s_Group20Funcs[5] = {
    &TalkSeq::Tag20_0,
    &TalkSeq::Tag20_1,
    &TalkSeq::Tag20_2,
    &TalkSeq::Tag20_3,
    &TalkSeq::Tag20_4,
};

// 0x008929BC
const TalkSeq::TagFunc TalkSeq::s_GroupFuncs[21] = {
    &TalkSeq::OnTagGroup0,
    &TalkSeq::Tag1,
    &TalkSeq::Tag2,
    &TalkSeq::Tag3,
    &TalkSeq::OnTagGroup4,
    &TalkSeq::OnTagGroup5,
    &TalkSeq::Tag6_,
    &TalkSeq::OnTagGroup7,
    &TalkSeq::OnTagGroup8,
    &TalkSeq::Tag9,
    &TalkSeq::OnTagGroup10,
    &TalkSeq::Tag11,
    &TalkSeq::Tag12,
    &TalkSeq::OnTagGroup13,
    &TalkSeq::OnTagGroup14,
    &TalkSeq::OnTagGroup15,
    &TalkSeq::Tag16,
    &TalkSeq::OnTagGroup17,
    &TalkSeq::OnTagGroup18,
    &TalkSeq::Tag19_,
    &TalkSeq::OnTagGroup20,
};

// 0x00893194
const TalkSeq::TagFunc TalkSeq::s_GenericFuncs[7] = {
    NULL,
    NULL,
    &TalkSeq::TagGeneric_2,
    &TalkSeq::TagGeneric_3,
    &TalkSeq::TagGeneric_4,
    &TalkSeq::TagGeneric_5,
    NULL,
};
} // namespace script
