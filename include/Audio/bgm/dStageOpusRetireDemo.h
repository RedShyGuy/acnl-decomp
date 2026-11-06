#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm19StageOpusRetireDemoE @ 0x008D2840
// vtable 0x00908680 (vptr 0x00908688), offset_to_top 0, 18 entries
class StageOpusRetireDemo : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusRetireDemo() {}
    virtual ~StageOpusRetireDemo();
    virtual void Leave(stage::Name next); // 0x00588CF0 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x00588CDC slot 0x30
};
ASSERT_SIZE(StageOpusRetireDemo, 0x20);
} // namespace bgm
} // namespace audio
