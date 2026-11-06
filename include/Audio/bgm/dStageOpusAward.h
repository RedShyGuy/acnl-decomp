#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm14StageOpusAwardE @ 0x008D2618
// vtable 0x00907844 (vptr 0x0090784C), offset_to_top 0, 19 entries
class StageOpusAward : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusAward() : StageOpusShopBase(STRM_BGM_IND_AWARD, 0, 0) {}
    virtual ~StageOpusAward();
    virtual void Leave(stage::Name next); // 0x00586A6C slot 0x2C
    virtual void Setup(stage::Name stage); // 0x005869D0 slot 0x30
};
ASSERT_SIZE(StageOpusAward, 0x2C);
} // namespace bgm
} // namespace audio
