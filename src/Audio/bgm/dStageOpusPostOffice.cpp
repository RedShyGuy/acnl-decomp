#include "Audio/bgm/dStageOpusPostOffice.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/dSoundLabel.h"

// 0x00588BD8
void audio::bgm::StageOpusPostOffice::Setup(stage::Name)
{
    m_IsReady = 0;
    s_ClockState.isShop = 1;
}

// 0x00588BF4
void audio::bgm::StageOpusPostOffice::Leave(stage::Name)
{
    Stop();
}

// 0x00588C00
void audio::bgm::StageOpusPostOffice::OnStart()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_SHOP, s_TownState.flag5 ? STRM_BGM_IND_POST_DAY : STRM_BGM_IND_POST_NIGHT, 20, times);
}

// 0x00588C50
void audio::bgm::StageOpusPostOffice::Update()
{
    if (m_IsCurrent || m_IsReady || !IsPlayerReady()) {
        return;
    }
    m_IsReady = 1;
    Start();
}

// 0x00588C98
void audio::bgm::StageOpusPostOffice::OnStop()
{
    GetRequestMgr()->MarkRemoved(KIND_SHOP, s_TownState.flag5 ? STRM_BGM_IND_POST_DAY : STRM_BGM_IND_POST_NIGHT);
}

// 0x00588CD0
// 0x00588CCC (deleting dtor)
audio::bgm::StageOpusPostOffice::~StageOpusPostOffice()
{
    // empty (in the original too)
}
