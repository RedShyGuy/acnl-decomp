#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm18StageOpusPoliceBoxE @ 0x008D27C8
// vtable 0x00908350 (vptr 0x00908358), offset_to_top 0, 19 entries
class StageOpusPoliceBox : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusPoliceBox() : StageOpusShopBase(STRM_BGM_IND_POLICEBOX, 0, 0) {}
    virtual ~StageOpusPoliceBox();
};
ASSERT_SIZE(StageOpusPoliceBox, 0x2C);
} // namespace bgm
} // namespace audio
