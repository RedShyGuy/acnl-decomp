#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm19StageOpusRealEstateE @ 0x008D2834
// vtable 0x0090862C (vptr 0x00908634), offset_to_top 0, 19 entries
class StageOpusRealEstate : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusRealEstate() : StageOpusShopBase(STRM_BGM_IND_TANUKICHI, 1, 0) {}
    virtual ~StageOpusRealEstate();
};
ASSERT_SIZE(StageOpusRealEstate, 0x2C);
} // namespace bgm
} // namespace audio
