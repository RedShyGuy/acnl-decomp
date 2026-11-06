#include "Audio/bgm/dStageOpusCeremony.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/dSoundLabel.h"

// 0x0058799C
void audio::bgm::StageOpusCeremony::Setup(stage::Name)
{
    GetTrackMixer()->AddMix(21);
}

// 0x005879B0
void audio::bgm::StageOpusCeremony::Ready()
{
    Start();
}

// 0x005879BC
void audio::bgm::StageOpusCeremony::Leave(stage::Name)
{
    Stop();
    GetTrackMixer()->RemoveMix(21);
}

// 0x005879DC
void audio::bgm::StageOpusCeremony::OnStart()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_STAGE, STRM_BGM_FLD_CEREMONY, 20, times);
}

// 0x00587A14
void audio::bgm::StageOpusCeremony::OnStop()
{
    GetRequestMgr()->Remove(KIND_STAGE, STRM_BGM_FLD_CEREMONY);
}

// 0x00587A34
// 0x00587A30 (deleting dtor)
audio::bgm::StageOpusCeremony::~StageOpusCeremony()
{
    // empty (in the original too)
}
