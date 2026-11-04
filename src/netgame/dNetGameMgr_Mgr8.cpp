#include "netgame/dNetGameMgr.h"

namespace netgame {
// 0x00617B60 (name is ours)
void netgame::NetGameMgr::Mgr8::Clear(PlayerNo no)
{
    if (no < PLAYER_NO_MAX) {
        m_Data[no] = 0;
    }
}

// 0x00617B70 | libgarden
void netgame::NetGameMgr::Mgr8::FUN_00617b70()
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no == 0) {
            m_Data[0] = 1;
        } else if (NetGameMgr::s_pInstance->m_Bits[0].IsSet(no)) {
            m_Data[no] = 0;
        } else {
            m_Data[no] = 1;
        }
    }
}

// 0x00617BCC (name is ours)
void netgame::NetGameMgr::Mgr8::Reset()
{
    m_Data[0] = 0;
    m_Data[1] = 0;
    m_Data[2] = 0;
    m_Data[3] = 0;
    m_Flag = 0;
}

// 0x00617BDC (name is ours)
void netgame::NetGameMgr::Mgr8::ResetFlag()
{
    m_Flag = 0;
}

// 0x0075ED9C (name is ours)
bool netgame::NetGameMgr::Mgr8::IsDone() const
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->m_MyPlayerNo != PLAYER_NO_HOST) {
        return false;
    }
    for (s32 i = 1; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (mgr->m_Bits[0].IsSet(no) && m_Data[no] == 0) {
            return false;
        }
    }
    return true;
}
} // namespace netgame
