#include "Audio/bgm/dStageOpusSuper.h"
#include "Sound/sound_Extern.h"
#include "Sv/dSvApi.h"

// 0x00586C4C
void audio::bgm::StageOpusSuper::InitCloseHour()
{
    m_CloseHour = GetGardenPlus()->data.townData.unk_0xEE2C.shopZakka.Func759E24() ? 24 : 20;
}

// 0x00586C7C
// 0x00586C78 (deleting dtor)
audio::bgm::StageOpusSuper::~StageOpusSuper()
{
    // empty (in the original too)
}
