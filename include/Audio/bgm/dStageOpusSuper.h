#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm14StageOpusSuperE @ 0x008D2654
// vtable 0x009079E4 (vptr 0x009079EC), offset_to_top 0, 19 entries
class StageOpusSuper : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusSuper() : StageOpusShopBase(STRM_BGM_IND_KODANUKI_SUPER, 1, 0) {}
    virtual ~StageOpusSuper();
    virtual void InitCloseHour(); // 0x00586C4C slot 0x48
};
ASSERT_SIZE(StageOpusSuper, 0x2C);
} // namespace bgm
} // namespace audio
