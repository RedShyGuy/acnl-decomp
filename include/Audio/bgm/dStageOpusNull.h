#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm13StageOpusNullE @ 0x008D25D0
// vtable 0x00907670 (vptr 0x00907678), offset_to_top 0, 18 entries
// The stages without music of their own.
class StageOpusNull : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusNull() {}
    virtual ~StageOpusNull();
    virtual void Setup(stage::Name stage); // 0x00586794 slot 0x30
};
ASSERT_SIZE(StageOpusNull, 0x20);
} // namespace bgm
} // namespace audio
