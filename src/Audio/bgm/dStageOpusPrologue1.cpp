#include "Audio/bgm/dStageOpusPrologue1.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Sound/dSoundLabel.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"

// 0x005882D4
void audio::bgm::StageOpusPrologue1::Setup(stage::Name)
{
    GetTrackMixer()->AddMix(8);
}

// 0x005882E8
void audio::bgm::StageOpusPrologue1::Ready()
{
    if (GetGardenPlus()->data.townData.unk_0x5C.oceanSide == 1) {
        RequestMgr::Times times = {40, -1, 0};
        GetRequestMgr()->Request(KIND_BGM_PREPARED, STRM_BGM_PROLOGUE_01, 0, times);
        return;
    }
    RequestMgr::Times times = {20, -1, 0};
    GetRequestMgr()->Request(KIND_BGM_PREPARED, STRM_BGM_PROLOGUE_01, 0, times);
}

// 0x0058836C
void audio::bgm::StageOpusPrologue1::Leave(stage::Name)
{
    GetRequestMgr()->Remove(KIND_BGM_PREPARED, STRM_BGM_PROLOGUE_01);
    GetTrackMixer()->RemoveMix(8);
}

// 0x00588394
void audio::bgm::StageOpusPrologue1::EnterKeep()
{
    GetTrackMixer()->m_IsSequenceAlt = 1;
}

// 0x005883AC
// 0x005883A8 (deleting dtor)
audio::bgm::StageOpusPrologue1::~StageOpusPrologue1()
{
    // empty (in the original too)
}
