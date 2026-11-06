#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm19StageOpusBarcarolleE @ 0x008D2804
// vtable 0x009084E4 (vptr 0x009084EC), offset_to_top 0, 18 entries
class StageOpusBarcarolle : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusBarcarolle() {}
    virtual ~StageOpusBarcarolle();
    virtual void Leave(stage::Name next); // 0x00588B78 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x00583E4C slot 0x30
};
ASSERT_SIZE(StageOpusBarcarolle, 0x20);
} // namespace bgm
} // namespace audio
