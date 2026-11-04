#include "netgame/dGameGlobals.h"
#include "netgame/dGameFunctions.h"
#include "netgame/dNetGameMgr.h"

#include "Bs/dBsTelop.h"
#include "Other/dFriendList.h"

namespace netgame {


namespace {
const u8 PACKET_PROFILE = 127;

enum PlayerFlag : u8
{
    PLAYER_FLAG_SENT = 1 << 0,
    PLAYER_FLAG_RECEIVED = 1 << 1,
    PLAYER_FLAG_NEW = 1 << 2,
    PLAYER_FLAG_DONE = 1 << 3,
};

const u8 NO_CURRENT = 4;
} // namespace

// 0x0062B848 (name is ours)
void netgame::NetGameMgr::Mgr8B8::Add(PlayerNo no)
{
    m_Players[no].flags = 0;
    FriendId id;
    id.friendCode[0] = 0;
    id.friendCode[1] = 0;
    NetGameMgr::s_pInstance->m_AvatarMgr.GetFriendId(&id, no);
    if (FriendList::s_pInstance->IsFriend(&id)) {
        m_Players[no].flags |= PLAYER_FLAG_DONE;
    }
}

// 0x0062B8B0 (name is ours)
void netgame::NetGameMgr::Mgr8B8::Process()
{
    FriendList* list = FriendList::s_pInstance;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        u8 flags = m_Players[i].flags;
        if ((flags & (PLAYER_FLAG_NEW | PLAYER_FLAG_DONE)) == 0 && (flags & PLAYER_FLAG_SENT) != 0 &&
            (flags & PLAYER_FLAG_RECEIVED) != 0) {
            m_Players[i].flags = flags | PLAYER_FLAG_NEW;
        }
        flags = m_Players[i].flags;
        if ((flags & PLAYER_FLAG_DONE) != 0 && (flags & PLAYER_FLAG_SENT) == 0 && (flags & PLAYER_FLAG_RECEIVED) != 0) {
            PlayerNo myNo = netgame::GetMyPlayerNo();
            if (myNo < PLAYER_NO_MAX && list->GetProfile(m_Profiles[myNo])) {
                EnqueuePacket(PACKET_PROFILE, static_cast<PlayerNo>(i), m_Profiles[myNo], PROFILE_SIZE);
                m_Players[i].flags |= PLAYER_FLAG_SENT;
            }
        }
    }
    if (m_Current != NO_CURRENT) {
        if (list->IsProfileDone()) {
            if (list->GetResult() >= 0) {
                m_Players[m_Current].flags |= PLAYER_FLAG_DONE;
                BsTelop::ShowNameTelop(static_cast<BsTelop::TelopName>(27), &m_Names[m_Current]);
            }
            m_Current = NO_CURRENT;
        }
        return;
    }
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        u8 flags = m_Players[i].flags;
        if ((flags & PLAYER_FLAG_DONE) == 0 && (flags & PLAYER_FLAG_NEW) != 0 && list->AddProfile(m_Profiles[i])) {
            m_Current = i;
            return;
        }
    }
}

// 0x0062BA50 (name is ours)
bool netgame::NetGameMgr::Mgr8B8::SendProfile(PlayerNo no)
{
    if ((m_Players[no].flags & PLAYER_FLAG_SENT) != 0) {
        return true;
    }
    PlayerNo myNo = netgame::GetMyPlayerNo();
    if (myNo >= PLAYER_NO_MAX || !FriendList::s_pInstance->GetProfile(m_Profiles[myNo])) {
        return false;
    }
    EnqueuePacket(PACKET_PROFILE, no, m_Profiles[myNo], PROFILE_SIZE);
    m_Players[no].flags |= PLAYER_FLAG_SENT;
    Func215508(g_pUnknown94FD84, no);
    return true;
}

// 0x0062BAE0 (name is ours)
netgame::NetGameMgr::Mgr8B8::Player::Player()
{
    flags = 0;
}

// 0x0062BAEC (name is ours)
netgame::NetGameMgr::Mgr8B8::Mgr8B8()
{
    m_Current = NO_CURRENT;
}

// 0x0062BB3C (name is ours)
netgame::NetGameMgr::Mgr8B8::~Mgr8B8()
{
    // (only the members are destroyed)
}
} // namespace netgame
