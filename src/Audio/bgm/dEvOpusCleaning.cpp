#include "Audio/bgm/dEvOpusCleaning.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/sound_Extern.h"

// 0x005868DC
void audio::bgm::EvOpusCleaning::Begin()
{
    m_IsStageStopped = 0;
    m_IsActive = 1;
}

// 0x005868F0
bool audio::bgm::EvOpusCleaning::StartRequest(u8, u32)
{
    return false;
}

// 0x0058690C
bool audio::bgm::EvOpusCleaning::StopRequest(u8 kind, u32 id)
{
    if (kind != KIND_STAGE_EVENT) {
        m_IsStageStopped = 1;
        GetRequestMgr()->SetFade(kind, id, 5, 0, 0);
        return true;
    }
    if (Func2932E4()) {
        return false;
    }
    GetRequestMgr()->SetFade(kind, id, 5, 30, 0);
    return true;
}

// 0x00586994
void audio::bgm::EvOpusCleaning::Update()
{
    if (m_IsActive && Func2932E4()) {
        m_IsActive = 0;
    }
}

// 0x005869BC
void audio::bgm::EvOpusCleaning::End(bool)
{
    m_IsActive = 0;
}

// 0x005869CC
// 0x005869C8 (deleting dtor)
audio::bgm::EvOpusCleaning::~EvOpusCleaning()
{
    // empty (in the original too)
}
