#pragma once

#include "decomp.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm16StageOpusCampingE @ 0x008D26F0
// vtable 0x00907DEC (vptr 0x00907DF4), offset_to_top 0, 18 entries
class StageOpusCamping : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusCamping() {
        m_Kind = KIND_STAGE;
        m_Value = 7;
    }
    virtual ~StageOpusCamping();
    virtual void OnStart(); // 0x0058767C slot 0x0C
    virtual void OnStop(); // 0x00587740 slot 0x10
    virtual void Update(); // 0x005876B4 slot 0x1C
    virtual void Leave(stage::Name next); // 0x00587670 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x0058762C slot 0x30
    virtual void Ready(); // 0x00587664 slot 0x38
    virtual u32 GetBgmId(bool isNextHour); // 0x005877E8 slot 0x44

    u32 m_Value; // 0x20 (set to 7, not read)
};
ASSERT_SIZE(StageOpusCamping, 0x24);
} // namespace bgm
} // namespace audio
