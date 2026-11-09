#include "script/dTalkMeasure.h"

namespace script {
// the handlers of the tags (in a file of their own, otherwise GCC calls them directly)
// 0x008955D4
const TalkMeasure::TagFunc TalkMeasure::s_Group0Funcs[5] = {
    &TalkMeasure::TagRuby,
    NULL,
    &TalkMeasure::TagScale,
    NULL,
    &TalkMeasure::TagPageBreak,
};

// 0x008955FC
const TalkMeasure::TagFunc TalkMeasure::s_Group4Funcs[21] = {
    &TalkMeasure::TagYear,
    &TalkMeasure::TagMonthName,
    &TalkMeasure::TagDay,
    &TalkMeasure::TagHour,
    &TalkMeasure::TagMinute,
    &TalkMeasure::TagSecond,
    &TalkMeasure::TagNumber1,
    &TalkMeasure::TagNumber2,
    &TalkMeasure::TagNumber3,
    &TalkMeasure::TagNumber4,
    &TalkMeasure::TagNumber5,
    &TalkMeasure::TagNumber6,
    &TalkMeasure::TagNumber7,
    &TalkMeasure::TagReceptYear,
    &TalkMeasure::TagReceptMonth,
    &TalkMeasure::TagReceptDay,
    &TalkMeasure::Tag4_16,
    &TalkMeasure::Tag4_17,
    &TalkMeasure::Tag4_18,
    &TalkMeasure::Tag4_19,
    &TalkMeasure::Tag4_20,
};

// 0x008956A4
const TalkMeasure::TagFunc TalkMeasure::s_Group5Funcs[38] = {
    &TalkMeasure::Tag5_0,
    &TalkMeasure::Tag5_1,
    &TalkMeasure::Tag5_2,
    &TalkMeasure::Tag5_3,
    &TalkMeasure::Tag5_4,
    &TalkMeasure::Tag5_5,
    &TalkMeasure::Tag5_6,
    &TalkMeasure::Tag5_7,
    &TalkMeasure::Tag5_8,
    &TalkMeasure::TagWeekday,
    &TalkMeasure::TagImpression,
    &TalkMeasure::TagConstellation,
    &TalkMeasure::Tag5_12,
    &TalkMeasure::TagGenderText,
    &TalkMeasure::TagDrink,
    &TalkMeasure::TagObject,
    &TalkMeasure::TagSweets,
    &TalkMeasure::TagSports,
    &TalkMeasure::TagHobby,
    &TalkMeasure::TagFood,
    &TalkMeasure::TagMusic,
    &TalkMeasure::Tag5_21,
    &TalkMeasure::TagAmPm,
    &TalkMeasure::TagPartOfDay,
    &TalkMeasure::Tag5_24,
    &TalkMeasure::Tag5_25,
    &TalkMeasure::TagTownFruit,
    &TalkMeasure::Tag5_27,
    &TalkMeasure::Tag5_28,
    &TalkMeasure::TagBook,
    &TalkMeasure::TagSeason,
    &TalkMeasure::Tag5_31,
    &TalkMeasure::Tag5_32,
    &TalkMeasure::Tag5_33,
    &TalkMeasure::Tag5_34,
    &TalkMeasure::Tag5_35,
    &TalkMeasure::Tag5_36,
    &TalkMeasure::Tag5_37,
};

// 0x008957D4
const TalkMeasure::TagFunc TalkMeasure::s_Group7Funcs[19] = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &TalkMeasure::TagWordRangeBegin,
    &TalkMeasure::TagWordRangeEnd,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

// 0x0089586C
const TalkMeasure::TagFunc TalkMeasure::s_Group8Funcs[11] = {
    &TalkMeasure::TagChoiceJump2,
    &TalkMeasure::TagChoiceJump3,
    &TalkMeasure::TagChoiceJump4,
    &TalkMeasure::TagChoiceJump5,
    &TalkMeasure::TagChoiceJump6,
    &TalkMeasure::TagChoiceJump2Cancel,
    &TalkMeasure::TagChoiceJump3Cancel,
    &TalkMeasure::TagChoiceJump4Cancel,
    &TalkMeasure::TagChoiceJump5Cancel,
    &TalkMeasure::TagChoiceJump6Cancel,
    &TalkMeasure::TagChoiceJumpList,
};

// 0x008958C4
const TalkMeasure::TagFunc TalkMeasure::s_Group14Funcs[19] = {
    &TalkMeasure::Tag14_0,
    &TalkMeasure::Tag14_1,
    &TalkMeasure::Tag14_2,
    &TalkMeasure::Tag14_3,
    &TalkMeasure::Tag14_4,
    &TalkMeasure::TagGenderP,
    &TalkMeasure::TagSexText,
    &TalkMeasure::TagGenderN,
    &TalkMeasure::TagGender,
    &TalkMeasure::TagNumberForm,
    &TalkMeasure::TagElision,
    &TalkMeasure::TagNumberText4,
    &TalkMeasure::TagNumberText5,
    &TalkMeasure::TagNumberText6,
    &TalkMeasure::TagPatchimForm,
    &TalkMeasure::TagPatchimFormRieulFree,
    &TalkMeasure::TagEpenthesisA,
    &TalkMeasure::TagEpenthesisE,
    &TalkMeasure::TagGenderEx,
};

// 0x0089595C
const TalkMeasure::TagFunc TalkMeasure::s_Group15Funcs[11] = {
    &TalkMeasure::TagChoice2,
    &TalkMeasure::TagChoice3,
    &TalkMeasure::TagChoice4,
    &TalkMeasure::TagChoice5,
    &TalkMeasure::TagChoice6,
    &TalkMeasure::TagChoice2Cancel,
    &TalkMeasure::TagChoice3Cancel,
    &TalkMeasure::TagChoice4Cancel,
    &TalkMeasure::TagChoice5Cancel,
    &TalkMeasure::TagChoice6Cancel,
    &TalkMeasure::TagChoiceList,
};

// 0x008959B4
const TalkMeasure::TagFunc TalkMeasure::s_Group17Funcs[11] = {
    &TalkMeasure::Tag17_0,
    &TalkMeasure::Tag17_1,
    &TalkMeasure::Tag17_2,
    &TalkMeasure::Tag17_3,
    &TalkMeasure::Tag17_4,
    &TalkMeasure::Tag17_5,
    &TalkMeasure::Tag17_6,
    &TalkMeasure::Tag17_7,
    &TalkMeasure::Tag17_8,
    &TalkMeasure::Tag17_9,
    &TalkMeasure::Tag17_10,
};

// 0x00895A0C
const TalkMeasure::TagFunc TalkMeasure::s_Group18Funcs[35] = {
    &TalkMeasure::Tag18_0,
    &TalkMeasure::Tag18_1,
    &TalkMeasure::Tag18_2,
    &TalkMeasure::Tag18_3,
    &TalkMeasure::Tag18_4,
    &TalkMeasure::Tag18_5,
    &TalkMeasure::Tag18_6,
    &TalkMeasure::Tag18_7,
    &TalkMeasure::Tag18_8,
    &TalkMeasure::Tag18_9,
    &TalkMeasure::Tag18_10,
    &TalkMeasure::Tag18_11,
    &TalkMeasure::Tag18_12,
    &TalkMeasure::Tag18_13,
    &TalkMeasure::Tag18_14,
    &TalkMeasure::Tag18_15,
    &TalkMeasure::Tag18_16,
    &TalkMeasure::Tag18_17,
    &TalkMeasure::Tag18_18,
    &TalkMeasure::Tag18_19,
    &TalkMeasure::Tag18_20,
    &TalkMeasure::Tag18_21,
    &TalkMeasure::Tag18_22,
    &TalkMeasure::Tag18_23,
    &TalkMeasure::Tag18_24,
    &TalkMeasure::Tag18_25,
    &TalkMeasure::Tag18_26,
    &TalkMeasure::Tag18_27,
    &TalkMeasure::Tag18_28,
    &TalkMeasure::Tag18_29,
    &TalkMeasure::Tag18_30,
    &TalkMeasure::Tag18_31,
    &TalkMeasure::Tag18_32,
    &TalkMeasure::Tag18_33,
    &TalkMeasure::Tag18_34,
};

// 0x00895B24
const TalkMeasure::TagFunc TalkMeasure::s_Group20Funcs[5] = {
    &TalkMeasure::Tag20_0,
    &TalkMeasure::Tag20_1,
    &TalkMeasure::Tag20_2,
    &TalkMeasure::Tag20_3,
    &TalkMeasure::Tag20_4,
};

// 0x0089552C
const TalkMeasure::TagFunc TalkMeasure::s_GroupFuncs[21] = {
    &TalkMeasure::OnTagGroup0,
    &TalkMeasure::Tag1_,
    NULL,
    NULL,
    &TalkMeasure::OnTagGroup4,
    &TalkMeasure::OnTagGroup5,
    &TalkMeasure::Tag6_,
    &TalkMeasure::OnTagGroup7,
    &TalkMeasure::OnTagGroup8,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &TalkMeasure::OnTagGroup14,
    &TalkMeasure::OnTagGroup15,
    NULL,
    &TalkMeasure::OnTagGroup17,
    &TalkMeasure::OnTagGroup18,
    NULL,
    &TalkMeasure::OnTagGroup20,
};

// 0x00895B4C
const TalkMeasure::TagFunc TalkMeasure::s_GenericFuncs[7] = {
    NULL,
    NULL,
    &TalkMeasure::TagGeneric_2,
    &TalkMeasure::TagGeneric_3,
    &TalkMeasure::TagGeneric_4,
    NULL,
    NULL,
};
} // namespace script
