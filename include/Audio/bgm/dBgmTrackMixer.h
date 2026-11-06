#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_DateTime.h"

namespace audio {
namespace bgm {
// The mixer of the five tracks of the music (in the file of the BGM manager; all names are ours): each request or event can
// switch on a mix (an index of s_Mixes); of all mixes that are on, the lowest index that sets a track decides its volume.
class TrackMixer
{
public:
    enum
    {
        TRACK_NUM = 5,
        MIX_NUM = 50,
        MIX_NONE = 50, // the last row of s_Mixes: all tracks full
        MIX_MUTE = 2,
    };

    // a mix: the volume of each track (negative: the mix does not set the track), the frames of the fade in and out
    struct Mix
    {
        f32 volumes[TRACK_NUM];
        s32 fadeIn;
        s32 fadeOut;
    };

    TrackMixer(); // 0x0058D410

    // switches the mix on (unless it was skipped)
    DECOMP_NOINLINE void AddMix(u8 index); // 0x0058CFFC
    DECOMP_NOINLINE void RemoveMix(u8 index); // 0x0058D0D8
    // the next AddMix of the index does nothing
    void SkipMix(u8 index); // 0x0058CFB4
    // AddMix(MIX_MUTE)
    void AddMuteMix(); // 0x0058CF90
    // the mix of the sequence (2/3/5/6 by the flags)
    void BeginSequence(u8 flag); // 0x0058D10C
    void EndSequence(); // 0x0058D054
    // clears StageOpusMgr::FLAG_8 (unless Func58E078)
    void Leave(); // 0x0058D1C8
    // the mixes outside s_KeepMask go
    void MaskMixes(); // 0x0058D1EC
    // all tracks silent at once, only the mute mix
    void Reset(); // 0x0058D370
    void UpdateVolumes(); // 0x0058D218
    void Update(); // 0x0058D3F4
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsMixActive(u8 index) const // 0x007540A4
    {
        return (m_ActiveMixes & (1ull << index)) != 0;
    }

    static const Mix s_Mixes[MIX_NUM + 1];
    static const u64 s_KeepMask;

    u8 m_SequenceMix;           // 0x00 MIX_NONE: none
    f32 m_Volumes[TRACK_NUM];   // 0x04
    u64 m_ActiveMixes;          // 0x18
    u64 m_SkippedMixes;         // 0x20
    u8 m_TrackMixes[TRACK_NUM]; // 0x28 the mix that decides each track
    u8 _2D;                     // 0x2D
    u8 m_IsSequenceAlt;         // 0x2E
    u8 m_IsSequenceDisabled;    // 0x2F
    u8 m_SequenceFlag;          // 0x30
    u8 m_IsSequenceAlt2;        // 0x31
    u8 m_IsSequenceSlow;        // 0x32
    u8 m_KeepsMix34;            // 0x33
};
ASSERT_OFFSET(TrackMixer, m_ActiveMixes, 0x18);
ASSERT_OFFSET(TrackMixer, m_TrackMixes, 0x28);
ASSERT_SIZE(TrackMixer, 0x38);

// the weather of the music: follows the weather of the hour (SSetRainState)
struct BgmWeather
{
    u8 isActive; // 0x0
    u8 weather;  // 0x1
    u8 state;    // 0x2
};

DECOMP_NOINLINE TrackMixer* GetTrackMixer(); // 0x00584C1C
// AddMix(21) / RemoveMix(21) on the mixer
void AddMix21(); // 0x0058CFE8
void RemoveMix21(); // 0x0058D0C4
void StartWeather(); // 0x0058D480
void RefreshWeather(); // 0x0058D4C8
void StopWeather(); // 0x0058D524
void UpdateWeather(); // 0x0058D538

extern TrackMixer s_TrackMixer;
extern u32 s_EventFade;
} // namespace bgm
} // namespace audio
