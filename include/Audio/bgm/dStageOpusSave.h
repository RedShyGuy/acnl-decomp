#pragma once

#include "decomp.h"
#include "audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm13StageOpusSaveE @ 0x008D25DC
// vtable 0x009076C0 (vptr 0x009076C8), offset_to_top 0, 18 entries
class StageOpusSave : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusSave(); // ctor candidate(s) 0x00815108 (unverified)
    virtual void vf_0x14(); // 0x005867A4 slot 0x14 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x18(); // 0x005867A0 slot 0x18 | virtual slot, introduced by audio::bgm::StageOpusBase
};
} // namespace bgm
} // namespace audio
