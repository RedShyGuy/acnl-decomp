#include "Audio/bgm/dEvOpusCountdown.h"
#include "Audio/bgm/dBgmClock.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/dSoundLabel.h"
#include "time/time_Api.h"

namespace audio {
namespace bgm {
namespace {

// the frames (30 per second) until the time of the hour, in milliseconds (the milliseconds of the clock are added, kept
// from the original)
inline s32 GetFramesUntil(s32 time)
{
    const nn::fnd::DateTimeParameters* date = time::GetCurrentDate();
    s32 ms = date->milliSecond + (time - (date->minute * 60 + date->second) * 1000);
    if (ms < 0) {
        ms = 0;
    }
    return ms * 30 / 1000;
}

} // namespace
} // namespace bgm
} // namespace audio

// 0x00586DB4
void audio::bgm::EvOpusCountdown::Begin()
{
    m_Second = IsInTime(59, 0, 0, 0) ? GetSecond() : -1;
    m_Fade = 20;
}

// 0x00586DEC
void audio::bgm::EvOpusCountdown::OnStart()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_STAGE_EVENT, m_BgmId, 20, times);
}

// 0x00586E24
void audio::bgm::EvOpusCountdown::Update()
{
    if (m_IsPlaying) {
        s32 time;
        if (IsInTime(29, 45, 30, 0)) {
            time = (29 * 60 + 55) * 1000;
        } else if (IsInTime(49, 45, 50, 0)) {
            time = (49 * 60 + 55) * 1000;
        } else if (IsInTime(54, 45, 55, 0)) {
            time = (54 * 60 + 55) * 1000;
        } else if (IsInTime(59, 0, 0, 0)) {
            time = (59 * 60 + 15) * 1000;
        } else {
            u32 id = m_BgmId;
            if (IsInTime(0, 15, 29, 50)) {
                id = STRM_BGM_EVT_COUNTDOWN_2300;
            } else if (IsInTime(30, 0, 49, 50)) {
                id = STRM_BGM_EVT_COUNTDOWN_2330;
            } else if (IsInTime(50, 0, 54, 50)) {
                id = STRM_BGM_EVT_COUNTDOWN_2350;
            } else if (IsInTime(55, 0, 59, 0)) {
                id = STRM_BGM_EVT_COUNTDOWN_2355;
            }
            if (m_BgmId == id) {
                return;
            }
            m_Fade = 0;
            Stop();
            return;
        }
        m_Fade = GetFramesUntil(time);
        Stop();
        return;
    }
    if (IsInTime(0, 15, 29, 45)) {
        m_BgmId = STRM_BGM_EVT_COUNTDOWN_2300;
    } else if (IsInTime(30, 0, 49, 45)) {
        m_BgmId = STRM_BGM_EVT_COUNTDOWN_2330;
    } else if (IsInTime(50, 0, 54, 45)) {
        m_BgmId = STRM_BGM_EVT_COUNTDOWN_2350;
    } else if (IsInTime(55, 0, 59, 0)) {
        m_BgmId = STRM_BGM_EVT_COUNTDOWN_2355;
    } else {
        return;
    }
    Start();
}

// 0x00587088
void audio::bgm::EvOpusCountdown::OnStop()
{
    GetRequestMgr()->SetFade(KIND_STAGE_EVENT, m_BgmId, m_Fade, 0, 0);
    m_Fade = 20;
}

// 0x005870C4
void audio::bgm::EvOpusCountdown::End(bool isReplaced)
{
    if (!isReplaced) {
        m_Fade = 0;
    }
    Stop();
}

// 0x005870E0
// 0x005870DC (deleting dtor)
audio::bgm::EvOpusCountdown::~EvOpusCountdown()
{
    // empty (in the original too)
}
