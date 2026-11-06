#include "Audio/bgm/dStageOpusTour.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmTrackMixer.h"

// 0x005867DC
void audio::bgm::StageOpusTour::Leave(stage::Name)
{
    GetFieldMusic()->StopIslandGame();
    GetTrackMixer()->RemoveMix(37);
}

// 0x005867F8
void audio::bgm::StageOpusTour::SetupKeep(stage::Name stage)
{
    if (stage::IsKind(8, stage)) {
        GetTrackMixer()->AddMix(37);
    }
}

// 0x00586820
void audio::bgm::StageOpusTour::EnterKeep()
{
    GetTrackMixer()->RemoveMix(37);
}

// 0x00586834
void audio::bgm::StageOpusTour::Update()
{
    if (GetTrackMixer()->IsMixActive(21) && !GetTrackMixer()->IsMixActive(34)) {
        GetTrackMixer()->RemoveMix(21);
    }
}

// 0x00586878
// 0x00586874 (deleting dtor)
audio::bgm::StageOpusTour::~StageOpusTour()
{
    // empty (in the original too)
}

// 0x0058B390
void audio::bgm::StageOpusTour::Setup(stage::Name)
{
    GetFieldMusic()->AddMix25();
}
