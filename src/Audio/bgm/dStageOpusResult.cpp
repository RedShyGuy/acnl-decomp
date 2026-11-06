#include "Audio/bgm/dStageOpusResult.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"

// 0x005874F0
void audio::bgm::StageOpusResult::Initialize()
{
    m_IsFinished = 0;
}

// 0x005874FC
void audio::bgm::StageOpusResult::Setup(stage::Name)
{
    GetTrackMixer()->AddMix(8);
}

// 0x00587510
void audio::bgm::StageOpusResult::Ready()
{
    m_IsFinished = 1;
    SoundMgr::SPlayBgmNow(STRM_BGM_ISL_AWARD);
}

// 0x00587524
void audio::bgm::StageOpusResult::Leave(stage::Name)
{
    GetTrackMixer()->RemoveMix(8);
    if (m_IsFinished) {
        SoundMgr::SStopBgmFade(1);
    }
}

// 0x00587558
// 0x00587554 (deleting dtor)
audio::bgm::StageOpusResult::~StageOpusResult()
{
    // empty (in the original too)
}
