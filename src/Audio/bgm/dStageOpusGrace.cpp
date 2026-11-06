#include "Audio/bgm/dStageOpusGrace.h"
#include "Audio/bgm/dBgmMgr.h"

// 0x00586C20
void audio::bgm::StageOpusGrace::Enter()
{
    s_TownState.isShopMixOff = 1;
}

// 0x00586C38
// 0x00586C34 (deleting dtor)
audio::bgm::StageOpusGrace::~StageOpusGrace()
{
    // empty (in the original too)
}
