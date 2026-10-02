#pragma once

#include "decomp.h"
#include "audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm13StageOpusLoadE @ 0x008D25C4
// vtable 0x00907620 (vptr 0x00907628), offset_to_top 0, 18 entries
class StageOpusLoad : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusLoad(); // ctor candidate(s) 0x00815094 (unverified)
    virtual void vf_0x14(); // 0x00586790 slot 0x14 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x18(); // 0x0058678C slot 0x18 | virtual slot, introduced by audio::bgm::StageOpusBase
};
} // namespace bgm
} // namespace audio
