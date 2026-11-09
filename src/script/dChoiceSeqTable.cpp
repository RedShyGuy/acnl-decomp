#include "script/dChoiceSeq.h"

// The tag tables of ChoiceSeq (a file of their own, else GCC folds them into the code).

namespace script {
// 0x00893930
const ChoiceSeq::TagFunc ChoiceSeq::s_Group0Funcs[5] = {
    &ChoiceSeq::TagRuby,
    NULL,
    &ChoiceSeq::TagScale,
    &ChoiceSeq::TagSetColor,
    NULL,
};

// 0x00893958
const ChoiceSeq::TagFunc ChoiceSeq::s_Group4Funcs[21] = {
    &ChoiceSeq::TagYear,
    &ChoiceSeq::TagMonthName,
    &ChoiceSeq::TagDay,
    &ChoiceSeq::TagHour,
    &ChoiceSeq::TagMinute,
    &ChoiceSeq::TagSecond,
    &ChoiceSeq::TagNumber1,
    &ChoiceSeq::TagNumber2,
    &ChoiceSeq::TagNumber3,
    &ChoiceSeq::TagNumber4,
    &ChoiceSeq::TagNumber5,
    &ChoiceSeq::TagNumber6,
    &ChoiceSeq::TagNumber7,
    &ChoiceSeq::TagReceptYear,
    &ChoiceSeq::TagReceptMonth,
    &ChoiceSeq::TagReceptDay,
    &ChoiceSeq::Tag4_16,
    &ChoiceSeq::Tag4_17,
    &ChoiceSeq::Tag4_18,
    &ChoiceSeq::Tag4_19,
    &ChoiceSeq::Tag4_20,
};

// 0x00893A00
const ChoiceSeq::TagFunc ChoiceSeq::s_Group5Funcs[38] = {
    &ChoiceSeq::Tag5_0,
    &ChoiceSeq::Tag5_1,
    &ChoiceSeq::Tag5_2,
    &ChoiceSeq::Tag5_3,
    &ChoiceSeq::Tag5_4,
    &ChoiceSeq::Tag5_5,
    &ChoiceSeq::Tag5_6,
    &ChoiceSeq::Tag5_7,
    &ChoiceSeq::Tag5_8,
    &ChoiceSeq::TagWeekday,
    &ChoiceSeq::TagImpression,
    &ChoiceSeq::TagConstellation,
    &ChoiceSeq::Tag5_12,
    &ChoiceSeq::TagGenderText,
    &ChoiceSeq::TagDrink,
    &ChoiceSeq::TagObject,
    &ChoiceSeq::TagSweets,
    &ChoiceSeq::TagSports,
    &ChoiceSeq::TagHobby,
    &ChoiceSeq::TagFood,
    &ChoiceSeq::TagMusic,
    &ChoiceSeq::Tag5_21,
    &ChoiceSeq::TagAmPm,
    &ChoiceSeq::TagPartOfDay,
    &ChoiceSeq::Tag5_24,
    &ChoiceSeq::Tag5_25,
    &ChoiceSeq::TagTownFruit,
    &ChoiceSeq::Tag5_27,
    &ChoiceSeq::Tag5_28,
    &ChoiceSeq::TagBook,
    &ChoiceSeq::TagSeason,
    &ChoiceSeq::Tag5_31,
    &ChoiceSeq::Tag5_32,
    &ChoiceSeq::Tag5_33,
    &ChoiceSeq::Tag5_34,
    &ChoiceSeq::Tag5_35,
    &ChoiceSeq::Tag5_36,
    &ChoiceSeq::Tag5_37,
};

// 0x00893B30
const ChoiceSeq::TagFunc ChoiceSeq::s_Group14Funcs[19] = {
    &ChoiceSeq::Tag14_0,
    &ChoiceSeq::Tag14_1,
    &ChoiceSeq::Tag14_2,
    &ChoiceSeq::Tag14_3,
    &ChoiceSeq::Tag14_4,
    &ChoiceSeq::TagGenderP,
    &ChoiceSeq::TagSexText,
    &ChoiceSeq::TagGenderN,
    &ChoiceSeq::TagGender,
    &ChoiceSeq::TagNumberForm,
    &ChoiceSeq::TagElision,
    &ChoiceSeq::TagNumberText4,
    &ChoiceSeq::TagNumberText5,
    &ChoiceSeq::TagNumberText6,
    &ChoiceSeq::TagPatchimForm,
    &ChoiceSeq::TagPatchimFormRieulFree,
    &ChoiceSeq::TagEpenthesisA,
    &ChoiceSeq::TagEpenthesisE,
    &ChoiceSeq::TagGenderEx,
};

// 0x00893BC8
const ChoiceSeq::TagFunc ChoiceSeq::s_Group17Funcs[11] = {
    &ChoiceSeq::Tag17_0,
    &ChoiceSeq::Tag17_1,
    &ChoiceSeq::Tag17_2,
    &ChoiceSeq::Tag17_3,
    &ChoiceSeq::Tag17_4,
    &ChoiceSeq::Tag17_5,
    &ChoiceSeq::Tag17_6,
    &ChoiceSeq::Tag17_7,
    &ChoiceSeq::Tag17_8,
    &ChoiceSeq::Tag17_9,
    &ChoiceSeq::Tag17_10,
};

// 0x00893C20
const ChoiceSeq::TagFunc ChoiceSeq::s_Group18Funcs[35] = {
    &ChoiceSeq::Tag18_0,
    &ChoiceSeq::Tag18_1,
    &ChoiceSeq::Tag18_2,
    &ChoiceSeq::Tag18_3,
    &ChoiceSeq::Tag18_4,
    &ChoiceSeq::Tag18_5,
    &ChoiceSeq::Tag18_6,
    &ChoiceSeq::Tag18_7,
    &ChoiceSeq::Tag18_8,
    &ChoiceSeq::Tag18_9,
    &ChoiceSeq::Tag18_10,
    &ChoiceSeq::Tag18_11,
    &ChoiceSeq::Tag18_12,
    &ChoiceSeq::Tag18_13,
    &ChoiceSeq::Tag18_14,
    &ChoiceSeq::Tag18_15,
    &ChoiceSeq::Tag18_16,
    &ChoiceSeq::Tag18_17,
    &ChoiceSeq::Tag18_18,
    &ChoiceSeq::Tag18_19,
    &ChoiceSeq::Tag18_20,
    &ChoiceSeq::Tag18_21,
    &ChoiceSeq::Tag18_22,
    &ChoiceSeq::Tag18_23,
    &ChoiceSeq::Tag18_24,
    &ChoiceSeq::Tag18_25,
    &ChoiceSeq::Tag18_26,
    &ChoiceSeq::Tag18_27,
    &ChoiceSeq::Tag18_28,
    &ChoiceSeq::Tag18_29,
    &ChoiceSeq::Tag18_30,
    &ChoiceSeq::Tag18_31,
    &ChoiceSeq::Tag18_32,
    &ChoiceSeq::Tag18_33,
    &ChoiceSeq::Tag18_34,
};

// 0x00893D38
const ChoiceSeq::TagFunc ChoiceSeq::s_Group20Funcs[5] = {
    &ChoiceSeq::Tag20_0,
    &ChoiceSeq::Tag20_1,
    &ChoiceSeq::Tag20_2,
    &ChoiceSeq::Tag20_3,
    &ChoiceSeq::Tag20_4,
};

// 0x00893888
const ChoiceSeq::TagFunc ChoiceSeq::s_GroupFuncs[21] = {
    &ChoiceSeq::OnTagGroup0,
    &ChoiceSeq::Tag1_,
    &ChoiceSeq::Tag2_,
    &ChoiceSeq::Tag3_,
    &ChoiceSeq::OnTagGroup4,
    &ChoiceSeq::OnTagGroup5,
    &ChoiceSeq::Tag6_,
    &ChoiceSeq::Tag7_,
    &ChoiceSeq::Tag8_,
    &ChoiceSeq::Tag9_,
    &ChoiceSeq::Tag10_,
    &ChoiceSeq::Tag11_,
    &ChoiceSeq::Tag12_,
    &ChoiceSeq::Tag13_,
    &ChoiceSeq::OnTagGroup14,
    &ChoiceSeq::Tag15_,
    &ChoiceSeq::Tag16_,
    &ChoiceSeq::OnTagGroup17,
    &ChoiceSeq::OnTagGroup18,
    &ChoiceSeq::Tag19_,
    &ChoiceSeq::OnTagGroup20,
};

// 0x00893D60
const ChoiceSeq::TagFunc ChoiceSeq::s_GenericFuncs[7] = {
    NULL,
    NULL,
    &ChoiceSeq::TagGeneric_2,
    &ChoiceSeq::TagGeneric_3,
    &ChoiceSeq::TagGeneric_4,
    NULL,
    NULL,
};
} // namespace script
