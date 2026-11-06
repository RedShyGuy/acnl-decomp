#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm14StageOpusShoesE @ 0x008D2648
// vtable 0x00907990 (vptr 0x00907998), offset_to_top 0, 19 entries
class StageOpusShoes : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusShoes() : StageOpusShopBase(STRM_BGM_IND_SHOESHOP, 1, 0) {}
    virtual ~StageOpusShoes();
};
ASSERT_SIZE(StageOpusShoes, 0x2C);
} // namespace bgm
} // namespace audio
