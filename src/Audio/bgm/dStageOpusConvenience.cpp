#include "Audio/bgm/dStageOpusConvenience.h"
#include "Sound/sound_Extern.h"
#include "Sv/dSvApi.h"

// 0x00588D9C
void audio::bgm::StageOpusConvenience::InitCloseHour()
{
    m_CloseHour = Func759E24(&GetGardenPlus()->data.townData.unk_0xEE2C) ? 29 : 24;
}

// 0x00588DCC
// 0x00588DC8 (deleting dtor)
audio::bgm::StageOpusConvenience::~StageOpusConvenience()
{
    // empty (in the original too)
}
