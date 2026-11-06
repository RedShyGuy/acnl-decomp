#include "Audio/bgm/dEvOpusCarnival.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/dSoundMgr.h"

// 0x0058687C
void audio::bgm::EvOpusCarnival::Begin()
{
    m_State = STATE_IDLE;
}

// 0x0058689C
void audio::bgm::EvOpusCarnival::Update()
{
    // empty (in the original too)
}

// 0x005868A0
void audio::bgm::EvOpusCarnival::End(bool)
{
    if (m_State == 1 || m_State == 3) {
        SoundMgr::SPlayCarnival(m_State);
    }
    m_State = STATE_IDLE;
    GetRequestMgr()->Remove(KIND_CARNIVAL, m_BgmId);
}

// 0x005868D8
// 0x005868D4 (deleting dtor)
audio::bgm::EvOpusCarnival::~EvOpusCarnival()
{
    // empty (in the original too)
}
