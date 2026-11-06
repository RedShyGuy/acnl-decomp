#include "Audio/bgm/dStageOpusRetireDemo.h"
#include "Audio/bgm/dBgmTrackMixer.h"

// 0x00588CDC
void audio::bgm::StageOpusRetireDemo::Setup(stage::Name)
{
    GetTrackMixer()->AddMix(8);
}

// 0x00588CF0
void audio::bgm::StageOpusRetireDemo::Leave(stage::Name)
{
    GetTrackMixer()->RemoveMix(8);
}

// 0x00588D08
// 0x00588D04 (deleting dtor)
audio::bgm::StageOpusRetireDemo::~StageOpusRetireDemo()
{
    // empty (in the original too)
}
