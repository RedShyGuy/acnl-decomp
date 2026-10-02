#pragma once

#include "decomp.h"
#include "audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm19StageOpusRetireDemoE @ 0x008D2840
// vtable 0x00908680 (vptr 0x00908688), offset_to_top 0, 18 entries
class StageOpusRetireDemo : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusRetireDemo(); // ctor candidate(s) 0x00815B34 (unverified)
    virtual void vf_0x14(); // 0x00588D08 slot 0x14 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x18(); // 0x00588D04 slot 0x18 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x2C(); // 0x00588CF0 slot 0x2C | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x30(); // 0x00588CDC slot 0x30 | virtual slot, introduced by audio::bgm::StageOpusBase
};
} // namespace bgm
} // namespace audio
