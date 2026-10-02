#pragma once

#include "decomp.h"
#include "audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm17StageOpusEvidenceE @ 0x008D275C
// vtable 0x009080C8 (vptr 0x009080D0), offset_to_top 0, 18 entries
class StageOpusEvidence : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusEvidence(); // ctor candidate(s) 0x00815754 (unverified)
    virtual void vf_0x14(); // 0x00587A98 slot 0x14 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x18(); // 0x00587A94 slot 0x18 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x1C(); // 0x00587A58 slot 0x1C | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x30(); // 0x00587A38 slot 0x30 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x40(); // 0x00753EF8 slot 0x40 | virtual slot, introduced by audio::bgm::StageOpusBase
};
} // namespace bgm
} // namespace audio
