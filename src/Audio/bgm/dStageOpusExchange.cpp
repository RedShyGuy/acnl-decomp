#include "Audio/bgm/dStageOpusExchange.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmRequestMgr.h"

// 0x00587A9C
void audio::bgm::StageOpusExchange::Setup(stage::Name)
{
    GetStageOpusMgr()->m_Flags &= ~StageOpusMgr::FLAG_4;
}

// 0x00587AB4
void audio::bgm::StageOpusExchange::Ready()
{
    // empty (in the original too)
}

// 0x00587AB8
void audio::bgm::StageOpusExchange::Leave(stage::Name)
{
    Stop();
    GetStageOpusMgr()->m_Flags |= StageOpusMgr::FLAG_4;
}

// 0x00587ADC
void audio::bgm::StageOpusExchange::OnStart()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(m_Kind, m_BgmId, 20, times);
}

// 0x00587B14
void audio::bgm::StageOpusExchange::OnStop()
{
    if (m_BgmId == STRM_BGM_FLD_RESET) {
        GetRequestMgr()->SetFade(m_Kind, m_BgmId, m_Fade, 0, 12);
    } else {
        GetRequestMgr()->SetFade(m_Kind, m_BgmId, m_Fade, 0, 0);
    }
}

// 0x00587B88
// 0x00587B84 (deleting dtor)
audio::bgm::StageOpusExchange::~StageOpusExchange()
{
    // empty (in the original too)
}
