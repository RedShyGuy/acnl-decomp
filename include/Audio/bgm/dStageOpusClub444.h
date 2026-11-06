#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"
#include "Audio/bgm/dBgmMgr.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm16StageOpusClub444E @ 0x008D26FC
// vtable 0x00907E3C (vptr 0x00907E44), offset_to_top 0, 19 entries
class StageOpusClub444 : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusClub444() : StageOpusShopBase(STRM_BGM_IND_CLUB444, 0, 0) {}
    virtual ~StageOpusClub444();
    virtual bool CanKeep(stage::Name next, bool isOnline) // 0x00753EB8 slot 0x40
    {
        if (!isOnline) {
            SetEvidence();
        }
        return next == stage::NAME_CLUB444 && isOnline;
    }
};
ASSERT_SIZE(StageOpusClub444, 0x2C);
} // namespace bgm
} // namespace audio
