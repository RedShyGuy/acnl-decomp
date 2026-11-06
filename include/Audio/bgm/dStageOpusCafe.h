#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm13StageOpusCafeE @ 0x008D25B8
// vtable 0x009075CC (vptr 0x009075D4), offset_to_top 0, 19 entries
class StageOpusCafe : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusCafe() : StageOpusShopBase(STRM_BGM_IND_CAFE, 0, 0) {}
    virtual ~StageOpusCafe();
};
ASSERT_SIZE(StageOpusCafe, 0x2C);
} // namespace bgm
} // namespace audio
