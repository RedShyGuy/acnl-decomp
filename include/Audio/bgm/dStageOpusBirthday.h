#pragma once

#include "decomp.h"
#include "audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm17StageOpusBirthdayE @ 0x008D2744
// vtable 0x00908028 (vptr 0x00908030), offset_to_top 0, 18 entries
class StageOpusBirthday : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusBirthday(); // ctor candidate(s) 0x00815704 (unverified)
    virtual void vf_0x14(); // 0x00587998 slot 0x14 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x18(); // 0x00587994 slot 0x18 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x2C(); // 0x00587980 slot 0x2C | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x30(); // 0x0058796C slot 0x30 | virtual slot, introduced by audio::bgm::StageOpusBase
};
} // namespace bgm
} // namespace audio
