#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm19StageOpusExhibitionE @ 0x008D2810
// vtable 0x00908534 (vptr 0x0090853C), offset_to_top 0, 19 entries
class StageOpusExhibition : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusExhibition() : StageOpusShopBase(STRM_BGM_EXHIBITION, 0, 0) {}
    virtual ~StageOpusExhibition();
    virtual bool CanKeep(stage::Name next, bool) // 0x002F740C slot 0x40
    {
        return stage::IsKind(0x400, next);
    }
};
ASSERT_SIZE(StageOpusExhibition, 0x2C);
} // namespace bgm
} // namespace audio
