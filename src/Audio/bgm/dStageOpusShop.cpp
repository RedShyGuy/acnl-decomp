#include "Audio/bgm/dStageOpusShop.h"
#include "Sound/sound_Extern.h"
#include "Sv/dSvApi.h"

// 0x005867A8
void audio::bgm::StageOpusShop::InitCloseHour()
{
    m_CloseHour = Func759E24(&GetGardenPlus()->data.townData.unk_0xEE2C) ? 24 : 22;
}

// 0x005867D8
// 0x005867D4 (deleting dtor)
audio::bgm::StageOpusShop::~StageOpusShop()
{
    // empty (in the original too)
}
