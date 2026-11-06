#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm20StageOpusAblesistersE @ 0x008D284C
// vtable 0x009086D0 (vptr 0x009086D8), offset_to_top 0, 19 entries
class StageOpusAblesisters : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusAblesisters() : StageOpusShopBase(STRM_BGM_IND_ABLESISTERS, 1, 0) {}
    virtual ~StageOpusAblesisters();
    virtual bool CanKeep(stage::Name next, bool isOnline) // 0x00753F9C slot 0x40
    {
        return !isOnline || next == stage::NAME_TAILOR || next == stage::NAME_ACCESSORY_SHOP;
    }
};
ASSERT_SIZE(StageOpusAblesisters, 0x2C);
} // namespace bgm
} // namespace audio
