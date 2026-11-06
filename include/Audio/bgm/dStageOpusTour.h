#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"
#include "stage/stage_Api.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm13StageOpusTourE @ 0x008D25F4
// vtable 0x00907764 (vptr 0x0090776C), offset_to_top 0, 18 entries
class StageOpusTour : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusTour() {}
    virtual ~StageOpusTour();
    virtual void Update(); // 0x00586834 slot 0x1C
    virtual void EnterKeep(); // 0x00586820 slot 0x28
    virtual void Leave(stage::Name next); // 0x005867DC slot 0x2C
    virtual void Setup(stage::Name stage); // 0x0058B390 slot 0x30
    virtual void SetupKeep(stage::Name stage); // 0x005867F8 slot 0x34
    virtual bool CanKeep(stage::Name next, bool) // 0x00753D90 slot 0x40
    {
        if (stage::IsKind(0xA0016, next)) {
            return false;
        }
        return stage::IsKind(0x80000, next) || stage::IsKind(0x100000, next);
    }
};
ASSERT_SIZE(StageOpusTour, 0x20);
} // namespace bgm
} // namespace audio
