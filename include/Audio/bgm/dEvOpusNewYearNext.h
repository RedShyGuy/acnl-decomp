#pragma once

#include "decomp.h"
#include "Audio/bgm/dEvOpusBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm17EvOpusNewYearNextE @ 0x008D2738
// vtable 0x00907FE0 (vptr 0x00907FE8), offset_to_top 0, 16 entries
// the event plays its own music (names are ours)
class EvOpusNewYearNext : public ::audio::bgm::EvOpusBase
{
public:
    EvOpusNewYearNext() {}
    virtual ~EvOpusNewYearNext();
    virtual bool HasBgm() const // 0x00753EE4 slot 0x2C
    {
        return true;
    }
    virtual u32 GetBgmId() const // 0x00753EEC slot 0x30
    {
        return STRM_BGM_EVT_GANTAN;
    }
};
ASSERT_SIZE(EvOpusNewYearNext, 0x8);
} // namespace bgm
} // namespace audio
