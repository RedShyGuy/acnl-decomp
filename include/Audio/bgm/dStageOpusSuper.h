#pragma once

#include "decomp.h"
#include "audio/bgm/dStageOpusShopBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm14StageOpusSuperE @ 0x008D2654
// vtable 0x009079E4 (vptr 0x009079EC), offset_to_top 0, 19 entries
class StageOpusSuper : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusSuper(); // ctor candidate(s) 0x00815320 (unverified)
    virtual void vf_0x0C(); // 0x00587DC0 slot 0x0C | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x10(); // 0x00587F38 slot 0x10 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x14(); // 0x00586C7C slot 0x14 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x18(); // 0x00586C78 slot 0x18 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x1C(); // 0x00587E30 slot 0x1C | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x28(); // 0x00587DAC slot 0x28 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x2C(); // 0x00587D94 slot 0x2C | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x30(); // 0x00587D08 slot 0x30 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x48(); // 0x00586C4C slot 0x48 | virtual slot, introduced by audio::bgm::StageOpusSuper
};
} // namespace bgm
} // namespace audio
