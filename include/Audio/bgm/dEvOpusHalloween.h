#pragma once

#include "decomp.h"
#include "Audio/bgm/dEvOpusBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm15EvOpusHalloweenE @ 0x008D2690
// vtable 0x00907B6C (vptr 0x00907B74), offset_to_top 0, 16 entries
// the event plays its own music (names are ours)
class EvOpusHalloween : public ::audio::bgm::EvOpusBase
{
public:
    EvOpusHalloween() {}
    virtual ~EvOpusHalloween();
    virtual bool HasBgm() const // 0x00753E4C slot 0x2C
    {
        return true;
    }
    virtual u32 GetBgmId() const // 0x00753E54 slot 0x30
    {
        return STRM_BGM_EVT_HALLOWEEN;
    }
};
ASSERT_SIZE(EvOpusHalloween, 0x8);
} // namespace bgm
} // namespace audio
