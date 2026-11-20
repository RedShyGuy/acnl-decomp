#pragma once

#include "decomp.h"
#include "g3d/dResourceLoader.h"
#include "g3d/dMaterialAnim.h"
#include "mutil/dBaseResourceMgr.h"
#include "ssys/ma/dHeapAllocator.h"

// The TV programs of the furniture (BsFtrMgr +0x4A0; code at 0x00343CE0..0x003449C0 in the shared area; no RTTI, names
// are ours): the receptor model "/Ftr/TV/tv_receptor.bcres" with the material animation of the screen, the program
// files "/Ftr/TV/tv_program%d[_US|_EU].bcres" and their sounds, in the heaps "CommonTV", "NextTV", "TVAudio0" and
// "TVAudio1".
class SoundTvHandle;

class FtrTvMgr
{
public:
    // the programs (the numbers of the SoundTv classes; names after them)
    enum Program : s16
    {
        PROGRAM_STORM = 0,
        PROGRAM_COLOR = 1,
        PROGRAM_NEWS = 2,
        PROGRAM_DRAMA = 3,
        PROGRAM_SONG = 4,
        PROGRAM_QUIZ = 5,
        PROGRAM_TRAVEL = 6,
        PROGRAM_HERO = 7,
        PROGRAM_VARIETY = 8,
        PROGRAM_MOVIE = 9,
        PROGRAM_SOCCER = 10,
        PROGRAM_TALK = 11,
        PROGRAM_ANIME = 12,
        PROGRAM_COOKING = 13,
        PROGRAM_EXERCISE = 14,
        PROGRAM_APPLE_CM = 15,
        PROGRAM_SING_CM = 16,
        PROGRAM_SNOW_CM = 17,
        PROGRAM_UFO_CM = 18,
        PROGRAM_SHOPPING_CM = 19,
        PROGRAM_JUICE_CM = 20,
        PROGRAM_LAUNDRY_CM = 21,
        PROGRAM_WEATHER = 255, // (its sound is the program 22 of SoundTvHandle)
    };
    // the schedule: from the hour and minute of an entry on its program runs
    struct ScheduleEntry
    {
        s16 program; // 0x0 Program
        u8 hour;     // 0x2
        u8 minute;   // 0x3
    };
    struct Schedule
    {
        const ScheduleEntry* entries; // 0x0
        u32 num;                      // 0x4
    };
    // the number of weather programs (by the weather of the day)
    static const u32 WEATHER_PROGRAM_NUM = 11;

    FtrTvMgr(); // 0x003449C0
    // (inline in ~BsFtrMgr)
    ~FtrTvMgr() {}

    // the heaps, the receptor and the sound of the program of the time are loaded (true: done or no sound heaps)
    bool Load(); // 0x00343DA4
    // channel: of the nearest TV, position: of it (zero: no TV, the program sound stops); the program of the time is
    // loaded into the second heap and replaces the shown one, the frame of the screen animation advances
    bool Calc(u8 channel, const nn::math::VEC3* position); // 0x003440D4
    void Unload(); // 0x00344918
    // (after drawing) the frame of the program that is due is copied into the screen texture of the receptor
    static void UpdateScreenTexture(); // 0x00343CE0

    // the TV of the stage (set when loaded) and the frame of the screen animation
    static FtrTvMgr* s_pInstance;
    static f32 s_Frame;
    static const char* s_WeatherPrograms[WEATHER_PROGRAM_NUM];

    g3d::MaterialAnim m_ScreenAnim;           // 0x000
    mutil::ResourceKey m_Key;                 // 0x020
    bool m_IsLoaded;                          // 0x028 (the sound heaps are there)
    u8 _29;
    s16 m_Program;                            // 0x02A the program to load (-1: none)
    s16 m_ShownProgram;                       // 0x02C the program on the screen (-1: none)
    u8 m_FrameIndex;                          // 0x02E the texture of the program due
    u8 m_ShownFrameIndex;                     // 0x02F the texture copied to the screen
    u8 m_Weather;                             // 0x030 of the weather program
    u8 _31[3];
    ssys::ma::HeapAllocator m_CommonAllocator; // 0x034 "CommonTV"
    ssys::ma::HeapAllocator m_NextAllocator;   // 0x03C "NextTV"
    ssys::ma::HeapAllocator m_AudioAllocators[2]; // 0x044 "TVAudio0", "TVAudio1" (the sounds of two programs)
    g3d::ResourceLoader m_ReceptorLoader;      // 0x054
    g3d::ResourceLoader m_ProgramLoader;       // 0x15C
    const void* m_pScreenTexture;              // 0x264 the texture of the receptor (ResImageTexture data)
    const void* m_pScreenAnim;                 // 0x268 the texture animation of the program
    u8 m_AudioIndex;                           // 0x26C the audio heap of the next sound
    bool m_IsSoundPlaying;                     // 0x26D
    u8 _26E[2];
    u8* m_pScreenImage;                        // 0x270 the pixels of the screen texture (2048 bytes)
    SoundTvHandle* m_pSound;                   // 0x274 of the shown program
    SoundTvHandle* m_pNextSound;               // 0x278 of the loading program
};
ASSERT_SIZE(FtrTvMgr, 0x27C);
