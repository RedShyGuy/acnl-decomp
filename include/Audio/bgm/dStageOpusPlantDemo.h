#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm18StageOpusPlantDemoE @ 0x008D27BC
// vtable 0x00908300 (vptr 0x00908308), offset_to_top 0, 18 entries
class StageOpusPlantDemo : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusPlantDemo() {}
    virtual ~StageOpusPlantDemo();
    virtual void OnStart(); // 0x00584044 slot 0x0C
    virtual void OnStop(); // 0x00583F24 slot 0x10
    virtual void Leave(stage::Name next); // 0x005882A4 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x00588284 slot 0x30
    virtual void Ready(); // 0x00588298 slot 0x38
};
ASSERT_SIZE(StageOpusPlantDemo, 0x20);
} // namespace bgm
} // namespace audio
