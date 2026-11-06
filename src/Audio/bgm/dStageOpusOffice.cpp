#include "Audio/bgm/dStageOpusOffice.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"

// 0x005873DC
void audio::bgm::StageOpusOffice::Setup(stage::Name)
{
    m_IsReady = 0;
    s_ClockState.isShop = 1;
}

// 0x005873F8
void audio::bgm::StageOpusOffice::Leave(stage::Name)
{
    Stop();
    if (m_IsPlantDemo == 1) {
        SoundMgr::SSetPlantTree();
    }
}

// 0x00587428
void audio::bgm::StageOpusOffice::OnStart()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_SHOP, STRM_BGM_IND_OFFICE, 20, times);
}

// 0x00587460
void audio::bgm::StageOpusOffice::Update()
{
    if (m_IsCurrent || m_IsReady || !IsPlayerReady()) {
        return;
    }
    m_IsReady = 1;
    Start();
}

// 0x005874A8
void audio::bgm::StageOpusOffice::OnStop()
{
    if (m_IsPlantDemo != 1) {
        GetRequestMgr()->Remove(KIND_SHOP, STRM_BGM_IND_OFFICE);
        return;
    }
    GetRequestMgr()->m_Player.Reset();
}

// 0x005874EC
// 0x005874E8 (deleting dtor)
audio::bgm::StageOpusOffice::~StageOpusOffice()
{
    // empty (in the original too)
}
