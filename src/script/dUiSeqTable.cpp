#include "script/dUiSeq.h"

namespace script {
// the handlers of the tags (in a file of their own, otherwise GCC calls them directly)
// 0x008942D8
const UiSeq::TagFunc UiSeq::s_Group0Funcs[5] = {
    &UiSeq::TagRuby,
    NULL,
    &UiSeq::TagScale,
    &UiSeq::TagSetColor,
    &UiSeq::TagPageBreak,
};

// 0x00894300
const UiSeq::TagFunc UiSeq::s_Group4Funcs[21] = {
    &UiSeq::TagYear,
    &UiSeq::TagMonthName,
    &UiSeq::TagDay,
    &UiSeq::TagHour,
    &UiSeq::TagMinute,
    &UiSeq::TagSecond,
    &UiSeq::TagNumber1,
    &UiSeq::TagNumber2,
    &UiSeq::TagNumber3,
    &UiSeq::TagNumber4,
    &UiSeq::TagNumber5,
    &UiSeq::TagNumber6,
    &UiSeq::TagNumber7,
    &UiSeq::TagReceptYear,
    &UiSeq::TagReceptMonth,
    &UiSeq::TagReceptDay,
    &UiSeq::Tag4_16,
    &UiSeq::Tag4_17,
    &UiSeq::Tag4_18,
    &UiSeq::Tag4_19,
    &UiSeq::Tag4_20,
};

// 0x008943A8
const UiSeq::TagFunc UiSeq::s_Group5Funcs[38] = {
    &UiSeq::Tag5_0,
    &UiSeq::Tag5_1,
    &UiSeq::Tag5_2,
    &UiSeq::Tag5_3,
    &UiSeq::Tag5_4,
    &UiSeq::Tag5_5,
    &UiSeq::Tag5_6,
    &UiSeq::Tag5_7,
    &UiSeq::Tag5_8,
    &UiSeq::TagWeekday,
    &UiSeq::TagImpression,
    &UiSeq::TagConstellation,
    &UiSeq::Tag5_12,
    &UiSeq::TagGenderText,
    &UiSeq::TagDrink,
    &UiSeq::TagObject,
    &UiSeq::TagSweets,
    &UiSeq::TagSports,
    &UiSeq::TagHobby,
    &UiSeq::TagFood,
    &UiSeq::TagMusic,
    &UiSeq::Tag5_21,
    &UiSeq::TagAmPm,
    &UiSeq::TagPartOfDay,
    &UiSeq::Tag5_24,
    &UiSeq::Tag5_25,
    &UiSeq::TagTownFruit,
    &UiSeq::Tag5_27,
    &UiSeq::Tag5_28,
    &UiSeq::TagBook,
    &UiSeq::TagSeason,
    &UiSeq::Tag5_31,
    &UiSeq::Tag5_32,
    &UiSeq::Tag5_33,
    &UiSeq::Tag5_34,
    &UiSeq::Tag5_35,
    &UiSeq::Tag5_36,
    &UiSeq::Tag5_37,
};

// 0x008944D8
const UiSeq::TagFunc UiSeq::s_Group14Funcs[19] = {
    &UiSeq::Tag14_0,
    &UiSeq::Tag14_1,
    &UiSeq::Tag14_2,
    &UiSeq::Tag14_3,
    &UiSeq::Tag14_4,
    &UiSeq::TagGenderP,
    &UiSeq::TagSexText,
    &UiSeq::TagGenderN,
    &UiSeq::TagGender,
    &UiSeq::TagNumberForm,
    &UiSeq::TagElision,
    &UiSeq::TagNumberText4,
    &UiSeq::TagNumberText5,
    &UiSeq::TagNumberText6,
    &UiSeq::TagPatchimForm,
    &UiSeq::TagPatchimFormRieulFree,
    &UiSeq::TagEpenthesisA,
    &UiSeq::TagEpenthesisE,
    &UiSeq::TagGenderEx,
};

// 0x00894570
const UiSeq::TagFunc UiSeq::s_Group17Funcs[11] = {
    &UiSeq::Tag17_0,
    &UiSeq::Tag17_1,
    &UiSeq::Tag17_2,
    &UiSeq::Tag17_3,
    &UiSeq::Tag17_4,
    &UiSeq::Tag17_5,
    &UiSeq::Tag17_6,
    &UiSeq::Tag17_7,
    &UiSeq::Tag17_8,
    &UiSeq::Tag17_9,
    &UiSeq::Tag17_10,
};

// 0x008945C8
const UiSeq::TagFunc UiSeq::s_Group18Funcs[35] = {
    &UiSeq::Tag18_0,
    &UiSeq::Tag18_1,
    &UiSeq::Tag18_2,
    &UiSeq::Tag18_3,
    &UiSeq::Tag18_4,
    &UiSeq::Tag18_5,
    &UiSeq::Tag18_6,
    &UiSeq::Tag18_7,
    &UiSeq::Tag18_8,
    &UiSeq::Tag18_9,
    &UiSeq::Tag18_10,
    &UiSeq::Tag18_11,
    &UiSeq::Tag18_12,
    &UiSeq::Tag18_13,
    &UiSeq::Tag18_14,
    &UiSeq::Tag18_15,
    &UiSeq::Tag18_16,
    &UiSeq::Tag18_17,
    &UiSeq::Tag18_18,
    &UiSeq::Tag18_19,
    &UiSeq::Tag18_20,
    &UiSeq::Tag18_21,
    &UiSeq::Tag18_22,
    &UiSeq::Tag18_23,
    &UiSeq::Tag18_24,
    &UiSeq::Tag18_25,
    &UiSeq::Tag18_26,
    &UiSeq::Tag18_27,
    &UiSeq::Tag18_28,
    &UiSeq::Tag18_29,
    &UiSeq::Tag18_30,
    &UiSeq::Tag18_31,
    &UiSeq::Tag18_32,
    &UiSeq::Tag18_33,
    &UiSeq::Tag18_34,
};

// 0x008946E0
const UiSeq::TagFunc UiSeq::s_Group20Funcs[5] = {
    &UiSeq::Tag20_0,
    &UiSeq::Tag20_1,
    &UiSeq::Tag20_2,
    &UiSeq::Tag20_3,
    &UiSeq::Tag20_4,
};

// 0x00894230
const UiSeq::TagFunc UiSeq::s_GroupFuncs[21] = {
    &UiSeq::OnTagGroup0,
    &UiSeq::Tag1_,
    NULL,
    NULL,
    &UiSeq::OnTagGroup4,
    &UiSeq::OnTagGroup5,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &UiSeq::OnTagGroup14,
    NULL,
    NULL,
    &UiSeq::OnTagGroup17,
    &UiSeq::OnTagGroup18,
    &UiSeq::Tag19_,
    &UiSeq::OnTagGroup20,
};

// 0x00894708
const UiSeq::TagFunc UiSeq::s_GenericFuncs[7] = {
    NULL,
    NULL,
    &UiSeq::TagGeneric_2,
    &UiSeq::TagGeneric_3,
    &UiSeq::TagGeneric_4,
    NULL,
    NULL,
};
} // namespace script
