#include "Audio/bgm/dStageOpusCamping.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sound/sound_Extern.h"
#include "time/time_Api.h"

// 0x0058762C
void audio::bgm::StageOpusCamping::Setup(stage::Name)
{
    m_IsReady = 0;
    m_IsSuspended = 0;
    m_BgmId = 0;
    m_NextBgmId = 0;
    m_State = STATE_STOPPED;
    m_IsSwitching = 0;
    m_IsClockChanged = 0;
    m_WaitsForChime = 1;
    s_ClockState.isShop = 0;
}

// 0x00587664
void audio::bgm::StageOpusCamping::Ready()
{
    m_Value = 7;
}

// 0x00587670
void audio::bgm::StageOpusCamping::Leave(stage::Name)
{
    Stop();
}

// 0x0058767C
void audio::bgm::StageOpusCamping::OnStart()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(m_Kind, m_BgmId, 20, times);
}

// 0x005876B4
void audio::bgm::StageOpusCamping::Update()
{
    if (m_IsSuspended || m_IsCurrent) {
        return;
    }
    bool force = false;
    if (!m_IsReady) {
        if (!(IsPlayerReady() | s_TownState.isCampForced)) {
            return;
        }
        m_IsReady = 1;
        u32 id = GetBgmId(false);
        m_BgmId = id;
        m_NextBgmId = id;
        force = true;
        s_TownState.isCampForced = 0;
    }
    UpdateHourly(false, true, force);
}

// 0x00587740
void audio::bgm::StageOpusCamping::OnStop()
{
    if (m_IsSwitching || m_IsCurrent) {
        GetRequestMgr()->SetFade(m_Kind, m_BgmId, 0, 0, 0);
        m_IsSwitching = 0;
        return;
    }
    if (m_IsSuspended) {
        GetRequestMgr()->SetFade(m_Kind, m_BgmId, m_Fade, 0, 0);
        return;
    }
    GetRequestMgr()->SetFade(m_Kind, m_BgmId, GetFadeToHour(), 0, 0);
}

// 0x005877E8
u32 audio::bgm::StageOpusCamping::GetBgmId(bool isNextHour)
{
    // (initialized from the images 0x008468A4 and 0x008468C0)
    const u32 day[7] = {STRM_BGM_AUTOCAMP_DAY_SUNNY,  STRM_BGM_AUTOCAMP_DAY_SUNNY,  STRM_BGM_AUTOCAMP_DAY_SUNNY,
                        STRM_BGM_AUTOCAMP_DAY_RAINY,  STRM_BGM_AUTOCAMP_DAY_RAINY,  STRM_BGM_AUTOCAMP_DAY_SNOWY,
                        STRM_BGM_AUTOCAMP_DAY_SNOWY};
    const u32 night[7] = {STRM_BGM_AUTOCAMP_NIGHT_SUNNY, STRM_BGM_AUTOCAMP_NIGHT_SUNNY, STRM_BGM_AUTOCAMP_NIGHT_SUNNY,
                          STRM_BGM_AUTOCAMP_NIGHT_RAINY, STRM_BGM_AUTOCAMP_NIGHT_RAINY, STRM_BGM_AUTOCAMP_NIGHT_SNOWY,
                          STRM_BGM_AUTOCAMP_NIGHT_SNOWY};
    nn::fnd::DateTimeParameters date = *time::GetCurrentDate();
    if (isNextHour) {
        time::AddHours(&date, 1);
    }
    const u32* table = static_cast<u32>(date.hour - 6) <= 12 ? day : night;
    u8 weather = isNextHour ? Func62FEC8() : Func62FB14();
    return table[weather];
}

// 0x00587888
// 0x00587884 (deleting dtor)
audio::bgm::StageOpusCamping::~StageOpusCamping()
{
    // empty (in the original too)
}
