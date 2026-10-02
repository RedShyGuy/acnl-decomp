#pragma once

#include "decomp.h"
#include "audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm18StageOpusCafeStaffE @ 0x008D27A4
// vtable 0x00908260 (vptr 0x00908268), offset_to_top 0, 18 entries
class StageOpusCafeStaff : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusCafeStaff(); // ctor candidate(s) 0x00815888 (unverified)
    virtual void vf_0x14(); // 0x00588234 slot 0x14 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x18(); // 0x00588230 slot 0x18 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x2C(); // 0x00588208 slot 0x2C | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x38(); // 0x005881C4 slot 0x38 | virtual slot, introduced by audio::bgm::StageOpusBase
};
} // namespace bgm
} // namespace audio
