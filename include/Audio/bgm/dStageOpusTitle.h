#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm14StageOpusTitleE @ 0x008D2660
// vtable 0x00907A38 (vptr 0x00907A40), offset_to_top 0, 18 entries
class StageOpusTitle : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusTitle() {}
    virtual ~StageOpusTitle();
    virtual void OnStart(); // 0x00586CE0 slot 0x0C
    virtual void OnStop(); // 0x00586D44 slot 0x10
    virtual void Update(); // 0x00586D14 slot 0x1C
    virtual void Leave(stage::Name next); // 0x00586CC0 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x00586C80 slot 0x30
    virtual void Ready(); // 0x00586CB4 slot 0x38
    virtual bool CanKeep(stage::Name, bool) // 0x00753E30 slot 0x40
    {
        return false;
    }
};
ASSERT_SIZE(StageOpusTitle, 0x20);
} // namespace bgm
} // namespace audio
