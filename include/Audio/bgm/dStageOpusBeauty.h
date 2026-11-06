#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm15StageOpusBeautyE @ 0x008D269C
// vtable 0x00907BB4 (vptr 0x00907BBC), offset_to_top 0, 19 entries
class StageOpusBeauty : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusBeauty() : StageOpusShopBase(STRM_BGM_IND_BEAUTYSALON, 1, 1) {}
    virtual ~StageOpusBeauty();
};
ASSERT_SIZE(StageOpusBeauty, 0x2C);
} // namespace bgm
} // namespace audio
