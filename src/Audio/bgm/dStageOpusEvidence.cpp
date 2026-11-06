#include "Audio/bgm/dStageOpusEvidence.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Sound/dSoundLabel.h"

// 0x00587A38
void audio::bgm::StageOpusEvidence::Setup(stage::Name)
{
    SoundMgr::SPlayJingleDelayed(STRM_BGM_UPA_OPEN);
    m_IsReady = 0;
}

// 0x00587A58
void audio::bgm::StageOpusEvidence::Update()
{
    if (m_IsCurrent || m_IsReady || !IsPlayerReady()) {
        return;
    }
    m_IsReady = 1;
    SoundMgr::SStartJinglePrepared();
}

// 0x00587A98
// 0x00587A94 (deleting dtor)
audio::bgm::StageOpusEvidence::~StageOpusEvidence()
{
    // empty (in the original too)
}
