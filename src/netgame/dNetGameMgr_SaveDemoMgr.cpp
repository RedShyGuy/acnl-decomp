#include "netgame/dGameFunctions.h"
#include "netgame/dNetGameMgr.h"

#include "Bs/dBsSvMgr.h"
#include "stage/dTransitionMgr.h"

namespace netgame {

// the callbacks of the game for each player (the calls are gone in the original: armlink replaced them by
// their single instruction; names are ours)
DECOMP_ALWAYS_INLINE bool CanLeave(PlayerNo /* player */, const MachineBitTable* /* others */)
{
    return true;
}
DECOMP_ALWAYS_INLINE void OnEvent(PlayerNo /* player */, const MachineBitTable* /* others */)
{
}

namespace {
const u8 PACKET_ENTRY_FLAG = 23;
const u8 PACKET_SAVE_BLOCK = 24;
const u8 PACKET_SAVE_READY = 22;

// 0x0087CDA0 (name is ours)
extern const u32 s_SaveBlockSize2 = sizeof(SaveBlock);

enum State : u8
{
    STATE_NONE,
    STATE_SEND_BLOCKS,
    STATE_WAIT_GUESTS,
    STATE_WAIT_SAVE_DATA,
    STATE_SEND_FLAG,
    STATE_WAIT_PACKETS,
    STATE_WAIT_SENT,
    STATE_WAIT_HOST,
    STATE_SEND_READY,
    STATE_WAIT_DISCONNECT,
    STATE_WAIT_CONFIRM,
    STATE_WAIT_OTHERS,
    STATE_DISCONNECT,
    STATE_FINISH,
};
} // namespace

// 0x00624B28 (name is ours)
void netgame::NetGameMgr::SaveDemoMgr::Process()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    switch (m_State) {
    case STATE_SEND_BLOCKS: {
        const size_t partSizes[] = {s_SaveBlockSize2};
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            PlayerNo no = static_cast<PlayerNo>(i);
            if (!m_Targets.IsSet(no)) {
                continue;
            }
            const void* block = mgr->GetSaveBlock(no);
            if (block != NULL) {
                const void* parts[] = {block};
                if (mgr->SendPacketBatch(no, parts, partSizes, 1, PACKET_SAVE_BLOCK, 0, 0, 0)) {
                    m_Targets.Remove(no);
                }
            }
        }
        if (m_Targets.GetCount() == 0) {
            m_State = STATE_WAIT_GUESTS;
        }
        break;
    }
    case STATE_WAIT_GUESTS:
        if (mgr->m_StageMgr.GetTransitioningPlayer() >= PLAYER_NO_MAX && m_Confirm.IsComplete()) {
            mgr->m_PacketQueue.m_IsClosed = true;
            MachineBitTable others;
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if (mgr->IsAdmitted(no)) {
                    OnPlayerLeaveFinished(no, &others);
                }
            }
            m_Confirm.Update(PLAYER_NO_HOST, false);
            m_State = STATE_WAIT_CONFIRM;
        }
        break;
    case STATE_WAIT_SAVE_DATA:
        if (mgr->unk_0x132B2) {
            m_State = STATE_SEND_FLAG;
        }
        break;
    case STATE_SEND_FLAG:
        if (mgr->m_StageMgr.AreAllPlayersIn(stage::NAME_SAVE) && mgr->m_StageMgr.GetTransitioningPlayer() >= PLAYER_NO_MAX) {
            PlayerNo myNo = mgr->m_MyPlayerNo;
            if (myNo < PLAYER_NO_MAX && myNo != PLAYER_NO_HOST) {
                EntryData& entry = mgr->m_EntryData;
                entry.BuildFlag(myNo);
                u8* flag = entry.GetFlag(myNo);
                if (flag != NULL) {
                    PacketQueue& queue = mgr->m_PacketQueue;
                    queue.BeginPacket();
                    queue.AddPacketData(flag, EntryData::s_PlayerFlagSize);
                    queue.EndPacket(PACKET_ENTRY_FLAG, PLAYER_NO_HOST);
                }
                entry.Reset();
                m_State = STATE_WAIT_PACKETS;
            }
        }
        break;
    case STATE_WAIT_HOST:
        if (m_IsDone) {
            mgr->m_Bits[2].Reset();
            m_State = STATE_SEND_READY;
        }
        break;
    case STATE_SEND_READY:
        if (mgr->SendPacketBatch(PLAYER_NO_HOST, NULL, NULL, 0, PACKET_SAVE_READY, 0, 0, 0)) {
            m_State = STATE_WAIT_SENT;
        } else if (!netgame::IsInSession(PLAYER_NO_HOST, true)) {
            m_State = STATE_WAIT_DISCONNECT;
        }
        break;
    case STATE_WAIT_DISCONNECT:
        if (!mgr->IsInSession(PLAYER_NO_HOST, false)) {
            MachineBitTable others;
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if (mgr->IsAdmitted(no)) {
                    OnSessionEnd(no, &others);
                }
            }
            m_State = STATE_DISCONNECT;
        }
        break;
    case STATE_WAIT_CONFIRM:
        if (m_Confirm.IsComplete()) {
            m_IsDone = true;
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if (no != PLAYER_NO_HOST && ExistsPlayer(no)) {
                    mgr->SendPacketBatch(no, NULL, NULL, 0, PACKET_ENTRY_FLAG, 0, 0, 0);
                }
            }
            m_Confirm.Update(PLAYER_NO_HOST, true);
            mgr->m_Bits[2].Reset();
            m_State = STATE_WAIT_OTHERS;
        }
        break;
    case STATE_WAIT_OTHERS:
        if (m_Confirm.IsComplete()) {
            m_State = STATE_DISCONNECT;
        } else {
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if (!m_Confirm.IsSet(no) && !netgame::IsInSession(no, true)) {
                    m_Confirm.Add(no);
                }
            }
        }
        break;
    case STATE_DISCONNECT:
        if (m_CanDisconnect && mgr->DisconnectCurrent()) {
            m_State = STATE_FINISH;
        }
        break;
    case STATE_FINISH:
        CloseSession();
        LoadNpcInfo(&mgr->m_NpcInfo, PLAYER_NO_HOST);
        mgr->ResetSession();
        break;
    default:
        break;
    }
}

// 0x006250C4 (name is ours)
void netgame::NetGameMgr::SaveDemoMgr::RemovePlayer(PlayerNo player)
{
    if (player < PLAYER_NO_MAX) {
        m_Confirm.Remove(player);
    }
}

// 0x006250D8 (name is ours)
void netgame::NetGameMgr::SaveDemoMgr::AdvanceAfterPackets()
{
    if (m_State == STATE_WAIT_PACKETS) {
        m_State = STATE_WAIT_HOST;
    } else if (m_State == STATE_WAIT_SENT) {
        NetGameMgr::s_pInstance->m_PacketQueue.m_IsClosed = true;
        m_State = STATE_WAIT_DISCONNECT;
    }
}

// 0x00625114 (name is ours)
void netgame::NetGameMgr::SaveDemoMgr::NotifyLeaveFinished()
{
    MachineBitTable others;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (NetGameMgr::s_pInstance->IsAdmitted(no)) {
            OnPlayerLeaveFinished(no, &others);
        }
    }
}

// 0x00625164 (name is ours)
bool netgame::NetGameMgr::SaveDemoMgr::CheckLeave()
{
    MachineBitTable others;
    bool result = true;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (NetGameMgr::s_pInstance->IsAdmitted(no) && !CanLeave(no, &others)) {
            result = false;
        }
    }
    return result;
}

// 0x006251C4 (name is ours)
void netgame::NetGameMgr::SaveDemoMgr::NotifySessionEnd()
{
    MachineBitTable others;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (NetGameMgr::s_pInstance->IsAdmitted(no)) {
            OnSessionEnd(no, &others);
        }
    }
}

// 0x00625214 (name is ours)
void netgame::NetGameMgr::SaveDemoMgr::NotifyEvent()
{
    MachineBitTable others;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (NetGameMgr::s_pInstance->IsAdmitted(no)) {
            OnEvent(no, &others);
        }
    }
}

// 0x00625264 (name is ours)
void netgame::NetGameMgr::SaveDemoMgr::Reset()
{
    m_State = STATE_NONE;
    m_Confirm.MachineBitTable::Reset();
    m_CanDisconnect = false;
    m_IsDone = false;
}

// 0x00625284 (name is ours)
void netgame::NetGameMgr::StartSave(bool endSession)
{
    if (!endSession) {
        stage::GetTransitionMgr()->SetReturnParam();
    }
    stage::Transition(stage::NAME_SAVE, 1, 1, 2, 2);
    if (endSession) {
        BsSvMgr::SetSaveInterruptNetHst();
    } else {
        BsSvMgr::SetSaveContinueNetHst();
    }
    m_SaveDemoMgr.OnStartSave(endSession);
}

// 0x006252EC (name after libgarden)
void netgame::NetGameMgr::SaveDemoMgr::OnStartSave(bool endSession)
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    PlayerNo myNo = mgr->GetMyPlayerNo();
    m_IsDone = false;
    if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST) {
        m_Confirm.Update(myNo, false);
        if (endSession) {
            m_CanDisconnect = false;
            m_State = STATE_WAIT_SAVE_DATA;
        } else {
            m_State = STATE_NONE;
        }
        return;
    }
    if (!endSession) {
        m_Confirm.Update(myNo, false);
        m_State = STATE_NONE;
        return;
    }
    m_Confirm.Update(myNo, true);
    mgr->m_EntryData.SendToGuests(reinterpret_cast<const u16*>(&mgr->m_Bits[1]));
    BuildSaveBlocks(mgr);
    m_Targets.Reset();
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no != PLAYER_NO_HOST && ExistsPlayer(no)) {
            m_Targets.Add(no);
        }
    }
    m_CanDisconnect = false;
    m_State = STATE_SEND_BLOCKS;
}

// 0x006253F8 (name is ours)
netgame::NetGameMgr::SaveDemoMgr::SaveDemoMgr()
{
    m_State = STATE_NONE;
    m_CanDisconnect = false;
    m_IsDone = false;
}

// 0x00624B10 (name is ours)
bool IsInSession(PlayerNo player, bool skipSelf)
{
    return NetGameMgr::s_pInstance->IsInSession(player, skipSelf);
}

// 0x0075EEF8 (name is ours)
netgame::SaveBlock* netgame::NetGameMgr::GetSaveBlock(PlayerNo player)
{
    if (player >= PLAYER_NO_MAX) {
        return NULL;
    }
    return &m_SaveBlocks[player];
}
} // namespace netgame
