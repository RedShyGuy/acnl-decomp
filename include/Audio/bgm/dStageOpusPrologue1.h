#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"
#include "stage/stage_Api.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm18StageOpusPrologue1E @ 0x008D27D4
// vtable 0x009083A4 (vptr 0x009083AC), offset_to_top 0, 18 entries
// STRM_BGM_PROLOGUE_01 (stages of the start of the game; names are ours)
class StageOpusPrologue1 : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusPrologue1() {}
    virtual ~StageOpusPrologue1();
    virtual void EnterKeep(); // 0x00588394 slot 0x28
    virtual void Leave(stage::Name next); // 0x0058836C slot 0x2C
    virtual void Setup(stage::Name stage); // 0x005882D4 slot 0x30
    virtual void Ready(); // 0x005882E8 slot 0x38
    virtual bool CanKeep(stage::Name next, bool) // 0x00753F34 slot 0x40
    {
        return next == stage::NAME_VILLAGE_PROLOGUE || next == stage::NAME_STATION_PROLOGUE_DEMO;
    }
};
ASSERT_SIZE(StageOpusPrologue1, 0x20);
} // namespace bgm
} // namespace audio
