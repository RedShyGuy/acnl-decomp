#pragma once

#include "decomp.h"
#include "Audio/bgm/dEvOpusBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm13EvOpusXmasEveE @ 0x008D25A0
// vtable 0x00907534 (vptr 0x0090753C), offset_to_top 0, 16 entries
// the event plays its own music (names are ours)
class EvOpusXmasEve : public ::audio::bgm::EvOpusBase
{
public:
    EvOpusXmasEve() {}
    virtual ~EvOpusXmasEve();
    virtual bool HasBgm() const // 0x00753D74 slot 0x2C
    {
        return true;
    }
    virtual u32 GetBgmId() const // 0x00753D7C slot 0x30
    {
        return STRM_BGM_EVT_CHRISTMAS;
    }
};
ASSERT_SIZE(EvOpusXmasEve, 0x8);
} // namespace bgm
} // namespace audio
