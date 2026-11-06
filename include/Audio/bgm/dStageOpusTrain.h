#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm14StageOpusTrainE @ 0x008D266C
// vtable 0x00907A88 (vptr 0x00907A90), offset_to_top 0, 19 entries
class StageOpusTrain : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusTrain() : StageOpusShopBase(SEQ_BGM_TRAIN, 0, 0) {}
    virtual ~StageOpusTrain();
};
ASSERT_SIZE(StageOpusTrain, 0x2C);
} // namespace bgm
} // namespace audio
