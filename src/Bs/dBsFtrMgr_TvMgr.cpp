#include "Bs/dBsFtrMgr_TvMgr.h"

#include <cmath>
#include <cstring>
#include "Bs/dBsFtrMgrExtern.h"
#include "Other/dHeapInit.h"
#include "Sound/dSoundMgr.h"
#include "Sound/dSoundTvBase.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "nw/anim/res/anim_ResAnimCurve.h"
#include "nw/ut/ut_Offset.h"
#include "sead/seadExpHeap.h"
#include "sead/seadFixedSafeString.h"
#include "time/time_Api.h"

// (the globals of the file of the TV programs)
// 0x0095D400
FtrTvMgr* FtrTvMgr::s_pInstance;
// 0x0095D404
f32 FtrTvMgr::s_Frame;
// the weather programs by the weather value of Func630B28 (two letters per file; their meaning is not checked)
// 0x0095D408
const char* FtrTvMgr::s_WeatherPrograms[WEATHER_PROGRAM_NUM] = {
    "/Ftr/TV/tv_program_ff.bcres", "/Ftr/TV/tv_program_cc.bcres", "/Ftr/TV/tv_program_rr.bcres",
    "/Ftr/TV/tv_program_fc.bcres", "/Ftr/TV/tv_program_cf.bcres", "/Ftr/TV/tv_program_fr.bcres",
    "/Ftr/TV/tv_program_cr.bcres", "/Ftr/TV/tv_program_rc.bcres", "/Ftr/TV/tv_program_ss.bcres",
    "/Ftr/TV/tv_program_cs.bcres", "/Ftr/TV/tv_program_sc.bcres",
};

// the schedule of the programs (the program that runs at a time of a day of the week)
// 0x008909B4
extern const FtrTvMgr::ScheduleEntry s_TvScheduleSunday[66] = {
    {FtrTvMgr::PROGRAM_COLOR, 0, 0},
    {FtrTvMgr::PROGRAM_STORM, 1, 0},
    {FtrTvMgr::PROGRAM_UFO_CM, 3, 33},
    {FtrTvMgr::PROGRAM_STORM, 3, 34},
    {FtrTvMgr::PROGRAM_COLOR, 5, 0},
    {FtrTvMgr::PROGRAM_NEWS, 6, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 6, 15},
    {FtrTvMgr::PROGRAM_EXERCISE, 6, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 6, 59},
    {FtrTvMgr::PROGRAM_HERO, 7, 0},
    {FtrTvMgr::PROGRAM_SNOW_CM, 7, 29},
    {FtrTvMgr::PROGRAM_HERO, 7, 30},
    {FtrTvMgr::PROGRAM_APPLE_CM, 7, 59},
    {FtrTvMgr::PROGRAM_ANIME, 8, 0},
    {FtrTvMgr::PROGRAM_SING_CM, 8, 29},
    {FtrTvMgr::PROGRAM_ANIME, 8, 30},
    {FtrTvMgr::PROGRAM_SNOW_CM, 8, 59},
    {FtrTvMgr::PROGRAM_QUIZ, 9, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 9, 29},
    {FtrTvMgr::PROGRAM_QUIZ, 9, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 9, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 10, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 10, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 10, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 10, 59},
    {FtrTvMgr::PROGRAM_NEWS, 11, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 11, 45},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 11, 59},
    {FtrTvMgr::PROGRAM_TALK, 12, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 12, 29},
    {FtrTvMgr::PROGRAM_TALK, 12, 30},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 12, 59},
    {FtrTvMgr::PROGRAM_SOCCER, 13, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 13, 29},
    {FtrTvMgr::PROGRAM_SOCCER, 13, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 14, 59},
    {FtrTvMgr::PROGRAM_QUIZ, 15, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 15, 29},
    {FtrTvMgr::PROGRAM_QUIZ, 15, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 15, 59},
    {FtrTvMgr::PROGRAM_COOKING, 16, 0},
    {FtrTvMgr::PROGRAM_APPLE_CM, 16, 29},
    {FtrTvMgr::PROGRAM_COOKING, 16, 30},
    {FtrTvMgr::PROGRAM_SING_CM, 16, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 17, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 17, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 17, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 17, 59},
    {FtrTvMgr::PROGRAM_ANIME, 18, 0},
    {FtrTvMgr::PROGRAM_SING_CM, 18, 29},
    {FtrTvMgr::PROGRAM_ANIME, 18, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 18, 59},
    {FtrTvMgr::PROGRAM_SOCCER, 19, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 19, 50},
    {FtrTvMgr::PROGRAM_SOCCER, 19, 51},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 20, 57},
    {FtrTvMgr::PROGRAM_JUICE_CM, 20, 58},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 20, 59},
    {FtrTvMgr::PROGRAM_MOVIE, 21, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 21, 53},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 21, 54},
    {FtrTvMgr::PROGRAM_MOVIE, 21, 55},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 22, 44},
    {FtrTvMgr::PROGRAM_WEATHER, 22, 45},
    {FtrTvMgr::PROGRAM_NEWS, 23, 0},
    {FtrTvMgr::PROGRAM_SNOW_CM, 23, 55},
};
// 0x00890ABC
extern const FtrTvMgr::ScheduleEntry s_TvScheduleMonday[79] = {
    {FtrTvMgr::PROGRAM_COLOR, 0, 0},
    {FtrTvMgr::PROGRAM_STORM, 1, 0},
    {FtrTvMgr::PROGRAM_UFO_CM, 3, 33},
    {FtrTvMgr::PROGRAM_STORM, 3, 34},
    {FtrTvMgr::PROGRAM_COLOR, 5, 0},
    {FtrTvMgr::PROGRAM_SING_CM, 5, 55},
    {FtrTvMgr::PROGRAM_NEWS, 6, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 6, 15},
    {FtrTvMgr::PROGRAM_EXERCISE, 6, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 6, 59},
    {FtrTvMgr::PROGRAM_NEWS, 7, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 7, 45},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 7, 59},
    {FtrTvMgr::PROGRAM_DRAMA, 8, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 8, 29},
    {FtrTvMgr::PROGRAM_TRAVEL, 8, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 9, 18},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 9, 19},
    {FtrTvMgr::PROGRAM_TRAVEL, 9, 20},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 9, 59},
    {FtrTvMgr::PROGRAM_HERO, 10, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 10, 29},
    {FtrTvMgr::PROGRAM_HERO, 10, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 10, 59},
    {FtrTvMgr::PROGRAM_COOKING, 11, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 11, 29},
    {FtrTvMgr::PROGRAM_NEWS, 11, 30},
    {FtrTvMgr::PROGRAM_WEATHER, 11, 45},
    {FtrTvMgr::PROGRAM_APPLE_CM, 11, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 12, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 12, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 12, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 12, 59},
    {FtrTvMgr::PROGRAM_DRAMA, 13, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 13, 28},
    {FtrTvMgr::PROGRAM_JUICE_CM, 13, 29},
    {FtrTvMgr::PROGRAM_TALK, 13, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 13, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 14, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 14, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 14, 30},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 14, 59},
    {FtrTvMgr::PROGRAM_EXERCISE, 15, 0},
    {FtrTvMgr::PROGRAM_APPLE_CM, 15, 29},
    {FtrTvMgr::PROGRAM_EXERCISE, 15, 30},
    {FtrTvMgr::PROGRAM_SING_CM, 15, 57},
    {FtrTvMgr::PROGRAM_ANIME, 16, 0},
    {FtrTvMgr::PROGRAM_SNOW_CM, 16, 29},
    {FtrTvMgr::PROGRAM_ANIME, 16, 30},
    {FtrTvMgr::PROGRAM_APPLE_CM, 16, 57},
    {FtrTvMgr::PROGRAM_VARIETY, 17, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 17, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 17, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 17, 58},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 17, 59},
    {FtrTvMgr::PROGRAM_NEWS, 18, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 18, 45},
    {FtrTvMgr::PROGRAM_JUICE_CM, 18, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 19, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 19, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 19, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 19, 59},
    {FtrTvMgr::PROGRAM_QUIZ, 20, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 20, 29},
    {FtrTvMgr::PROGRAM_QUIZ, 20, 30},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 20, 57},
    {FtrTvMgr::PROGRAM_JUICE_CM, 20, 58},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 20, 59},
    {FtrTvMgr::PROGRAM_DRAMA, 21, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 21, 29},
    {FtrTvMgr::PROGRAM_DRAMA, 21, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 21, 59},
    {FtrTvMgr::PROGRAM_NEWS, 22, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 22, 45},
    {FtrTvMgr::PROGRAM_APPLE_CM, 22, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 23, 0},
    {FtrTvMgr::PROGRAM_NEWS, 23, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 23, 30},
    {FtrTvMgr::PROGRAM_SING_CM, 23, 55},
};
// 0x00890BF8
extern const FtrTvMgr::ScheduleEntry s_TvScheduleTuesday[75] = {
    {FtrTvMgr::PROGRAM_VARIETY, 0, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 0, 57},
    {FtrTvMgr::PROGRAM_MOVIE, 1, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 2, 45},
    {FtrTvMgr::PROGRAM_COLOR, 3, 0},
    {FtrTvMgr::PROGRAM_STORM, 4, 0},
    {FtrTvMgr::PROGRAM_COLOR, 5, 0},
    {FtrTvMgr::PROGRAM_APPLE_CM, 5, 55},
    {FtrTvMgr::PROGRAM_NEWS, 6, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 6, 15},
    {FtrTvMgr::PROGRAM_EXERCISE, 6, 30},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 6, 59},
    {FtrTvMgr::PROGRAM_NEWS, 7, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 7, 45},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 7, 59},
    {FtrTvMgr::PROGRAM_DRAMA, 8, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 8, 29},
    {FtrTvMgr::PROGRAM_TRAVEL, 8, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 9, 18},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 9, 19},
    {FtrTvMgr::PROGRAM_TRAVEL, 9, 20},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 9, 59},
    {FtrTvMgr::PROGRAM_ANIME, 10, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 10, 29},
    {FtrTvMgr::PROGRAM_ANIME, 10, 30},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 10, 59},
    {FtrTvMgr::PROGRAM_COOKING, 11, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 11, 29},
    {FtrTvMgr::PROGRAM_NEWS, 11, 30},
    {FtrTvMgr::PROGRAM_WEATHER, 11, 45},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 11, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 12, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 12, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 12, 30},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 12, 59},
    {FtrTvMgr::PROGRAM_DRAMA, 13, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 13, 29},
    {FtrTvMgr::PROGRAM_TALK, 13, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 13, 59},
    {FtrTvMgr::PROGRAM_TRAVEL, 14, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 14, 29},
    {FtrTvMgr::PROGRAM_TRAVEL, 14, 30},
    {FtrTvMgr::PROGRAM_SING_CM, 14, 58},
    {FtrTvMgr::PROGRAM_EXERCISE, 15, 0},
    {FtrTvMgr::PROGRAM_APPLE_CM, 15, 29},
    {FtrTvMgr::PROGRAM_EXERCISE, 15, 30},
    {FtrTvMgr::PROGRAM_SING_CM, 15, 58},
    {FtrTvMgr::PROGRAM_HERO, 16, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 16, 29},
    {FtrTvMgr::PROGRAM_HERO, 16, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 16, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 17, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 17, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 17, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 17, 59},
    {FtrTvMgr::PROGRAM_NEWS, 18, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 18, 45},
    {FtrTvMgr::PROGRAM_JUICE_CM, 18, 59},
    {FtrTvMgr::PROGRAM_SOCCER, 19, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 19, 55},
    {FtrTvMgr::PROGRAM_SOCCER, 19, 56},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 20, 57},
    {FtrTvMgr::PROGRAM_JUICE_CM, 20, 58},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 20, 59},
    {FtrTvMgr::PROGRAM_TALK, 21, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 21, 29},
    {FtrTvMgr::PROGRAM_TALK, 21, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 21, 59},
    {FtrTvMgr::PROGRAM_NEWS, 22, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 22, 45},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 22, 59},
    {FtrTvMgr::PROGRAM_TRAVEL, 23, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 23, 29},
    {FtrTvMgr::PROGRAM_TRAVEL, 23, 30},
    {FtrTvMgr::PROGRAM_APPLE_CM, 23, 55},
};
// 0x00890D24
extern const FtrTvMgr::ScheduleEntry s_TvScheduleWednesday[79] = {
    {FtrTvMgr::PROGRAM_TRAVEL, 0, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 0, 57},
    {FtrTvMgr::PROGRAM_SONG, 1, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 2, 45},
    {FtrTvMgr::PROGRAM_COLOR, 3, 0},
    {FtrTvMgr::PROGRAM_STORM, 4, 0},
    {FtrTvMgr::PROGRAM_COLOR, 5, 0},
    {FtrTvMgr::PROGRAM_SNOW_CM, 5, 55},
    {FtrTvMgr::PROGRAM_NEWS, 6, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 6, 15},
    {FtrTvMgr::PROGRAM_EXERCISE, 6, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 6, 59},
    {FtrTvMgr::PROGRAM_NEWS, 7, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 7, 45},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 7, 59},
    {FtrTvMgr::PROGRAM_DRAMA, 8, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 8, 29},
    {FtrTvMgr::PROGRAM_TRAVEL, 8, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 9, 18},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 9, 19},
    {FtrTvMgr::PROGRAM_TRAVEL, 9, 20},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 9, 59},
    {FtrTvMgr::PROGRAM_HERO, 10, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 10, 29},
    {FtrTvMgr::PROGRAM_HERO, 10, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 10, 59},
    {FtrTvMgr::PROGRAM_COOKING, 11, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 11, 29},
    {FtrTvMgr::PROGRAM_NEWS, 11, 30},
    {FtrTvMgr::PROGRAM_WEATHER, 11, 45},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 11, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 12, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 12, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 12, 30},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 12, 59},
    {FtrTvMgr::PROGRAM_DRAMA, 13, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 13, 29},
    {FtrTvMgr::PROGRAM_TALK, 13, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 13, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 14, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 14, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 14, 30},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 14, 59},
    {FtrTvMgr::PROGRAM_EXERCISE, 15, 0},
    {FtrTvMgr::PROGRAM_APPLE_CM, 15, 29},
    {FtrTvMgr::PROGRAM_EXERCISE, 15, 30},
    {FtrTvMgr::PROGRAM_SING_CM, 15, 59},
    {FtrTvMgr::PROGRAM_ANIME, 16, 0},
    {FtrTvMgr::PROGRAM_SNOW_CM, 16, 29},
    {FtrTvMgr::PROGRAM_ANIME, 16, 30},
    {FtrTvMgr::PROGRAM_SING_CM, 16, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 17, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 17, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 17, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 17, 59},
    {FtrTvMgr::PROGRAM_NEWS, 18, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 18, 45},
    {FtrTvMgr::PROGRAM_JUICE_CM, 18, 59},
    {FtrTvMgr::PROGRAM_COOKING, 19, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 19, 29},
    {FtrTvMgr::PROGRAM_COOKING, 19, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 19, 59},
    {FtrTvMgr::PROGRAM_SONG, 20, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 20, 29},
    {FtrTvMgr::PROGRAM_SONG, 20, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 20, 57},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 20, 58},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 20, 59},
    {FtrTvMgr::PROGRAM_DRAMA, 21, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 21, 29},
    {FtrTvMgr::PROGRAM_DRAMA, 21, 30},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 21, 59},
    {FtrTvMgr::PROGRAM_NEWS, 22, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 22, 45},
    {FtrTvMgr::PROGRAM_JUICE_CM, 22, 59},
    {FtrTvMgr::PROGRAM_QUIZ, 23, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 23, 29},
    {FtrTvMgr::PROGRAM_QUIZ, 23, 30},
    {FtrTvMgr::PROGRAM_SNOW_CM, 23, 55},
};
// 0x00890E60
extern const FtrTvMgr::ScheduleEntry s_TvScheduleThursday[79] = {
    {FtrTvMgr::PROGRAM_VARIETY, 0, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 0, 57},
    {FtrTvMgr::PROGRAM_SOCCER, 1, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 2, 45},
    {FtrTvMgr::PROGRAM_COLOR, 3, 0},
    {FtrTvMgr::PROGRAM_STORM, 4, 0},
    {FtrTvMgr::PROGRAM_COLOR, 5, 0},
    {FtrTvMgr::PROGRAM_SING_CM, 5, 55},
    {FtrTvMgr::PROGRAM_NEWS, 6, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 6, 15},
    {FtrTvMgr::PROGRAM_EXERCISE, 6, 30},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 6, 59},
    {FtrTvMgr::PROGRAM_NEWS, 7, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 7, 45},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 7, 59},
    {FtrTvMgr::PROGRAM_DRAMA, 8, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 8, 29},
    {FtrTvMgr::PROGRAM_TRAVEL, 8, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 9, 18},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 9, 19},
    {FtrTvMgr::PROGRAM_TRAVEL, 9, 20},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 9, 59},
    {FtrTvMgr::PROGRAM_HERO, 10, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 10, 29},
    {FtrTvMgr::PROGRAM_HERO, 10, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 10, 59},
    {FtrTvMgr::PROGRAM_COOKING, 11, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 11, 29},
    {FtrTvMgr::PROGRAM_NEWS, 11, 30},
    {FtrTvMgr::PROGRAM_WEATHER, 11, 45},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 11, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 12, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 12, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 12, 30},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 12, 59},
    {FtrTvMgr::PROGRAM_DRAMA, 13, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 13, 29},
    {FtrTvMgr::PROGRAM_TALK, 13, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 13, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 14, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 14, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 14, 30},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 14, 59},
    {FtrTvMgr::PROGRAM_EXERCISE, 15, 0},
    {FtrTvMgr::PROGRAM_APPLE_CM, 15, 29},
    {FtrTvMgr::PROGRAM_EXERCISE, 15, 30},
    {FtrTvMgr::PROGRAM_SING_CM, 15, 59},
    {FtrTvMgr::PROGRAM_HERO, 16, 0},
    {FtrTvMgr::PROGRAM_SING_CM, 16, 29},
    {FtrTvMgr::PROGRAM_HERO, 16, 30},
    {FtrTvMgr::PROGRAM_SNOW_CM, 16, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 17, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 17, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 17, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 17, 59},
    {FtrTvMgr::PROGRAM_NEWS, 18, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 18, 45},
    {FtrTvMgr::PROGRAM_JUICE_CM, 18, 59},
    {FtrTvMgr::PROGRAM_ANIME, 19, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 19, 29},
    {FtrTvMgr::PROGRAM_ANIME, 19, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 19, 59},
    {FtrTvMgr::PROGRAM_TRAVEL, 20, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 20, 29},
    {FtrTvMgr::PROGRAM_TRAVEL, 20, 30},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 20, 57},
    {FtrTvMgr::PROGRAM_JUICE_CM, 20, 58},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 20, 59},
    {FtrTvMgr::PROGRAM_DRAMA, 21, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 21, 29},
    {FtrTvMgr::PROGRAM_DRAMA, 21, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 21, 59},
    {FtrTvMgr::PROGRAM_NEWS, 22, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 22, 45},
    {FtrTvMgr::PROGRAM_JUICE_CM, 22, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 23, 0},
    {FtrTvMgr::PROGRAM_NEWS, 23, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 23, 30},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 23, 55},
};
// 0x00890F9C
extern const FtrTvMgr::ScheduleEntry s_TvScheduleFriday[75] = {
    {FtrTvMgr::PROGRAM_ANIME, 0, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 0, 57},
    {FtrTvMgr::PROGRAM_TRAVEL, 1, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 2, 45},
    {FtrTvMgr::PROGRAM_COLOR, 3, 0},
    {FtrTvMgr::PROGRAM_STORM, 4, 0},
    {FtrTvMgr::PROGRAM_COLOR, 5, 0},
    {FtrTvMgr::PROGRAM_APPLE_CM, 5, 55},
    {FtrTvMgr::PROGRAM_NEWS, 6, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 6, 15},
    {FtrTvMgr::PROGRAM_EXERCISE, 6, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 6, 59},
    {FtrTvMgr::PROGRAM_NEWS, 7, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 7, 45},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 7, 59},
    {FtrTvMgr::PROGRAM_DRAMA, 8, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 8, 29},
    {FtrTvMgr::PROGRAM_TRAVEL, 8, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 9, 18},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 9, 19},
    {FtrTvMgr::PROGRAM_TRAVEL, 9, 20},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 9, 59},
    {FtrTvMgr::PROGRAM_ANIME, 10, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 10, 29},
    {FtrTvMgr::PROGRAM_ANIME, 10, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 10, 59},
    {FtrTvMgr::PROGRAM_COOKING, 11, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 11, 29},
    {FtrTvMgr::PROGRAM_NEWS, 11, 30},
    {FtrTvMgr::PROGRAM_WEATHER, 11, 45},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 11, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 12, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 12, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 12, 30},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 12, 59},
    {FtrTvMgr::PROGRAM_DRAMA, 13, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 13, 29},
    {FtrTvMgr::PROGRAM_DRAMA, 13, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 13, 59},
    {FtrTvMgr::PROGRAM_TALK, 14, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 14, 29},
    {FtrTvMgr::PROGRAM_TALK, 14, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 14, 59},
    {FtrTvMgr::PROGRAM_EXERCISE, 15, 0},
    {FtrTvMgr::PROGRAM_SING_CM, 15, 29},
    {FtrTvMgr::PROGRAM_EXERCISE, 15, 30},
    {FtrTvMgr::PROGRAM_APPLE_CM, 15, 59},
    {FtrTvMgr::PROGRAM_HERO, 16, 0},
    {FtrTvMgr::PROGRAM_SING_CM, 16, 29},
    {FtrTvMgr::PROGRAM_HERO, 16, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 16, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 17, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 17, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 17, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 17, 59},
    {FtrTvMgr::PROGRAM_NEWS, 18, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 18, 45},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 18, 59},
    {FtrTvMgr::PROGRAM_SOCCER, 19, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 19, 50},
    {FtrTvMgr::PROGRAM_SOCCER, 19, 51},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 20, 57},
    {FtrTvMgr::PROGRAM_JUICE_CM, 20, 58},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 20, 59},
    {FtrTvMgr::PROGRAM_TALK, 21, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 21, 29},
    {FtrTvMgr::PROGRAM_TALK, 21, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 21, 59},
    {FtrTvMgr::PROGRAM_NEWS, 22, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 22, 45},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 22, 59},
    {FtrTvMgr::PROGRAM_QUIZ, 23, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 23, 29},
    {FtrTvMgr::PROGRAM_QUIZ, 23, 30},
    {FtrTvMgr::PROGRAM_APPLE_CM, 23, 55},
};
// 0x008910C8
extern const FtrTvMgr::ScheduleEntry s_TvScheduleSaturday[66] = {
    {FtrTvMgr::PROGRAM_SONG, 0, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 0, 57},
    {FtrTvMgr::PROGRAM_MOVIE, 1, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 2, 45},
    {FtrTvMgr::PROGRAM_COLOR, 3, 0},
    {FtrTvMgr::PROGRAM_STORM, 4, 0},
    {FtrTvMgr::PROGRAM_COLOR, 5, 0},
    {FtrTvMgr::PROGRAM_SNOW_CM, 5, 55},
    {FtrTvMgr::PROGRAM_NEWS, 6, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 6, 15},
    {FtrTvMgr::PROGRAM_EXERCISE, 6, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 6, 59},
    {FtrTvMgr::PROGRAM_NEWS, 7, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 7, 45},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 7, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 8, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 8, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 8, 30},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 8, 59},
    {FtrTvMgr::PROGRAM_TRAVEL, 9, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 9, 29},
    {FtrTvMgr::PROGRAM_TRAVEL, 9, 30},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 9, 59},
    {FtrTvMgr::PROGRAM_HERO, 10, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 10, 29},
    {FtrTvMgr::PROGRAM_HERO, 10, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 10, 59},
    {FtrTvMgr::PROGRAM_NEWS, 11, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 11, 45},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 11, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 12, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 12, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 12, 30},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 13, 59},
    {FtrTvMgr::PROGRAM_TRAVEL, 14, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 14, 29},
    {FtrTvMgr::PROGRAM_TRAVEL, 14, 30},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 14, 59},
    {FtrTvMgr::PROGRAM_SOCCER, 15, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 15, 50},
    {FtrTvMgr::PROGRAM_SOCCER, 15, 51},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 16, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 17, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 17, 29},
    {FtrTvMgr::PROGRAM_VARIETY, 17, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 17, 59},
    {FtrTvMgr::PROGRAM_NEWS, 18, 0},
    {FtrTvMgr::PROGRAM_WEATHER, 18, 45},
    {FtrTvMgr::PROGRAM_JUICE_CM, 18, 59},
    {FtrTvMgr::PROGRAM_QUIZ, 19, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 19, 29},
    {FtrTvMgr::PROGRAM_QUIZ, 19, 30},
    {FtrTvMgr::PROGRAM_JUICE_CM, 19, 59},
    {FtrTvMgr::PROGRAM_VARIETY, 20, 0},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 20, 57},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 20, 58},
    {FtrTvMgr::PROGRAM_JUICE_CM, 20, 59},
    {FtrTvMgr::PROGRAM_MOVIE, 21, 0},
    {FtrTvMgr::PROGRAM_JUICE_CM, 21, 54},
    {FtrTvMgr::PROGRAM_MOVIE, 21, 55},
    {FtrTvMgr::PROGRAM_LAUNDRY_CM, 22, 44},
    {FtrTvMgr::PROGRAM_WEATHER, 22, 45},
    {FtrTvMgr::PROGRAM_NEWS, 23, 0},
    {FtrTvMgr::PROGRAM_SHOPPING_CM, 23, 29},
    {FtrTvMgr::PROGRAM_COOKING, 23, 30},
    {FtrTvMgr::PROGRAM_SING_CM, 23, 55},
};
// the schedule of each day of the week (Sunday first)
// 0x008911D0
extern const FtrTvMgr::Schedule s_TvSchedules[7] = {
    {s_TvScheduleSunday, 66},
    {s_TvScheduleMonday, 79},
    {s_TvScheduleTuesday, 75},
    {s_TvScheduleWednesday, 79},
    {s_TvScheduleThursday, 79},
    {s_TvScheduleFriday, 75},
    {s_TvScheduleSaturday, 66},
};

namespace {
// the sizes of the heaps
const u32 COMMON_HEAP_SIZE = 0x1800;
const u32 NEXT_HEAP_SIZE = 0x5000;
const u32 AUDIO_HEAP_SIZE = 0xC800;
// the program of the weather in SoundTvHandle::Create
const u32 SOUND_WEATHER = 22;
// the size of the pixels of the screen texture
const u32 SCREEN_IMAGE_SIZE = 2048;
// the type bits of an image texture (ResImageTexture)
const u32 TYPE_IMAGE_TEXTURE = 0x20000011;

// the target of a self-relative offset at offset bytes into data (NULL: none)
inline const u8* GetTarget(const void* data, u32 offset)
{
    return static_cast<const u8*>(
        reinterpret_cast<const nw::ut::Offset*>(static_cast<const u8*>(data) + offset)->ToPtr());
}

// the image texture of the textures of a graphics file at index (NULL: none or not an image texture)
inline const u8* GetImageTexture(const nw::gfx::res::ResGraphicsFile& file, s32 index)
{
    const u8* texture = static_cast<const u8*>(file.ptr()->textures.GetData(index));
    if (texture != NULL && (TYPE_IMAGE_TEXTURE & ~*reinterpret_cast<const u32*>(texture)) != 0) {
        texture = NULL;
    }
    return texture;
}

// the pixels of an image texture (the image at 0x38, its data at 0xC)
inline u8* GetImagePixels(const u8* texture)
{
    return const_cast<u8*>(GetTarget(GetTarget(texture, 0x38), 0xC));
}

// the program that runs now by the schedule of the day (from the last entry back; 0 if none)
inline s16 GetScheduledProgram()
{
    const nn::fnd::DateTimeParameters* date = time::GetCurrentDate();
    const s8 hour = date->hour;
    const s8 minute = date->minute;
    const FtrTvMgr::Schedule& schedule = s_TvSchedules[static_cast<u8>(date->dayOfWeek)];
    for (s32 i = schedule.num - 1; i >= 0; i--) {
        const FtrTvMgr::ScheduleEntry& entry = schedule.entries[i];
        if (entry.hour < hour || (entry.hour == hour && entry.minute <= minute)) {
            if (entry.program < 0) {
                return 0;
            }
            return entry.program;
        }
    }
    return 0;
}

// the sound of a program in the audio heap of index (the heap allocator serves as the create argument)
inline SoundTvHandle* CreateSound(s16 program, ssys::ma::HeapAllocator* allocator)
{
    const u32 index = (program == FtrTvMgr::PROGRAM_WEATHER) ? SOUND_WEATHER : static_cast<u8>(program);
    return SoundTvHandle::Create(index, reinterpret_cast<const SoundTvHandle::CreateArg*>(allocator));
}
} // namespace

// 0x00343CE0
void FtrTvMgr::UpdateScreenTexture()
{
    FtrTvMgr* mgr = s_pInstance;
    if (mgr == NULL) {
        return;
    }
    if (mgr->m_ProgramLoader.m_SetupResult == g3d::ResourceSet::SETUP_RESULT_NONE ||
        !mgr->m_ProgramLoader.m_GraphicsFile.IsValid()) {
        return;
    }
    if (mgr->m_FrameIndex == mgr->m_ShownFrameIndex) {
        return;
    }
    const u8* texture = GetImageTexture(mgr->m_ProgramLoader.m_GraphicsFile, mgr->m_FrameIndex);
    std::memcpy(mgr->m_pScreenImage, GetImagePixels(texture), SCREEN_IMAGE_SIZE);
    s_pInstance->m_ShownFrameIndex = s_pInstance->m_FrameIndex;
}

// 0x00343DA4
bool FtrTvMgr::Load()
{
    m_IsLoaded = SoundMgr::SHasHeaps23();
    if (!m_IsLoaded) {
        return true;
    }
    sead::Heap* heap = HeapInit::s_pStageHeap;
    bool isDone = true;
    m_Program = GetScheduledProgram();
    if (m_CommonAllocator.m_pHeap != NULL) {
        isDone &= m_ReceptorLoader.LoadAsync(sead::SafeStringBase<char>("/Ftr/TV/tv_receptor.bcres"),
                                             m_CommonAllocator.m_pHeap, g3d::ResourceLoader::CGFX_ALIGNMENT);
        if (isDone) {
            m_ReceptorLoader.InitializeCgfx(&m_CommonAllocator, NULL, true);
        }
    } else {
        isDone &= m_CommonAllocator.CreateExpHeap(COMMON_HEAP_SIZE, heap, sead::SafeStringBase<char>("CommonTV"),
                                                  sead::Heap::HEAP_DIRECTION_FORWARD, false);
    }
    if (m_NextAllocator.m_pHeap == NULL) {
        isDone &= m_NextAllocator.CreateExpHeap(NEXT_HEAP_SIZE, heap, sead::SafeStringBase<char>("NextTV"),
                                                sead::Heap::HEAP_DIRECTION_FORWARD, false);
    }
    if (m_AudioAllocators[0].m_pHeap == NULL) {
        isDone &= m_AudioAllocators[0].CreateExpHeap(AUDIO_HEAP_SIZE, heap, sead::SafeStringBase<char>("TVAudio0"),
                                                     sead::Heap::HEAP_DIRECTION_FORWARD, false);
    }
    if (m_AudioAllocators[1].m_pHeap == NULL) {
        isDone &= m_AudioAllocators[1].CreateExpHeap(AUDIO_HEAP_SIZE, heap, sead::SafeStringBase<char>("TVAudio1"),
                                                     sead::Heap::HEAP_DIRECTION_FORWARD, false);
    }
    if (m_ReceptorLoader.m_SetupResult != g3d::ResourceSet::SETUP_RESULT_NONE &&
        m_ReceptorLoader.m_GraphicsFile.IsValid()) {
        const u8* texture = GetImageTexture(m_ReceptorLoader.m_GraphicsFile, 0);
        m_pScreenTexture = texture;
        m_pScreenImage = GetImagePixels(texture);
    }
    m_pNextSound = CreateSound(m_Program, &m_AudioAllocators[m_AudioIndex]);
    if (m_pNextSound != NULL) {
        m_pNextSound->Start();
    }
    m_AudioIndex = (m_AudioIndex == 0) ? 1 : 0;
    if (isDone && s_pInstance == NULL) {
        s_pInstance = this;
    }
    return isDone;
}

// 0x003440D4
bool FtrTvMgr::Calc(u8 channel, const nn::math::VEC3* position)
{
    if (!m_IsLoaded) {
        return true;
    }
    if (std::fabs(position->x * position->x + position->y * position->y + position->z * position->z) <= 0x1p-23f) {
        // no TV: the sound stops
        if (m_IsSoundPlaying && m_pSound != NULL && m_pSound->Load()) {
            m_pSound->Mute();
            m_pSound->Stop();
            SoundTvHandle::Destroy(m_pSound);
            m_pSound = NULL;
            m_IsSoundPlaying = false;
        }
    } else {
        if (m_Program < 0) {
            const s16 program = GetScheduledProgram();
            if (program >= 0 && m_ShownProgram != program) {
                // the next program: its file into the second heap, its sound into the free audio heap
                m_Program = program;
                m_ProgramLoader.Unload();
                m_NextAllocator.DestroyHeap();
                m_NextAllocator.CreateExpHeap(NEXT_HEAP_SIZE, HeapInit::s_pStageHeap,
                                              sead::SafeStringBase<char>("NextTV"),
                                              sead::Heap::HEAP_DIRECTION_FORWARD, false);
                if (m_pNextSound == NULL) {
                    m_pNextSound = CreateSound(program, &m_AudioAllocators[m_AudioIndex]);
                    if (m_pNextSound != NULL) {
                        m_pNextSound->Start();
                        m_AudioIndex = (m_AudioIndex == 0) ? 1 : 0;
                    }
                }
            } else if (!m_IsSoundPlaying) {
                // the sound of the shown program starts once it is there
                if (m_pSound != NULL) {
                    if (m_pSound->Load()) {
                        m_pSound->Play(0);
                        m_IsSoundPlaying = true;
                    }
                } else {
                    m_pSound = CreateSound(program, &m_AudioAllocators[m_AudioIndex]);
                    if (m_pSound != NULL) {
                        m_pSound->Start();
                        m_AudioIndex = (m_AudioIndex == 0) ? 1 : 0;
                    }
                }
            }
        }
        if (m_Program >= 0) {
            // the file of the program (the weather by the weather of the day; program 1 has regional versions)
            sead::FixedSafeString<64> path;
            if (m_Program == PROGRAM_WEATHER) {
                m_Weather = Func630B28();
                if (m_Weather >= WEATHER_PROGRAM_NUM) {
                    m_Weather = 0;
                }
                path.format("%s", s_WeatherPrograms[m_Weather]);
            } else if (m_Program == PROGRAM_COLOR) {
                const u32 region = GetGardenData()->townData.unk_0xEDAA >> 4;
                if (region == 1) {
                    path.format("/Ftr/TV/tv_program%d_US.bcres", m_Program);
                } else if (region == 2) {
                    path.format("/Ftr/TV/tv_program%d_EU.bcres", m_Program);
                } else {
                    path.format("/Ftr/TV/tv_program%d.bcres", m_Program);
                }
            } else {
                path.format("/Ftr/TV/tv_program%d.bcres", m_Program);
            }
            bool isLoaded = false;
            if (m_pSound == NULL || m_pSound->Load()) {
                if (m_ProgramLoader.LoadAsync(path, m_NextAllocator.m_pHeap, g3d::ResourceLoader::CGFX_ALIGNMENT)) {
                    isLoaded = m_pNextSound->Load();
                }
            }
            if (isLoaded) {
                // the program is shown, its sound replaces the old one
                m_ProgramLoader.InitializeCgfx(&m_NextAllocator, NULL, true);
                m_pScreenAnim = m_ProgramLoader.m_GraphicsFile.GetTextureAnims(0);
                s_Frame = 0.0f;
                m_ShownFrameIndex = 1;
                m_FrameIndex = 0;
                m_ShownProgram = m_Program;
                m_Program = -1;
                SoundTvHandle* old = m_pSound;
                m_pSound = m_pNextSound;
                m_pSound->Play(1);
                if (old != NULL) {
                    old->Mute();
                    old->Stop();
                    SoundTvHandle::Destroy(old);
                }
                m_pNextSound = NULL;
                m_IsSoundPlaying = true;
            }
        }
        // the texture of the frame: the digit at [3] of the name of the texture of the pattern animation
        s32 frameIndex = -1;
        if (m_ProgramLoader.m_SetupResult != g3d::ResourceSet::SETUP_RESULT_NONE &&
            m_ProgramLoader.m_GraphicsFile.IsValid() && m_pScreenAnim != NULL) {
            const u8* members = GetTarget(m_pScreenAnim, 0x1C);
            const u8* member = (members != NULL) ? GetTarget(members, 0x28) : NULL;
            if (member != NULL) {
                f32 value;
                if (*reinterpret_cast<const u32*>(member) & 1) {
                    value = *reinterpret_cast<const f32*>(member + 0xC);
                } else {
                    value = nw::anim::res::CalcFloatCurve(
                        reinterpret_cast<const nw::anim::res::ResFloatCurveData*>(GetTarget(member, 0xC)), s_Frame);
                }
                const u8* textures = GetTarget(member, 0x14);
                const u8* reference = GetTarget(textures, static_cast<u32>(value) * 4);
                const char* name = reinterpret_cast<const char*>(GetTarget(reference, 0x18));
                frameIndex = name[3] - '0';
            }
        }
        if (m_Program == -1 && frameIndex != -1 && m_ShownFrameIndex != frameIndex) {
            m_FrameIndex = frameIndex;
        }
        if (m_IsSoundPlaying && m_pSound != NULL) {
            m_pSound->Update(static_cast<s32>(s_Frame), channel, position);
        }
    }
    s_Frame += 1.0f;
    if (m_pScreenAnim != NULL && s_Frame > *reinterpret_cast<const f32*>(static_cast<const u8*>(m_pScreenAnim) + 0x14)) {
        s_Frame = 0.0f;
    }
    return true;
}

// 0x00344918
void FtrTvMgr::Unload()
{
    s_pInstance = NULL;
    if (!m_IsLoaded) {
        return;
    }
    if (m_pSound != NULL) {
        m_pSound->Stop();
        SoundTvHandle::Destroy(m_pSound);
        m_pSound = NULL;
    }
    if (m_pNextSound != NULL) {
        m_pNextSound->Stop();
        SoundTvHandle::Destroy(m_pNextSound);
        m_pNextSound = NULL;
    }
    m_AudioAllocators[0].DestroyHeap();
    m_AudioAllocators[1].DestroyHeap();
    m_ReceptorLoader.Unload();
    m_CommonAllocator.DestroyHeap();
    m_ProgramLoader.Unload();
    m_NextAllocator.DestroyHeap();
}

// 0x003449C0
FtrTvMgr::FtrTvMgr()
    : m_IsLoaded(false), m_Program(-1), m_ShownProgram(-1), m_FrameIndex(0), m_ShownFrameIndex(0), m_Weather(0),
      m_pScreenTexture(NULL), m_pScreenAnim(NULL), m_AudioIndex(0), m_IsSoundPlaying(false), m_pScreenImage(NULL),
      m_pSound(NULL), m_pNextSound(NULL)
{
}
