#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm21StageOpusComeHomeDemoE @ 0x008D2888
// vtable 0x00908870 (vptr 0x00908878), offset_to_top 0, 18 entries
class StageOpusComeHomeDemo : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusComeHomeDemo() {}
    virtual ~StageOpusComeHomeDemo();
    virtual void Leave(stage::Name next); // 0x005891E0 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x005891CC slot 0x30
};
ASSERT_SIZE(StageOpusComeHomeDemo, 0x20);
} // namespace bgm
} // namespace audio
