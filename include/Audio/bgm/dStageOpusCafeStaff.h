#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm18StageOpusCafeStaffE @ 0x008D27A4
// vtable 0x00908260 (vptr 0x00908268), offset_to_top 0, 18 entries
class StageOpusCafeStaff : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusCafeStaff() {}
    virtual ~StageOpusCafeStaff();
    virtual void Leave(stage::Name next); // 0x00588208 slot 0x2C
    virtual void Ready(); // 0x005881C4 slot 0x38
};
ASSERT_SIZE(StageOpusCafeStaff, 0x20);
} // namespace bgm
} // namespace audio
