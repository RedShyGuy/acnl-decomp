#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm18StageOpusEntryDemoE @ 0x008D27B0
// vtable 0x009082B0 (vptr 0x009082B8), offset_to_top 0, 18 entries
class StageOpusEntryDemo : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusEntryDemo() {}
    virtual ~StageOpusEntryDemo();
    virtual void Leave(stage::Name next); // 0x00588268 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x00588238 slot 0x30
    virtual void Ready(); // 0x0058824C slot 0x38
};
ASSERT_SIZE(StageOpusEntryDemo, 0x20);
} // namespace bgm
} // namespace audio
