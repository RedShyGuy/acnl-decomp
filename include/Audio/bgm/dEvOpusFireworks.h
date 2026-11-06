#pragma once

#include "decomp.h"
#include "Audio/bgm/dEvOpusBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm15EvOpusFireworksE @ 0x008D2684
// vtable 0x00907B24 (vptr 0x00907B2C), offset_to_top 0, 16 entries
// the event plays its own music (names are ours)
class EvOpusFireworks : public ::audio::bgm::EvOpusBase
{
public:
    EvOpusFireworks() {}
    virtual ~EvOpusFireworks();
    virtual bool HasBgm() const // 0x00753E38 slot 0x2C
    {
        return true;
    }
    virtual u32 GetBgmId() const // 0x00753E40 slot 0x30
    {
        return STRM_BGM_EVT_FIREWORKS;
    }
};
ASSERT_SIZE(EvOpusFireworks, 0x8);
} // namespace bgm
} // namespace audio
