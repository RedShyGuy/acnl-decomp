#include "Audio/bgm/dBgmTrackMixer.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "time/time_Api.h"
#include "Audio/bgm/dBgmMgr.h"

namespace audio {
namespace bgm {

// the tables of dBgmConstants.cpp
extern const u8 s_WeatherStatesFromDry[7];
extern const u8 s_WeatherStates[7];

// the fade of the mixes in this frame (0: the fade of the mix)
// 0x0094F4C0
u32 s_EventFade = 1;
// 0x0094F498
BgmWeather s_BgmWeather;
// the date of the last weather check
// 0x0098B068
nn::fnd::DateTimeParameters s_WeatherDate;

} // namespace bgm
} // namespace audio

// 0x0058D410
audio::bgm::TrackMixer::TrackMixer()
    : m_SequenceMix(MIX_MUTE), m_ActiveMixes(0), m_SkippedMixes(0), m_IsSequenceAlt(0), m_IsSequenceDisabled(0),
      m_IsSequenceSlow(0), m_KeepsMix34(0)
{
    for (s32 i = 0; i < TRACK_NUM; i++) {
        m_Volumes[i] = 1.0f;
        m_TrackMixes[i] = MIX_NONE;
    }
}

// 0x0058CF90
void audio::bgm::TrackMixer::AddMuteMix()
{
    u32 skipped = static_cast<u32>(m_SkippedMixes);
    if (skipped & (1 << MIX_MUTE)) {
        m_SkippedMixes = (m_SkippedMixes & ~0xFFFFFFFFull) | (skipped & ~(1 << MIX_MUTE));
    } else {
        m_ActiveMixes |= 1 << MIX_MUTE;
    }
}

// 0x0058CFB4
void audio::bgm::TrackMixer::SkipMix(u8 index)
{
    m_SkippedMixes |= 1ull << index;
}

// 0x0058CFFC
void audio::bgm::TrackMixer::AddMix(u8 index)
{
    u64 bit = 1ull << index;
    if (m_SkippedMixes & bit) {
        m_SkippedMixes &= ~bit;
    } else {
        m_ActiveMixes |= bit;
    }
}

// 0x0058D054
void audio::bgm::TrackMixer::EndSequence()
{
    if (m_SequenceFlag && !m_KeepsMix34) {
        m_ActiveMixes &= ~(1ull << 34);
    }
    u8 index = m_SequenceMix;
    m_KeepsMix34 = 0;
    if (index != MIX_NONE) {
        m_ActiveMixes &= ~(1ull << index);
    }
    m_IsSequenceAlt = 0;
    m_IsSequenceDisabled = 0;
    m_IsSequenceSlow = 0;
}

// 0x0058D0D8
void audio::bgm::TrackMixer::RemoveMix(u8 index)
{
    m_ActiveMixes &= ~(1ull << index);
}

// 0x0058D10C
void audio::bgm::TrackMixer::BeginSequence(u8 flag)
{
    if (SoundMgr::SIsReset()) {
        return;
    }
    m_SequenceFlag = flag;
    m_SequenceMix = MIX_NONE;
    if (m_IsSequenceDisabled) {
        return;
    }
    u8 index;
    if (!m_IsSequenceAlt && !m_IsSequenceAlt2) {
        index = m_IsSequenceSlow ? 5 : 2;
    } else {
        index = m_IsSequenceSlow ? 6 : 3;
    }
    m_SequenceMix = index;
    if (index == MIX_NONE) {
        return;
    }
    AddMix(index);
}

// 0x0058D1C8
void audio::bgm::TrackMixer::Leave()
{
    if (!SoundMgr::SIsReset()) {
        GetStageOpusMgr()->m_Flags &= ~StageOpusMgr::FLAG_8;
    }
}

// 0x0058D1EC
void audio::bgm::TrackMixer::MaskMixes()
{
    m_ActiveMixes &= s_KeepMask;
}

// 0x0058D218
void audio::bgm::TrackMixer::UpdateVolumes()
{
    for (s32 track = 0; track < TRACK_NUM; track++) {
        u64 active = m_ActiveMixes;
        u8 index = MIX_NONE;
        for (s32 i = 0; i < MIX_NUM; i++) {
            if ((active & (1ull << i)) && s_Mixes[i].volumes[track] >= 0.0f) {
                index = i;
                break;
            }
        }
        u8 last = m_TrackMixes[track];
        if (last == index) {
            continue;
        }
        // a mix with a lower index fades in, back to a higher one with the fade out of the old mix
        s32 fade = last > index ? s_Mixes[index].fadeIn : s_Mixes[last].fadeOut;
        f32 volume = s_Mixes[index].volumes[track];
        if (fade == 0) {
            fade = s_EventFade;
        }
        if (volume != m_Volumes[track]) {
            SoundMgr::SSetVolume(track, volume, fade);
            m_Volumes[track] = s_Mixes[index].volumes[track];
        }
        m_TrackMixes[track] = index;
    }
    s_EventFade = 0;
}

// 0x0058D370
void audio::bgm::TrackMixer::Reset()
{
    m_SequenceMix = MIX_MUTE;
    m_IsSequenceAlt2 = 0;
    m_ActiveMixes = 0;
    AddMuteMix();
    for (s32 track = 0; track < TRACK_NUM; track++) {
        m_Volumes[track] = 0.0f;
        SoundMgr::SSetVolume(track, 0.0f, 1);
        m_TrackMixes[track] = MIX_MUTE;
    }
}

// 0x0058D3F4
void audio::bgm::TrackMixer::Update()
{
    UpdateVolumes();
    s_EventFade = 1;
}

// 0x00584C1C
audio::bgm::TrackMixer* audio::bgm::GetTrackMixer()
{
    return &s_TrackMixer;
}

// 0x0058CFE8
void audio::bgm::AddMix21()
{
    GetTrackMixer()->AddMix(21);
}

// 0x0058D0C4
void audio::bgm::RemoveMix21()
{
    GetTrackMixer()->RemoveMix(21);
}

// 0x0058D480
void audio::bgm::StartWeather()
{
    s_WeatherDate = *time::GetCurrentDate();
    u8 weather = Func62FB14();
    s_BgmWeather.weather = weather;
    s_BgmWeather.state = s_WeatherStates[weather];
    SoundMgr::SSetRainState(s_BgmWeather.state);
    s_BgmWeather.isActive = 1;
}

// 0x0058D4C8
void audio::bgm::RefreshWeather()
{
    s_BgmWeather.isActive = 1;
    u8 weather = Func62FB14();
    if (s_BgmWeather.weather == weather) {
        return;
    }
    s_BgmWeather.weather = weather;
    s_WeatherDate = *time::GetCurrentDate();
    s_BgmWeather.state = s_WeatherStates[s_BgmWeather.weather];
    SoundMgr::SSetRainState(s_BgmWeather.state);
}

// 0x0058D524
void audio::bgm::StopWeather()
{
    s_BgmWeather.isActive = 0;
}

// 0x0058D538
void audio::bgm::UpdateWeather()
{
    if (!s_BgmWeather.isActive) {
        return;
    }
    nn::fnd::DateTimeParameters date = *time::GetCurrentDate();
    if (date.hour == s_WeatherDate.hour && date.day == s_WeatherDate.day && date.month == s_WeatherDate.month &&
        date.year == s_WeatherDate.year) {
        return;
    }
    u8 weather = Func62FB14();
    if (weather != s_BgmWeather.weather) {
        const u8* states = s_BgmWeather.weather < 3 ? s_WeatherStatesFromDry : s_WeatherStates;
        s_BgmWeather.state = states[weather];
        s_BgmWeather.weather = weather;
        SoundMgr::SSetRainState(s_BgmWeather.state);
    }
    s_WeatherDate = date;
}
