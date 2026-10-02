#pragma once

#include "decomp.h"
#include "audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm15StageOpusResultE @ 0x008D26D8
// vtable 0x00907D4C (vptr 0x00907D54), offset_to_top 0, 18 entries
class StageOpusResult : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusResult(); // ctor candidate(s) 0x00815500 (unverified)
    virtual void vf_0x14(); // 0x00587558 slot 0x14 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x18(); // 0x00587554 slot 0x18 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x20(); // 0x005874F0 slot 0x20 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x2C(); // 0x00587524 slot 0x2C | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x30(); // 0x005874FC slot 0x30 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x38(); // 0x00587510 slot 0x38 | virtual slot, introduced by audio::bgm::StageOpusBase
};
} // namespace bgm
} // namespace audio
