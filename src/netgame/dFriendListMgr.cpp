#include "netgame/dFriendListMgr.h"

#include "Other/dFriendList.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvBestFriendList.h"
#include "Sv/dSvPlayer.h"
#include "net/dNet.h"
#include "net/dNetBase64.h"
#include "net/nex/dImpl.h"
#include "netgame/dGameFunctions.h"
#include "netgame/dNetGameMgr.h"

namespace sead {
class Heap;
} // namespace sead

namespace netgame {


namespace {
// 0x00622EAC (name is ours)
s32 CompareByPrincipalId(const FriendListMgr::OnlineFriend* element, const void* key)
{
    u32 principalId = *static_cast<const u32*>(key);
    if (element->principalId > principalId) {
        return 1;
    }
    return element->principalId < principalId ? -1 : 0;
}

// 0x00623534 (name is ours)
s32 CompareByPrincipal(const FriendListMgr::OnlineFriend* element, const void* key)
{
    const Principal* principal = static_cast<const Principal*>(key);
    if (element->avatarId == principal->avatarId &&
        (principal->principalId == 0 || principal->principalId == element->principalId)) {
        return 0;
    }
    return element->principalId < principal->principalId ? -1 : 1;
}
} // namespace

// 0x00622C54 (name is ours)
void netgame::FriendListMgr::Initialize()
{
    FriendList::s_pInstance->Update();
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        m_MyAvatarId.Set(player->playerInfo);
    } else {
        m_MyAvatarId.Reset();
    }
    Refresh();
    m_IsInitialized = true;
}

// 0x00622CBC (name is ours)
void netgame::FriendListMgr::Refresh()
{
    m_CriticalSection.Enter();
    if (SvGetCurrentPlayer() != NULL) {
        m_BestFriendIds.SetFromSave();
    } else {
        m_BestFriendIds.Reset();
    }
    s32 i = 0;
    GetGardenPlus();
    SvBestFriendList* list = SvGetBestFriendListAlways();
    if (list != NULL) {
        u32 num = list->GetFriendNum();
        for (; static_cast<u32>(i) < num; i++) {
            SvBestFriend* bestFriend = list->GetFriend(i);
            FriendId& id = m_BestFriends[i];
            id.principalId = bestFriend->principalId;
            id.localFriendCode[0] = bestFriend->localFriendCode[0];
            id.localFriendCode[1] = bestFriend->localFriendCode[1];
            AssignPersonalId(&m_PersonalIds[i], &bestFriend->personalId);
            Principal principal;
            principal.Set(bestFriend->personalId, id.principalId);
            s32 index = m_OnlineFriends.search(&principal, CompareByPrincipal);
            m_BestFriendsOnline[i] = index >= 0 ? m_OnlineFriends.at(index) : NULL;
        }
    }
    for (; i < BEST_FRIEND_MAX; i++) {
        FriendId& id = m_BestFriends[i];
        id.principalId = 0;
        id.localFriendCode[0] = 0;
        id.localFriendCode[1] = 0;
        ClearPersonalId(&m_PersonalIds[i]);
        m_BestFriendsOnline[i] = NULL;
    }
    m_CriticalSection.Exit();
    net::EncodeBase64(reinterpret_cast<const u8*>(&m_BestFriendIds), sizeof(m_BestFriendIds) + sizeof(m_MyAvatarId), m_Text, 255,
                      static_cast<sead::Heap*>(NULL));
}

// 0x00622EC8 (name is ours)
bool netgame::FriendListMgr::SendStatus(bool isConnected, bool isFirst)
{
    if (!m_IsInitialized) {
        return true;
    }
    net::Net& net = net::Net::s_Instance;
    if (net.IsError()) {
        m_IsSending = false;
        return false;
    }
    if (!m_IsSending) {
        m_StatusFlags[0] = isConnected;
        m_IsSending = true;
        m_StatusFlags[1] = isFirst;
    }
    bool result = net.SendNotification103(1, m_StatusParam, reinterpret_cast<const wchar_t*>(m_Text));
    if (result) {
        m_IsSending = false;
    }
    return result;
}

// 0x00622F50 (name is ours)
void netgame::FriendListMgr::UpdateBestFriend(PlayerNo player)
{
    SvPlayer* svPlayer = GetSvPlayer(player);
    if (svPlayer == NULL) {
        return;
    }
    GetSharedData();
    SvBestFriendList* list = SvGetBestFriendList();
    if (list == NULL) {
        return;
    }
    FriendId id;
    NetGameMgr::s_pInstance->m_AvatarMgr.GetFriendId(&id, player);
    if (list->Update(svPlayer, &id)) {
        Refresh();
    }
}

// 0x00622FCC (name is ours)
void netgame::FriendListMgr::OnFriendOnline(const u32* principalId, u32 param, const wchar_t* text, u32 length, bool isFromHandler)
{
    SvPersonalId personalId;
    m_CriticalSection.Enter();
    s32 index = m_OnlineFriends.search(principalId, CompareByPrincipalId);
    OnlineFriend* onlineFriend = index >= 0 ? m_OnlineFriends.at(index) : NULL;
    if (onlineFriend == NULL) {
        onlineFriend = m_OnlineFriends.emplaceBack();
        if (onlineFriend == NULL) {
            m_CriticalSection.Exit();
            return;
        }
    }
    onlineFriend->principalId = *principalId;
    net::DecodeBase64(reinterpret_cast<const char16*>(text), length, reinterpret_cast<u8*>(&onlineFriend->ids),
                      sizeof(onlineFriend->ids) + sizeof(onlineFriend->avatarId), net::g_pNetHeap);
    onlineFriend->param = param;
    s32 bestFriend = FindBestFriend(*principalId, onlineFriend->avatarId);
    if (bestFriend >= 0) {
        m_BestFriendsOnline[bestFriend] = onlineFriend;
        if (onlineFriend->flags[0] != 0 && onlineFriend->flags[1] != 0 && onlineFriend->ids.Find(m_MyAvatarId) >= 0) {
            AssignPersonalId(&personalId, &m_PersonalIds[bestFriend]);
        } else {
            ClearPersonalId(&personalId);
        }
    }
    m_CriticalSection.Exit();
    if (isFromHandler && bestFriend >= 0 && personalId.IsValid()) {
        NetGameMgr::s_pInstance->m_BestFriendMgr.OnBestFriendOnline(principalId, &personalId);
    }
}

DECOMP_ALWAYS_INLINE void netgame::FriendListMgr::OnFriendOffline(const u32* principalId)
{
    m_CriticalSection.Enter();
    s32 index = m_OnlineFriends.search(principalId, CompareByPrincipalId);
    if (index >= 0) {
        OnlineFriend* onlineFriend = index < m_OnlineFriends.size() ? m_OnlineFriends.at(index) : NULL;
        s32 bestFriend = FindBestFriend(*principalId, onlineFriend->avatarId);
        if (bestFriend >= 0) {
            m_BestFriendsOnline[bestFriend] = NULL;
        }
        m_OnlineFriends.erase(index);
    }
    m_CriticalSection.Exit();
}

// 0x006231E4 (name is ours)
void netgame::FriendListMgr::OnNotification(const u32* principalId, u32 param1, u32 param2, const wchar_t* text, u32 length,
                                            bool isFromHandler)
{
    if (param1 != 0) {
        OnFriendOnline(principalId, param2, text, length, isFromHandler);
    } else {
        OnFriendOffline(principalId);
    }
}

// 0x0062333C (name is ours)
bool netgame::FriendListMgr::ProcessAddBestFriend()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    switch (m_State) {
    case STATE_FETCH_FRIEND_CODE:
        if (mgr->m_AvatarMgr.FetchFriendCode(m_Player)) {
            m_State = STATE_ADD_BEST_FRIEND;
            return false;
        }
        break;
    case STATE_ADD_BEST_FRIEND: {
        SvPlayer* svPlayer = GetSvPlayer(m_Player);
        if (svPlayer != NULL) {
            GetSharedData();
            SvBestFriendList* list = SvGetBestFriendList();
            if (list != NULL) {
                FriendId id;
                mgr->m_AvatarMgr.GetFriendId(&id, m_Player);
                list->Add(svPlayer, &id);
                Refresh();
            }
        }
        m_State = STATE_SEND_STATUS;
        return false;
    }
    case STATE_SEND_STATUS: {
        bool isConnected = mgr->m_ConnectMgr.IsConnectedOrConnecting(ConnectMgr::CONNECTION_BEST_FRIEND);
        if (!m_IsInitialized || SendStatus(isConnected, false) || net::Net::s_Instance.IsError()) {
            m_State = STATE_NONE;
        } else if (!net::Net::s_Instance.IsError()) {
            return m_State == STATE_NONE;
        }
        return true;
    }
    default:
        m_State = STATE_NONE;
        return true;
    }
    return m_State == STATE_NONE;
}

// 0x006234CC (name is ours)
bool netgame::FriendListMgr::StartAddBestFriend(PlayerNo player)
{
    if (m_State != STATE_NONE) {
        return false;
    }
    m_Player = player;
    FriendId id;
    NetGameMgr::s_pInstance->m_AvatarMgr.GetFriendId(&id, player);
    bool hasId = id.principalId != 0 || (id.localFriendCode[0] | id.localFriendCode[1]) != 0;
    m_State = hasId ? STATE_ADD_BEST_FRIEND : STATE_FETCH_FRIEND_CODE;
    return true;
}

// 0x00623588 (name is ours)
bool netgame::FriendListMgr::ProcessSendStatus()
{
    if (m_State == STATE_SEND_STATUS) {
        NetGameMgr* mgr = NetGameMgr::s_pInstance;
        bool isConnected = mgr->m_ConnectMgr.IsConnectedOrConnecting(ConnectMgr::CONNECTION_BEST_FRIEND);
        if (!m_IsInitialized || SendStatus(isConnected, false) || net::Net::s_Instance.IsError()) {
            m_State = STATE_NONE;
            return true;
        }
    }
    return m_State == STATE_NONE;
}

// 0x00623664 (name is ours)
void netgame::FriendListMgr::RemoveBestFriend(u32 index)
{
    if (m_State != STATE_NONE) {
        return;
    }
    GetSharedData();
    SvBestFriendList* list = SvGetBestFriendList();
    if (list == NULL) {
        return;
    }
    list->RemoveAt(index);
    if (m_IsInitialized) {
        Refresh();
        m_State = STATE_SEND_STATUS;
    }
}

// 0x006236C0 (name is ours)
void netgame::FriendListMgr::GetBestFriend(Principal* principal, u32 index)
{
    if (index < BEST_FRIEND_MAX) {
        m_CriticalSection.Enter();
        OnlineFriend* onlineFriend = m_BestFriendsOnline[index];
        if (onlineFriend != NULL) {
            principal->principalId = onlineFriend->principalId;
            principal->avatarId = m_BestFriendIds.m_Ids[index];
            m_CriticalSection.Exit();
            return;
        }
        m_CriticalSection.Exit();
    }
    principal->Reset();
}

// 0x00623734 (name is ours)
void netgame::FriendListMgr::Finalize()
{
    m_OnlineFriends.clear();
    for (s32 i = 0; i < BEST_FRIEND_MAX; i++) {
        m_BestFriendsOnline[i] = NULL;
    }
    m_IsSending = false;
    m_IsInitialized = false;
}

// 0x006237DC (name is ours)
netgame::FriendListMgr::FriendListMgr()
{
    m_CriticalSection.Initialize();
    m_StatusParam = 0;
    m_State = STATE_NONE;
    m_Player = PLAYER_NO_INVALID;
    m_IsInitialized = false;
    m_IsSending = false;
    for (s32 i = 0; i < BEST_FRIEND_MAX; i++) {
        FriendId& id = m_BestFriends[i];
        id.principalId = 0;
        id.localFriendCode[0] = 0;
        id.localFriendCode[1] = 0;
        ClearPersonalId(&m_PersonalIds[i]);
        m_BestFriendsOnline[i] = NULL;
    }
    m_Text[0] = 0;
}

// 0x0075F4BC (name is ours)
bool netgame::FriendListMgr::IsBestFriend(PlayerNo player)
{
    const AvatarMgr::AvatarInfo* avatar = GetAvatar(player);
    if (avatar == NULL) {
        return false;
    }
    FriendId id;
    id.localFriendCode[0] = 0;
    id.localFriendCode[1] = 0;
    NetGameMgr::s_pInstance->m_AvatarMgr.GetFriendId(&id, player);
    s32 index = -1;
    if (m_IsInitialized && (id.principalId != 0 || (id.localFriendCode[0] | id.localFriendCode[1]) != 0)) {
        m_CriticalSection.Enter();
        index = m_BestFriendIds.Find(avatar->avatarId);
        if (index >= 0 && m_BestFriends[index].IsDifferent(id)) {
            index = -1;
        }
        m_CriticalSection.Exit();
    }
    return index >= 0;
}

// 0x0075F660 (name is ours)
const netgame::AvatarId& netgame::FriendListMgr::GetMyAvatarId() const
{
    return m_MyAvatarId;
}

// 0x0075F66C (name is ours)
s8 netgame::FriendListMgr::GetBestFriendFlag(u32 index)
{
    if (index >= BEST_FRIEND_MAX) {
        return 0;
    }
    s8 flag = 0;
    m_CriticalSection.Enter();
    OnlineFriend* onlineFriend = m_BestFriendsOnline[index];
    if (onlineFriend != NULL && onlineFriend->ids.Find(m_MyAvatarId) >= 0) {
        flag = static_cast<s8>(onlineFriend->flags[0]);
    }
    m_CriticalSection.Exit();
    return flag;
}

// 0x0075F6CC (name is ours)
bool netgame::FriendListMgr::HasMeAsBestFriend(const u32* principalId)
{
    bool result = false;
    m_CriticalSection.Enter();
    s32 index = m_OnlineFriends.search(principalId, CompareByPrincipalId);
    if (index >= 0) {
        OnlineFriend* onlineFriend = m_OnlineFriends.at(index);
        if (onlineFriend != NULL) {
            result = onlineFriend->ids.Find(m_MyAvatarId) >= 0;
        }
    }
    m_CriticalSection.Exit();
    return result;
}

// 0x0075F598 (name after libgarden)
s32 netgame::FriendListMgr::FindBestFriend(const u32& principal, const AvatarId& avatar)
{
    FriendId id;
    id.localFriendCode[0] = 0;
    id.localFriendCode[1] = 0;
    id.principalId = principal;
    FriendList::s_pInstance->GetFriendCode(&principal, id.localFriendCode);
    s32 index = -1;
    if (m_IsInitialized && (id.principalId != 0 || (id.localFriendCode[0] | id.localFriendCode[1]) != 0)) {
        m_CriticalSection.Enter();
        index = m_BestFriendIds.Find(avatar);
        if (index >= 0 && m_BestFriends[index].IsDifferent(id)) {
            index = -1;
        }
        m_CriticalSection.Exit();
    }
    return index;
}
} // namespace netgame
