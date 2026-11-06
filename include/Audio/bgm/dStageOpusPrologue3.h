#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm18StageOpusPrologue3E @ 0x008D27EC
// vtable 0x00908444 (vptr 0x0090844C), offset_to_top 0, 18 entries
// STRM_BGM_IND_OFFICE at the start of the game (names are ours)
class StageOpusPrologue3 : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusPrologue3() {}
    virtual ~StageOpusPrologue3();
    virtual void OnStart(); // 0x005886FC slot 0x0C
    virtual void OnStop(); // 0x00588778 slot 0x10
    virtual void Initialize(); // 0x005886DC slot 0x20
    virtual void Leave(stage::Name next); // 0x005886E8 slot 0x2C

    u8 m_IsDelayed; // 0x1E the first start waits 10 frames
};
ASSERT_SIZE(StageOpusPrologue3, 0x20);
} // namespace bgm
} // namespace audio
