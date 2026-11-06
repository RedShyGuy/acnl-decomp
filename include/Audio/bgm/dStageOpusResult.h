#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm15StageOpusResultE @ 0x008D26D8
// vtable 0x00907D4C (vptr 0x00907D54), offset_to_top 0, 18 entries
class StageOpusResult : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusResult() {}
    virtual ~StageOpusResult();
    virtual void Initialize(); // 0x005874F0 slot 0x20
    virtual void Leave(stage::Name next); // 0x00587524 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x005874FC slot 0x30
    virtual void Ready(); // 0x00587510 slot 0x38

    u8 m_IsFinished; // 0x1E
};
ASSERT_SIZE(StageOpusResult, 0x20);
} // namespace bgm
} // namespace audio
