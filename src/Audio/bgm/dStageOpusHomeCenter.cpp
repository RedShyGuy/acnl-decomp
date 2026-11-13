#include "Audio/bgm/dStageOpusHomeCenter.h"
#include "Sound/sound_Extern.h"
#include "Sv/dSvApi.h"

// 0x00588BA4
void audio::bgm::StageOpusHomeCenter::InitCloseHour()
{
    m_CloseHour = GetGardenPlus()->data.townData.unk_0xEE2C.shopZakka.IsNightOwlOrdinance() ? 27 : 23;
}

// 0x00588BD4
// 0x00588BD0 (deleting dtor)
audio::bgm::StageOpusHomeCenter::~StageOpusHomeCenter()
{
    // empty (in the original too)
}
