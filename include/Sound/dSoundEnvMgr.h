#pragma once

#include "decomp.h"
#include "Sound/dSoundFader.h"
#include "nw/snd/snd_SoundHandle.h"
#include "sead/seadAudioSubsetBase.h"
#include "stage/stage_Api.h"

// RTTI 11SoundEnvMgr @ 0x008CB33C
// vtable 0x008ECD2C (vptr 0x008ECD34), offset_to_top 0, 20 entries
// The environment sounds: rain and heavy rain (faded in and out with the rain level of the weather), the sea on the
// boat back from the island, the loops of some rooms and the day/night loops of the island (the class name is from the
// RTTI, everything else is ours).
class SoundEnvMgr : public ::sead::AudioSubsetBase
{
public:
    // m_RainState
    enum RainState
    {
        RAIN_NONE = 0,
        RAIN_RAIN = 2,
        RAIN_START = 3,
        RAIN_HEAVY = 4,
    };
    // where the rain is heard (m_RainArea)
    enum RainArea
    {
        RAIN_AREA_NONE = 0,
        RAIN_AREA_OUTDOOR = 1,
        RAIN_AREA_INDOOR = 2,  // muffled
    };
    // the rain level of the weather at which the sounds are full (the original calls functions that armlink replaced
    // by these values)
    enum
    {
        RAIN_LEVEL = 32,
        HEAVY_RAIN_LEVEL = 128,
    };
    // a fade of a rain sound over the rain level: the volume goes from `from` to `pivot` as the level does
    struct LevelRange
    {
        void SetTarget(s32 target, f32 ratio)
        {
            to = target;
            pivot = from + static_cast<s32>(static_cast<f32>(target - from) * ratio);
            if (target > from) {
                if (pivot > target) {
                    pivot = target;
                }
            } else if (pivot < target) {
                pivot = target;
            }
        }

        s32 from;   // 0x0
        s32 to;     // 0x4
        s32 pivot;  // 0x8
    };
    // how a rain sound is played in an area (outdoor, train station)
    struct RainSound
    {
        f32 volume[2];        // 0x00
        f32 surroundPan[2];   // 0x08
        s32 biquadType[2];    // 0x10
        f32 biquadValue[2];   // 0x18
    };

    SoundEnvMgr(); // 0x00128868
    virtual ~SoundEnvMgr();
    virtual void calc() override; // 0x001D9548 slot 0x1C

    // it rains (the state, not the weather)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsRaining() const // 0x0070F628
    {
        return m_RainState == RAIN_START || m_RainState == RAIN_RAIN || m_RainState == RAIN_HEAVY;
    }
    void SetRainState(s32 state); // 0x001D8494
    // the sea on the boat back from the island
    void StartSea(); // 0x001D849C
    void SetPosition(s32 x, s32 y); // 0x001D84D4
    // a loop in slot 0 or 1
    void StartLoop(u32 soundId, s32 slot); // 0x001D84E0
    void StopLoops(s32 fadeFrames); // 0x001D8698
    // a rain that is already there when the stage starts
    void StartRainNow(); // 0x001D8518
    // the rain sounds for the current rain state
    void UpdateRain(); // 0x001D86CC
    // the stage changes
    void OnStageChange(); // 0x001D8D1C
    void StartShaper(); // 0x001D8DE8
    // the rain level for the sequences (global variables 10 and 11)
    void UpdateRainVariables(); // 0x001D8E18
    // the sounds of the new stage
    void StartStage(); // 0x001D8F0C
    // the volume (by distance) and the pan of the loops
    void SetLoopParam(f32 distance, f32 pan); // 0x001D9300
    // the island day/night loops cross-fade
    void UpdateTour(); // 0x001D9418
    // all off
    void Stop(); // 0x001D98E4

    static const s32 s_OutdoorFadeFrames[2];
    static const RainSound s_RainSounds[2];
    static const u32 s_TourSoundIds[2][2];

    s32 m_RainState;                  // 0x0C RainState
    s32 m_PrevRainState;              // 0x10
    bool m_IsStopped;                 // 0x14
    s32 m_RainArea;                   // 0x18 RainArea
    s8 m_IsStation;                   // 0x1C index into s_RainSounds
    nw::snd::SoundHandle m_RainHandle;      // 0x20
    nw::snd::SoundHandle m_HeavyRainHandle; // 0x24
    nw::snd::SoundHandle m_SeaHandle;       // 0x28
    nw::snd::SoundHandle m_LoopHandles[2];  // 0x2C
    s32 m_RainLevel;                  // 0x34 the last rain level of the weather (-1: none)
    SoundFader m_RainFader;           // 0x38
    SoundFader m_HeavyRainFader;      // 0x4C
    bool m_IsRainFading;              // 0x60
    bool m_IsHeavyRainFading;         // 0x61
    LevelRange m_RainRange;           // 0x64
    LevelRange m_HeavyRainRange;      // 0x70
    SoundFader m_OutdoorFader;        // 0x7C
    bool m_IsRainPending;             // 0x90 the rain starts when the boat arrives
    s32 m_TourKind;                   // 0x94 0 lobby, 1 tour, 2 none
    SoundFader m_TourFaders[2];       // 0x98 day, night
    s32 m_PositionX;                  // 0xC0
    s32 m_PositionY;                  // 0xC4
};
ASSERT_OFFSET(SoundEnvMgr, m_RainFader, 0x38);
ASSERT_OFFSET(SoundEnvMgr, m_OutdoorFader, 0x7C);
ASSERT_OFFSET(SoundEnvMgr, m_TourFaders, 0x98);
ASSERT_SIZE(SoundEnvMgr, 0xC8);
