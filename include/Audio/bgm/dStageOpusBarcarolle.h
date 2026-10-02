#pragma once

#include "decomp.h"
#include "audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm19StageOpusBarcarolleE @ 0x008D2804
// vtable 0x009084E4 (vptr 0x009084EC), offset_to_top 0, 18 entries
class StageOpusBarcarolle : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusBarcarolle(); // ctor candidate(s) 0x00815A00 (unverified)
    virtual void vf_0x14(); // 0x00588B98 slot 0x14 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x18(); // 0x00588B94 slot 0x18 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x2C(); // 0x00588B78 slot 0x2C | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x30(); // 0x00583E4C slot 0x30 | virtual slot, introduced by audio::bgm::StageOpusBase
};
} // namespace bgm
} // namespace audio
