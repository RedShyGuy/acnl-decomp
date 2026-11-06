#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm17StageOpusShowroomE @ 0x008D278C
// vtable 0x009081B8 (vptr 0x009081C0), offset_to_top 0, 19 entries
class StageOpusShowroom : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusShowroom() : StageOpusShopBase(STRM_BGM_IND_SHOWROOM, 0, 0) {}
    virtual ~StageOpusShowroom();
    virtual void OnStart(); // 0x00588050 slot 0x0C
    virtual void OnStop(); // 0x00588184 slot 0x10
    virtual void Update(); // 0x00588088 slot 0x1C
    virtual void Initialize(); // 0x0058800C slot 0x20
    virtual void Leave(stage::Name next); // 0x00588018 slot 0x2C

    u8 m_IsFading; // 0x2A the music of kind 14 fades out
};
ASSERT_SIZE(StageOpusShowroom, 0x2C);
} // namespace bgm
} // namespace audio
