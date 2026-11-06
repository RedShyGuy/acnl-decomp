#include "Audio/bgm/dStageOpusAward.h"
#include "Audio/bgm/dBgmTrackMixer.h"

// 0x005869D0
void audio::bgm::StageOpusAward::Setup(stage::Name stage)
{
    StageOpusShopBase::Setup(stage);
    GetTrackMixer()->AddMix(8);
}

// 0x00586A6C
void audio::bgm::StageOpusAward::Leave(stage::Name)
{
    Stop();
    GetTrackMixer()->RemoveMix(8);
}

// 0x00586A90
// 0x00586A8C (deleting dtor)
audio::bgm::StageOpusAward::~StageOpusAward()
{
    // empty (in the original too)
}
