#include "Audio/bgm/dStageOpusSelect.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/dSoundLabel.h"

// 0x0058755C
void audio::bgm::StageOpusSelect::Setup(stage::Name)
{
    s_ClockState.SetChime(false, false);
    GetStageOpusMgr()->m_Flags &= ~StageOpusMgr::FLAG_4;
}

// 0x00587588
void audio::bgm::StageOpusSelect::Ready()
{
    Start();
}

// 0x00587594
void audio::bgm::StageOpusSelect::Leave(stage::Name)
{
    Stop();
    GetStageOpusMgr()->m_Flags |= StageOpusMgr::FLAG_4;
}

// 0x005875B8
void audio::bgm::StageOpusSelect::OnStart()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_SHOP, STRM_BGM_PLAYERSELECT, 20, times);
}

// 0x005875F0
void audio::bgm::StageOpusSelect::OnStop()
{
    GetRequestMgr()->SetFade(KIND_SHOP, STRM_BGM_PLAYERSELECT, 0, 0, 0);
}

// 0x00587628
// 0x00587624 (deleting dtor)
audio::bgm::StageOpusSelect::~StageOpusSelect()
{
    // empty (in the original too)
}
