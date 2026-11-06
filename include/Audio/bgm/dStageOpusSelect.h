#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm15StageOpusSelectE @ 0x008D26E4
// vtable 0x00907D9C (vptr 0x00907DA4), offset_to_top 0, 18 entries
class StageOpusSelect : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusSelect() {}
    virtual ~StageOpusSelect();
    virtual void OnStart(); // 0x005875B8 slot 0x0C
    virtual void OnStop(); // 0x005875F0 slot 0x10
    virtual void Leave(stage::Name next); // 0x00587594 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x0058755C slot 0x30
    virtual void Ready(); // 0x00587588 slot 0x38
};
ASSERT_SIZE(StageOpusSelect, 0x20);
} // namespace bgm
} // namespace audio
