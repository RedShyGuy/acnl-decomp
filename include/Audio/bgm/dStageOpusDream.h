#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm14StageOpusDreamE @ 0x008D2624
// vtable 0x00907898 (vptr 0x009078A0), offset_to_top 0, 18 entries
class StageOpusDream : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusDream() {}
    virtual ~StageOpusDream();
    virtual void OnStart(); // 0x00586B28 slot 0x0C
    virtual void OnStop(); // 0x00586BA8 slot 0x10
    virtual void Update(); // 0x00586B60 slot 0x1C
    virtual void Enter(); // 0x00586AE4 slot 0x24
    virtual void Leave(stage::Name next); // 0x00586B1C slot 0x2C
    virtual void Setup(stage::Name stage); // 0x00586A94 slot 0x30
};
ASSERT_SIZE(StageOpusDream, 0x20);
} // namespace bgm
} // namespace audio
