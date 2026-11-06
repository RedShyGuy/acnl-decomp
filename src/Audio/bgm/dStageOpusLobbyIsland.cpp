#include "Audio/bgm/dStageOpusLobbyIsland.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sound/sound_Extern.h"
#include "time/time_Api.h"

namespace audio {
namespace bgm {
namespace {

// the weather of the hour is rain (3, 4) or it rains
inline bool IsRaining()
{
    u8 weather = Func62FB14();
    return weather == 3 || weather == 4 || Func22A20C() != 0;
}

} // namespace
} // namespace bgm
} // namespace audio

// 0x00588DD0
void audio::bgm::StageOpusLobbyIsland::Initialize()
{
    m_IsRaining = 0;
}

// 0x00588DDC
void audio::bgm::StageOpusLobbyIsland::Setup(stage::Name)
{
    m_IsReady = 0;
    if (IsRaining()) {
        m_IsRaining = 1;
    }
    m_BgmId = 0;
    m_NextBgmId = 0;
    m_State = STATE_STOPPED;
    m_IsSwitching = 0;
    m_IsClockChanged = 0;
    m_WaitsForChime = 1;
}

// 0x00588E2C
void audio::bgm::StageOpusLobbyIsland::Leave(stage::Name)
{
    Stop();
    if (m_IsRaining) {
        GetRequestMgr()->Remove(17, 0);
    }
}

// 0x00588E68
void audio::bgm::StageOpusLobbyIsland::OnStart()
{
    if (m_IsRaining) {
        return;
    }
    RequestMgr::EventParam param = {20, 0, -1, 0};
    GetRequestMgr()->RequestEvent(19, m_BgmId, 18, param, 0);
}

// 0x00588EC0
void audio::bgm::StageOpusLobbyIsland::Update()
{
    if (m_IsCurrent) {
        return;
    }
    bool force = false;
    if (!m_IsReady) {
        if (!IsPlayerReady()) {
            return;
        }
        m_IsReady = 1;
        m_BgmId = GetBgmId(false);
        m_NextBgmId = GetBgmId(false);
        force = true;
    }
    if (m_IsRaining && !IsRaining()) {
        RequestMgr::EventParam param = {0, 75, -1, 0};
        GetRequestMgr()->RequestEvent(19, m_BgmId, 18, param, 0);
        m_IsRaining = 0;
    } else if (!m_IsRaining && IsRaining()) {
        GetRequestMgr()->SetFade(19, m_BgmId, 410, 0, 0);
        m_IsRaining = 1;
    }
    UpdateHourly(false, true, force);
}

// 0x00589020
void audio::bgm::StageOpusLobbyIsland::OnStop()
{
    if (m_IsSwitching || m_IsCurrent) {
        GetRequestMgr()->SetFade(19, m_BgmId, 0, 0, 0);
        m_IsSwitching = 0;
        return;
    }
    if (m_IsRaining) {
        return;
    }
    GetRequestMgr()->SetFade(19, m_BgmId, GetFadeToHour(), 0, 0);
}

// 0x005890A8
u32 audio::bgm::StageOpusLobbyIsland::GetBgmId(bool isNextHour)
{
    nn::fnd::DateTimeParameters date = *time::GetCurrentDate();
    if (isNextHour) {
        time::AddHours(&date, 1);
    }
    return static_cast<u32>(date.hour - 7) < 12 ? STRM_BGM_ISL_LOBBY_DAY : STRM_BGM_ISL_LOBBY_NIGHT;
}

// 0x00589104
// 0x00589100 (deleting dtor)
audio::bgm::StageOpusLobbyIsland::~StageOpusLobbyIsland()
{
    // empty (in the original too)
}
