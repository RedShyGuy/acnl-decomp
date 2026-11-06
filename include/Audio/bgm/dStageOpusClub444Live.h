#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm20StageOpusClub444LiveE @ 0x008D2858
// vtable 0x00908724 (vptr 0x0090872C), offset_to_top 0, 19 entries
class StageOpusClub444Live : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusClub444Live() : StageOpusShopBase(STRM_BGM_IND_CLUB444, 0, 0)
    {
        m_IsStopped = 0;
    }
    virtual ~StageOpusClub444Live();
    virtual void OnStart(); // 0x00588D48 slot 0x0C
    virtual void OnStop(); // 0x0058B92C slot 0x10
    virtual void Update(); // 0x00588D80 slot 0x1C
    virtual void Leave(stage::Name next); // 0x00588D28 slot 0x2C

    u8 m_IsStopped; // 0x2A (StageOpusMgr::StopClub444Live)
};
ASSERT_SIZE(StageOpusClub444Live, 0x2C);
} // namespace bgm
} // namespace audio
