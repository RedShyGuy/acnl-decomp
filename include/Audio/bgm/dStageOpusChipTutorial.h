#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm21StageOpusChipTutorialE @ 0x008D287C
// vtable 0x0090881C (vptr 0x00908824), offset_to_top 0, 19 entries
class StageOpusChipTutorial : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusChipTutorial() : StageOpusShopBase(STRM_BGM_TUTORIAL_TAKUMI, 0, 0) {}
    virtual ~StageOpusChipTutorial();
    virtual void Leave(stage::Name next); // 0x005891A4 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x00589108 slot 0x30
};
ASSERT_SIZE(StageOpusChipTutorial, 0x2C);
} // namespace bgm
} // namespace audio
