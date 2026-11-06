#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm14StageOpusImproE @ 0x008D263C
// vtable 0x0090793C (vptr 0x00907944), offset_to_top 0, 19 entries
class StageOpusImpro : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusImpro() : StageOpusShopBase(STRM_BGM_IND_FLORISTS, 1, 0) {}
    virtual ~StageOpusImpro();
    virtual bool CanKeep(stage::Name next, bool) // 0x00753E20 slot 0x40
    {
        return next == stage::NAME_FLORISTS;
    }
};
ASSERT_SIZE(StageOpusImpro, 0x2C);
} // namespace bgm
} // namespace audio
