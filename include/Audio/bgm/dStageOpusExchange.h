#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm17StageOpusExchangeE @ 0x008D2768
// vtable 0x00908118 (vptr 0x00908120), offset_to_top 0, 18 entries
class StageOpusExchange : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusExchange() {
        m_Kind = KIND_SHOP;
        m_Fade = 0;
        m_BgmId = STRM_BGM_IND_TANUKICHI;
    }
    virtual ~StageOpusExchange();
    virtual void OnStart(); // 0x00587ADC slot 0x0C
    virtual void OnStop(); // 0x00587B14 slot 0x10
    virtual void Leave(stage::Name next); // 0x00587AB8 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x00587A9C slot 0x30
    virtual void Ready(); // 0x00587AB4 slot 0x38
};
ASSERT_SIZE(StageOpusExchange, 0x20);
} // namespace bgm
} // namespace audio
