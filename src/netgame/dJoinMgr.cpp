#include "netgame/dJoinMgr.h"

#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "net/dNet.h"
#include "netgame/dGameGlobals.h"
#include "netgame/dGameFunctions.h"
#include "netgame/dNetGameMgr.h"
#include "stage/stage_Api.h"

namespace netgame {

// (names are ours)
struct StaticData
{
    u8 unk_0x0[0x10];
    u8 data[0x10];
};
extern StaticData* g_pStaticData;

// 0x00954650
u32 s_SaveHeaderSize = NetGameMgr::EntryData::s_BufferSize + PacketQueue::s_StateBufferSize + 58;

namespace {
const u8 PACKET_ENTRY_ACK = 5;
const u8 PACKET_TOWN_DATA = 6;
const u8 PACKET_CLOCK = 9;
const u8 PACKET_ENTRY_DONE = 10;
const u8 PACKET_PLAYER_DATA = 11;
const u8 PACKET_SAVE_DATA = 12;
const u8 PACKET_ENTRY_START = 13;
const u8 PACKET_RANDOM_MATCH_DATA = 14;
const u8 PACKET_STAGES = 15;
const u8 PACKET_ENTRY_READY = 16;
const u8 PACKET_ENTRY_FINISHED_GUEST = 31;
const u8 PACKET_ENTRY_FINISHED_HOST = 30;

const u32 PLAYER_DATA_SIZE = 0xA480;
const u32 NPC_INFO_SIZE = 50;
const u32 ISLAND_MAP_SIZE = 0x102A;
const u32 SAVE_DATA_SIZE = 0x71880;
// (fits into the send buffer)
const u32 SAVE_PART_SIZE = 0xCFFC;
const u32 SAVE_FIRST_PART_SIZE = 0xCFC8;

enum State : u8
{
    STATE_NONE,
    STATE_WAIT_PLAYERS,
    STATE_SEND_TOWN_DATA,
    STATE_WAIT_ALL_SAVE,
    STATE_WAIT_CLOCK,
    STATE_WAIT_ACK,
    STATE_SEND_SAVE_DATA,
    STATE_SEND_STAGES,
    STATE_WAIT_GUEST,
    STATE_SEND_PLAYER_DATA,
    STATE_SEND_RANDOM_MATCH_DATA,
    STATE_DONE,
};

DECOMP_ALWAYS_INLINE u16 MakeStageEntry(const StageMgr::PlayerStages& stages, PlayerNo no)
{
    return (stages.HasControl(no) ? 0x100 : 0) | stages.GetStage(no);
}
} // namespace

// 0x006208E4 (name is ours)
void netgame::JoinMgr::Process()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->m_PacketQueue.m_IsClosed) {
        return;
    }
    PlayerNo myNo = mgr->m_MyPlayerNo;
    bool isBusy = false;
    if (unk_0xD) {
        if (mgr->SendPacketBatch(PLAYER_NO_HOST, NULL, NULL, 0, PACKET_ENTRY_ACK, 0, 1, 0)) {
            unk_0xD = false;
        } else {
            isBusy = true;
        }
    } else if (unk_0x12) {
        if (mgr->SendPacketBatch(PLAYER_NO_INVALID, NULL, NULL, 0, PACKET_ENTRY_READY, 0, 1, 0)) {
            unk_0x12 = false;
        } else {
            isBusy = true;
        }
    }
    switch (m_State) {
    case STATE_WAIT_PLAYERS:
        if (unk_0x10) {
            bool isReady = true;
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if (no != myNo && mgr->m_Bits[0].IsSet(no) && !mgr->IsEntering(no)) {
                    isReady = false;
                    break;
                }
            }
            if (isReady) {
                m_State = STATE_SEND_TOWN_DATA;
            }
        }
        break;
    case STATE_SEND_TOWN_DATA:
        if (!isBusy && SendTownData()) {
            m_State = STATE_DONE;
        }
        break;
    case STATE_WAIT_ALL_SAVE:
        if (!isBusy && (mgr->m_StageMgr.AreAllPlayersIn(stage::NAME_SAVE) ||
                        mgr->m_StageMgr.AreAllPlayersIn(stage::NAME_RANDOM_MATCHING_ENTRY))) {
            PlayerNo source = mgr->m_LockMgr.m_SourcePlayer;
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if (no == PLAYER_NO_HOST) {
                    unk_0x15 = true;
                } else if (no != source && mgr->m_Bits[0].IsSet(no) && no < PLAYER_NO_MAX) {
                    mgr->SendPacketBatch(no, NULL, NULL, 0, PACKET_ENTRY_START, 0, 1, 0);
                }
            }
            if (source < PLAYER_NO_MAX) {
                UpdateClock();
                u64 clock = GetClock();
                s64 time = net::Net::s_Instance.GetSessionTime();
                const void* parts[] = {&clock, &time};
                const size_t partSizes[] = {sizeof(clock), sizeof(time)};
                if (mgr->SendPacketBatch(source, parts, partSizes, 2, PACKET_CLOCK, 0, 1, 0)) {
                    m_State = STATE_WAIT_CLOCK;
                }
            }
        }
        break;
    case STATE_WAIT_CLOCK:
        if (unk_0x11) {
            m_State = STATE_WAIT_ACK;
        }
        break;
    case STATE_WAIT_ACK:
        if (unk_0xE) {
            m_State = mgr->m_IsConnectRandomMatch ? STATE_SEND_RANDOM_MATCH_DATA : STATE_SEND_SAVE_DATA;
        }
        break;
    case STATE_SEND_SAVE_DATA:
        if (!isBusy && unk_0x16 && !IsSaveIoOnMainData() && mgr->m_StageMgr.AreAllPlayersIn(stage::NAME_SAVE) &&
            mgr->m_StageMgr.GetTransitioningPlayer() >= PLAYER_NO_MAX && SendSaveData(mgr->m_LockMgr.m_SourcePlayer)) {
            SetNetGameType(TYPE_FRIEND_MATCH);
            unk_0x16 = false;
            m_State = STATE_DONE;
        }
        break;
    case STATE_SEND_STAGES:
        if (!isBusy && mgr->m_StageMgr.m_Stages.GetPlayerCount(stage::NAME_SAVE) == 0 &&
            mgr->m_StageMgr.GetTransitioningPlayer() >= PLAYER_NO_MAX) {
            PlayerNo source = mgr->m_LockMgr.m_SourcePlayer;
            if (source < PLAYER_NO_MAX) {
                if (mgr->CanSend(source, false, 1)) {
                    u32 count = 0;
                    u16 entries[PLAYER_NO_MAX];
                    const void* parts[3];
                    size_t partSizes[3];
                    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                        PlayerNo no = static_cast<PlayerNo>(i);
                        if (no == source) {
                            continue;
                        }
                        entries[no] = MakeStageEntry(mgr->m_StageMgr.m_Stages, no);
                        parts[count] = &entries[no];
                        partSizes[count] = sizeof(u16);
                        count++;
                    }
                    if (mgr->SendPacketBatch(source, parts, partSizes, count, PACKET_STAGES, 0, 1, 0)) {
                        m_State = STATE_DONE;
                    }
                } else {
                    mgr->m_PacketQueue.IncrementCounter(source);
                }
            }
        }
        break;
    case STATE_WAIT_GUEST:
        if (unk_0x11) {
            m_State = STATE_SEND_PLAYER_DATA;
        }
        break;
    case STATE_SEND_PLAYER_DATA:
        if (unk_0x16 && !isBusy) {
            PlayerNo source = mgr->m_LockMgr.m_SourcePlayer;
            if (source < PLAYER_NO_MAX) {
                if (mgr->CanSend(source, false, 1)) {
                    u32 count = 0;
                    const void* parts[2];
                    size_t partSizes[2];
                    SvPlayer* player = SvGetCurrentPlayer();
                    if (player != NULL) {
                        player->PrepareSend();
                        count = 1;
                        parts[0] = player;
                        partSizes[0] = PLAYER_DATA_SIZE;
                    }
                    parts[count] = &mgr->m_NpcInfo;
                    partSizes[count] = NPC_INFO_SIZE;
                    if (mgr->SendPacketBatch(source, parts, partSizes, count + 1, PACKET_PLAYER_DATA, 0, 1, 0)) {
                        if (player != NULL) {
                            player->FinishSend();
                        }
                        unk_0x16 = false;
                        m_State = STATE_DONE;
                    }
                } else {
                    mgr->m_PacketQueue.IncrementCounter(source);
                }
            }
        }
        break;
    case STATE_SEND_RANDOM_MATCH_DATA:
        if (!isBusy && mgr->m_StageMgr.AreAllPlayersIn(stage::NAME_RANDOM_MATCHING_ENTRY) &&
            mgr->m_StageMgr.GetTransitioningPlayer() >= PLAYER_NO_MAX && unk_0x16 &&
            SendRandomMatchData(mgr->m_LockMgr.m_SourcePlayer)) {
            unk_0x16 = false;
            m_State = STATE_DONE;
        }
        break;
    default:
        break;
    }
    if (!unk_0xF) {
        return;
    }
    PlayerNo source = mgr->m_LockMgr.m_SourcePlayer;
    if (myNo == source) {
        if (mgr->SendPacketBatch(PLAYER_NO_HOST, NULL, NULL, 0, PACKET_ENTRY_DONE, 0, 1, 0)) {
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if (mgr->m_Bits[0].IsSet(no)) {
                    mgr->m_Bits[1].Add(no);
                } else {
                    mgr->m_Bits[1].Remove(no);
                }
            }
            mgr->m_IsInNetgame = true;
            Reset();
        }
        return;
    }
    stage::Name stage = m_Stage;
    mgr->m_Bits[1].Add(source);
    mgr->m_StageMgr.AddPlayer(source, stage);
    Reset();
    PacketQueue& queue = mgr->m_PacketQueue;
    queue.BeginPacket();
    if (myNo != PLAYER_NO_HOST) {
        queue.EndPacket(PACKET_ENTRY_FINISHED_GUEST, PLAYER_NO_HOST);
    } else {
        queue.EndPacket(PACKET_ENTRY_FINISHED_HOST, PLAYER_NO_INVALID);
    }
}

// 0x00621138 (name is ours)
void netgame::JoinMgr::RemovePlayer(PlayerNo player)
{
    if (player < PLAYER_NO_MAX) {
        m_PlayerFlags[player] = false;
    }
}

// 0x00621150 (name is ours)
void netgame::JoinMgr::SetPlayerFlag(PlayerNo player, bool isSet)
{
    if (player < PLAYER_NO_MAX) {
        m_PlayerFlags[player] = isSet;
    }
}

// 0x00621160 (name is ours)
bool netgame::JoinMgr::SendTownData()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    PlayerNo myNo = mgr->m_MyPlayerNo;
    MachineBitTable targets;
    if (!mgr->CollectPlayers(&targets, false, 1)) {
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            PlayerNo no = static_cast<PlayerNo>(i);
            if (targets.IsSet(no)) {
                mgr->m_PacketQueue.IncrementCounter(no);
            }
        }
        return false;
    }
    const void* parts[5] = {};
    size_t partSizes[5] = {};
    u32 count = 0;
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        player->PrepareSend();
        count = 1;
        partSizes[0] = PLAYER_DATA_SIZE;
        parts[0] = player;
    }
    u8 hasUnknown = g_pUnknown94EB2C != NULL;
    parts[count] = &hasUnknown;
    partSizes[count] = sizeof(hasUnknown);
    parts[count + 1] = &mgr->m_SaveBlock;
    partSizes[count + 1] = sizeof(SaveBlock);
    parts[count + 2] = &mgr->m_NpcInfo;
    partSizes[count + 2] = NPC_INFO_SIZE;
    count += 3;
    u8 current = stage::GetCurrent();
    if (count < 5) {
        parts[count] = &current;
        partSizes[count] = sizeof(current);
        count++;
    }
    if (!mgr->SendPacketBatch(PLAYER_NO_INVALID, parts, partSizes, count, PACKET_TOWN_DATA, 0, 1, 0)) {
        return false;
    }
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no != PLAYER_NO_HOST && no != myNo && mgr->m_Bits[0].IsSet(no)) {
            mgr->m_Bits[2].Add(no);
        }
    }
    if (player != NULL) {
        player->FinishSend();
    }
    return true;
}

// 0x00621364 (name is ours)
bool netgame::JoinMgr::SendSaveData(PlayerNo target)
{
    if (target >= PLAYER_NO_MAX) {
        return false;
    }
    if (m_SendOffset >= s_SaveHeaderSize + SAVE_DATA_SIZE) {
        return true;
    }
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->CanSend(target, false, 1)) {
        const void* parts[10];
        size_t partSizes[10];
        parts[0] = &m_SendOffset;
        partSizes[0] = sizeof(m_SendOffset);
        u32 count = 1;
        u32 sent = 0;
        u32 remaining = SAVE_PART_SIZE;
        u8 saveNo = GetDefaultSaveNo(4);
        const u8 townByte = GetGardenPlus()->data.townData.unk_0x1DAC7;
        u8 flag = (townByte & 1) != 0 || (townByte & 4) != 0;
        u16 entries[PLAYER_NO_MAX];
        if (m_SendOffset < s_SaveHeaderSize) {
            parts[1] = &saveNo;
            partSizes[1] = sizeof(saveNo);
            parts[2] = &flag;
            partSizes[2] = sizeof(flag);
            parts[3] = &mgr->m_NpcInfo;
            partSizes[3] = NPC_INFO_SIZE;
            mgr->m_EntryData.Save(target);
            parts[4] = mgr->m_EntryData.m_pBuffer;
            partSizes[4] = NetGameMgr::EntryData::s_BufferSize;
            parts[5] = mgr->m_PacketQueue.m_pReceiveStateBuffer;
            partSizes[5] = PacketQueue::s_StateBufferSize;
            count = 6;
            remaining = SAVE_FIRST_PART_SIZE - NetGameMgr::EntryData::s_BufferSize - PacketQueue::s_StateBufferSize;
            sent = NPC_INFO_SIZE + 2 + NetGameMgr::EntryData::s_BufferSize + PacketQueue::s_StateBufferSize;
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if (no == target) {
                    continue;
                }
                entries[no] = MakeStageEntry(mgr->m_StageMgr.m_Stages, no);
                parts[count] = &entries[no];
                partSizes[count] = sizeof(u16);
                count++;
                remaining -= sizeof(u16);
                sent += sizeof(u16);
            }
        }
        bool isGardenPrepared = false;
        SvGardenData* garden = GetGardenData();
        if (remaining != 0) {
            u32 offset = 0;
            if (m_SendOffset >= s_SaveHeaderSize) {
                offset = m_SendOffset - s_SaveHeaderSize;
            }
            u32 left = SAVE_DATA_SIZE - offset;
            if (left <= remaining) {
                remaining = left;
            }
            PrepareSendGarden(garden);
            parts[count] = reinterpret_cast<u8*>(garden) + offset;
            partSizes[count] = remaining;
            isGardenPrepared = true;
            sent += remaining;
            count++;
        }
        if (mgr->SendPacketBatch(target, parts, partSizes, count, PACKET_SAVE_DATA, 0, 1, 0)) {
            m_SendOffset += sent;
            if (isGardenPrepared) {
                FinishSendGarden(garden);
            }
        }
    } else {
        mgr->m_PacketQueue.IncrementCounter(target);
    }
    return m_SendOffset >= s_SaveHeaderSize + SAVE_DATA_SIZE;
}

// 0x0062167C (name is ours)
bool netgame::JoinMgr::SendRandomMatchData(PlayerNo target)
{
    if (target >= PLAYER_NO_MAX) {
        return false;
    }
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (!mgr->CanSend(target, false, 1)) {
        mgr->m_PacketQueue.IncrementCounter(target);
        return true;
    }
    const void* parts[11];
    size_t partSizes[11];
    u8 kind = 7;
    parts[0] = &kind;
    partSizes[0] = sizeof(kind);
    mgr->m_EntryData.Save(target);
    parts[1] = mgr->m_EntryData.m_pBuffer;
    partSizes[1] = NetGameMgr::EntryData::s_BufferSize;
    parts[2] = mgr->m_PacketQueue.m_pReceiveStateBuffer;
    partSizes[2] = PacketQueue::s_StateBufferSize;
    u32 count = 3;
    u16 entries[PLAYER_NO_MAX];
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no == target) {
            continue;
        }
        entries[no] = MakeStageEntry(mgr->m_StageMgr.m_Stages, no);
        parts[count] = &entries[no];
        partSizes[count] = sizeof(u16);
        count++;
    }
    parts[count] = g_pStaticData->data;
    partSizes[count] = sizeof(g_pStaticData->data);
    count++;
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        player->PrepareSend();
        parts[count] = player;
        partSizes[count] = PLAYER_DATA_SIZE;
        count++;
    }
    void* map = GetIslandMap();
    if (map != NULL) {
        parts[count] = map;
        partSizes[count] = ISLAND_MAP_SIZE;
        count++;
    }
    parts[count] = &mgr->m_NpcInfo;
    partSizes[count] = NPC_INFO_SIZE;
    u32 islandParam = mgr->m_IslandMgr.m_Timestamp;
    parts[count + 1] = &islandParam;
    partSizes[count + 1] = sizeof(islandParam);
    if (mgr->SendPacketBatch(target, parts, partSizes, count + 2, PACKET_RANDOM_MATCH_DATA, 0, 1, 0) && player != NULL) {
        player->FinishSend();
    }
    return true;
}

// 0x006218A4 (name is ours)
void netgame::JoinMgr::Reset()
{
    m_State = STATE_NONE;
    m_SendOffset = 0;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        m_PlayerFlags[i] = false;
    }
    unk_0xD = false;
    unk_0xE = false;
    unk_0xF = false;
    unk_0x10 = false;
    m_Stage = stage::NAME_NONE;
    unk_0x11 = false;
    unk_0x15 = false;
    unk_0x12 = false;
    unk_0x13 = false;
    unk_0x14 = false;
    unk_0x16 = false;
}

// 0x006218E8 (name is ours)
void netgame::NetGameMgr::StartEntry()
{
    m_JoinMgr.StartEntry();
}

// 0x006218F0 (name after libgarden)
void netgame::JoinMgr::StartEntry()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    PlayerNo source = mgr->m_LockMgr.m_SourcePlayer;
    if (source >= PLAYER_NO_MAX) {
        return;
    }
    unk_0x15 = false;
    PlayerNo myNo = mgr->m_MyPlayerNo;
    if (myNo == source) {
        m_State = STATE_WAIT_PLAYERS;
    } else if (myNo == PLAYER_NO_HOST) {
        m_State = STATE_WAIT_ALL_SAVE;
    } else {
        m_State = STATE_WAIT_GUEST;
    }
}

// 0x00621940 (name is ours)
netgame::JoinMgr::JoinMgr()
{
    m_State = STATE_NONE;
    m_SendOffset = 0;
    m_Stage = stage::NAME_NONE;
    unk_0xD = false;
    unk_0xE = false;
    unk_0xF = false;
    unk_0x10 = false;
    unk_0x11 = false;
    unk_0x12 = false;
    unk_0x13 = false;
    unk_0x14 = false;
    unk_0x15 = false;
    unk_0x16 = false;
}

// 0x0075F298 (name is ours)
bool netgame::JoinMgr::IsPlayerFlagSet() const
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    PlayerNo myNo = mgr->m_MyPlayerNo;
    if (myNo >= PLAYER_NO_MAX) {
        return false;
    }
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no != myNo && mgr->m_Bits[0].IsSet(no) && !m_PlayerFlags[no]) {
            return false;
        }
    }
    return true;
}
} // namespace netgame
