#include "Audio/bgm/dEvOpusHarvest.h"
#include "Audio/bgm/dBgmRequestMgr.h"

// 0x00585F6C
void audio::bgm::EvOpusHarvest::Begin()
{
    m_State = STATE_IDLE;
}

// 0x00585F78
void audio::bgm::EvOpusHarvest::Resume()
{
    if (m_State == STATE_COOKING) {
        m_State = STATE_COOKING_RESUMED;
        GetRequestMgr()->SetDelay(KIND_JINGLE_B, m_CookBgmId, m_CookDelay);
    }
}

// 0x00585FC8
void audio::bgm::EvOpusHarvest::End(bool)
{
    if (m_State != STATE_IDLE) {
        GetRequestMgr()->Remove(KIND_JINGLE_B, m_CookBgmId);
    }
}

// 0x00585FF8
// 0x00585FF4 (deleting dtor)
audio::bgm::EvOpusHarvest::~EvOpusHarvest()
{
    // empty (in the original too)
}
