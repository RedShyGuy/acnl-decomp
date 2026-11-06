#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm16StageOpusFortuneE @ 0x008D2708
// vtable 0x00907E90 (vptr 0x00907E98), offset_to_top 0, 19 entries
class StageOpusFortune : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusFortune() : StageOpusShopBase(STRM_BGM_IND_FORTUNE, 0, 0) {}
    virtual ~StageOpusFortune();
    virtual void Leave(stage::Name next); // 0x00587894 slot 0x2C
};
ASSERT_SIZE(StageOpusFortune, 0x2C);
} // namespace bgm
} // namespace audio
