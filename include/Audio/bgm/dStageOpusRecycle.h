#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm16StageOpusRecycleE @ 0x008D2720
// vtable 0x00907F38 (vptr 0x00907F40), offset_to_top 0, 19 entries
class StageOpusRecycle : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusRecycle() : StageOpusShopBase(STRM_BGM_IND_RECYCLE, 1, 0) {}
    virtual ~StageOpusRecycle();
};
ASSERT_SIZE(StageOpusRecycle, 0x2C);
} // namespace bgm
} // namespace audio
