#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm19StageOpusPostOfficeE @ 0x008D2828
// vtable 0x009085DC (vptr 0x009085E4), offset_to_top 0, 18 entries
class StageOpusPostOffice : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusPostOffice() {}
    virtual ~StageOpusPostOffice();
    virtual void OnStart(); // 0x00588C00 slot 0x0C
    virtual void OnStop(); // 0x00588C98 slot 0x10
    virtual void Update(); // 0x00588C50 slot 0x1C
    virtual void Leave(stage::Name next); // 0x00588BF4 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x00588BD8 slot 0x30
};
ASSERT_SIZE(StageOpusPostOffice, 0x20);
} // namespace bgm
} // namespace audio
