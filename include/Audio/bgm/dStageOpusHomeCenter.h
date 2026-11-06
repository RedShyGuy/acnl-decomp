#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm19StageOpusHomeCenterE @ 0x008D281C
// vtable 0x00908588 (vptr 0x00908590), offset_to_top 0, 19 entries
class StageOpusHomeCenter : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusHomeCenter() : StageOpusShopBase(STRM_BGM_IND_KODANUKI_HOMECENTER, 1, 0) {}
    virtual ~StageOpusHomeCenter();
    virtual bool CanKeep(stage::Name next, bool) // 0x00753F88 slot 0x40
    {
        return next == stage::NAME_HOME_CENTER || next == stage::NAME_FLORISTS_HOME_IMPROVEMENT_CENTER;
    }
    virtual void InitCloseHour(); // 0x00588BA4 slot 0x48
};
ASSERT_SIZE(StageOpusHomeCenter, 0x2C);
} // namespace bgm
} // namespace audio
