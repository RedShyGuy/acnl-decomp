#include "Audio/bgm/dStageOpusFortune.h"
#include "Audio/bgm/dBgmRequestMgr.h"

// 0x00587894
void audio::bgm::StageOpusFortune::Leave(stage::Name)
{
    Stop();
    GetRequestMgr()->Remove(KIND_JINGLE_B, STRM_BGM_IND_FORTUNEDEMO);
}

// 0x005878C0
// 0x005878BC (deleting dtor)
audio::bgm::StageOpusFortune::~StageOpusFortune()
{
    // empty (in the original too)
}
