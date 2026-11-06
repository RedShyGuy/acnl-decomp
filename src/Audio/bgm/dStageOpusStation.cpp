#include "Audio/bgm/dStageOpusStation.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Audio/bgm/dBgmTrackMixer.h"

// 0x005878D4
void audio::bgm::StageOpusStation::Initialize()
{
    m_IsTrainLeaving = 0;
}

// 0x005878E0
void audio::bgm::StageOpusStation::Leave(stage::Name)
{
    Stop();
    GetTrackMixer()->RemoveMix(21);
}

// 0x00587900
void audio::bgm::StageOpusStation::OnStop()
{
    if (m_IsTrainLeaving) {
        GetRequestMgr()->SetFade(KIND_SHOP, m_ShopBgmId, 160, 0, 0);
        return;
    }
    GetRequestMgr()->Remove(KIND_SHOP, m_ShopBgmId);
}

// 0x00587960
// 0x0058795C (deleting dtor)
audio::bgm::StageOpusStation::~StageOpusStation()
{
    // empty (in the original too)
}
