#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm17StageOpusPavilionE @ 0x008D2774
// vtable 0x00908168 (vptr 0x00908170), offset_to_top 0, 18 entries
class StageOpusPavilion : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusPavilion() {
        m_PavilionKind = 29;
    }
    virtual ~StageOpusPavilion();
    virtual void OnStart(); // 0x00587C4C slot 0x0C
    virtual void OnStop(); // 0x00587CCC slot 0x10
    virtual void Update(); // 0x00587C84 slot 0x1C
    virtual void Leave(stage::Name next); // 0x00587C40 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x00587B8C slot 0x30

    u32 m_PavilionBgmId; // 0x20
    u8 m_PavilionKind;  // 0x24
};
ASSERT_SIZE(StageOpusPavilion, 0x28);
} // namespace bgm
} // namespace audio
