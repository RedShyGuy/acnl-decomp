#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusShopBase.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm16StageOpusGalleryE @ 0x008D2714
// vtable 0x00907EE4 (vptr 0x00907EEC), offset_to_top 0, 19 entries
class StageOpusGallery : public ::audio::bgm::StageOpusShopBase
{
public:
    StageOpusGallery() : StageOpusShopBase(STRM_BGM_IND_GALLERY, 1, 1) {}
    virtual ~StageOpusGallery();
};
ASSERT_SIZE(StageOpusGallery, 0x2C);
} // namespace bgm
} // namespace audio
