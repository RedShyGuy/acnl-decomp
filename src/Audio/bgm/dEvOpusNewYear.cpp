#include "Audio/bgm/dEvOpusNewYear.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "time/time_Api.h"

// 0x00585FFC
void audio::bgm::EvOpusNewYear::Begin()
{
    m_Fade = 20;
    const nn::fnd::DateTimeParameters* date = time::GetCurrentDate();
    if (date->month == 1 && date->day == 1 && date->hour == 0 && date->minute == 0 && date->second == 0) {
        // the stroke of midnight
        SoundMgr::SPlayMuffledSound(SE_EVT_COUNTDOWN_COUNT_F);
        SoundMgr::SetNextBgmParam(1);
    }
}

// 0x0058605C
void audio::bgm::EvOpusNewYear::OnStart()
{
    m_BgmId = time::GetCurrentDate()->hour < 2 ? STRM_BGM_EVT_NEWYEAR : STRM_BGM_EVT_COUNTDOWN_0200;
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_STAGE_EVENT, m_BgmId, 5, times);
}

// 0x005860B8
void audio::bgm::EvOpusNewYear::Update()
{
    if (s_ClockState.isNearHour) {
        if (m_IsPlaying) {
            m_Fade = GetFadeToHour();
            Stop();
        }
    } else if (!m_IsPlaying) {
        Start();
    }
}

// 0x0058611C
void audio::bgm::EvOpusNewYear::OnStop()
{
    GetRequestMgr()->SetFade(KIND_STAGE_EVENT, m_BgmId, m_Fade, 0, 0);
}

// 0x00586150
void audio::bgm::EvOpusNewYear::End(bool isReplaced)
{
    m_Fade = isReplaced ? 20 : 0;
    Stop();
}

// 0x0058616C
// 0x00586168 (deleting dtor)
audio::bgm::EvOpusNewYear::~EvOpusNewYear()
{
    // empty (in the original too)
}
