#include "netgame/dGameFunctions.h"
#include "netgame/dNetGameMgr.h"

#include "Other/dFriendList.h"
#include "Sv/dSvApi.h"
#include "applet/dErrEulaMgr.h"
#include "net/dNet.h"
#include "nn/os/os_Api.h"
#include <cstring>

namespace netgame {

// 0x00952680 (name is ours)
s32 g_FrameCounter;

// (name after libgarden)
// 0x00954648
NetGameMgr* netgame::NetGameMgr::s_pInstance;

// 0x00617BF8 (name is ours)
bool netgame::NetGameMgr::IsEntering(PlayerNo player)
{
    m_CriticalSection.lock();
    if (m_Bits[5].IsSet(player) && !m_Bits[6].IsSet(player) && m_Bits[3].IsSet(player) && !m_Bits[4].IsSet(player)) {
        m_CriticalSection.unlock();
        return true;
    }
    m_CriticalSection.unlock();
    return false;
}

// 0x00617C88 (name is ours)
bool netgame::NetGameMgr::InitNet()
{
    bool isInitialized = true;
    if (m_NetRefCount == 0) {
        isInitialized = net::Net::s_Instance.Initialize(&m_AvatarMgr);
    }
    if (m_NetRefCount == 0 && isInitialized) {
        m_FriendListMgr.Initialize();
        m_ConnectMgr.SetPlayerName();
    }
    if (isInitialized) {
        if (m_NetRefCount != -1) {
            m_NetRefCount++;
        }
        net::Net::s_Instance.ClearError();
    }
    return isInitialized;
}

// 0x00617D20 (name is ours)
void netgame::NetGameMgr::CheckError()
{
    if (unk_0x132B4) {
        return;
    }
    applet::ErrEulaMgr* errEula = applet::ErrEulaMgr::s_pInstance;
    net::Net& net = net::Net::s_Instance;
    if (m_ErrorFlags != 0) {
        u32 code = 0;
        if (net.IsError() && net.GetErrorLevel() >= 2 && net.IsInternet(false)) {
            code = net.GetErrorCode();
        }
        if (IsErrorReady(errEula)) {
            ShowError(code, true);
        }
        return;
    }
    if (netgame::IsMultiPlayer() && net.IsError() && net.GetErrorLevel() >= 2) {
        u32 code = 0;
        if (net.IsInternet(false)) {
            code = net.GetErrorCode();
        }
        if (IsErrorReady(errEula)) {
            ShowError(code, true);
        }
        return;
    }
    if (unk_0x132B6) {
        if (!netgame::IsHost()) {
            unk_0x132B7 = false;
            unk_0x132B6 = false;
        } else if (unk_0x132B7 && m_StageMgr.GetTransitioningPlayer() >= PLAYER_NO_MAX &&
                   m_StageMgr.m_Stages.GetPlayerCount(static_cast<stage::Name>(96)) == 0 &&
                   m_StageMgr.m_Stages.GetPlayerCount(static_cast<stage::Name>(140)) == 0) {
            unk_0x132B6 = false;
            unk_0x132B7 = false;
        }
    } else if (unk_0x132B7) {
        unk_0x132B7 = false;
    }
    m_PacketQueue.Process();
    m_StageMgr.ProcessFrame();
}

// 0x00617ED8 (name is ours)
bool netgame::NetGameMgr::StopNetAndConnection()
{
    if (m_ConnectMgr.IsConnectedOrConnecting(ConnectMgr::CONNECTION_BEST_FRIEND)) {
        netgame::StopConnectBestFriend();
    }
    if (netgame::IsConnectedBestFriend() || !DisconnectCurrent()) {
        m_ConnectMgr.Process();
        return false;
    }
    CloseCurrentMenu();
    u8 saveNo = m_SaveNo;
    InitEntryOrExit(false);
    if (saveNo < 8) {
        SetPlayerActive(0, saveNo, NULL);
    }
    for (s32 i = 1; i < PLAYER_NO_MAX; i++) {
        ClearPlayer(i);
    }
    FiniNet();
    m_ConnectMgr.Clear();
    m_MatchingState.Reset();
    m_IsConnectRandomMatch = false;
    return true;
}

// 0x00617FD4 | libgarden
void netgame::NetGameMgr::FiniNet()
{
    if (m_NetRefCount != 0 && --m_NetRefCount == 0) {
        net::Net::s_Instance.Finalize();
        m_FriendListMgr.Finalize();
        m_GateMgr.Reset();
    }
}

// 0x00618024 (name is ours)
void netgame::NetGameMgr::Update()
{
    if (m_ErrorFlags == 0 && !applet::ErrEulaMgr::IsInputBlocked() && !unk_0x132B4) {
        m_ConnectMgr.Process();
        m_GateMgr.Process();
        m_StageMgr.Process();
        m_LockMgr.Process();
        m_JoinMgr.Process();
        m_SaveDemoMgr.Process();
        m_LeaveMgr.Process();
        m_IslandMgr.Process();
        m_AvatarMgr.Process();
        m_PacketQueue.ProcessEnd();
        m_BestFriendMgr.Process();
    }
    m_Mgr8B8.Process();
    if (!(m_Flags & FLAG_STALLED)) {
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            if (m_PacketQueue.GetCounter(static_cast<PlayerNo>(i)) > 0x708) {
                m_Flags |= FLAG_STALLED;
                break;
            }
        }
    }
    if (!(m_Flags & FLAG_STATION_LOST)) {
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            PlayerNo no = static_cast<PlayerNo>(i);
            if (!m_Bits[2].IsSet(no)) {
                continue;
            }
            if ((no == 0 && net::Net::s_Instance.IsError() && net::Net::s_Instance.GetErrorLevel() >= 2) ||
                !m_AvatarMgr.IsInSession(no, true)) {
                m_Flags |= FLAG_STATION_LOST;
                break;
            }
        }
    }
    if (m_ErrorFlags != m_Flags) {
        m_ErrorFlags |= m_Flags;
    }
}

// 0x006181FC (name is ours)
bool netgame::NetGameMgr::InitNetForConnect()
{
    bool isInitialized = true;
    if (m_NetRefCount == 0) {
        isInitialized = net::Net::s_Instance.Initialize(&m_AvatarMgr);
    }
    if (m_NetRefCount == 0 && isInitialized) {
        m_FriendListMgr.Initialize();
        m_ConnectMgr.SetPlayerName();
    }
    if (isInitialized) {
        if (m_NetRefCount != -1) {
            m_NetRefCount++;
        }
        net::Net::s_Instance.ClearError();
    }
    return isInitialized;
}

// 0x00618318 | libgarden
bool netgame::NetGameMgr::SendPacketBatch(PlayerNo target, const void* const* parts, const size_t* partSizes, size_t partCount,
                                          u8 type, u32 unknown1, u32 unknown2, u32 unknown3)
{
    bool hasData = partCount != 0;
    MachineBitTable targets;
    bool canSend;
    if (target < PLAYER_NO_MAX) {
        canSend = CanSend(target, hasData, unknown2);
        targets.Add(target);
    } else {
        canSend = true;
        PlayerNo myNo = m_MyPlayerNo;
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            PlayerNo no = static_cast<PlayerNo>(i);
            if (no != myNo && m_Bits[0].IsSet(no)) {
                if (!CanSend(no, hasData, unknown2)) {
                    canSend = false;
                }
                targets.Add(no);
            }
        }
    }
    if (canSend) {
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            PlayerNo no = static_cast<PlayerNo>(i);
            if (targets.IsSet(no)) {
                SendPacket(no, parts, partSizes, partCount, type, unknown1, unknown2, unknown3);
            }
        }
        return true;
    }
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (targets.IsSet(no)) {
            m_PacketQueue.IncrementCounter(no);
        }
    }
    return false;
}

// 0x006184A8 (name is ours)
bool netgame::NetGameMgr::ProcessLeave()
{
    bool isDone = true;
    if (!m_LeaveMgr.ProcessLeave()) {
        isDone = false;
    }
    if (!m_IslandMgr.ProcessLeave()) {
        isDone = false;
    }
    return isDone;
}

// 0x006184E0 (name is ours)
bool netgame::NetGameMgr::SendPacket(PlayerNo player, const void* const* parts, const size_t* partSizes, size_t partCount, u8 type,
                                     u32 unknown1, u32 unknown2, u32 unknown3)
{
    if (player >= PLAYER_NO_MAX) {
        return false;
    }
    bool hasData = false;
    if (partCount != 0) {
        if (parts == NULL || partSizes == NULL) {
            m_PacketQueue.IncrementCounter(player);
            return false;
        }
        hasData = true;
    }
    if (!CanSend(player, hasData, unknown2)) {
        m_PacketQueue.IncrementCounter(player);
        return false;
    }
    u32 total = 2;
    if (partCount != 0) {
        for (u32 i = 0; i < partCount; i++) {
            total += partSizes[i];
        }
        if (total > 0xD002) {
            m_PacketQueue.IncrementCounter(player);
            return false;
        }
    }
    m_SendMgr.Prepare(player);
    u8* buffer = m_SendMgr.GetBuffer(player);
    if (buffer == NULL) {
        m_PacketQueue.IncrementCounter(player);
        return false;
    }
    u8* start = buffer;
    if (player < PLAYER_NO_MAX) {
        PacketHead head;
        *reinterpret_cast<u16*>(&head) = 0;
        head.handled = m_Receiver.WasHandled(player);
        head.isSystem = 1;
        head.kind = type < 27 ? type : 27;
        head.counter = m_SendCounters[player];
        std::memcpy(buffer, &head, sizeof(head));
        buffer += sizeof(head);
    }
    if (type < 27) {
        for (u32 i = 0; i < partCount; i++) {
            if (parts[i] != NULL && partSizes[i] != 0) {
                std::memcpy(buffer, parts[i], partSizes[i]);
                buffer += partSizes[i];
            }
        }
    }
    bool isCommitted = m_SendMgr.Commit(player, total);
    bool isSent = m_AvatarMgr.Send(player, start, total);
    if (unknown3 == 0) {
        m_SendCounters[player]++;
    }
    m_Receiver.SetWasHandled(player, false);
    if (unknown1 != 0) {
        m_PacketQueue.IncrementCounter(player);
    } else {
        m_PacketQueue.ResetCounter(player);
    }
    return isCommitted & isSent;
}

// 0x0061875C (name is ours)
bool netgame::NetGameMgr::ConnectFriendMatch()
{
    if (m_CurrentConnection == ConnectMgr::CONNECTION_NONE && m_ConnectMgr.Connect(ConnectMgr::CONNECTION_FRIEND_MATCH) &&
        m_ConnectMgr.IsConnected(ConnectMgr::CONNECTION_FRIEND_MATCH)) {
        m_AvatarMgr.Reset();
        m_CurrentConnection = ConnectMgr::CONNECTION_FRIEND_MATCH;
        CopyNpcInfo(reinterpret_cast<u8*>(GetGardenPlus()) + 0x292A4, &m_NpcInfo, 0);
        return true;
    }
    return false;
}

// 0x006187E0 (name is ours)
void netgame::NetGameMgr::ProcessEnd()
{
    if (unk_0x132AF) {
        ResetStageData();
        unk_0x132AF = false;
    }
    m_LockMgr.ProcessEnd();
    m_StageMgr.ProcessEnd();
    m_LeaveMgr.ProcessEnd();
    m_IslandMgr.ProcessEnd();
    if (m_Type != TYPE_RANDOM_MATCH && m_IsConnectRandomMatch &&
        m_ConnectMgr.IsConnected(ConnectMgr::CONNECTION_RANDOM_MATCH)) {
        m_Type = TYPE_RANDOM_MATCH;
    }
}

// 0x0061886C (name is ours)
bool netgame::NetGameMgr::DisconnectCurrent()
{
    if (net::Net::s_Instance.LeaveSession()) {
        if (m_CurrentConnection == ConnectMgr::CONNECTION_NONE) {
            if (m_ConnectMgr.IsConnectedOrConnecting(ConnectMgr::CONNECTION_LOCAL)) {
                m_CurrentConnection = ConnectMgr::CONNECTION_LOCAL;
            } else if (m_ConnectMgr.IsConnectedOrConnecting(ConnectMgr::CONNECTION_RANDOM_MATCH)) {
                m_CurrentConnection = ConnectMgr::CONNECTION_RANDOM_MATCH;
            } else if (m_ConnectMgr.IsConnectedOrConnecting(ConnectMgr::CONNECTION_FRIEND_MATCH)) {
                m_CurrentConnection = ConnectMgr::CONNECTION_FRIEND_MATCH;
            } else {
                return true;
            }
        }
        if (m_LastConnection == ConnectMgr::CONNECTION_NONE) {
            bool isDisconnecting = m_ConnectMgr.Disconnect(m_CurrentConnection);
            m_LastConnection = m_CurrentConnection;
            if (!isDisconnecting) {
                return true;
            }
        }
        if (!m_ConnectMgr.IsConnected(m_CurrentConnection)) {
            m_CurrentConnection = ConnectMgr::CONNECTION_NONE;
            m_LastConnection = ConnectMgr::CONNECTION_NONE;
            m_GateMgr.Reset();
            return true;
        }
    }
    if (!m_PacketQueue.m_IsClosed) {
        m_PacketQueue.m_IsClosed = true;
    }
    return false;
}

// 0x00618978 (name is ours)
bool netgame::NetGameMgr::ConnectLocal()
{
    if (m_CurrentConnection != ConnectMgr::CONNECTION_NONE) {
        return false;
    }
    m_BestFriendMgr.Stop();
    if (m_ConnectMgr.IsConnectedOrConnecting(ConnectMgr::CONNECTION_BEST_FRIEND)) {
        m_ConnectMgr.Disconnect(ConnectMgr::CONNECTION_BEST_FRIEND);
        return false;
    }
    if (!m_ConnectMgr.IsConnected(ConnectMgr::CONNECTION_BEST_FRIEND) &&
        !m_ConnectMgr.IsCurrentCmd(ConnectMgr::CONNECTION_BEST_FRIEND, false) &&
        m_ConnectMgr.Connect(ConnectMgr::CONNECTION_LOCAL) && m_ConnectMgr.IsConnected(ConnectMgr::CONNECTION_LOCAL)) {
        m_AvatarMgr.Reset();
        m_CurrentConnection = ConnectMgr::CONNECTION_LOCAL;
        CopyNpcInfo(reinterpret_cast<u8*>(GetGardenPlus()) + 0x292A4, &m_NpcInfo, 0);
        return true;
    }
    return false;
}

// 0x00618A68 (name is ours)
void netgame::NetGameMgr::OnSessionStarted(u32 unknown, bool isRestart)
{
    if (m_SaveNo >= 8) {
        m_SaveNo = GetDefaultSaveNo(4);
    }
    if (!net::Net::s_Instance.IsHost()) {
        m_IsInNetSession = true;
        m_PacketQueue.m_Tick = g_FrameCounter;
        return;
    }
    m_SendMgr.AssignBuffers(PLAYER_NO_0);
    m_Receiver.AssignBuffers(PLAYER_NO_0);
    m_Bits[0].Add(PLAYER_NO_0);
    m_Bits[1].Add(PLAYER_NO_0);
    m_IsInNetSession = true;
    m_IsInNetgame = true;
    m_MyPlayerNo = PLAYER_NO_0;
    m_IsPlayerAssigned = true;
    m_AvatarMgr.Start();
    m_StageMgr.Reset();
    if (isRestart) {
        LoadNpcInfo(&m_NpcInfo, PLAYER_NO_0);
    } else {
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            if (i == 0) {
                SetPlayerActive(0, 7, SvGetCurrentPlayer());
            } else {
                ClearPlayer(i);
            }
        }
    }
    m_PacketQueue.ClearAllStates();
    OnPlayerAssigned();
    m_IsSessionClosedForIsland = unknown;
}

// 0x00618C3C (name is ours)
void netgame::NetGameMgr::RemovePlayer(PlayerNo player)
{
    if (player >= PLAYER_NO_MAX) {
        return;
    }
    m_SendCounters[player] = 0;
    m_CriticalSection.lock();
    m_ReceiveCounters[player] = 0;
    m_CriticalSection.unlock();
    m_StageMgr.RemovePlayer(player);
    m_Receiver.Clear(player);
    m_SendMgr.Reset(player);
    m_PacketQueue.ResetCounters(player);
    m_IslandMgr.RemovePlayer(player);
    m_EntryData.ClearSize(player);
    m_Mgr8.Clear(player);
    m_LeaveMgr.RemovePlayer(player);
    m_SaveDemoMgr.RemovePlayer(player);
    m_JoinMgr.RemovePlayer(player);
    m_LockMgr.RemovePlayer(player);
    m_CriticalSection.lock();
    m_Bits[3].Remove(player);
    m_Bits[4].Remove(player);
    m_Bits[5].Remove(player);
    m_Bits[6].Remove(player);
    m_Entry.Remove(player);
    m_Bits[2].Remove(player);
    m_Bits[1].Remove(player);
    m_Bits[0].Remove(player);
    m_CriticalSection.unlock();
    m_AvatarMgr.RemovePlayer(player);
}

// 0x00618DCC (name is ours)
void netgame::NetGameMgr::CheckReceiveCounter(PlayerNo player, u8 counter, bool checks)
{
    if (player >= PLAYER_NO_MAX) {
        return;
    }
    m_CriticalSection.lock();
    if (checks) {
        u8 difference = counter - m_ReceiveCounters[player];
        if (difference != 1) {
            m_Flags |= FLAG_COUNTER_MISMATCH;
        }
    }
    m_ReceiveCounters[player] = counter;
    m_CriticalSection.unlock();
}

// 0x00618E5C (name is ours)
void netgame::NetGameMgr::ResetSaveBlock()
{
    m_SaveBlock.Reset();
    ClearSaveBlock(&m_SaveBlock);
    m_SaveBlock.m_IsValid = false;
}

// 0x00618E90 (name is ours)
bool netgame::NetGameMgr::CollectGuests(MachineBitTable* targets, bool hasData, u32 unknown)
{
    if (targets == NULL) {
        return false;
    }
    bool canSend = true;
    PlayerNo myNo = m_MyPlayerNo;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no == myNo || m_LockMgr.m_SourcePlayer == no || !m_Bits[1].IsSet(no)) {
            continue;
        }
        if (!CanSend(no, hasData, unknown)) {
            canSend = false;
        }
        targets->Add(no);
    }
    return canSend;
}

// 0x00618F28 (name is ours)
bool netgame::NetGameMgr::CollectPlayers(MachineBitTable* targets, bool hasData, u32 unknown)
{
    if (targets == NULL) {
        return false;
    }
    bool canSend = true;
    PlayerNo myNo = m_MyPlayerNo;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no == myNo || !m_Bits[0].IsSet(no)) {
            continue;
        }
        if (!CanSend(no, hasData, unknown)) {
            canSend = false;
        }
        targets->Add(no);
    }
    return canSend;
}

// 0x00618FB0 (name is ours)
s32 netgame::NetGameMgr::GetLockResult()
{
    if ((m_LockMgr.m_Cmd == LockMgr::CMD_FRIEND_MATCH_ENTRY || m_LockMgr.m_Cmd == LockMgr::CMD_RANDOM_MATCH_ENTRY) &&
        !m_AvatarMgr.IsInSession(PLAYER_NO_0, true)) {
        m_LockMgr.m_State = LockMgr::STATE_UNLOCKED;
        return -1;
    }
    if (m_LockMgr.m_State == LockMgr::STATE_LOCKED) {
        return 1;
    }
    return m_LockMgr.m_State == LockMgr::STATE_UNLOCKED ? -1 : 0;
}

// 0x0061900C (name is ours)
bool netgame::NetGameMgr::RequestLock0()
{
    return m_LockMgr.Request(LockMgr::CMD_FRIEND_MATCH_ENTRY);
}

// 0x0061903C (name is ours)
bool netgame::NetGameMgr::IsJoinDone() const
{
    return m_JoinMgr.m_State == 11;
}

// 0x00619050 (name is ours)
void netgame::NetGameMgr::AssignPlayer(bool keepsPlayers)
{
    PlayerNo myNo = m_MyPlayerNo;
    if (myNo == PLAYER_NO_0 || m_IsPlayerAssigned) {
        return;
    }
    if (myNo >= PLAYER_NO_MAX) {
        myNo = m_AvatarMgr.FindMyPlayerNo();
        if (myNo >= PLAYER_NO_MAX) {
            return;
        }
    }
    SvPlayer* player = SvGetCurrentPlayer();
    m_MyPlayerNo = myNo;
    m_IsPlayerAssigned = true;
    m_SendMgr.AssignBuffers(myNo);
    m_Receiver.AssignBuffers(myNo);
    LoadNpcInfo(&m_NpcInfo, myNo);
    if (keepsPlayers) {
        return;
    }
    for (s32 i = 1; i < PLAYER_NO_MAX; i++) {
        if (i == myNo) {
            SetPlayerActive(i, i + 3, player);
        } else {
            ClearPlayer(i);
        }
    }
}

// 0x0061912C (name is ours)
bool netgame::NetGameMgr::RequestLock1()
{
    return m_LockMgr.Request(LockMgr::CMD_FRIEND_MATCH_LEAVE);
}

// 0x0061915C (name is ours)
bool netgame::NetGameMgr::CreateOrJoinRandomSession(u32 param)
{
    if (!net::Net::s_Instance.CreateOrJoinRandomSession(param)) {
        return false;
    }
    OnSessionStarted(1, false);
    return true;
}

// 0x00619190 (name is ours)
void netgame::NetGameMgr::SendSessionClosed(u8 isClosed)
{
    if (netgame::IsMultiPlayer()) {
        if (m_IsInNetSession && m_IsPlayerAssigned && m_MyPlayerNo != PLAYER_NO_0) {
            EnqueuePacket(118, PLAYER_NO_HOST, &isClosed, sizeof(isClosed));
        } else {
            EnqueuePacket(118, PLAYER_NO_TARGET_ALL, &isClosed, sizeof(isClosed));
        }
    }
    m_IsSessionClosedForIsland = isClosed;
}

// 0x00619200 (name is ours)
bool netgame::NetGameMgr::CreateSession(const char16* text, u32 length, u16 param)
{
    if (!net::Net::s_Instance.CreateSession(text, length, param, 0)) {
        return false;
    }
    if (m_SaveNo >= 8) {
        m_SaveNo = GetDefaultSaveNo(4);
    }
    m_SendMgr.AssignBuffers(PLAYER_NO_0);
    m_Receiver.AssignBuffers(PLAYER_NO_0);
    u8 saveNo = GetDefaultSaveNo(4);
    m_Bits[0].Add(PLAYER_NO_0);
    m_Bits[1].Add(PLAYER_NO_0);
    m_IsInNetSession = true;
    m_IsInNetgame = true;
    m_MyPlayerNo = PLAYER_NO_0;
    m_IsPlayerAssigned = true;
    m_AvatarMgr.Start();
    m_StageMgr.Reset();
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        if (i == 0) {
            SetPlayerActive(0, saveNo, SvGetCurrentPlayer());
        } else {
            ClearPlayer(i);
        }
    }
    m_PacketQueue.ClearAllStates();
    OnPlayerAssigned();
    return true;
}

// 0x00619310 (name is ours)
bool netgame::NetGameMgr::RequestLock2()
{
    return m_LockMgr.Request(LockMgr::CMD_SAVE_CONTINUE);
}

// 0x0061933C (name is ours)
bool netgame::NetGameMgr::IsJoining() const
{
    if (m_LockMgr.m_SourcePlayer >= PLAYER_NO_MAX) {
        return false;
    }
    if (m_MyPlayerNo != PLAYER_NO_0) {
        return m_JoinMgr.m_State > 8;
    }
    return m_JoinMgr.m_State > 4;
}

// 0x0061937C | libgarden
bool netgame::NetGameMgr::ConnectRandomMatch()
{
    if (m_CurrentConnection == ConnectMgr::CONNECTION_NONE && m_ConnectMgr.Connect(ConnectMgr::CONNECTION_RANDOM_MATCH) &&
        m_ConnectMgr.IsConnected(ConnectMgr::CONNECTION_RANDOM_MATCH)) {
        m_AvatarMgr.Reset();
        m_CurrentConnection = ConnectMgr::CONNECTION_RANDOM_MATCH;
        m_IsConnectRandomMatch = true;
        CopyNpcInfo(reinterpret_cast<u8*>(GetGardenPlus()) + 0x292A4, &m_NpcInfo, 0);
        return true;
    }
    return false;
}

// 0x00619408 (name is ours)
bool netgame::NetGameMgr::StopNet()
{
    if (!DisconnectCurrent()) {
        return false;
    }
    CloseCurrentMenu();
    u8 saveNo = m_SaveNo;
    InitEntryOrExit(false);
    if (saveNo < 8) {
        SetPlayerActive(0, saveNo, NULL);
    }
    for (s32 i = 1; i < PLAYER_NO_MAX; i++) {
        ClearPlayer(i);
    }
    FiniNet();
    return true;
}

// 0x006194A8 (name is ours)
bool netgame::NetGameMgr::RequestLock4()
{
    bool isRequested = m_LockMgr.Request(LockMgr::CMD_TOUR_END);
    if (!netgame::IsMultiPlayer()) {
        m_LockMgr.m_HandleCmdInSinglePlayer = true;
    }
    return isRequested;
}

// 0x006194F4 (name is ours)
bool netgame::NetGameMgr::JoinSession(u32 sessionId)
{
    if (!net::Net::s_Instance.JoinSession(sessionId)) {
        return false;
    }
    if (m_SaveNo >= 8) {
        m_SaveNo = GetDefaultSaveNo(4);
    }
    m_IsInNetSession = true;
    m_PacketQueue.m_Tick = g_FrameCounter;
    return true;
}

// 0x0061954C (name is ours)
bool netgame::NetGameMgr::RequestLock3()
{
    return m_LockMgr.Request(LockMgr::CMD_SAVE_INTERRUPT);
}

// 0x0061957C (name is ours)
bool netgame::NetGameMgr::JoinSessionByOwner(const u32* principalId)
{
    if (!net::Net::s_Instance.JoinSessionByOwner(principalId)) {
        return false;
    }
    OnSessionStarted(1, true);
    return true;
}

// 0x006195B0 (name is ours)
bool netgame::NetGameMgr::RequestLock7()
{
    return m_LockMgr.Request(LockMgr::CMD_RANDOM_MATCH_START_TOUR);
}

// 0x006195E0 (name is ours)
bool netgame::NetGameMgr::RequestLock5()
{
    return m_LockMgr.Request(LockMgr::CMD_RANDOM_MATCH_ENTRY);
}

// 0x00619610 (name is ours)
bool netgame::NetGameMgr::CreateRandomSession(u32 param, bool isRestart)
{
    if (!net::Net::s_Instance.CreateRandomSession(param)) {
        return false;
    }
    OnSessionStarted(0, isRestart);
    u32 principalId = FriendList::s_pInstance->GetMyPrincipalId();
    m_IslandMgr.AddPrincipal(PLAYER_NO_HOST, &principalId, false);
    return true;
}

// 0x00619674 (name is ours)
bool netgame::NetGameMgr::RequestLock6()
{
    bool isRequested = m_LockMgr.Request(LockMgr::CMD_RANDOM_MATCH_LEAVE);
    if (isRequested && netgame::IsHost()) {
        unk_0x132B5 = 1;
    }
    return isRequested;
}

// 0x006196C4 (name is ours)
bool netgame::NetGameMgr::IsJoinStarted() const
{
    return m_JoinMgr.m_State > 2;
}

// 0x006196D8 | libgarden
void netgame::NetGameMgr::InitEntryOrExit(bool isExit)
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        m_SendCounters[i] = 0;
        m_ReceiveCounters[i] = 0;
    }
    m_Flags = 0;
    m_ErrorFlags = 0;
    unk_0x132AF = false;
    m_StageMgr.InitState();
    m_PayloadA.Reset();
    m_PayloadC.Reset();
    m_PayloadE.Reset();
    m_PayloadB.Reset();
    m_PayloadD.Reset();
    m_EntryData.Reset();
    m_Mgr8.Reset();
    m_LeaveMgr.Reset();
    m_SaveDemoMgr.Reset();
    m_JoinMgr.Reset();
    m_LockMgr.Reset();
    m_PacketQueue.Reset();
    m_Receiver.Reset();
    m_SendMgr.ResetAll();
    m_TransitionMgr.Reset();
    for (s32 i = 0; i < 7; i++) {
        m_Bits[i].Reset();
    }
    m_Entry.Reset();
    m_ExistingPlayers.Reset();
    m_AvatarMgr.Reset();
    m_IsSessionClosedForIsland = false;
    m_MyPlayerNo = PLAYER_NO_INVALID;
    m_IsPlayerAssigned = false;
    unk_0x132B2 = 0;
    m_NewPlayerJoined = false;
    unk_0x132B4 = false;
    unk_0x132B5 = 0;
    unk_0x132B6 = false;
    unk_0x132B7 = false;
    if (isExit) {
        return;
    }
    m_IsInNetSession = false;
    m_SaveNo = 8;
    m_IsInNetgame = false;
    m_IslandMgr.Reset();
    m_CurrentConnection = ConnectMgr::CONNECTION_NONE;
    m_Type = TYPE_NONE;
    m_LastConnection = ConnectMgr::CONNECTION_NONE;
    std::memset(static_cast<void*>(&m_NpcInfo), 0, sizeof(m_NpcInfo));
}

// 0x006198AC (name is ours)
void netgame::NetGameMgr::AllocBuffers(sead::Heap* heap)
{
    m_NetRefCount = 0;
    m_PacketQueue.AllocBuffers(heap);
    m_Receiver.AllocBuffer(heap);
    m_SendMgr.AllocBuffers(heap);
    m_TransitionMgr.AllocBuffer(heap);
    m_EntryData.AllocBuffers(heap);
    m_PayloadD.AllocBuffer(heap);
    m_PayloadB.AllocBuffer(heap);
    m_PayloadE.AllocBuffer(heap);
    m_PayloadC.AllocBuffer(heap);
    m_PayloadA.AllocBuffer(heap);
    m_IslandMgr.AllocBuffers(heap);
    m_BestFriendMgr.AllocBuffers(heap);
    m_ConnectMgr.Clear();
    m_GateMgr.Reset();
    m_MatchingState.Reset();
    InitEntryOrExit(false);
    m_Type = TYPE_NONE;
}

// 0x006199A4 (name is ours)
bool netgame::NetGameMgr::CanSend(PlayerNo player, bool hasData, u32 unknown)
{
    if (unknown != 0 && !IsEntering(player)) {
        return false;
    }
    if (!m_AvatarMgr.CanSend(player)) {
        return false;
    }
    if (hasData && m_SendMgr.IsBusy(player)) {
        return false;
    }
    return true;
}

// 0x00619A88 (name is ours)
void netgame::NetGameMgr::ResetSession()
{
    CloseCurrentMenu();
    u8 saveNo = m_SaveNo;
    InitEntryOrExit(false);
    if (saveNo < 8) {
        SetPlayerActive(0, saveNo, NULL);
    }
    for (s32 i = 1; i < PLAYER_NO_MAX; i++) {
        ClearPlayer(i);
    }
    FiniNet();
}

// 0x00619B1C (name is ours)
netgame::NetGameMgr::NetGameMgr()
    : m_CurrentConnection(ConnectMgr::CONNECTION_NONE), m_LastConnection(ConnectMgr::CONNECTION_NONE),
      m_MyPlayerNo(PLAYER_NO_INVALID), unk_0x13269(PLAYER_NO_INVALID), m_Flags(0), m_ErrorFlags(0), m_SaveNo(8),
      m_Type(TYPE_NONE), m_NetRefCount(0), m_IsInNetSession(false), m_IsInNetgame(false), m_IsPlayerAssigned(false),
      unk_0x132AF(false), m_IsConnectRandomMatch(false), m_IsSessionClosedForIsland(false), unk_0x132B2(0),
      m_NewPlayerJoined(false), unk_0x132B4(false), unk_0x132B5(0), unk_0x132B6(false), unk_0x132B7(false)
{
}

// 0x00619D70 (name is ours)
netgame::NetGameMgr::~NetGameMgr()
{
    // (only the members are destroyed)
}

// 0x00626794 (name is ours)
void netgame::NetGameMgr::BuildPayloadA()
{
    m_PayloadA.Build(m_StageMgr.m_Stages.GetStage(m_MyPlayerNo));
}

// 0x00626A80 (name is ours)
void netgame::NetGameMgr::BuildPayloadD(u32 param)
{
    m_PayloadD.Build(param, m_StageMgr.m_Stages.GetStage(m_MyPlayerNo));
}

// 0x00626FD4 (name is ours)
void netgame::NetGameMgr::BuildPayloadE(u32 param)
{
    m_PayloadE.Build(param, m_StageMgr.m_Stages.GetStage(m_MyPlayerNo));
}

// 0x0075EFF8 (name after libgarden)
u8 netgame::NetGameMgr::GetPlayerCount() const
{
    return m_Bits[1].GetCount();
}

// 0x0075F004 (name after libgarden)
netgame::PlayerNo netgame::NetGameMgr::GetMyPlayerNo() const
{
    if (m_IsInNetSession && m_IsPlayerAssigned) {
        return m_MyPlayerNo;
    }
    return PLAYER_NO_HOST;
}

// 0x0075F02C (name after libgarden)
bool netgame::NetGameMgr::IsAdmitted(PlayerNo player) const
{
    if (m_PacketQueue.m_LeavingPlayer == player) {
        return false;
    }
    return m_Bits[1].IsSet(player);
}

// 0x0075F064 (name after libgarden)
bool netgame::NetGameMgr::IsMultiPlayer() const
{
    return m_IsInNetgame;
}

// 0x00760658 (name after libgarden; falls into AvatarMgr::IsInSession in the original)
bool netgame::NetGameMgr::IsInSession(PlayerNo player, bool skipSelf) const
{
    return m_AvatarMgr.IsInSession(player, skipSelf);
}
} // namespace netgame
