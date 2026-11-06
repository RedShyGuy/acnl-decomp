#include "Audio/bgm/dStageOpusBarcarolle.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Sound/dSoundMgr.h"

// 0x00583E4C
void audio::bgm::StageOpusBarcarolle::Setup(stage::Name)
{
    GetTrackMixer()->AddMix(8);
    SoundMgr::SMuteSystemAndSpeech(true);
}

// 0x00588B78
void audio::bgm::StageOpusBarcarolle::Leave(stage::Name)
{
    GetTrackMixer()->RemoveMix(8);
    SoundMgr::SMuteSystemAndSpeech(false);
}

// 0x00588B98
// 0x00588B94 (deleting dtor)
audio::bgm::StageOpusBarcarolle::~StageOpusBarcarolle()
{
    // empty (in the original too)
}
