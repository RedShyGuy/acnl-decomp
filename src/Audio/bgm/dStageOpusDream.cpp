#include "Audio/bgm/dStageOpusDream.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sound/sound_Extern.h"

// 0x00586A94
void audio::bgm::StageOpusDream::Setup(stage::Name)
{
    if (!Func6A6640()) {
        m_IsReady = 0;
    } else {
        m_IsReady = 1;
        Start();
        GetTrackMixer()->AddMix(36);
    }
    s_ClockState.isShop = 1;
}

// 0x00586AE4
void audio::bgm::StageOpusDream::Enter()
{
    if (Func6A6640()) {
        GetTrackMixer()->m_IsSequenceSlow = 1;
        Stop();
    }
}

// 0x00586B1C
void audio::bgm::StageOpusDream::Leave(stage::Name)
{
    Stop();
}

// 0x00586B28
void audio::bgm::StageOpusDream::OnStart()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_SHOP, STRM_BGM_IND_YUMEMI, 20, times);
}

// 0x00586B60
void audio::bgm::StageOpusDream::Update()
{
    if (m_IsCurrent || m_IsReady || !IsPlayerReady()) {
        return;
    }
    m_IsReady = 1;
    Start();
}

// 0x00586BA8
void audio::bgm::StageOpusDream::OnStop()
{
    if (Func6A6640()) {
        GetRequestMgr()->SetFade(KIND_SHOP, STRM_BGM_IND_YUMEMI, 0, 0, 60);
        return;
    }
    GetRequestMgr()->SetFade(KIND_SHOP, STRM_BGM_IND_YUMEMI, 0, 0, 0);
}

// 0x00586C1C
// 0x00586C18 (deleting dtor)
audio::bgm::StageOpusDream::~StageOpusDream()
{
    // empty (in the original too)
}
