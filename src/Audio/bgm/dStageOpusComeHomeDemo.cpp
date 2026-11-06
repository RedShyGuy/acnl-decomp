#include "Audio/bgm/dStageOpusComeHomeDemo.h"
#include "Audio/bgm/dBgmTrackMixer.h"

// 0x005891CC
void audio::bgm::StageOpusComeHomeDemo::Setup(stage::Name)
{
    GetTrackMixer()->AddMix(8);
}

// 0x005891E0
void audio::bgm::StageOpusComeHomeDemo::Leave(stage::Name)
{
    GetTrackMixer()->RemoveMix(8);
}

// 0x005891F8
// 0x005891F4 (deleting dtor)
audio::bgm::StageOpusComeHomeDemo::~StageOpusComeHomeDemo()
{
    // empty (in the original too)
}
