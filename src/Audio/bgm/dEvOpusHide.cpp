#include "Audio/bgm/dEvOpusHide.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/sound_Extern.h"

// 0x00584978
void audio::bgm::EvOpusHide::Begin()
{
    if (m_IsStarted) {
        m_IsWaiting = 0;
        m_IsResumed = 0;
        m_StopsAtOnce = 0;
    }
}

// 0x00584998
bool audio::bgm::EvOpusHide::StartRequest(u8 kind, u32 id)
{
    if (!m_IsStarted && !m_IsResumed) {
        return false;
    }
    if (!GetRequestMgr()->Has(kind, id)) {
        RequestMgr::Times times = {-1, -1, 0};
        GetRequestMgr()->Request(kind, id, 20, times);
    }
    return true;
}

// 0x00584A24
bool audio::bgm::EvOpusHide::StopRequest(u8 kind, u32 id)
{
    if (kind != KIND_STAGE_EVENT || m_IsStarted || m_IsResumed) {
        GetRequestMgr()->Remove(kind, id);
        return true;
    }
    if (m_IsWaiting && g_Unknown951718.mode == 1 && g_Unknown951718.flag5 == 0) {
        return false;
    }
    if (m_StopsAtOnce) {
        GetRequestMgr()->Remove(kind, id);
    } else {
        GetRequestMgr()->SetFade(kind, id, 15, 180, 0);
    }
    return true;
}

// 0x00584AF8
void audio::bgm::EvOpusHide::Update()
{
    if (m_IsStarted && m_IsWaiting) {
        GetRequestMgr()->SetDelay(KIND_STAGE_EVENT, GetBgmId(), 1);
        m_IsStarted = 0;
    }
}

// 0x00584B48
void audio::bgm::EvOpusHide::End(bool)
{
    m_IsStarted = 0;
}

// 0x00584B58
// 0x00584B54 (deleting dtor)
audio::bgm::EvOpusHide::~EvOpusHide()
{
    // empty (in the original too)
}
