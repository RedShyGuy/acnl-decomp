#include "Audio/bgm/dStageOpusEntryDemo.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/dSoundLabel.h"

// 0x00588238
void audio::bgm::StageOpusEntryDemo::Setup(stage::Name)
{
    GetTrackMixer()->AddMix(8);
}

// 0x0058824C
void audio::bgm::StageOpusEntryDemo::Ready()
{
    GetRequestMgr()->RequestJingleDelayed(STRM_BGM_FNFR_TRAIN_ENTRY, 60);
}

// 0x00588268
void audio::bgm::StageOpusEntryDemo::Leave(stage::Name)
{
    GetTrackMixer()->RemoveMix(8);
}

// 0x00588280
// 0x0058827C (deleting dtor)
audio::bgm::StageOpusEntryDemo::~StageOpusEntryDemo()
{
    // empty (in the original too)
}
