#pragma once

#include "decomp.h"
#include "audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm18StageOpusEntryDemoE @ 0x008D27B0
// vtable 0x009082B0 (vptr 0x009082B8), offset_to_top 0, 18 entries
class StageOpusEntryDemo : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusEntryDemo(); // ctor candidate(s) 0x008158B0 (unverified)
    virtual void vf_0x14(); // 0x00588280 slot 0x14 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x18(); // 0x0058827C slot 0x18 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x2C(); // 0x00588268 slot 0x2C | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x30(); // 0x00588238 slot 0x30 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x38(); // 0x0058824C slot 0x38 | virtual slot, introduced by audio::bgm::StageOpusBase
};
} // namespace bgm
} // namespace audio
