#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm14StageOpusGraceE @ 0x008D2630
// vtable 0x009078E8 (vptr 0x009078F0), offset_to_top 0, 19 entries
class StageOpusGrace : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusGrace() : StageOpusShopBase(STRM_BGM_IND_GRACE, 0, 0) {}
    virtual ~StageOpusGrace();
    virtual void Enter(); // 0x00586C20 slot 0x24
};
ASSERT_SIZE(StageOpusGrace, 0x2C);
} // namespace bgm
} // namespace audio
