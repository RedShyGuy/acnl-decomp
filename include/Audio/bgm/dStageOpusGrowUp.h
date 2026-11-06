#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm15StageOpusGrowUpE @ 0x008D26B4
// vtable 0x00907C5C (vptr 0x00907C64), offset_to_top 0, 18 entries
class StageOpusGrowUp : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusGrowUp() : m_HasBirds(0) {}
    virtual ~StageOpusGrowUp();
    virtual void Update(); // 0x00587180 slot 0x1C
    virtual void Setup(stage::Name stage); // 0x0058713C slot 0x30

    u8 m_HasBirds; // 0x1E the birds of the morning sing
};
ASSERT_SIZE(StageOpusGrowUp, 0x20);
} // namespace bgm
} // namespace audio
