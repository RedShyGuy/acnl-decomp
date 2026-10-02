#pragma once

#include "decomp.h"
#include "audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm13StageOpusNullE @ 0x008D25D0
// vtable 0x00907670 (vptr 0x00907678), offset_to_top 0, 18 entries
class StageOpusNull : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusNull(); // ctor candidate(s) 0x008150E0 (unverified)
    virtual void vf_0x14(); // 0x0058679C slot 0x14 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x18(); // 0x00586798 slot 0x18 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x30(); // 0x00586794 slot 0x30 | virtual slot, introduced by audio::bgm::StageOpusBase
};
} // namespace bgm
} // namespace audio
