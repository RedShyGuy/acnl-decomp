#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm15StageOpusOfficeE @ 0x008D26CC
// vtable 0x00907CFC (vptr 0x00907D04), offset_to_top 0, 18 entries
class StageOpusOffice : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusOffice() : m_IsPlantDemo(0) {}
    virtual ~StageOpusOffice();
    virtual void OnStart(); // 0x00587428 slot 0x0C
    virtual void OnStop(); // 0x005874A8 slot 0x10
    virtual void Update(); // 0x00587460 slot 0x1C
    virtual void Leave(stage::Name next); // 0x005873F8 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x005873DC slot 0x30

    u8 m_IsPlantDemo; // 0x1E the music stops for StageOpusPlantDemo
};
ASSERT_SIZE(StageOpusOffice, 0x20);
} // namespace bgm
} // namespace audio
