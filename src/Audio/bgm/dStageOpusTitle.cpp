#include "Audio/bgm/dStageOpusTitle.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"

// 0x00586C80
void audio::bgm::StageOpusTitle::Setup(stage::Name)
{
    GetTrackMixer()->AddMix(35);
    GetStageOpusMgr()->m_Flags |= StageOpusMgr::FLAG_8;
    s_ClockState.isShop = 0;
}

// 0x00586CB4
void audio::bgm::StageOpusTitle::Ready()
{
    Start();
}

// 0x00586CC0
void audio::bgm::StageOpusTitle::Leave(stage::Name)
{
    Stop();
    GetTrackMixer()->RemoveMix(35);
}

// 0x00586CE0
void audio::bgm::StageOpusTitle::OnStart()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_COMMAND, STRM_BGM_TITLE, 20, times);
}

// 0x00586D14
void audio::bgm::StageOpusTitle::Update()
{
    if (!m_IsCurrent && !SoundMgr::SIsBgmActive()) {
        GetTrackMixer()->RemoveMix(35);
    }
}

// 0x00586D44
void audio::bgm::StageOpusTitle::OnStop()
{
    GetRequestMgr()->SetFade(KIND_COMMAND, STRM_BGM_TITLE, 0, 0, 0);
}

// 0x00586D78
// 0x00586D74 (deleting dtor)
audio::bgm::StageOpusTitle::~StageOpusTitle()
{
    // empty (in the original too)
}
