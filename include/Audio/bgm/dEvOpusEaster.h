#pragma once

#include "decomp.h"
#include "Audio/bgm/dEvOpusBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm12EvOpusEasterE @ 0x008D257C
// vtable 0x0090745C (vptr 0x00907464), offset_to_top 0, 16 entries
// the event plays its own music (names are ours)
class EvOpusEaster : public ::audio::bgm::EvOpusBase
{
public:
    EvOpusEaster() {}
    virtual ~EvOpusEaster();
    virtual bool HasBgm() const // 0x00753D40 slot 0x2C
    {
        return true;
    }
    virtual u32 GetBgmId() const // 0x00753D48 slot 0x30
    {
        return STRM_BGM_EVT_EASTER;
    }
};
ASSERT_SIZE(EvOpusEaster, 0x8);
} // namespace bgm
} // namespace audio
