#include "Audio/bgm/dStageOpusPlantDemo.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Sound/dSoundMgr.h"

// 0x00583F24
void audio::bgm::StageOpusPlantDemo::OnStop()
{
    SoundMgr::SResetSub();
}

// 0x00584044
void audio::bgm::StageOpusPlantDemo::OnStart()
{
    SoundMgr::SPlayPrologue();
}

// 0x00588284
void audio::bgm::StageOpusPlantDemo::Setup(stage::Name)
{
    GetTrackMixer()->AddMix(8);
}

// 0x00588298
void audio::bgm::StageOpusPlantDemo::Ready()
{
    Start();
}

// 0x005882A4
void audio::bgm::StageOpusPlantDemo::Leave(stage::Name)
{
    Stop();
    GetTrackMixer()->RemoveMix(8);
}

// 0x005882C8
// 0x005882C4 (deleting dtor)
audio::bgm::StageOpusPlantDemo::~StageOpusPlantDemo()
{
    // empty (in the original too)
}
