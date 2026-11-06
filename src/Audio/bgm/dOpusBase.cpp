#include "Audio/bgm/dOpusBase.h"
#include "Sound/dSoundMgr.h"
#include "time/time_Api.h"

namespace audio {
namespace bgm {
namespace {

// the time chime of the hour plays (the music waits for its end, except at midnight of New Year's Day)
inline bool IsWaitingForChime(const nn::fnd::DateTimeParameters* date)
{
    if (!SoundMgr::SIsTimeChimeActive()) {
        return false;
    }
    return date->day != 1 || date->month != 1 || date->hour != 0;
}

} // namespace
} // namespace bgm
} // namespace audio

// 0x0058CC9C
void audio::bgm::OpusBase::OnStart()
{
    // empty (in the original too)
}

// 0x0058CCA0
void audio::bgm::OpusBase::OnStop()
{
    // empty (in the original too)
}

// 0x0058CCA4
bool audio::bgm::OpusBase::Start()
{
    if (m_IsPlaying) {
        return false;
    }
    const nn::fnd::DateTimeParameters* date = time::GetCurrentDate();
    if (m_WaitsForChime && IsWaitingForChime(date)) {
        return false;
    }
    m_IsPlaying = 1;
    OnStart();
    return true;
}

// 0x0058CD20
bool audio::bgm::OpusBase::Stop()
{
    if (m_IsPlaying != 1) {
        return false;
    }
    m_IsPlaying = 0;
    OnStop();
    m_IsSwitching = 0;
    return true;
}
