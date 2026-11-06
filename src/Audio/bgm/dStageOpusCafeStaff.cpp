#include "Audio/bgm/dStageOpusCafeStaff.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/dSoundLabel.h"

// 0x005881C4
void audio::bgm::StageOpusCafeStaff::Ready()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_COMMAND, SEQ_BGM_IND_CAFEARBEIT, 5, times);
    GetTrackMixer()->AddMix(8);
}

// 0x00588208
void audio::bgm::StageOpusCafeStaff::Leave(stage::Name)
{
    GetRequestMgr()->Remove(KIND_COMMAND, SEQ_BGM_IND_CAFEARBEIT);
    GetTrackMixer()->RemoveMix(8);
}

// 0x00588234
// 0x00588230 (deleting dtor)
audio::bgm::StageOpusCafeStaff::~StageOpusCafeStaff()
{
    // empty (in the original too)
}
