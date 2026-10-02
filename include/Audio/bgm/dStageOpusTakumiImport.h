#pragma once

#include "decomp.h"
#include "audio/bgm/dStageOpusSelect.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm21StageOpusTakumiImportE @ 0x008D2894
// vtable 0x009088C0 (vptr 0x009088C8), offset_to_top 0, 18 entries
class StageOpusTakumiImport : public ::audio::bgm::StageOpusSelect
{
public:
    StageOpusTakumiImport(); // ctor candidate(s) 0x00815CE8 (unverified)
    virtual void vf_0x0C(); // 0x005891FC slot 0x0C | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x10(); // 0x00589234 slot 0x10 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x14(); // 0x0058926C slot 0x14 | virtual slot, introduced by audio::bgm::StageOpusBase
    virtual void vf_0x18(); // 0x00589268 slot 0x18 | virtual slot, introduced by audio::bgm::StageOpusBase
};
} // namespace bgm
} // namespace audio
