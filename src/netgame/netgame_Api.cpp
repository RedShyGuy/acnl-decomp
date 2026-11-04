#include "netgame/netgame_Api.h"

#include "netgame/dAvatarId.h"
#include "netgame/dMatchingList.h"
#include "netgame/dGameGlobals.h"
#include "netgame/dGameFunctions.h"
#include "netgame/dNetGameMgr.h"
#include "net/dNet.h"
#include <cstring>

namespace netgame {


// 0x0087CD98 (name is ours)
extern const u32 s_NoPrincipalId = 0;

void MakeParam(u32* param, u16 low, u32 high);
void ClearState(u32 id);
void ClearPlayerStates(PlayerNo player);
void OnPlayerEntered(PlayerNo player, const MachineBitTable* players);
void OnEntered(PlayerNo player, const MachineBitTable* players);
void StartSave(bool endSession);
namespace {
// the island data of the game (game code that is not decompiled yet; names are ours)
struct IslandData
{
    u32 unk_0x0;
    u32 unk_0x4;
    u32 unk_0x8;
    void* map; // 0xC
};
} // namespace

// 0x008902A0 (name is ours)
extern const u16 INVALID_PLAYER_ID = 0;
// the player id of a session that has none (name is ours)
// 0x008902A2
extern const u16 UNKNOWN_PLAYER_ID = 0xFFFF;
// 0x009536FC (name is ours)
IslandData g_IslandData;

// 0x002FC990 | libgarden
bool IsMultiplePlayer()
{
    return NetGameMgr::s_pInstance->GetPlayerCount() > 1;
}

// 0x00300838 | libgarden
bool IsMultiPlayer()
{
    return NetGameMgr::s_pInstance->IsMultiPlayer();
}

// 0x00305ED8 | libgarden
Type GetNetgameType()
{
    return NetGameMgr::s_pInstance->m_Type;
}

// 0x0030B824 | libgarden
bool IsHost()
{
    return NetGameMgr::s_pInstance->GetMyPlayerNo() == PLAYER_NO_HOST;
}

// 0x00305EF0 (name after libgarden)
PlayerNo GetMyPlayerNo()
{
    return NetGameMgr::s_pInstance->GetMyPlayerNo();
}

// 0x00617BE8 | libgarden
bool InitNet()
{
    return NetGameMgr::s_pInstance->InitNet();
}

// 0x00625488 | libgarden
void EnqueuePacket(u8 type, PlayerNo target, const void* data, size_t size)
{
    PacketQueue& queue = NetGameMgr::s_pInstance->m_PacketQueue;
    queue.BeginPacket();
    if (data != NULL && size != 0) {
        queue.AddPacketData(data, size);
    }
    queue.EndPacket(type, target);
}

// 0x006259D8 | libgarden
bool ExistsPlayer(PlayerNo player)
{
    return NetGameMgr::s_pInstance->IsExisting(player);
}

// 0x00625B88 | libgarden
void SetNetGameType(Type type)
{
    NetGameMgr::s_pInstance->m_Type = type;
}

// 0x00626150 | libgarden
const AvatarMgr::AvatarInfo* GetAvatar(PlayerNo player)
{
    return NetGameMgr::s_pInstance->m_AvatarMgr.GetAvatarInfo(player);
}

// 0x00626168 | libgarden
u8 GetPlayerCount()
{
    return NetGameMgr::s_pInstance->GetPlayerCount();
}

// 0x00626218 | libgarden
bool IsConnectedBestFriend()
{
    return NetGameMgr::s_pInstance->m_ConnectMgr.IsConnected(ConnectMgr::CONNECTION_BEST_FRIEND);
}

// 0x00627368 | libgarden
bool StopNet()
{
    return NetGameMgr::s_pInstance->StopNet();
}

// 0x0062739C | libgarden
bool StartConnectBestFriend()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    mgr->m_BestFriendMgr.Stop();
    return mgr->m_ConnectMgr.Connect(ConnectMgr::CONNECTION_BEST_FRIEND);
}

// 0x00627408 (name is ours)
bool StopConnectBestFriend()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    mgr->m_BestFriendMgr.Stop();
    return mgr->m_ConnectMgr.Disconnect(ConnectMgr::CONNECTION_BEST_FRIEND);
}

// 0x0061B278 | libgarden
bool IsStageControl()
{
    return NetGameMgr::s_pInstance->IsStageControl();
}

// 0x0061B2E8 | libgarden
bool RequestOpenGateRandomMatch(GateMgr::Flag flag)
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->GetMyPlayerNo() == PLAYER_NO_HOST) {
        return mgr->m_GateMgr.Open(flag);
    }
    u16 data = flag << 8;
    EnqueuePacket(48, PLAYER_NO_HOST, &data, sizeof(data));
    return true;
}

// 0x00620840 | libgarden
bool RequestCloseGateRandomMatch(GateMgr::Flag flag)
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->GetMyPlayerNo() == PLAYER_NO_HOST) {
        return mgr->m_GateMgr.Close(flag);
    }
    u16 data = (flag << 8) | 1;
    EnqueuePacket(48, PLAYER_NO_HOST, &data, sizeof(data));
    return true;
}

// 0x006A7988 | libgarden
void* GetIslandMap()
{
    return g_IslandData.map;
}

// 0x006181D4 (name is ours)
u32 netgame::NetGameMgr::GetBestFriendHistory(BestFriendMgr::Message* messages, u32 maxNum, u32 start, s32* totalNum, bool clearsNew)
{
    return m_BestFriendMgr.GetHistory(messages, maxNum, start, totalNum, clearsNew);
}

// 0x006182FC (name is ours)
bool netgame::NetGameMgr::SendBestFriendTextToAll(const sead::SafeStringBase<wchar_t>& text)
{
    return m_BestFriendMgr.SendText(text, &s_NoPrincipalId, &INVALID_PLAYER_ID);
}

// 0x00618E44 (name is ours)
void MakeParam(u32* param, u16 low, u32 high)
{
    if (param != NULL) {
        *param = low | (high << 16);
    }
}

// 0x00623930 (name is ours)
bool IsMgr8Done()
{
    return NetGameMgr::s_pInstance->m_Mgr8.IsDone();
}

// 0x006254F8 (name is ours)
bool IsJoinFlag15Set()
{
    return NetGameMgr::s_pInstance->m_JoinMgr.unk_0x15;
}

// 0x00625514 (name is ours)
void ResetMgr8()
{
    NetGameMgr::s_pInstance->m_Mgr8.Reset();
}

// 0x0062552C (name is ours)
s8 GetMgr8Flag()
{
    return NetGameMgr::s_pInstance->m_Mgr8.m_Flag;
}

// 0x00625544 (name is ours)
bool IsMyPlayerHost()
{
    return NetGameMgr::s_pInstance->GetMyPlayerNo() == PLAYER_NO_HOST;
}

// 0x0062585C (name is ours)
bool IsPastLeavingPoint()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->m_Type != TYPE_RANDOM_MATCH) {
        PlayerNo leaving = mgr->m_LeaveMgr.m_LeavingPlayer;
        if (leaving >= PLAYER_NO_MAX) {
            return false;
        }
        u8 state = mgr->m_LeaveMgr.m_State;
        if (mgr->GetMyPlayerNo() == leaving) {
            return state >= 3 && mgr->unk_0x132B2 != 0;
        }
        return state > 11;
    }
    PlayerNo leaving = mgr->m_IslandMgr.m_LeavingPlayer;
    if (leaving >= PLAYER_NO_MAX) {
        return false;
    }
    u8 state = mgr->m_IslandMgr.m_State;
    if (mgr->m_IslandMgr.m_LeaveKind == 1) {
        return mgr->GetMyPlayerNo() == leaving ? state >= 52 : state >= 58;
    }
    return mgr->GetMyPlayerNo() == leaving ? state >= 39 : state > 47;
}

// 0x00625958 (name is ours)
void SendSessionOpen()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST) {
        return;
    }
    PacketQueue& queue = mgr->m_PacketQueue;
    queue.BeginPacket();
    queue.EndPacket(mgr->m_Type == TYPE_RANDOM_MATCH ? 34 : 32, PLAYER_NO_INVALID);
}

// 0x006259C0 (name is ours)
u32 GetLatency(PlayerNo player)
{
    return NetGameMgr::s_pInstance->m_AvatarMgr.GetLatency(player);
}

// 0x00625B78 (name is ours)
bool ConnectFriendMatch()
{
    return NetGameMgr::s_pInstance->ConnectFriendMatch();
}

// 0x00625C04 (name is ours)
void UpdateState(u32 id, const void* value, bool shouldSend)
{
    NetGameMgr::s_pInstance->m_PacketQueue.UpdateState(id, value, shouldSend);
}

// 0x00625C24 (name is ours)
bool AreAllPlayersInSave()
{
    StageMgr& stageMgr = NetGameMgr::s_pInstance->m_StageMgr;
    return stageMgr.AreAllPlayersIn(stage::NAME_SAVE) && stageMgr.GetTransitioningPlayer() >= PLAYER_NO_MAX;
}

// 0x00625C68 (name is ours)
bool ConnectLocal()
{
    return NetGameMgr::s_pInstance->ConnectLocal();
}

// 0x00626178 (name is ours)
PlayerNo GetLeavingPlayer()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    return mgr->m_Type == TYPE_RANDOM_MATCH ? mgr->m_IslandMgr.m_LeavingPlayer : mgr->m_LeaveMgr.m_LeavingPlayer;
}

// 0x006263AC (name is ours)
void ResetSaveDemo()
{
    NetGameMgr::s_pInstance->m_SaveDemoMgr.Reset();
}

// 0x00626438 (name is ours)
bool IsSaveConfirmed()
{
    return NetGameMgr::s_pInstance->m_SaveDemoMgr.m_Confirm.IsComplete();
}

// 0x0062644C (name is ours)
u8 GetSaveNo()
{
    return NetGameMgr::s_pInstance->m_SaveNo;
}

// 0x00626464 (name is ours)
bool ConfirmLeaving()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    PlayerNo myNo = mgr->m_MyPlayerNo;
    PacketQueue& queue = mgr->m_PacketQueue;
    if (mgr->m_Type != TYPE_RANDOM_MATCH) {
        if (myNo == PLAYER_NO_HOST) {
            mgr->m_LeaveMgr.m_GuestConfirm.Add(PLAYER_NO_HOST);
        } else if (mgr->m_LeaveMgr.m_LeavingPlayer == myNo) {
            return mgr->SendPacketBatch(PLAYER_NO_HOST, NULL, NULL, 0, 17, 0, 0, 0);
        } else {
            queue.BeginPacket();
            queue.EndPacket(33, PLAYER_NO_HOST);
        }
        return true;
    }
    PlayerNo leaving = mgr->m_IslandMgr.m_LeavingPlayer;
    if (myNo == PLAYER_NO_HOST) {
        mgr->m_IslandMgr.m_Confirms[6].Add(PLAYER_NO_HOST);
        if (leaving == PLAYER_NO_HOST) {
            mgr->m_IslandMgr.unk_0x142 = true;
        }
    } else if (leaving == myNo) {
        return mgr->SendPacketBatch(PLAYER_NO_HOST, NULL, NULL, 0, 19, 0, 0, 0);
    } else if (leaving == PLAYER_NO_HOST) {
        return mgr->SendPacketBatch(PLAYER_NO_HOST, NULL, NULL, 0, 21, 0, 0, 0);
    } else {
        queue.BeginPacket();
        queue.EndPacket(35, PLAYER_NO_HOST);
    }
    return true;
}

// 0x00626600 (name is ours)
bool IsJoinNotFinished()
{
    return !NetGameMgr::s_pInstance->m_JoinMgr.unk_0xF;
}

// 0x00626C14 (name is ours)
u8 GetPlayerIndex(PlayerNo player)
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->IsMultiPlayer() && player < PLAYER_NO_MAX && mgr->IsExisting(player)) {
        u8 index = 0;
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            PlayerNo no = static_cast<PlayerNo>(i);
            if (no == player) {
                return index;
            }
            if (mgr->IsExisting(no)) {
                index++;
            }
        }
    }
    return PLAYER_NO_INVALID;
}

// 0x00626C90 (name is ours)
bool IsLeaveSaved()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->m_Type == TYPE_RANDOM_MATCH) {
        return mgr->m_IslandMgr.unk_0x141;
    }
    return mgr->m_LeaveMgr.unk_0x253D;
}

// 0x00626CE0 (name is ours)
bool IsSaveDemoWaiting()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    u8 state = mgr->m_SaveDemoMgr.m_State;
    return mgr->GetMyPlayerNo() != PLAYER_NO_HOST ? state == 7 : state == 10;
}

// 0x00626D24 (name is ours)
s32 GetLockResult()
{
    return NetGameMgr::s_pInstance->GetLockResult();
}

// 0x00626D34 (name is ours)
bool RequestLock0()
{
    return NetGameMgr::s_pInstance->RequestLock0();
}

// 0x00626D44 (name is ours)
void SetJoinFinished()
{
    NetGameMgr::s_pInstance->m_JoinMgr.unk_0xF = true;
}

// 0x00626D5C (name is ours)
void ConfirmSave()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST) {
        mgr->SendPacketBatch(PLAYER_NO_HOST, NULL, NULL, 0, 22, 0, 0, 0);
    } else {
        mgr->m_SaveDemoMgr.m_Confirm.Add(PLAYER_NO_HOST);
    }
}

// 0x00626DC8 (name is ours)
void SetLockUnlocked()
{
    NetGameMgr::s_pInstance->m_LockMgr.m_Unlocked = true;
}

// 0x00626DE0 (name is ours)
bool IsJoinDone()
{
    return NetGameMgr::s_pInstance->IsJoinDone();
}

// 0x00626DF0 (name is ours)
bool RequestLock1()
{
    return NetGameMgr::s_pInstance->RequestLock1();
}

// 0x00626E00 (name is ours)
bool CanSendToAll(PlayerNo player)
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST) {
        return false;
    }
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no != PLAYER_NO_HOST && (mgr->IsExisting(no) || player == no) && !mgr->CanSend(no, false, 0)) {
            return false;
        }
    }
    return true;
}

// 0x00626E84 (name is ours)
void ClearLockSource()
{
    NetGameMgr::s_pInstance->m_LockMgr.m_SourcePlayer = PLAYER_NO_INVALID;
}

// 0x0062712C (name is ours)
bool StartLeaveExchange()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST) {
        return true;
    }
    if (mgr->m_Type == TYPE_RANDOM_MATCH) {
        return mgr->m_IslandMgr.RequestTourData(mgr->m_IslandMgr.m_LeavingPlayer);
    }
    PlayerNo leaving = mgr->m_LeaveMgr.m_LeavingPlayer;
    if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST) {
        return false;
    }
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no != PLAYER_NO_HOST && (mgr->IsExisting(no) || leaving == no) && !mgr->CanSend(no, false, 0)) {
            return false;
        }
    }
    mgr->m_LeaveMgr.unk_0x253D = true;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no != PLAYER_NO_HOST && (mgr->IsExisting(no) || leaving == no)) {
            mgr->SendPacketBatch(no, NULL, NULL, 0, 18, 0, 0, 0);
        }
    }
    return true;
}

// 0x0062727C (name is ours)
bool CreateSession(const char16* text, u32 length, u16 param)
{
    return NetGameMgr::s_pInstance->CreateSession(text, length, param);
}

// 0x00627298 (name is ours)
bool IsSaveDemoDone()
{
    return NetGameMgr::s_pInstance->m_SaveDemoMgr.m_IsDone;
}

// 0x006272B4 (name is ours)
bool IsMySaveConfirmed()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    return mgr->m_SaveDemoMgr.m_Confirm.IsSet(mgr->m_MyPlayerNo);
}

// 0x006272E4 (name is ours)
bool RequestLock2()
{
    return NetGameMgr::s_pInstance->RequestLock2();
}

// 0x006272F4 (name is ours)
bool IsLeavingPlayerGone()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (!mgr->IsMultiPlayer()) {
        return true;
    }
    PlayerNo leaving = mgr->m_Type == TYPE_RANDOM_MATCH ? mgr->m_IslandMgr.m_LeavingPlayer : mgr->m_LeaveMgr.m_LeavingPlayer;
    return leaving >= PLAYER_NO_MAX || !mgr->IsInSession(leaving, true);
}

// 0x00627358 (name is ours)
bool IsJoining()
{
    return NetGameMgr::s_pInstance->IsJoining();
}

// 0x00627378 (name is ours)
bool RequestLock4()
{
    return NetGameMgr::s_pInstance->RequestLock4();
}

// 0x00627388 (name is ours)
bool JoinSession(u32 sessionId)
{
    return NetGameMgr::s_pInstance->JoinSession(sessionId);
}

// 0x006273D0 (name is ours)
void* GetState(u32 id)
{
    return NetGameMgr::s_pInstance->m_PacketQueue.GetState(id);
}

// 0x006273E8 (name is ours)
void StartSaveContinue()
{
    StartSave(false);
}

// 0x006273F0 (name is ours)
void StartSave(bool endSession)
{
    NetGameMgr::s_pInstance->StartSave(endSession);
}

// 0x0062743C (name is ours)
void ClearState(u32 id)
{
    NetGameMgr::s_pInstance->m_PacketQueue.ClearState(id);
}

// the states of the actions of the players
// 0x00882B94 (name is ours)
extern const u8 s_PlayerStates[4] = {1, 2, 3, 4};
// 0x00882B98 (name is ours)
extern const u8 s_PlayerActionStates[4] = {5, 6, 7, 8};

// 0x00627454 (name is ours)
void ClearPlayerStates(PlayerNo player)
{
    if (player < PLAYER_NO_MAX) {
        ClearState(s_PlayerStates[player]);
        ClearState(s_PlayerActionStates[player]);
    }
}

// 0x00627488 (name is ours)
bool RequestLock3()
{
    return NetGameMgr::s_pInstance->RequestLock3();
}

// 0x00627498 (name is ours)
void StartLeave()
{
    NetGameMgr::s_pInstance->StartLeave();
}

// 0x006274A8 (name is ours)
void netgame::NetGameMgr::OnPlayerEntered()
{
    PlayerNo player = m_LockMgr.m_CmdIssuePlayer;
    m_JoinMgr.unk_0x15 = false;
    netgame::OnPlayerEntered(player, &m_Bits[0]);
}

// 0x006274C4 (name is ours)
void OnPlayerEntered(PlayerNo player, const MachineBitTable* /* players */)
{
    Func30804C();
    Func29D43C(g_pUnknown954884);
    g_EnteringPlayer = player;
    Func582F4C();
    Func30048C(1, 1);
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    mgr->m_Mgr8B8.Add(player);
    mgr->m_FriendListMgr.UpdateBestFriend(player);
    g_PlayerFlags |= 1 << player;
    g_PlayerFlags &= ~(1 << (player + 4));
}

// 0x00627564 (name is ours)
void StartEntry()
{
    NetGameMgr::s_pInstance->StartEntry();
}

// 0x00627574 (name is ours)
void netgame::NetGameMgr::LoadEntry()
{
    PlayerNo myNo = m_MyPlayerNo;
    m_EntryData.Load(myNo);
    OnEntered(myNo, &m_Bits[0]);
}

// 0x006275A8 (name is ours)
void OnEntered(PlayerNo player, const MachineBitTable* /* players */)
{
    Func30804C();
    Func29D318(g_pUnknown954884);
    g_EnteringPlayer = PLAYER_NO_INVALID;
    Func582F4C();
    Func30048C(1, 1);
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no != player && IsInSession(no, true)) {
            mgr->m_Mgr8B8.Add(no);
            mgr->m_FriendListMgr.UpdateBestFriend(no);
        }
        g_PlayerFlags = (g_PlayerFlags | (1 << no)) & ~(1 << (no + 4));
    }
}

// 0x0062766C (name is ours)
bool AreJoinFlagsSet()
{
    return NetGameMgr::s_pInstance->m_JoinMgr.IsPlayerFlagSet();
}

// 0x00627680 (name is ours)
bool IsJoinStarted()
{
    return NetGameMgr::s_pInstance->IsJoinStarted();
}

// 0x00627690 (name is ours)
void OnPlayerEntered()
{
    NetGameMgr::s_pInstance->OnPlayerEntered();
}

// 0x006276A0 (name is ours)
void LoadEntry()
{
    NetGameMgr::s_pInstance->LoadEntry();
}

// 0x006276B0 (name is ours)
void OnPlayerLeaveFinished(PlayerNo player, const MachineBitTable* others)
{
    ClearPlayerStates(player);
    Func30804C();
    Func29D43C(g_pUnknown954884);
    Func4E83C0(player, 0);
    if (others->GetCount() == 0) {
        Func2C2644(g_pUnknown95488C);
    }
}

// 0x00627704 (name is ours)
void OnSessionEnd(PlayerNo /* player */, const MachineBitTable* /* others */)
{
    Func30804C();
    Func29D3CC(g_pUnknown954884);
    Func582F4C();
    Func30048C(1, 1);
    for (u32 i = 0; i < PLAYER_NO_MAX; i++) {
        Func4E83C0(i, 0);
    }
    Func3043F4(g_UnknownA83000);
    Func2FA1FC(g_pUnknown95488C);
    g_UnknownAAFF14 &= ~4;
}

// 0x0062777C (name is ours)
void SetJoinAck()
{
    NetGameMgr::s_pInstance->m_JoinMgr.unk_0xD = true;
}

// 0x006278C8 (name is ours)
void CreateNetGameMgr()
{
    sead::Heap* heap = g_pNetGameHeap;
    if (heap != NULL) {
        NetGameMgr::createInstance(heap);
        NetGameMgr::s_pInstance->AllocBuffers(heap);
    }
}

// 0x00626270 (name is ours)
bool IsLeaveConfirmed()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    PlayerNo myNo = mgr->m_MyPlayerNo;
    if (mgr->m_Type != TYPE_RANDOM_MATCH) {
        LeaveMgr& leave = mgr->m_LeaveMgr;
        if (myNo != PLAYER_NO_HOST) {
            return leave.m_GuestConfirm.IsSet(myNo);
        }
        return leave.m_GuestConfirm.IsComplete() && leave.unk_0x253E;
    }
    IslandMgr& island = mgr->m_IslandMgr;
    if (myNo != PLAYER_NO_HOST) {
        return island.m_Confirms[6].IsSet(myNo);
    }
    if (island.m_LeaveKind == 0) {
        return true;
    }
    return island.m_Confirms[6].IsComplete() && island.unk_0x142;
}

// 0x0061B260 (name after libgarden)
bool IsStageControl(stage::Name stage)
{
    return NetGameMgr::s_pInstance->IsStageControl(stage);
}

// a second entry into IsStageControl (only a nop in front of it)
// 0x0061B274 (name is ours)
bool IsStageControlEntry()
{
    return IsStageControl();
}

// 0x0075EE14 (name is ours)
bool netgame::NetGameMgr::IsStageControl(stage::Name stage) const
{
    if (stage >= stage::NAME_MAX) {
        return false;
    }
    if (!m_IsInNetgame) {
        return true;
    }
    PlayerNo myNo = m_MyPlayerNo;
    if (myNo >= PLAYER_NO_MAX) {
        return true;
    }
    if (m_StageMgr.m_Stages.GetStage(myNo) != stage) {
        return false;
    }
    return m_StageMgr.m_Stages.HasControl(myNo);
}

// 0x0075EF1C (name is ours)
bool netgame::NetGameMgr::WritePacketHead(u8** buffer, PlayerNo player, bool isSystem, u32 kind)
{
    if (player >= PLAYER_NO_MAX || buffer == NULL || *buffer == NULL) {
        return false;
    }
    bool wasHandled = m_Receiver.WasHandled(player);
    PacketHead head;
    head.handled = wasHandled;
    head.isSystem = isSystem;
    head.kind = kind < 27 ? kind : 27;
    head.counter = m_SendCounters[player];
    std::memcpy(*buffer, &head, sizeof(head));
    *buffer += sizeof(head);
    return wasHandled;
}

// 0x0075F074 (name is ours)
bool netgame::LockMgr::IsWaitingForCmd(Cmd cmd) const
{
    return cmd < CMD_COUNT && m_Cmd == cmd && m_Action == ACTION_WAIT;
}

// 0x0075F09C (name after libgarden)
s32 netgame::IslandMgr::GetRandomMatchingResult() const
{
    if (!NetGameMgr::s_pInstance->m_IsConnectRandomMatch || m_State == 0 || m_State == 3) {
        return 1;
    }
    return m_State == 95 ? -1 : 0;
}

// 0x0075F0DC (name is ours)
bool netgame::IslandMgr::CanDoTrip() const
{
    return m_State == 0 || m_State == 7 || m_State == 54 || m_State == 42;
}

// 0x0075F104 (name is ours)
bool netgame::IslandMgr::IsDepartingInRandomMatch() const
{
    if (!netgame::IsMultiPlayer() || GetNetgameType() != TYPE_RANDOM_MATCH) {
        return false;
    }
    return m_PlayersThatDepartForTour.IsSet(netgame::GetMyPlayerNo());
}

// 0x0075F140 (name after libgarden)
bool netgame::IslandMgr::IsRetiringRandomMatch()
{
    if (netgame::GetPlayerCount() <= 1) {
        return true;
    }
    if (!netgame::IsHost()) {
        return m_State == 39;
    }
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    StageMgr& stageMgr = mgr->m_StageMgr;
    return stageMgr.GetTransitioningPlayer() >= PLAYER_NO_MAX &&
           stageMgr.m_Stages.GetPlayerCount(stage::NAME_RANDOM_MATCHING_RETIRE) == mgr->GetPlayerCount() - 1;
}

// 0x0075F1C4 (name after libgarden)
bool netgame::IslandMgr::CanRetireRandomMatch()
{
    return netgame::IsMultiPlayer() && GetNetgameType() == TYPE_RANDOM_MATCH && m_State == 0;
}

// 0x0075F1FC (name is ours)
s32 netgame::IslandMgr::GetRetireLockResult()
{
    if (!IsInSession(PLAYER_NO_HOST, false)) {
        return -1;
    }
    return netgame::GetLockResult();
}

// 0x0075F224 (name is ours)
bool netgame::NetGameMgr::MatchingState::IsInvited(const u32* principalId) const
{
    u32 id = *principalId;
    return id == 0 || m_Invited[0] == id || m_Invited[1] == id || m_Invited[2] == id;
}

// 0x006200E0 (name is ours)
bool netgame::NetGameMgr::MatchingState::Invite(const u32* principalId, const wchar_t* text)
{
    if (IsInvited(principalId)) {
        return true;
    }
    net::Net& net = net::Net::s_Instance;
    if (net.IsError() && net.GetErrorLevel() >= 2) {
        return false;
    }
    if (!net.CallMatchmakeMethod25(principalId, text)) {
        return false;
    }
    Add(principalId);
    return true;
}

// 0x00620178 (name is ours)
void netgame::NetGameMgr::MatchingState::Add(const u32* principalId)
{
    if (IsInvited(principalId)) {
        return;
    }
    if (m_Invited[0] != 0 && m_Invited[1] != 0 && m_Invited[2] != 0 && netgame::GetMyPlayerNo() < PLAYER_NO_MAX) {
        // the one that is not in the session makes room
        AvatarMgr& avatars = NetGameMgr::s_pInstance->m_AvatarMgr;
        for (s32 i = 0; i < 3 && m_Invited[i] != 0; i++) {
            bool isPlaying = false;
            for (s32 j = 0; j < PLAYER_NO_MAX; j++) {
                const AvatarMgr::AvatarInfo* info = avatars.GetAvatarInfo(static_cast<PlayerNo>(j));
                if (info != NULL && info->principalId != 0 && info->avatarId.IsValid() && info->principalId == m_Invited[i]) {
                    isPlaying = true;
                    break;
                }
            }
            if (!isPlaying) {
                m_Invited[i] = 0;
                break;
            }
        }
    }
    for (s32 i = 0; i < 3; i++) {
        if (m_Invited[i] == 0) {
            m_Invited[i] = *principalId;
            return;
        }
    }
}

// 0x006202A4 (name is ours)
void netgame::NetGameMgr::MatchingState::Reset()
{
    m_Invited[0] = 0;
    m_Invited[1] = 0;
    m_Invited[2] = 0;
}

// 0x0075F258 (name is ours)
u32 netgame::MatchingList::GetEntryNum() const
{
    return m_EntryNum;
}

// 0x00760744 (name is ours)
bool netgame::NetGameMgr::BestFriendMgr::IsConnectPending() const
{
    if (!m_IsDisconnecting && !m_IsConnectRequested) {
        return false;
    }
    return !netgame::IsMultiPlayer() &&
           !NetGameMgr::s_pInstance->m_ConnectMgr.IsConnectingOrDisconnecting(ConnectMgr::CONNECTION_LOCAL);
}

// 0x0076079C (name is ours)
s32 netgame::NetGameMgr::BestFriendMgr::GetHistoryCount()
{
    m_CriticalSection.Enter();
    s32 count = m_History.m_Count;
    m_CriticalSection.Exit();
    return count;
}
} // namespace netgame
