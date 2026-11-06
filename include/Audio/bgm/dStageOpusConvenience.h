#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm20StageOpusConvenienceE @ 0x008D2864
// vtable 0x00908778 (vptr 0x00908780), offset_to_top 0, 19 entries
class StageOpusConvenience : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusConvenience() : StageOpusShopBase(STRM_BGM_IND_KODANUKI_CONVENIENCE, 1, 0) {}
    virtual ~StageOpusConvenience();
    virtual void InitCloseHour(); // 0x00588D9C slot 0x48
};
ASSERT_SIZE(StageOpusConvenience, 0x2C);
} // namespace bgm
} // namespace audio
