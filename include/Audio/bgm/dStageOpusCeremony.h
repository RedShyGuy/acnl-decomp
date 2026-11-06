#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm17StageOpusCeremonyE @ 0x008D2750
// vtable 0x00908078 (vptr 0x00908080), offset_to_top 0, 18 entries
class StageOpusCeremony : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusCeremony() {}
    virtual ~StageOpusCeremony();
    virtual void OnStart(); // 0x005879DC slot 0x0C
    virtual void OnStop(); // 0x00587A14 slot 0x10
    virtual void Leave(stage::Name next); // 0x005879BC slot 0x2C
    virtual void Setup(stage::Name stage); // 0x0058799C slot 0x30
    virtual void Ready(); // 0x005879B0 slot 0x38
};
ASSERT_SIZE(StageOpusCeremony, 0x20);
} // namespace bgm
} // namespace audio
