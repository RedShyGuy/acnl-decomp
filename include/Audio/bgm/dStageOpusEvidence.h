#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"
#include "Sound/dSoundMgr.h"
#include "stage/stage_Api.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm17StageOpusEvidenceE @ 0x008D275C
// vtable 0x009080C8 (vptr 0x009080D0), offset_to_top 0, 18 entries
class StageOpusEvidence : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusEvidence() {}
    virtual ~StageOpusEvidence();
    virtual void Update(); // 0x00587A58 slot 0x1C
    virtual void Setup(stage::Name stage); // 0x00587A38 slot 0x30
    virtual bool CanKeep(stage::Name next, bool isOnline) // 0x00753EF8 slot 0x40
    {
        if (isOnline && SoundMgr::SIsJingleAttached()) {
            SoundMgr::SStopJingle(1);
        }
        return next == stage::NAME_CLUB444 && isOnline;
    }
};
ASSERT_SIZE(StageOpusEvidence, 0x20);
} // namespace bgm
} // namespace audio
