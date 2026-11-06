#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm13StageOpusShopE @ 0x008D25E8
// vtable 0x00907710 (vptr 0x00907718), offset_to_top 0, 19 entries
class StageOpusShop : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusShop() : StageOpusShopBase(STRM_BGM_IND_KODANUKI_SHOTEN, 1, 0) {}
    virtual ~StageOpusShop();
    virtual void InitCloseHour(); // 0x005867A8 slot 0x48
};
ASSERT_SIZE(StageOpusShop, 0x2C);
} // namespace bgm
} // namespace audio
