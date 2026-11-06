#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm15StageOpusDepartE @ 0x008D26A8
// vtable 0x00907C08 (vptr 0x00907C10), offset_to_top 0, 19 entries
class StageOpusDepart : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusDepart() : StageOpusShopBase(STRM_BGM_IND_KODANUKI_DEPART, 1, 0) {}
    virtual ~StageOpusDepart();
    virtual bool CanKeep(stage::Name next, bool) // 0x00753E60 slot 0x40
    {
        return next == stage::NAME_DEPARTMENT_STORE_1F || next == stage::NAME_DEPARTMENT_STORE_2F;
    }
    virtual void InitCloseHour(); // 0x00587108 slot 0x48
};
ASSERT_SIZE(StageOpusDepart, 0x2C);
} // namespace bgm
} // namespace audio
