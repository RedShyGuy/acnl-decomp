#include "Audio/bgm/dStageOpusChipTutorial.h"
#include "Audio/bgm/dBgmTrackMixer.h"

// 0x00589108
void audio::bgm::StageOpusChipTutorial::Setup(stage::Name stage)
{
    StageOpusShopBase::Setup(stage);
    GetTrackMixer()->AddMix(21);
}

// 0x005891A4
void audio::bgm::StageOpusChipTutorial::Leave(stage::Name)
{
    Stop();
    GetTrackMixer()->RemoveMix(21);
}

// 0x005891C8
// 0x005891C4 (deleting dtor)
audio::bgm::StageOpusChipTutorial::~StageOpusChipTutorial()
{
    // empty (in the original too)
}
