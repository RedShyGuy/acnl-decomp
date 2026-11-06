#include "Audio/bgm/dStageOpusPrologue3.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/dSoundLabel.h"

// 0x005886DC
void audio::bgm::StageOpusPrologue3::Initialize()
{
    m_IsDelayed = 1;
}

// 0x005886E8
void audio::bgm::StageOpusPrologue3::Leave(stage::Name)
{
    m_IsSwitching = 1;
    Stop();
}

// 0x005886FC
void audio::bgm::StageOpusPrologue3::OnStart()
{
    if (m_IsDelayed) {
        RequestMgr::Times times = {10, -1, 0};
        GetRequestMgr()->Request(KIND_SHOP, STRM_BGM_IND_OFFICE, 20, times);
        m_IsDelayed = 0;
        return;
    }
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_SHOP, STRM_BGM_IND_OFFICE, 20, times);
}

// 0x00588778
void audio::bgm::StageOpusPrologue3::OnStop()
{
    if (m_IsSwitching) {
        GetRequestMgr()->Remove(KIND_SHOP, STRM_BGM_IND_OFFICE);
        return;
    }
    GetRequestMgr()->SetFade(KIND_SHOP, STRM_BGM_IND_OFFICE, 5, 0, 0);
}

// 0x005887DC
// 0x005887D8 (deleting dtor)
audio::bgm::StageOpusPrologue3::~StageOpusPrologue3()
{
    // empty (in the original too)
}
