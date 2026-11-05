#include "netgame/dLeaveMgr.h"

#include "netgame/dGameFunctions.h"
#include "netgame/dNetGameMgr.h"
#include "stage/stage_Api.h"

namespace netgame {

namespace {
const u8 PACKET_SAVE_BLOCK = 24;
const u8 PACKET_ENTRY_FLAG = 23;
const u8 PACKET_LEAVE_READY = 25;
const u8 PACKET_LEAVE_DONE = 26;

// 0x0087CD9C (name is ours)
extern const u32 s_SaveBlockSize = sizeof(SaveBlock);

enum State : u8
{
    STATE_NONE,
    STATE_WAIT_SAVE,
    STATE_SEND_FLAG,
    STATE_WAIT_CLOSED,
    STATE_WAIT_HOST,
    STATE_DISCONNECT,
    STATE_DISCONNECTED,
    STATE_SEND_SAVE_BLOCK,
    STATE_WAIT_GUEST_SAVE,
    STATE_SEND_READY,
    STATE_WAIT_LEFT,
    STATE_LEFT,
    STATE_SEND_DONE,
    STATE_WAIT_OUT,
    STATE_DONE,
};

DECOMP_ALWAYS_INLINE MachineBitTable GetOtherPlayers(PlayerNo player)
{
    MachineBitTable table = NetGameMgr::s_pInstance->m_Bits[1];
    table.Remove(player);
    return table;
}

DECOMP_ALWAYS_INLINE void SendEntryFlag(NetGameMgr* mgr, PlayerNo myNo)
{
    NetGameMgr::EntryData& entry = mgr->m_EntryData;
    entry.BuildFlag(myNo);
    u8* flag = entry.GetFlag(myNo);
    if (flag != NULL) {
        PacketQueue& queue = mgr->m_PacketQueue;
        queue.BeginPacket();
        queue.AddPacketData(flag, NetGameMgr::EntryData::s_PlayerFlagSize);
        queue.EndPacket(PACKET_ENTRY_FLAG, PLAYER_NO_HOST);
    }
    entry.Reset();
}

DECOMP_ALWAYS_INLINE bool AreOthersInSave(NetGameMgr* mgr)
{
    return mgr->GetPlayerCount() - 1 == mgr->m_StageMgr.m_Stages.GetPlayerCount(stage::NAME_SAVE) &&
           mgr->m_StageMgr.GetTransitioningPlayer() >= PLAYER_NO_MAX;
}
} // namespace

// 0x00623C6C (name is ours)
void netgame::LeaveMgr::Process()
{
    if (m_State == STATE_NONE) {
        return;
    }
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    PacketQueue& queue = mgr->m_PacketQueue;
    PlayerNo player = m_LeavingPlayer;
    PlayerNo myNo = mgr->m_MyPlayerNo;
    switch (m_State) {
    case STATE_WAIT_SAVE:
        if (AreOthersInSave(mgr)) {
            SendEntryFlag(mgr, myNo);
            queue.BeginPacket();
            queue.EndPacket(PACKET_SAVE_BLOCK, PLAYER_NO_INVALID);
            m_State = STATE_SEND_FLAG;
        }
        break;
    case STATE_SEND_FLAG:
        if (mgr->m_AvatarMgr.CanSend(myNo) && m_Confirm.IsComplete()) {
            queue.m_LeavingPlayer = myNo;
            queue.m_IsClosed = true;
            m_State = STATE_WAIT_CLOSED;
        }
        break;
    case STATE_WAIT_CLOSED:
        if (stage::GetCurrent() == stage::NAME_SAVE) {
            m_State = STATE_WAIT_HOST;
        }
        break;
    case STATE_WAIT_HOST:
        if (unk_0x253C && m_HostConfirm.IsComplete()) {
            mgr->m_Bits[2].Reset();
            m_State = STATE_DISCONNECT;
        }
        break;
    case STATE_DISCONNECT:
        if (mgr->DisconnectCurrent()) {
            GetOtherPlayers(m_LeavingPlayer);
            m_State = STATE_DISCONNECTED;
        }
        break;
    case STATE_SEND_SAVE_BLOCK: {
        const void* parts[] = {&m_SaveBlock};
        const size_t partSizes[] = {s_SaveBlockSize};
        if (mgr->SendPacketBatch(player, parts, partSizes, 1, PACKET_SAVE_BLOCK, 0, 0, 0)) {
            ClearSaveBlock(&m_SaveBlock);
            m_SaveBlock.m_IsValid = false;
            m_State = STATE_WAIT_GUEST_SAVE;
        }
        break;
    }
    case STATE_WAIT_GUEST_SAVE:
        if (AreOthersInSave(mgr)) {
            m_State = STATE_SEND_READY;
        }
        break;
    case STATE_SEND_READY:
        if (mgr->m_StageMgr.GetTransitioningPlayer() >= PLAYER_NO_MAX && m_Confirm.IsSet(myNo)) {
            queue.BeginPacket();
            queue.EndPacket(PACKET_LEAVE_READY, player);
            m_State = STATE_WAIT_LEFT;
        }
        break;
    case STATE_LEFT:
        GetOtherPlayers(player);
        m_State = STATE_SEND_DONE;
        break;
    case STATE_SEND_DONE:
        if (unk_0x253F) {
            mgr->m_Bits[2].Remove(player);
            if (mgr->SendPacketBatch(player, NULL, NULL, 0, PACKET_LEAVE_DONE, 0, 0, 0)) {
                unk_0x253F = false;
                m_State = STATE_WAIT_OUT;
            }
        }
        break;
    case STATE_WAIT_OUT:
        if (!mgr->IsInSession(player, true)) {
            m_State = STATE_DONE;
        }
        break;
    default:
        break;
    }
}

// 0x006240A4 (name is ours)
bool netgame::LeaveMgr::ProcessLeave()
{
    if (m_State == STATE_DISCONNECTED) {
        GetOtherPlayers(m_LeavingPlayer);
        GetOtherPlayers(m_LeavingPlayer);
    }
    return true;
}

// 0x0062413C (name is ours)
void netgame::LeaveMgr::ProcessEnd()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->unk_0x132B4 || IsSaving()) {
        return;
    }
    if (m_State == STATE_DISCONNECTED) {
        MachineBitTable others = GetOtherPlayers(m_LeavingPlayer);
        OnSessionEnd(m_LeavingPlayer, &others);
        mgr->ResetSession();
        return;
    }
    if (m_State != STATE_DONE) {
        return;
    }
    MachineBitTable others = GetOtherPlayers(m_LeavingPlayer);
    OnPlayerLeaveFinished(m_LeavingPlayer, &others);
    PlayerNo player = m_LeavingPlayer;
    if (player < PLAYER_NO_MAX) {
        mgr->RemovePlayer(player);
        mgr->m_LockMgr.m_SourcePlayer = PLAYER_NO_INVALID;
        mgr->m_PacketQueue.m_LeavingPlayer = PLAYER_NO_INVALID;
        mgr->m_PacketQueue.m_IsClosed = false;
        netgame::OnPlayerLeft(player);
        Reset();
    }
}

// 0x0062427C (name is ours)
void netgame::LeaveMgr::RemovePlayer(PlayerNo player)
{
    if (player >= PLAYER_NO_MAX) {
        return;
    }
    m_Confirm.Remove(player);
    m_GuestConfirm.Remove(player);
    m_HostConfirm.Remove(player);
    unk_0x253E = false;
}

// 0x006242C4 (name is ours)
void netgame::LeaveMgr::OnPlayerLeft()
{
    if (m_State != STATE_WAIT_LEFT) {
        return;
    }
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    mgr->m_PacketQueue.m_LeavingPlayer = m_LeavingPlayer;
    mgr->m_StageMgr.RemovePlayer(m_LeavingPlayer);
    GetOtherPlayers(m_LeavingPlayer);
    m_State = STATE_LEFT;
}

// 0x00624330 (name is ours)
bool netgame::LeaveMgr::CheckLeaveA(PlayerNo player)
{
    GetOtherPlayers(player);
    return true;
}

// 0x0062436C (name is ours)
bool netgame::LeaveMgr::CheckLeaveB(PlayerNo player)
{
    GetOtherPlayers(player);
    return true;
}

// 0x006243A8 (name is ours)
void netgame::LeaveMgr::NotifyLeaveFinished(PlayerNo player)
{
    MachineBitTable others = GetOtherPlayers(player);
    OnPlayerLeaveFinished(player, &others);
}

// 0x006243E4 (name is ours)
bool netgame::LeaveMgr::CheckLeaveC(PlayerNo player)
{
    GetOtherPlayers(player);
    return true;
}

// 0x00624420 (name is ours)
void netgame::LeaveMgr::NotifySessionEnd(PlayerNo player)
{
    MachineBitTable others = GetOtherPlayers(player);
    OnSessionEnd(player, &others);
}

// 0x0062445C (name is ours)
void netgame::LeaveMgr::NotifyEventA(PlayerNo player)
{
    GetOtherPlayers(player);
}

// 0x00624498 (name is ours)
void netgame::LeaveMgr::NotifyEventB(PlayerNo player)
{
    GetOtherPlayers(player);
}

// 0x006244D4 (name is ours)
void netgame::LeaveMgr::Reset()
{
    m_LeavingPlayer = PLAYER_NO_INVALID;
    m_State = STATE_NONE;
    m_Confirm.MachineBitTable::Reset();
    m_GuestConfirm.MachineBitTable::Reset();
    m_HostConfirm.MachineBitTable::Reset();
    unk_0x253C = false;
    unk_0x253D = false;
    unk_0x253E = false;
    unk_0x253F = false;
}

// 0x00624538 (name is ours)
void netgame::NetGameMgr::StartLeave()
{
    m_LeaveMgr.StartLeave(false);
}

// 0x00624544 (name after libgarden)
void netgame::LeaveMgr::StartLeave(bool isLocked)
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    LockMgr& lock = mgr->m_LockMgr;
    if (!isLocked) {
        lock.m_SuccessfulCmd = GetNetgameType() == TYPE_RANDOM_MATCH ? LockMgr::CMD_RANDOM_MATCH_LEAVE : LockMgr::CMD_FRIEND_MATCH_LEAVE;
        lock.m_SourcePlayer = mgr->m_MyPlayerNo;
    }
    PlayerNo player = lock.m_SourcePlayer;
    PlayerNo myNo = mgr->GetMyPlayerNo();
    if (myNo == player) {
        m_LeavingPlayer = player;
        m_Confirm.Update(myNo, true);
        m_HostConfirm.Update(myNo, true);
        m_State = STATE_WAIT_SAVE;
        return;
    }
    if (mgr->GetMyPlayerNo() == PLAYER_NO_HOST) {
        MachineBitTable table;
        table.Add(player);
        mgr->m_EntryData.SendToGuests(reinterpret_cast<const u16*>(&table));
        mgr->unk_0x132B6 = true;
        mgr->unk_0x132B7 = false;
    }
    m_LeavingPlayer = player;
    m_Confirm.Update(myNo, false);
    m_GuestConfirm.Update(myNo, false);
    m_GuestConfirm.Add(player);
    ClearSaveBlock(&m_SaveBlock);
    m_SaveBlock.m_IsValid = false;
    if (IsHost()) {
        m_SaveBlock.Build(player);
        m_State = STATE_SEND_SAVE_BLOCK;
    } else {
        m_State = STATE_WAIT_GUEST_SAVE;
    }
}

// 0x00624700 (name is ours)
netgame::LeaveMgr::~LeaveMgr()
{
    // (only the members are destroyed)
}

// 0x0062468C (name is ours)
netgame::LeaveMgr::LeaveMgr()
{
    m_State = STATE_NONE;
    m_LeavingPlayer = PLAYER_NO_INVALID;
    unk_0x253C = false;
    unk_0x253D = false;
    unk_0x253E = false;
    unk_0x253F = false;
}
} // namespace netgame
