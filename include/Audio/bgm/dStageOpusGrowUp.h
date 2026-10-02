#pragma once

#include "decomp.h"
#include "audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm15StageOpusGrowUpE @ 0x008D26B4
// vtable 0x00907C5C (vptr 0x00907C64), offset_to_top 0, 18 entries
class StageOpusGrowUp : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusGrowUp(); // ctor candidate(s) 0x00815478 (unverified)
    virtual void vf_0x14(); // 0x005871A8 slot 0x14 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x18(); // 0x005871A4 slot 0x18 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x1C(); // 0x00587180 slot 0x1C | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x30(); // 0x0058713C slot 0x30 | virtual slot, introduced by audio::bgm::StageOpusBase
};
} // namespace bgm
} // namespace audio
