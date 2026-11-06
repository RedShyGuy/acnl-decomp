#include "Audio/bgm/dStageOpusShopBase.h"
#include "Audio/bgm/dBgmClock.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Sound/dSoundLabel.h"
#include "netgame/netgame_Api.h"
#include "time/time_Api.h"

// 0x00587D08
void audio::bgm::StageOpusShopBase::Setup(stage::Name)
{
    m_IsReady = 0;
    InitCloseHour();
    m_LastHour = m_CloseHour - 1;
    m_LastSecond = GetSecond();
    s_ClockState.isShop = 1;
    if (!s_TownState.isShopMixOff && m_HasMix36 == 1 && (m_HasMix36Online || !netgame::IsMultiplePlayer())) {
        GetTrackMixer()->AddMix(36);
    }
}

// 0x00587D94
void audio::bgm::StageOpusShopBase::Leave(stage::Name)
{
    Stop();
}

// 0x00587DA0
void audio::bgm::StageOpusShopBase::InitCloseHour()
{
    m_CloseHour = 32;
}

// 0x00587DAC
void audio::bgm::StageOpusShopBase::EnterKeep()
{
    GetTrackMixer()->m_IsSequenceAlt = 1;
}

// 0x00587DC0
void audio::bgm::StageOpusShopBase::OnStart()
{
    if (m_ShopState == SHOP_CLOSED) {
        RequestMgr::Times times = {0, -1, 0};
        GetRequestMgr()->Request(KIND_SHOP_CLOSE, STRM_BGM_IND_KODANUKI_CLOSE, 1, times);
    } else if (m_ShopState == SHOP_OPEN) {
        RequestMgr::Times times = {0, -1, 0};
        GetRequestMgr()->Request(KIND_SHOP, m_ShopBgmId, 1, times);
    }
}

// 0x00587E30
void audio::bgm::StageOpusShopBase::Update()
{
    if (m_IsCurrent) {
        return;
    }
    if (!m_IsReady) {
        if (!IsPlayerReady()) {
            return;
        }
        m_IsReady = 1;
    }
    if (m_ShopState == SHOP_CLOSED) {
        return;
    }
    if (!UpdateSecond(&m_LastSecond)) {
        return;
    }
    u8 hour = GetHour();
    if (hour < 6) {
        hour += 24;
    }
    u8 state;
    if (m_LastHour == hour) {
        if (IsInTime(49, 50, 50, 0)) {
            state = SHOP_CLOSING;
        } else if (GetMinute() >= 50) {
            state = SHOP_CLOSED;
        } else {
            state = SHOP_OPEN;
        }
    } else if (m_CloseHour > hour) {
        state = SHOP_OPEN;
    } else {
        state = SHOP_CLOSED;
    }
    if (m_ShopState == state) {
        return;
    }
    m_ShopState = state;
    Stop();
    Start();
}

// 0x00587F38
void audio::bgm::StageOpusShopBase::OnStop()
{
    if (m_ShopState != SHOP_CLOSING) {
        GetRequestMgr()->SetFade(KIND_SHOP_CLOSE, STRM_BGM_IND_KODANUKI_CLOSE, 0, 0, 0);
        GetRequestMgr()->SetFade(KIND_SHOP, m_ShopBgmId, 0, 0, 0);
        return;
    }
    // the music fades out until hh:50
    const nn::fnd::DateTimeParameters* date = time::GetCurrentDate();
    s32 ms = 60000 - (date->milliSecond + date->second * 1000);
    GetRequestMgr()->SetFade(KIND_SHOP, m_ShopBgmId, ms * 30 / 1000 - 1, 0, 0);
}
