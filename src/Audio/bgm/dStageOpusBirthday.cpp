#include "Audio/bgm/dStageOpusBirthday.h"
#include "Audio/bgm/dBgmTrackMixer.h"

// 0x0058796C
void audio::bgm::StageOpusBirthday::Setup(stage::Name)
{
    GetTrackMixer()->AddMix(28);
}

// 0x00587980
void audio::bgm::StageOpusBirthday::Leave(stage::Name)
{
    GetTrackMixer()->RemoveMix(28);
}

// 0x00587998
// 0x00587994 (deleting dtor)
audio::bgm::StageOpusBirthday::~StageOpusBirthday()
{
    // empty (in the original too)
}
