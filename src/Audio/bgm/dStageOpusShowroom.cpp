#include "Audio/bgm/dStageOpusShowroom.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/sound_Extern.h"

// 0x0058800C
void audio::bgm::StageOpusShowroom::Initialize()
{
    m_IsFading = 0;
}

// 0x00588018
void audio::bgm::StageOpusShowroom::Leave(stage::Name)
{
    Stop();
    Stop();
    GetRequestMgr()->Remove(14, 0);
}

// 0x00588050
void audio::bgm::StageOpusShowroom::OnStart()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_SHOP, m_ShopBgmId, 0, times);
}

// 0x00588088
void audio::bgm::StageOpusShowroom::Update()
{
    StageOpusShopBase::Update();
    if (m_IsCurrent) {
        return;
    }
    if (GetRequestMgr()->Has(14, 0)) {
        if (!m_IsFading && !Func4E212C()) {
            GetRequestMgr()->SetFade(14, 0, 0, 0, 36);
            m_IsFading = 1;
            return;
        }
        if (!Func4E212C()) {
            return;
        }
        GetRequestMgr()->SetFade(14, 0, 0, 0, -1);
        m_IsFading = 0;
        return;
    }
    if (!Func4E212C()) {
        return;
    }
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(14, 0, 0, times);
    m_IsFading = 0;
}

// 0x00588184
void audio::bgm::StageOpusShowroom::OnStop()
{
    GetRequestMgr()->Remove(KIND_SHOP, m_ShopBgmId);
}

// 0x005881A4
// 0x005881A0 (deleting dtor)
audio::bgm::StageOpusShowroom::~StageOpusShowroom()
{
    // empty (in the original too)
}
