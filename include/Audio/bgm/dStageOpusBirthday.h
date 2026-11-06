#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm17StageOpusBirthdayE @ 0x008D2744
// vtable 0x00908028 (vptr 0x00908030), offset_to_top 0, 18 entries
class StageOpusBirthday : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusBirthday() {}
    virtual ~StageOpusBirthday();
    virtual void Leave(stage::Name next); // 0x00587980 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x0058796C slot 0x30
};
ASSERT_SIZE(StageOpusBirthday, 0x20);
} // namespace bgm
} // namespace audio
