#pragma once

#include "decomp.h"

namespace script {
// (name is ours) the id of a tag of the message texts: group in the upper, index in the lower 16 bits
constexpr u32 MakeTagId(u32 group, u32 index)
{
    return (group << 16) | index;
}

// The tags whose meaning is sure (sources in docs/script_tags.md: libgarden Tag.hpp, Slattz' tag list, the handler code
// and the string lists the words come from). The handlers of a tag are named after it (TalkSeq::TagDrink for TAG_5_DRINK);
// tags whose meaning is not sure yet keep the handler name TagN_M. Names are ours unless noted in docs/script_tags.md.
enum TagId : u32
{
    // group 0: text format (libgarden: PREDEFINED)
    TAG_0_RUBY = MakeTagId(0, 0),
    TAG_0_FONT = MakeTagId(0, 1),
    TAG_0_SCALE = MakeTagId(0, 2),
    TAG_0_SET_COLOR = MakeTagId(0, 3),
    TAG_0_PAGE_BREAK = MakeTagId(0, 4),
    // group 4: numbers, date and time
    TAG_4_YEAR = MakeTagId(4, 0),
    TAG_4_MONTH_NAME = MakeTagId(4, 1),
    TAG_4_DAY = MakeTagId(4, 2),
    TAG_4_HOUR = MakeTagId(4, 3),
    TAG_4_MINUTE = MakeTagId(4, 4),
    TAG_4_SECOND = MakeTagId(4, 5),
    TAG_4_NUMBER1 = MakeTagId(4, 6),
    TAG_4_NUMBER2 = MakeTagId(4, 7),
    TAG_4_NUMBER3 = MakeTagId(4, 8),
    TAG_4_NUMBER4 = MakeTagId(4, 9),
    TAG_4_NUMBER5 = MakeTagId(4, 10),
    TAG_4_NUMBER6 = MakeTagId(4, 11),
    TAG_4_NUMBER7 = MakeTagId(4, 12),
    TAG_4_RECEPT_YEAR = MakeTagId(4, 13),
    TAG_4_RECEPT_MONTH = MakeTagId(4, 14),
    TAG_4_RECEPT_DAY = MakeTagId(4, 15),
    // group 5: words
    TAG_5_WEEKDAY = MakeTagId(5, 9),
    TAG_5_IMPRESSION = MakeTagId(5, 10),
    TAG_5_CONSTELLATION = MakeTagId(5, 11),
    TAG_5_GENDER_TEXT = MakeTagId(5, 13),
    TAG_5_DRINK = MakeTagId(5, 14),
    TAG_5_OBJECT = MakeTagId(5, 15),
    TAG_5_SWEETS = MakeTagId(5, 16),
    TAG_5_SPORTS = MakeTagId(5, 17),
    TAG_5_HOBBY = MakeTagId(5, 18),
    TAG_5_FOOD = MakeTagId(5, 19),
    TAG_5_MUSIC = MakeTagId(5, 20),
    TAG_5_AM_PM = MakeTagId(5, 22),
    TAG_5_PART_OF_DAY = MakeTagId(5, 23),
    TAG_5_TOWN_FRUIT = MakeTagId(5, 26),
    TAG_5_BOOK = MakeTagId(5, 29),
    TAG_5_SEASON = MakeTagId(5, 30),
    // group 7: text control
    TAG_7_PAUSE = MakeTagId(7, 0),
    TAG_7_WAIT_BUTTON = MakeTagId(7, 1),
    TAG_7_INSTANT_BEGIN = MakeTagId(7, 2),
    TAG_7_INSTANT_END = MakeTagId(7, 3),
    TAG_7_NO_FAST_BEGIN = MakeTagId(7, 4),
    TAG_7_NO_FAST_END = MakeTagId(7, 5),
    TAG_7_WORD_RANGE_BEGIN = MakeTagId(7, 6),
    TAG_7_WORD_RANGE_END = MakeTagId(7, 7),
    TAG_7_AUTO_NEXT = MakeTagId(7, 8),
    TAG_7_AUTO_NEXT_ONLY = MakeTagId(7, 9),
    TAG_7_SHAKE0 = MakeTagId(7, 10),
    TAG_7_SHAKE1 = MakeTagId(7, 11),
    TAG_7_SHAKE2 = MakeTagId(7, 12),
    // group 8: choices that jump to labels
    TAG_8_CHOICE_JUMP2 = MakeTagId(8, 0),
    TAG_8_CHOICE_JUMP3 = MakeTagId(8, 1),
    TAG_8_CHOICE_JUMP4 = MakeTagId(8, 2),
    TAG_8_CHOICE_JUMP5 = MakeTagId(8, 3),
    TAG_8_CHOICE_JUMP6 = MakeTagId(8, 4),
    TAG_8_CHOICE_JUMP2_CANCEL = MakeTagId(8, 5),
    TAG_8_CHOICE_JUMP3_CANCEL = MakeTagId(8, 6),
    TAG_8_CHOICE_JUMP4_CANCEL = MakeTagId(8, 7),
    TAG_8_CHOICE_JUMP5_CANCEL = MakeTagId(8, 8),
    TAG_8_CHOICE_JUMP6_CANCEL = MakeTagId(8, 9),
    TAG_8_CHOICE_JUMP_LIST = MakeTagId(8, 10),
    // group 10: branches
    TAG_10_BRANCH_HOUR = MakeTagId(10, 6),
    TAG_10_BRANCH_PART_OF_DAY = MakeTagId(10, 7),
    TAG_10_BRANCH_WEEKDAY = MakeTagId(10, 9),
    TAG_10_BRANCH_SEASON = MakeTagId(10, 16),
    TAG_10_BRANCH_MONTH = MakeTagId(10, 17),
    TAG_10_BRANCH_OUTDOOR = MakeTagId(10, 21),
    // group 13: sound and effects
    TAG_13_SOUND = MakeTagId(13, 0),
    TAG_13_ISLAND_GAME_MUSIC = MakeTagId(13, 1),
    TAG_13_UPA_CLOSE_MUSIC = MakeTagId(13, 2),
    TAG_13_CHOICE_KIND = MakeTagId(13, 8),
    TAG_13_FX_BEGIN = MakeTagId(13, 9),
    TAG_13_FX_END = MakeTagId(13, 10),
    TAG_13_SPEECH_FILTER_BEGIN = MakeTagId(13, 11),
    TAG_13_SPEECH_FILTER_END = MakeTagId(13, 12),
    // group 14: grammar
    TAG_14_GENDER_P = MakeTagId(14, 5),
    TAG_14_SEX_TEXT = MakeTagId(14, 6),
    TAG_14_GENDER_N = MakeTagId(14, 7),
    TAG_14_GENDER = MakeTagId(14, 8),
    TAG_14_NUMBER_FORM = MakeTagId(14, 9),
    TAG_14_ELISION = MakeTagId(14, 10),
    TAG_14_NUMBER_TEXT4 = MakeTagId(14, 11),
    TAG_14_NUMBER_TEXT5 = MakeTagId(14, 12),
    TAG_14_NUMBER_TEXT6 = MakeTagId(14, 13),
    TAG_14_PATCHIM_FORM = MakeTagId(14, 14),
    TAG_14_PATCHIM_FORM_RIEUL_FREE = MakeTagId(14, 15),
    TAG_14_EPENTHESIS_A = MakeTagId(14, 16),
    TAG_14_EPENTHESIS_E = MakeTagId(14, 17),
    TAG_14_GENDER_EX = MakeTagId(14, 18),
    // group 15: choices
    TAG_15_CHOICE2 = MakeTagId(15, 0),
    TAG_15_CHOICE3 = MakeTagId(15, 1),
    TAG_15_CHOICE4 = MakeTagId(15, 2),
    TAG_15_CHOICE5 = MakeTagId(15, 3),
    TAG_15_CHOICE6 = MakeTagId(15, 4),
    TAG_15_CHOICE2_CANCEL = MakeTagId(15, 5),
    TAG_15_CHOICE3_CANCEL = MakeTagId(15, 6),
    TAG_15_CHOICE4_CANCEL = MakeTagId(15, 7),
    TAG_15_CHOICE5_CANCEL = MakeTagId(15, 8),
    TAG_15_CHOICE6_CANCEL = MakeTagId(15, 9),
    TAG_15_CHOICE_LIST = MakeTagId(15, 10),
};
} // namespace script
