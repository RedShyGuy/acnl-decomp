#include "Audio/bgm/dStageOpusGrowUp.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "time/time_Api.h"

// 0x0058713C
void audio::bgm::StageOpusGrowUp::Setup(stage::Name)
{
    if (static_cast<u32>(time::GetCurrentDate()->hour - 6) < 12) {
        u8 weather = Func62FB14();
        if (weather != 3 && weather != 4) {
            m_HasBirds = 1;
        }
    }
    s_ClockState.isShop = 0;
}

// 0x00587180
void audio::bgm::StageOpusGrowUp::Update()
{
    if (m_HasBirds) {
        SoundMgr::SSetHeldSoundPan(SE_ENV_MORNING_BIRD, 0.0f);
    }
}

// 0x005871A8
// 0x005871A4 (deleting dtor)
audio::bgm::StageOpusGrowUp::~StageOpusGrowUp()
{
    // empty (in the original too)
}
