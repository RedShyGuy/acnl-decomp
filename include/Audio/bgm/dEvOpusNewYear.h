#pragma once

#include "decomp.h"
#include "Audio/bgm/dEvOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm13EvOpusNewYearE @ 0x008D2594
// vtable 0x009074EC (vptr 0x009074F4), offset_to_top 0, 16 entries
// The music of the first hours of New Year's Day: the event starts and stops its music itself (silent around the change of
// the hour). Member names are ours.
class EvOpusNewYear : public ::audio::bgm::EvOpusBase
{
public:
    EvOpusNewYear() {}
    virtual ~EvOpusNewYear();

    virtual void OnStart(); // 0x0058605C slot 0x0C
    virtual void OnStop(); // 0x0058611C slot 0x10
    virtual void Begin(); // 0x00585FFC slot 0x1C
    virtual void End(bool isReplaced); // 0x00586150 slot 0x20
    virtual void Update(); // 0x005860B8 slot 0x24

    u32 m_Fade;  // 0x08 the frames of the fade out
    u32 m_BgmId; // 0x0C
};
ASSERT_SIZE(EvOpusNewYear, 0x10);
} // namespace bgm
} // namespace audio
