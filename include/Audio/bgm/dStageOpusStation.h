#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm16StageOpusStationE @ 0x008D272C
// vtable 0x00907F8C (vptr 0x00907F94), offset_to_top 0, 19 entries
class StageOpusStation : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusStation() : StageOpusShopBase(STRM_BGM_IND_STATION, 0, 0) {}
    virtual ~StageOpusStation();
    virtual void OnStop(); // 0x00587900 slot 0x10
    virtual void Initialize(); // 0x005878D4 slot 0x20
    virtual void Leave(stage::Name next); // 0x005878E0 slot 0x2C

    u8 m_IsTrainLeaving; // 0x2A the music fades out slowly (StageOpusMgr::StopStation)
};
ASSERT_SIZE(StageOpusStation, 0x2C);
} // namespace bgm
} // namespace audio
