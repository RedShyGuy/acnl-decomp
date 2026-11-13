#include "Audio/bgm/dStageOpusDepart.h"
#include "Sound/sound_Extern.h"
#include "Sv/dSvApi.h"

// 0x00587108
void audio::bgm::StageOpusDepart::InitCloseHour()
{
    m_CloseHour = GetGardenPlus()->data.townData.unk_0xEE2C.shopZakka.IsNightOwlOrdinance() ? 24 : 21;
}

// 0x00587138
// 0x00587134 (deleting dtor)
audio::bgm::StageOpusDepart::~StageOpusDepart()
{
    // empty (in the original too)
}
