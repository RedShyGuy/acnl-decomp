#pragma once

#include "decomp.h"
#include "Audio/bgm/dEvOpusCarnival.h"
#include "Audio/bgm/dEvOpusCleaning.h"
#include "Audio/bgm/dEvOpusCountdown.h"
#include "Audio/bgm/dEvOpusEaster.h"
#include "Audio/bgm/dEvOpusFireworks.h"
#include "Audio/bgm/dEvOpusHalloween.h"
#include "Audio/bgm/dEvOpusHarvest.h"
#include "Audio/bgm/dEvOpusHide.h"
#include "Audio/bgm/dEvOpusNewYear.h"
#include "Audio/bgm/dEvOpusNewYearNext.h"
#include "Audio/bgm/dEvOpusXmasEve.h"
#include "Other/dParameterHIO.h"
#include "ssys/ma/dHeapAllocator.h"
#include "stage/stage_Api.h"

// The manager of the music of the stages and events (the file of the BGM manager; all names are ours unless noted):
// StageOpusMgr creates the opus of the current stage, EventOpusList holds the opus of the events, FieldMusic plays the
// music the game requests directly (hornets, fanfares, island games ...).

namespace audio {
namespace bgm {
class StageOpusBase;

// 0x0094F494: the state of the music of the stage
struct BgmState
{
    u8 isOnline;      // 0x0 the flag of Begin
    s8 isPlayerReady; // 0x1 the player can be seen (Func5C68C8)
    s8 isGrowUp;      // 0x2 StageOpusGrowUp is used for every stage
    s8 isEvidence;    // 0x3 StageOpusEvidence is used for every stage
};

// 0x0094F4A0: the state of the music of the town (StageOpusVillageDowntown)
struct TownState
{
    s8 isLow;          // 0x0 the player is below the level of the town (mix 45)
    u8 isOceanLeft;    // 0x1 SvTownData::oceanSide is 0
    s8 isForced;       // 0x2 the music of the hour is selected again
    u8 isShopMixOff;   // 0x3 no mix 36 in the shops (StageOpusGrace sets it; cleared when the stage is left)
    s8 isCampForced;   // 0x4 the music of StageOpusCamping is selected again
    u8 flag5;          // 0x5 set by the game
    u32 envSoundId;    // 0x8 the ambience of the main street (SE_ENV_DOWNTOWN_*)
    f32 distance;      // 0xC
};

// 0x0094F4B4: the clock of the music: the change of the hour and the time chime
class ClockState
{
public:
    // each frame: tracks the last seconds of the hour and plays the chime (and the countdown of New Year's Eve)
    void Update(bool isCountdown); // 0x0058A6F8
    // enables or disables the time chime (keepChime: a playing chime is not stopped)
    void SetChime(bool isEnabled, bool keepChime); // 0x0058A890

    s8 hour;           // 0x0
    s8 second;         // 0x1
    u8 isNearHour;     // 0x2 the last 15 seconds of the hour (not during the countdown event)
    u8 isChimeEnabled; // 0x3
    u8 isShop;         // 0x4 the stage is a shop
};

// 0x0094F4B9: the music of kind 1 with the mixes 16 and 17
struct SubState
{
    s8 isActive;  // 0x0
    s8 isPlaying; // 0x1
};

// 0x0098ABC0: the music the game requests directly (the methods are called through GetFieldMusic)
class FieldMusic
{
public:
    FieldMusic(); // 0x0058B5D0
    void Update(); // 0x0058B590
    void FadeHornet(s32 stopWait, u32 fade); // 0x0058A948
    void FadeFanfareGet(u32 fade, s32 stopWait); // 0x0058A984
    void RequestObjSong(u32 song, u32 fade); // 0x0058A9CC
    void RequestCoffee(u32 fade); // 0x0058AA1C
    void RequestFieldBgm(bool isRacket); // 0x0058AA5C
    void FadeObjSong(u32 fade); // 0x0058AABC
    void RequestShobonLoop(); // 0x0058AB00
    void SetHornetDelay(s32 delay); // 0x0058AB3C
    void FadeComplete(u32 fade, s32 stopWait); // 0x0058AB60
    void FadeCoffee(s32 stopWait); // 0x0058AB9C
    void RequestHornetEvent(); // 0x0058ABD4
    void FadeFanfareGetWait(u32 fade); // 0x0058AC20
    void StopIslandGame(); // 0x0058AC64
    void RequestFanfareGet(); // 0x0058ACC0
    void FadeKind10(s32 time, u32 fade); // 0x0058AD04
    void StartCoffee(); // 0x0058AD38
    void SetHornetWait(s32 wait); // 0x0058AD58
    void RestartFieldBgm(); // 0x0058AD7C
    void FadeFieldBgm(); // 0x0058ADC4
    void StartIslandGame(); // 0x0058ADF8
    void FadeResetScramble(u32 fade); // 0x0058AE74
    void RequestComplete(); // 0x0058AEAC
    void SetShobonLoopDelay(s32 delay); // 0x0058AEE8
    void RequestBingo(); // 0x0058AF0C
    void PlayUpaClose(); // 0x0058AF48
    void SetFanfareGetWait(s32 wait); // 0x0058AF7C
    void StartBingo(); // 0x0058AFAC
    void StartResetScramble(); // 0x0058AFC8
    void FadeFieldBgm2(); // 0x0058AFE8
    void RequestKind10(u32 fade); // 0x0058B01C
    void FadeFieldBgmMix(s32 stopWait); // 0x0058B054
    void RemoveMix25(); // 0x0058B098
    void RemoveMix24(); // 0x0058B0AC
    void FadeKind17(s32 time); // 0x0058B0C0
    void StartFanfareGet(s32 delay); // 0x0058B0F0
    void FadeBingo(); // 0x0058B120
    void SetupIslandGame(); // 0x0058B164
    void StopShobon(); // 0x0058B1F0
    void SetCompleteDelay(s32 delay); // 0x0058B214
    void SetBoostOn(); // 0x0058B238
    void RequestResetScramble(); // 0x0058B240
    void RequestFieldBgmNoMix(bool isRacket); // 0x0058B27C
    void PlayAwardFanfare(); // 0x0058B2D0
    void StartFieldBgm(); // 0x0058B2DC
    void StartFieldBgm2(); // 0x0058C704
    void RequestKind17(); // 0x0058B2FC
    void RemoveFieldBgm(); // 0x0058B368
    void SetBoostOff(); // 0x0058B384
    void StartJinglePrepared(); // 0x0058B38C
    void AddMix25(); // 0x0058B3A0
    void AddMix24(); // 0x0058B3B4
    void FadeHornetNow(u32 fade); // 0x0058B3C8
    void RequestShobon(s32 delay); // 0x0058B400
    void RemoveMix27(); // 0x0058B440
    void SpeedUpIslandGame(); // 0x0058B454
    void RemoveBingo(); // 0x0058B508
    void AddMix27(); // 0x0058B520
    void RemoveMix28(); // 0x0058B534
    void RequestBingoWithParam(bool resetParam); // 0x0058B548
    void FadeShobonLoop(u32 fade, s32 stopWait); // 0x0058B594

    static const u32 s_IslandGameBgm[12][4];

    u8 _0;                  // 0x00
    u8 isFast;              // 0x01 the fast version of the music of the island game
    u32 gameIndex;          // 0x04 the row of s_IslandGameBgm
    u32 objSongId;          // 0x08
    u32 gameColumn;         // 0x0C 0 day, 1 night, + 2: fast
    u32 fieldBgmId;         // 0x10 STRM_BGM_FLD_RACKET or _RESET
    u8 fanfareState;        // 0x14 SEQ_BGM_FNFR_GET
};
ASSERT_SIZE(FieldMusic, 0x18);

// 0x0098B018: creates the opus of the current stage (the name of its heap is "StageOpusMgr")
class StageOpusMgr
{
public:
    enum
    {
        OPUS_NUM = 57,
        OPUS_NONE = 57,
    };
    // m_Flags
    enum
    {
        FLAG_KEEP = 0x1,       // the opus stays for the next stage
        FLAG_READY = 0x2,
        FLAG_4 = 0x4,
        FLAG_8 = 0x8,
        FLAG_HIDE = 0x10,      // hide-and-seek begins with the next stage
        FLAG_HIDE_END = 0x20,
        FLAG_CEREMONY = 0x40,  // the next opus is StageOpusCeremony
        FLAG_PLANT_DEMO = 0x80, // the next opus is StageOpusPlantDemo
    };

    typedef StageOpusBase* (StageOpusMgr::*CreateFunc)();

    StageOpusMgr(); // 0x00585EF8
    ~StageOpusMgr();

    void Initialize(); // 0x00584C38
    void ChangeStage(stage::Name stage); // 0x00584D88
    void Begin(stage::Name stage, bool isOnline); // 0x00585104
    void Leave(stage::Name next); // 0x005852D0
    void Ready(); // 0x00584EF4
    void Clear(); // 0x00585D10
    void Update(); // 0x00585DDC
    // stops the music of the opus at once
    void StopOpus(); // 0x00584F80

    void StartCarnival(); // 0x00584D20
    void ResetHide(); // 0x00584D38
    void StartFortuneDemo(); // 0x00584D50
    void StopFortuneDemo(s32 stopWait); // 0x00584EA0
    void RequestFortuneDemo(u32 fade); // 0x00584FF0
    void EndCarnivalLong(); // 0x0058504C
    void StartHarvestCook(u32 fade, u32 delay); // 0x005850B8
    void StopHarvestCook(s32 stopWait); // 0x0058528C
    void SetFortuneDemoWait(s32 wait); // 0x00585388
    void Nothing(); // 0x0058585C
    void RequestPrologue10(); // 0x00585860
    void SetPlantDemoStep2(); // 0x005858C8
    void DisableChime(); // 0x005858E0
    void StartPrologue3(); // 0x005858F4
    void ClearHarvestCookAccepted(); // 0x00585914
    void RequestRetireFanfare(); // 0x00585928
    void AddMix9(u32 fade); // 0x00585954
    void EndResult(); // 0x00585974
    void AcceptHarvestCook(); // 0x005859A8
    void StopOffice(); // 0x005859D0
    void RequestKind10(); // 0x00585A50
    void StartCarnival2(); // 0x00585A94
    void StopStation(); // 0x00585AC8
    void RemoveMix21(); // 0x00585AF0
    void SetTownChanged(); // 0x00585B04
    void StartCarnival4(); // 0x00585B30
    void RequestPrologue03(); // 0x00585B64
    void RequestPrologue05(); // 0x00585B80
    void SetPlantDemoStep1(); // 0x00585B9C
    void RequestPrologue04(); // 0x00585BB4
    void AddMix8(); // 0x00585BD0
    void RequestPrologue06(); // 0x00585BF4
    void StopUpdatePrologue(); // 0x00585C40
    void StopExchange(); // 0x00585C68
    void StartExchangeReset(); // 0x00585C90
    void StopExchange2(); // 0x00585CBC
    void StartExchangeTanukichi(); // 0x00585CE4
    void StartClub444Live(); // 0x00585D58
    void StopCarnival(); // 0x00585D9C
    void SetTownFlag5(); // 0x00585E1C
    void ClearTownFlag5(); // 0x00585E30
    void SetHideWaiting(); // 0x00585E44
    void StopClub444Live(); // 0x00585E68
    void EndCarnivalShort(); // 0x00585EA8

    template <typename T>
    StageOpusBase* Create();

    static const CreateFunc s_CreateFuncs[OPUS_NUM];

    ssys::ma::HeapAllocator m_Allocator; // 0x00 the heap of the opus
    StageOpusBase* m_pOpus; // 0x08
    u8 m_OpusIndex;         // 0x0C OPUS_NONE: none
    u16 m_Flags;            // 0x0E

private:
    // the opus of the stage (an index of s_CreateFuncs)
    u8 SelectOpus(stage::Name stage); // 0x005853BC
};
ASSERT_SIZE(StageOpusMgr, 0x10);

// 0x0098B074: the opus of the events
class EventOpusList
{
public:
    DECOMP_NOINLINE EventOpusList(); // 0x0058CB74
    ~EventOpusList();

    // the opus of the current event (NULL: none)
    EvOpusBase* Select(); // 0x0058C930
    static void Reset(); // 0x0058CB0C

    EvOpusCleaning m_Cleaning;       // 0x00
    EvOpusHide m_Hide;               // 0x0C
    EvOpusFireworks m_Fireworks;     // 0x18
    EvOpusCountdown m_Countdown;     // 0x20
    EvOpusNewYear m_NewYear;         // 0x30
    EvOpusNewYearNext m_NewYearNext; // 0x40
    EvOpusHalloween m_Halloween;     // 0x48
    EvOpusHarvest m_Harvest;         // 0x50
    EvOpusXmasEve m_XmasEve;         // 0x64
    EvOpusCarnival m_Carnival;       // 0x6C
    EvOpusEaster m_Easter;           // 0x78
};
ASSERT_SIZE(EventOpusList, 0x80);

// the frames of the fade out until the music stops at hh:59:55 (20 before the last minute)
s32 GetFadeToHour(); // 0x0058A67C

void Initialize(); // 0x00584B68
s8 IsEvidence(); // 0x00584B94
void ChangeStage(stage::Name stage); // 0x00584BA4
void Ready(); // 0x00584BE0
s8 IsGrowUp(); // 0x00584C28
void Begin(stage::Name stage, bool isOnline); // 0x00585F20
s8 IsPlayerReady(); // 0x00585F5C
void Leave(stage::Name next); // 0x00586178
void Update(); // 0x0058B86C
// the music is not blocked by a request of kind 26 (id 0x100)
bool IsSubFree(); // 0x0058A190
// the next opus is StageOpusEvidence
void SetEvidence(); // 0x00588D0C
// the next opus is StageOpusGrowUp
void SetGrowUp(); // 0x005881B0
// everything stops (the mixer, the opus, the requests, the events)
void Reset(); // 0x00586D84

// the sub music (kind 1, id 0) with the mixes 16 and 17 (all names are ours)
s8 IsSubActive(); // 0x0058A1AC
void StartSub(); // 0x0058A1BC
// the sub music ends when nothing requests the kind 26 any more
void CheckSubEnd(); // 0x0058A268
bool IsBgmInactive(); // 0x0058A298
void FadeSubToMix17(); // 0x0058A2A8
bool IsSubStopped(); // 0x0058A33C
void EndSub(); // 0x0058A380
bool IsSubPlayingE2(); // 0x0058A494
void FadeSubLong(); // 0x0058A4C8
void ResumeSub(); // 0x0058A544
bool IsSubPlaying(); // 0x0058A584
bool IsSubPlayingE1(); // 0x0058A5BC
void StopSub(); // 0x0058A5F0

// the constants of the file (in the order of the binary; names are ours)
extern const u32 s_SubStartFade;   // 120
extern const u32 s_SubResumeDelay; // 5
extern const u32 s_SubLongFade;    // 390
extern const u32 s_SubLongWait;    // 100
extern const u32 s_SubEndFade;     // 120
extern const u32 s_SubMixFade;     // 25
extern const u32 s_SubEndTime;     // 5
extern const u32 s_SubEndFadeShort; // 30
extern const u32 s_Kind5Fade;      // 5
extern const u32 s_Kind5Fade2;     // 10
extern const u32 s_Kind5Wait;      // 30

DECOMP_NOINLINE StageOpusMgr* GetStageOpusMgr(); // 0x005870F4
DECOMP_NOINLINE FieldMusic* GetFieldMusic(); // 0x0058CC84

extern BgmState s_State;
extern TownState s_TownState;
extern ClockState s_ClockState;
extern SubState s_SubState;
extern FieldMusic s_FieldMusic;
extern StageOpusMgr s_StageOpusMgr;
extern EventOpusList s_EventOpusList;
} // namespace bgm
} // namespace audio
