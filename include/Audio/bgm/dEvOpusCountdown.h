#pragma once

#include "decomp.h"
#include "Audio/bgm/dEvOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm15EvOpusCountdownE @ 0x008D2678
// vtable 0x00907ADC (vptr 0x00907AE4), offset_to_top 0, 16 entries
// The music of the last hour of the year (STRM_BGM_EVT_COUNTDOWN_2300/2330/2350/2355): each part fades out exactly at
// 23:29:55, 23:49:55, 23:54:55 and 23:59:15. Member names are ours.
class EvOpusCountdown : public ::audio::bgm::EvOpusBase
{
public:
    EvOpusCountdown() {}
    virtual ~EvOpusCountdown();

    virtual void OnStart(); // 0x00586DEC slot 0x0C
    virtual void OnStop(); // 0x00587088 slot 0x10
    virtual void Begin(); // 0x00586DB4 slot 0x1C
    virtual void End(bool isReplaced); // 0x005870C4 slot 0x20
    virtual void Update(); // 0x00586E24 slot 0x24

    s8 m_Second;  // 0x07 the second when the event began in the last minute, else -1
    u32 m_Fade;   // 0x08 the frames of the fade out
    u32 m_BgmId;  // 0x0C
};
ASSERT_SIZE(EvOpusCountdown, 0x10);
} // namespace bgm
} // namespace audio
