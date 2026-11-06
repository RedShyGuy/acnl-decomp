#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Audio/bgm/dStageOpusVillageDowntown.h"
#include "Sound/dSoundLabel.h"

// the constants of the BGM manager (in a file of their own, GCC folds them otherwise)

namespace audio {
namespace bgm {

// the fade of the requests that wait for the stage in a museum or showroom
// 0x00846574
extern const u32 s_ShowroomStageFade = 10;

// the rain state of the music for a weather (after a weather below 3 the first table)
// 0x00846394
extern const u8 s_WeatherStatesFromDry[7] = {0, 0, 1, 3, 4, 5, 6};
// 0x0084639B
extern const u8 s_WeatherStates[7] = {0, 0, 1, 2, 4, 5, 6};

// the fades and times of the sub music and of the bingo requests
// 0x00846578
extern const u32 s_SubStartFade = 120;
// 0x0084657C
extern const u32 s_SubResumeDelay = 5;
// 0x00846580
extern const u32 s_SubLongFade = 390;
// 0x00846584
extern const u32 s_SubLongWait = 100;
// 0x00846588
extern const u32 s_SubEndFade = 120;
// 0x0084658C
extern const u32 s_SubMixFade = 25;
// 0x00846590
extern const u32 s_SubEndTime = 5;
// 0x00846594
extern const u32 s_SubEndFadeShort = 30;
// 0x00846598
extern const u32 s_Kind5Fade = 5;
// 0x0084659C
extern const u32 s_Kind5Fade2 = 10;
// 0x008465A0
extern const u32 s_Kind5Wait = 30;

// the music of Main Street [sunny, rainy, snowy][day, quiet]
// 0x008465A4
const u32 StageOpusVillageDowntown::s_DowntownBgm[3][2] = {
    {STRM_BGM_DWNTWN_DAY_1_SUNNY, STRM_BGM_DWNTWN_QUIET_1_SUNNY},
    {STRM_BGM_DWNTWN_DAY_1_RAINY, STRM_BGM_DWNTWN_QUIET_1_RAINY},
    {STRM_BGM_DWNTWN_DAY_1_SNOWY, STRM_BGM_DWNTWN_QUIET_1_SNOWY},
};

// the music of the town for each hour [sunny, rainy, snowy][hour]
// 0x00846784
const u32 StageOpusVillageDowntown::s_OutdoorBgm[3][24] = {
    {
        STRM_BGM_OUTDOOR00_SUNNY, STRM_BGM_OUTDOOR01_SUNNY, STRM_BGM_OUTDOOR02_SUNNY, STRM_BGM_OUTDOOR03_SUNNY,
        STRM_BGM_OUTDOOR04_SUNNY, STRM_BGM_OUTDOOR05_SUNNY, STRM_BGM_OUTDOOR06_SUNNY, STRM_BGM_OUTDOOR07_SUNNY,
        STRM_BGM_OUTDOOR08_SUNNY, STRM_BGM_OUTDOOR09_SUNNY, STRM_BGM_OUTDOOR10_SUNNY, STRM_BGM_OUTDOOR11_SUNNY,
        STRM_BGM_OUTDOOR12_SUNNY, STRM_BGM_OUTDOOR13_SUNNY, STRM_BGM_OUTDOOR14_SUNNY, STRM_BGM_OUTDOOR15_SUNNY,
        STRM_BGM_OUTDOOR16_SUNNY, STRM_BGM_OUTDOOR17_SUNNY, STRM_BGM_OUTDOOR18_SUNNY, STRM_BGM_OUTDOOR19_SUNNY,
        STRM_BGM_OUTDOOR20_SUNNY, STRM_BGM_OUTDOOR21_SUNNY, STRM_BGM_OUTDOOR22_SUNNY, STRM_BGM_OUTDOOR23_SUNNY,
    },
    {
        STRM_BGM_OUTDOOR00_RAINY, STRM_BGM_OUTDOOR01_RAINY, STRM_BGM_OUTDOOR02_RAINY, STRM_BGM_OUTDOOR03_RAINY,
        STRM_BGM_OUTDOOR04_RAINY, STRM_BGM_OUTDOOR05_RAINY, STRM_BGM_OUTDOOR06_RAINY, STRM_BGM_OUTDOOR07_RAINY,
        STRM_BGM_OUTDOOR08_RAINY, STRM_BGM_OUTDOOR09_RAINY, STRM_BGM_OUTDOOR10_RAINY, STRM_BGM_OUTDOOR11_RAINY,
        STRM_BGM_OUTDOOR12_RAINY, STRM_BGM_OUTDOOR13_RAINY, STRM_BGM_OUTDOOR14_RAINY, STRM_BGM_OUTDOOR15_RAINY,
        STRM_BGM_OUTDOOR16_RAINY, STRM_BGM_OUTDOOR17_RAINY, STRM_BGM_OUTDOOR18_RAINY, STRM_BGM_OUTDOOR19_RAINY,
        STRM_BGM_OUTDOOR20_RAINY, STRM_BGM_OUTDOOR21_RAINY, STRM_BGM_OUTDOOR22_RAINY, STRM_BGM_OUTDOOR23_RAINY,
    },
    {
        STRM_BGM_OUTDOOR00_SNOWY, STRM_BGM_OUTDOOR01_SNOWY, STRM_BGM_OUTDOOR02_SNOWY, STRM_BGM_OUTDOOR03_SNOWY,
        STRM_BGM_OUTDOOR04_SNOWY, STRM_BGM_OUTDOOR05_SNOWY, STRM_BGM_OUTDOOR06_SNOWY, STRM_BGM_OUTDOOR07_SNOWY,
        STRM_BGM_OUTDOOR08_SNOWY, STRM_BGM_OUTDOOR09_SNOWY, STRM_BGM_OUTDOOR10_SNOWY, STRM_BGM_OUTDOOR11_SNOWY,
        STRM_BGM_OUTDOOR12_SNOWY, STRM_BGM_OUTDOOR13_SNOWY, STRM_BGM_OUTDOOR14_SNOWY, STRM_BGM_OUTDOOR15_SNOWY,
        STRM_BGM_OUTDOOR16_SNOWY, STRM_BGM_OUTDOOR17_SNOWY, STRM_BGM_OUTDOOR18_SNOWY, STRM_BGM_OUTDOOR19_SNOWY,
        STRM_BGM_OUTDOOR20_SNOWY, STRM_BGM_OUTDOOR21_SNOWY, STRM_BGM_OUTDOOR22_SNOWY, STRM_BGM_OUTDOOR23_SNOWY,
    },
};

// the music of the island games [the tour][day, night, fast day, fast night]
// 0x008468DC
const u32 FieldMusic::s_IslandGameBgm[12][4] = {
    {STRM_BGM_ISL_GAME_KAKURENBO, STRM_BGM_ISL_GAME_KAKURENBO, STRM_BGM_ISL_GAME_KAKURENBO_FAST, STRM_BGM_ISL_GAME_KAKURENBO_FAST},
    {STRM_BGM_ISL_GAME_DAY, STRM_BGM_ISL_GAME_NIGHT, STRM_BGM_ISL_GAME_DAY_FAST, STRM_BGM_ISL_GAME_NIGHT_FAST},
    {STRM_BGM_ISL_GAME_DAY, STRM_BGM_ISL_GAME_NIGHT, STRM_BGM_ISL_GAME_DAY_FAST, STRM_BGM_ISL_GAME_NIGHT_FAST},
    {STRM_BGM_ISL_GAME_DAY, STRM_BGM_ISL_GAME_NIGHT, STRM_BGM_ISL_GAME_DAY_FAST, STRM_BGM_ISL_GAME_NIGHT_FAST},
    {STRM_BGM_ISL_GAME_DAY, STRM_BGM_ISL_GAME_NIGHT, STRM_BGM_ISL_GAME_DAY_FAST, STRM_BGM_ISL_GAME_NIGHT_FAST},
    {STRM_BGM_ISL_GAME_DAY, STRM_BGM_ISL_GAME_NIGHT, STRM_BGM_ISL_GAME_DAY_FAST, STRM_BGM_ISL_GAME_NIGHT_FAST},
    {STRM_BGM_ISL_GAME_HAMMER, STRM_BGM_ISL_GAME_HAMMER, STRM_BGM_ISL_GAME_HAMMER_FAST, STRM_BGM_ISL_GAME_HAMMER_FAST},
    {STRM_BGM_ISL_GAME_DAY, STRM_BGM_ISL_GAME_NIGHT, STRM_BGM_ISL_GAME_DAY_FAST, STRM_BGM_ISL_GAME_NIGHT_FAST},
    {STRM_BGM_ISL_GAME_DAY, STRM_BGM_ISL_GAME_NIGHT, STRM_BGM_ISL_GAME_DAY_FAST, STRM_BGM_ISL_GAME_NIGHT_FAST},
    {STRM_BGM_ISL_GAME_DAY, STRM_BGM_ISL_GAME_NIGHT, STRM_BGM_ISL_GAME_DAY_FAST, STRM_BGM_ISL_GAME_NIGHT_FAST},
    {STRM_BGM_ISL_GAME_DAY, STRM_BGM_ISL_GAME_NIGHT, STRM_BGM_ISL_GAME_DAY_FAST, STRM_BGM_ISL_GAME_NIGHT_FAST},
    {STRM_BGM_ISL_GAME_DAY, STRM_BGM_ISL_GAME_NIGHT, STRM_BGM_ISL_GAME_DAY_FAST, STRM_BGM_ISL_GAME_NIGHT_FAST},
};

// 0x0084699C
const TrackMixer::Mix TrackMixer::s_Mixes[MIX_NUM + 1] = {
    {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 5, 5}, // 0
    {{1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, 1, 1}, // 1
    {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 20, 20}, // 2
    {{-1.0f, -1.0f, -1.0f, 0.0f, 0.0f}, 20, 20}, // 3
    {{0.0f, 0.0f, 0.0f, -1.0f, -1.0f}, 20, 20}, // 4
    {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 60, 20}, // 5
    {{-1.0f, -1.0f, -1.0f, 0.0f, 0.0f}, 60, 20}, // 6
    {{0.0f, 0.0f, 0.0f, -1.0f, -1.0f}, 60, 20}, // 7
    {{1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, 0, 5}, // 8
    {{0.0f, 0.0f, 0.0f, -1.0f, -1.0f}, 0, 20}, // 9
    {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 0, 30}, // 10
    {{0.3125f, 0.3125f, 0.3125f, 0.3125f, 0.3125f}, 29, 8}, // 11
    {{0.3125f, 0.3125f, 0.3125f, 0.3125f, 0.3125f}, 15, 15}, // 12
    {{0.0f, 0.0f, 0.0f, -1.0f, -1.0f}, 8, 8}, // 13
    {{-1.0f, -1.0f, -1.0f, 0.0f, -1.0f}, 30, 8}, // 14
    {{0.0f, -1.0f, -1.0f, -1.0f, -1.0f}, 5, 60}, // 15
    {{-1.0f, -1.0f, -1.0f, 0.0f, 0.0f}, 60, 120}, // 16
    {{-1.0f, -1.0f, -1.0f, 0.0f, 0.0f}, 60, 30}, // 17
    {{0.0f, -1.0f, -1.0f, -1.0f, -1.0f}, 0, 5}, // 18
    {{-1.0f, -1.0f, 0.0f, -1.0f, -1.0f}, 0, 5}, // 19
    {{-1.0f, 0.0f, -1.0f, -1.0f, -1.0f}, 0, 5}, // 20
    {{1.0f, -1.0f, -1.0f, -1.0f, -1.0f}, 0, 5}, // 21
    {{-1.0f, -1.0f, 1.0f, -1.0f, -1.0f}, 0, 5}, // 22
    {{-1.0f, 1.0f, -1.0f, -1.0f, -1.0f}, 0, 5}, // 23
    {{-1.0f, -1.0f, -1.0f, 1.0f, -1.0f}, 1, 1}, // 24
    {{-1.0f, -1.0f, -1.0f, -1.0f, 1.0f}, 1, 1}, // 25
    {{0.3125f, 0.3125f, 0.3125f, 0.3125f, 0.3125f}, 8, 8}, // 26
    {{1.0f, 1.0f, 0.3125f, -1.0f, -1.0f}, 8, 8}, // 27
    {{-1.0f, -1.0f, 1.0f, -1.0f, -1.0f}, 8, 8}, // 28
    {{0.295f, 0.295f, -1.0f, -1.0f, -1.0f}, 1, 1}, // 29
    {{0.3125f, 0.3125f, 0.3125f, 0.0f, 0.0f}, 8, 8}, // 30
    {{0.0f, 0.0f, 1.0f, 0.0f, 0.0f}, 8, 8}, // 31
    {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 8, 8}, // 32
    {{0.3125f, 0.3125f, 0.3125f, 0.3125f, 0.3125f}, 29, 8}, // 33
    {{0.3125f, 0.3125f, 0.3125f, 0.3125f, 0.3125f}, 8, 8}, // 34
    {{-1.0f, -1.0f, -1.0f, 0.0f, 0.0f}, 0, 100}, // 35
    {{0.3125f, 0.3125f, 0.3125f, 0.3125f, 0.3125f}, 8, 8}, // 36
    {{0.3125f, 1.0f, 1.0f, 1.0f, 1.0f}, 23, 23}, // 37
    {{0.0f, 0.0f, 1.0f, 0.3125f, 0.3125f}, 5, 23}, // 38
    {{0.3125f, 1.0f, 1.0f, 1.0f, 1.0f}, 23, 23}, // 39
    {{0.3125f, 1.0f, 1.0f, 1.0f, 1.0f}, 23, 23}, // 40
    {{0.3125f, 1.0f, 1.0f, 1.0f, 1.0f}, 60, 45}, // 41
    {{0.3125f, 1.0f, 1.0f, 1.0f, 1.0f}, 23, 23}, // 42
    {{0.3125f, 1.0f, 1.0f, 1.0f, 1.0f}, 90, 90}, // 43
    {{0.3125f, 1.0f, 1.0f, 1.0f, 1.0f}, 90, 90}, // 44
    {{0.3125f, 1.0f, 1.0f, 1.0f, 1.0f}, 90, 90}, // 45
    {{0.45f, 1.0f, 1.0f, 1.0f, 1.0f}, 90, 90}, // 46
    {{0.3125f, 1.0f, 1.0f, 1.0f, 1.0f}, 90, 90}, // 47
    {{0.5f, 1.0f, 1.0f, 1.0f, 1.0f}, 60, 60}, // 48
    {{0.47f, 1.0f, 1.0f, 1.0f, 1.0f}, 30, 30}, // 49
    {{1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, 1, 1}, // 50
};

// 0x00846F30
const u64 TrackMixer::s_KeepMask = 0x00000004200000FFull;

} // namespace bgm
} // namespace audio
