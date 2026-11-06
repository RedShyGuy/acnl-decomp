#include "Audio/bgm/dStageOpusClub444Live.h"
#include "Audio/bgm/dBgmRequestMgr.h"

// 0x00588D28
void audio::bgm::StageOpusClub444Live::Leave(stage::Name)
{
    Stop();
    m_IsStopped = 0;
}

// 0x00588D48
void audio::bgm::StageOpusClub444Live::OnStart()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_SHOP, m_ShopBgmId, 1, times);
}

// 0x00588D80
void audio::bgm::StageOpusClub444Live::Update()
{
    if (!m_IsStopped) {
        StageOpusShopBase::Update();
    }
}

// 0x00588D98
// 0x00588D94 (deleting dtor)
audio::bgm::StageOpusClub444Live::~StageOpusClub444Live()
{
    // empty (in the original too)
}

// 0x0058B92C
void audio::bgm::StageOpusClub444Live::OnStop()
{
    if (m_IsCurrent != 1) {
        GetRequestMgr()->SetFade(KIND_SHOP, m_ShopBgmId, 90, 0, 30);
        return;
    }
    GetRequestMgr()->MarkRemoved(KIND_SHOP, m_ShopBgmId);
}
