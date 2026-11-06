#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm17StageOpusTourDeskE @ 0x008D2798
// vtable 0x0090820C (vptr 0x00908214), offset_to_top 0, 19 entries
class StageOpusTourDesk : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusTourDesk() : StageOpusShopBase(STRM_BGM_ISL_TOURDESK, 0, 0) {}
    virtual ~StageOpusTourDesk();
};
ASSERT_SIZE(StageOpusTourDesk, 0x2C);
} // namespace bgm
} // namespace audio
