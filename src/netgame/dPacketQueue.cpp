#include "netgame/dPacketQueue.h"

#include "netgame/dNetGameMgr.h"
#include "sead/seadHeap.h"
#include "stage/stage_Api.h"
#include <cstring>

namespace netgame {
namespace {
// the marker around the copy of a state (detects a function that writes outside of the state)
const u8 GUARD = 0xFE;
} // namespace

// 0x0062881C | libgarden
void netgame::PacketQueue::AddPacketData(const void* data, size_t size)
{
    if (data == NULL || m_IsClosed) {
        return;
    }
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if ((mgr->m_Flags & NetGameMgr::FLAG_PACKET_OVERFLOW) || m_pFront == m_pBack) {
        return;
    }
    if (m_pBack == NULL || static_cast<u32>(m_pBack - m_pBuffer) + size > BUFFER_SIZE) {
        mgr->m_Flags |= NetGameMgr::FLAG_PACKET_OVERFLOW;
        return;
    }
    std::memcpy(m_pBack, data, size);
    m_pBack += size;
}

// 0x006288AC (name is ours)
void netgame::PacketQueue::UpdateState(u32 id, const void* value, bool shouldSend)
{
    if (m_IsClosed || m_pSendStateBuffer == NULL || id >= STATE_NUM || m_pReceiveStateBuffer == NULL) {
        return;
    }
    u8* state = m_pReceiveStateBuffer + s_StateOffsets[id];
    if (state == NULL) {
        return;
    }
    u32 size = s_StateSizes[id];
    if (size == 0 || size + 8 > m_SendStateBufferSize) {
        return;
    }
    u8* buffer = m_pSendStateBuffer;
    for (s32 i = 0; i < 4; i++) {
        buffer[i] = GUARD;
        buffer[size + 4 + i] = GUARD;
    }
    u8* copy = buffer + 4;
    std::memcpy(copy, state, size);
    StateUpdateFunction function = s_StateUpdateFunctions[id];
    if (function != NULL) {
        function(copy, value);
    }
    for (s32 i = 0; i < 4; i++) {
        if (buffer[i] != GUARD || buffer[size + 4 + i] != GUARD) {
            return;
        }
    }
    if (std::memcmp(copy, state, size) != 0) {
        std::memcpy(state, copy, size);
        if (m_pReceiveStateBuffer != NULL && m_pShouldSendStateUpdate != NULL) {
            m_pShouldSendStateUpdate[id] = shouldSend;
        }
    }
}

// 0x00628A4C | libgarden
void netgame::PacketQueue::BeginPacket()
{
    if (m_pFront == NULL || m_IsClosed) {
        return;
    }
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if ((mgr->m_Flags & NetGameMgr::FLAG_PACKET_OVERFLOW) || m_pFront != m_pBack) {
        return;
    }
    if (m_pBack == NULL || static_cast<u32>(m_pBack - m_pBuffer) + sizeof(PacketHeader) > BUFFER_SIZE) {
        mgr->m_Flags |= NetGameMgr::FLAG_PACKET_OVERFLOW;
        return;
    }
    m_pBack += sizeof(PacketHeader);
}

// 0x00628AB8 (name is ours)
void netgame::PacketQueue::IncrementCounter(PlayerNo player)
{
    if (player < PLAYER_NO_MAX && m_Counter[player] != -1) {
        m_Counter[player]++;
    }
}

// 0x00628AD8 (name is ours)
void netgame::PacketQueue::ResetCounters(PlayerNo player)
{
    if (player < PLAYER_NO_MAX) {
        m_Counter[player] = 0;
        m_Counter2[player] = 0;
    }
}

// 0x00628AF4 | libgarden
void netgame::PacketQueue::EndPacket(u8 id, PlayerNo target)
{
    if (m_IsClosed) {
        return;
    }
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->m_Flags & NetGameMgr::FLAG_PACKET_OVERFLOW) {
        return;
    }
    u8* front = m_pFront;
    u32 total = m_pBack - front;
    if (total < sizeof(PacketHeader)) {
        return;
    }
    u32 size = total - sizeof(PacketHeader);
    u8 stage = stage::GetCurrent();
    PlayerNo sender = mgr->m_MyPlayerNo;
    if (front != NULL) {
        PacketHeader header;
        header.id = id;
        header.sizeLow = size;
        header.sizeMid = size >> 8;
        header.stage = stage;
        header.sender = sender;
        header.unknown = 0;
        header.sizeHigh = size >> 16;
        header.target = target;
        std::memcpy(front, &header, sizeof(header));
    }
    m_pFront = m_pBack;
}

// 0x00628BCC (name is ours)
void netgame::PacketQueue::ResetCounter(PlayerNo player)
{
    if (player < PLAYER_NO_MAX) {
        m_Counter[player] = 0;
    }
}

// 0x00628BE4 (name is ours)
s8 netgame::PacketQueue::GetSystemPacketKind(u32 kind)
{
    if (kind < SYSTEM_PACKET_NUM) {
        return s_SystemPacketKinds[kind];
    }
    return 1;
}

// 0x00628BFC (name is ours)
void netgame::PacketQueue::DispatchSystemPacket(u32 sender, const void* data, u32 size, u32 kind)
{
    if (kind < SYSTEM_PACKET_NUM && s_SystemPacketHandlers[kind] != NULL) {
        s_SystemPacketHandlers[kind](sender, data, size, kind);
    }
}

// 0x00628C20 (name is ours)
void netgame::PacketQueue::DispatchPackets(const void* data, u32 size)
{
    if (data == NULL || size == 0 || size < sizeof(PacketHeader)) {
        return;
    }
    const u8* it = static_cast<const u8*>(data);
    u32 position = 0;
    do {
        if (it == NULL) {
            return;
        }
        PacketHeader header;
        std::memcpy(&header, it, sizeof(header));
        u32 packetSize = header.GetSize();
        if (packetSize >= 0x40000 || header.target > 7 || header.sender > 3) {
            return;
        }
        position += sizeof(PacketHeader);
        it += sizeof(PacketHeader);
        if (position + packetSize > size) {
            return;
        }
        if (header.id < PACKET_NUM && s_PacketHandlers[header.id] != NULL) {
            s_PacketHandlers[header.id](header.sender, it, packetSize, header.target, header.stage);
            it += packetSize;
            position += packetSize;
        }
    } while (position + sizeof(PacketHeader) <= size);
}

// 0x00628CF4 (name is ours)
void netgame::PacketQueue::ClearState(u32 id)
{
    if (id >= STATE_NUM) {
        return;
    }
    u32 size = s_StateSizes[id];
    u8* state = m_pReceiveStateBuffer != NULL ? m_pReceiveStateBuffer + s_StateOffsets[id] : NULL;
    if (size == 0 || state == NULL) {
        return;
    }
    std::memset(state, 0, size);
    if (s_StateInitFunctions[id] != NULL) {
        s_StateInitFunctions[id](state);
    }
    if (m_pReceiveStateBuffer != NULL && m_pShouldSendStateUpdate != NULL) {
        m_pShouldSendStateUpdate[id] = false;
    }
}

// 0x00628D80 (name is ours)
void netgame::PacketQueue::ClearStateRequests()
{
    if (m_pShouldSendStateUpdate != NULL) {
        std::memset(m_pShouldSendStateUpdate, 0, STATE_NUM);
    }
}

// 0x00628D98 (name is ours)
void netgame::PacketQueue::ClearAllStates()
{
    for (u32 i = 0; i < STATE_NUM; i++) {
        ClearState(i);
    }
}

// 0x00628E24 (name is ours)
void netgame::PacketQueue::Reset()
{
    m_pFront = m_pBuffer;
    m_pBack = m_pBuffer;
    ClearAllStates();
    m_Tick = 0;
    m_IsClosed = false;
    m_LeavingPlayer = PLAYER_NO_INVALID;
    for (s32 i = 0; i < 4; i++) {
        m_Counter[i] = 0;
        m_Counter2[i] = 0;
    }
}

// 0x00628EF0 (name is ours)
void netgame::PacketQueue::AllocBuffers(sead::Heap* heap)
{
    if (heap == NULL) {
        return;
    }
    if (m_pBuffer == NULL) {
        m_pBuffer = static_cast<u8*>(heap->tryAlloc(BUFFER_SIZE, 4));
    }
    if (m_pReceiveStateBuffer == NULL) {
        m_pReceiveStateBuffer = static_cast<u8*>(heap->tryAlloc(STATE_BUFFER_SIZE, 4));
    }
    if (m_pSendStateBuffer == NULL) {
        u32 maxSize = 0;
        for (u32 i = 0; i < STATE_NUM; i++) {
            if (maxSize < s_StateSizes[i]) {
                maxSize = s_StateSizes[i];
            }
        }
        if (maxSize != 0) {
            u32 size = maxSize + 8;
            m_pSendStateBuffer = static_cast<u8*>(heap->tryAlloc(size, 4));
            m_SendStateBufferSize = size;
        }
    }
    if (m_pShouldSendStateUpdate == NULL) {
        m_pShouldSendStateUpdate = static_cast<bool*>(heap->tryAlloc(STATE_NUM, 4));
    }
}

// 0x00628FF0 (name is ours)
netgame::PacketQueue::PacketQueue()
{
    m_pBuffer = NULL;
    m_pFront = NULL;
    m_pBack = NULL;
    m_pReceiveStateBuffer = NULL;
    m_pSendStateBuffer = NULL;
    m_SendStateBufferSize = 0;
    m_pShouldSendStateUpdate = NULL;
    m_LeavingPlayer = PLAYER_NO_INVALID;
    m_Tick = 0;
    m_IsClosed = false;
}

// 0x00627904 (name is ours)
void netgame::PacketQueue::Process()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (!mgr->IsMultiPlayer() || m_IsClosed) {
        return;
    }
    PlayerNo myNo = mgr->m_MyPlayerNo;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no == myNo || !mgr->IsAdmitted(no)) {
            continue;
        }
        sead::CriticalSection& section = mgr->m_CriticalSection;
        for (s32 count = 0;; count++) {
            section.lock();
            const void* packet = mgr->m_Receiver.GetPacket(no);
            if (packet == NULL || count != 0) {
                section.unlock();
                break;
            }
            const u8* data = static_cast<const u8*>(packet);
            u32 sizes = *reinterpret_cast<const u32*>(data);
            data += sizeof(u32);
            u32 stateSize = sizes & 0xFFFF;
            u32 packetSize = sizes >> 16;
            if (stateSize != 0) {
                const u8* state = data;
                u32 offset = 0;
                while (offset + 1 <= stateSize) {
                    u32 id = *state;
                    if (id >= STATE_NUM || m_pReceiveStateBuffer == NULL) {
                        break;
                    }
                    u8* target = m_pReceiveStateBuffer + s_StateOffsets[id];
                    if (target == NULL) {
                        break;
                    }
                    u32 size = s_StateSizes[id];
                    std::memcpy(target, state + 1, size);
                    offset += 1 + size;
                    state += 1 + size;
                }
                data += stateSize;
            }
            if (packetSize != 0 && data != NULL && packetSize >= sizeof(PacketHeader)) {
                u32 offset = 0;
                while (data != NULL) {
                    PacketHeader header;
                    std::memcpy(&header, data, sizeof(header));
                    u32 size = header.GetSize();
                    if (size >= 0x40000 || header.target > 7 || header.sender > 3) {
                        break;
                    }
                    offset += sizeof(PacketHeader);
                    if (offset + size > packetSize) {
                        break;
                    }
                    data += sizeof(PacketHeader);
                    if (header.id < PACKET_NUM && s_PacketHandlers[header.id] != NULL) {
                        s_PacketHandlers[header.id](header.sender, data, size, header.target, header.stage);
                        data += size;
                        offset += size;
                    }
                    if (offset + sizeof(PacketHeader) > packetSize) {
                        break;
                    }
                }
            }
            mgr->m_Receiver.SetIsEnabled(no, false);
            mgr->m_Receiver.SetWasHandled(no, true);
            section.unlock();
        }
    }
    NetGameMgr::TransitionMgr& transition = mgr->m_TransitionMgr;
    if (transition.m_ExitingPlayer >= PLAYER_NO_MAX || transition.m_State != 2) {
        return;
    }
    StageMgr& stageMgr = mgr->m_StageMgr;
    if (stageMgr.m_Stages.FindController(stageMgr.m_Stages.GetStage(myNo)) != myNo) {
        return;
    }
    if (transition.m_BufferSize != 0 && transition.m_pBuffer != NULL) {
        DispatchPackets(transition.m_pBuffer, transition.m_BufferSize);
    }
    transition.Reset();
}

namespace {
const u32 SEND_LIMIT = 0xD000;
const u8 PACKET_PLAYERS_FULL = 1;
const u8 PACKET_PLAYERS = 2;
const u8 PACKET_PLAYER_ADDED = 3;
const u8 PACKET_PLAYER_REMOVED = 4;
const u8 PACKET_FRAME = 27;

const u32 STATIONS_SIZE = 4 * sizeof(nn::pia::StationId);

enum PacketTarget : u8
{
    PACKET_TARGET_ALL = 4,
    PACKET_TARGET_CONTROLLER = 5,
    PACKET_TARGET_SAME_STAGE = 6,
};

DECOMP_ALWAYS_INLINE void AppendPacket(u8** writes, u32* extraSizes, s32 no, const void* data, u32 size, u32 headerSize)
{
    if (writes[no] != NULL && extraSizes[no] + size + headerSize <= SEND_LIMIT) {
        std::memcpy(writes[no], data, size);
        extraSizes[no] += size;
        writes[no] += size;
    }
}
} // namespace

// 0x00627BFC (name is ours)
void netgame::PacketQueue::ProcessEnd()
{
    if (m_IsClosed) {
        return;
    }
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    PlayerNo myNo = mgr->m_MyPlayerNo;
    if (myNo >= PLAYER_NO_MAX) {
        return;
    }
    sead::CriticalSection& section = mgr->m_CriticalSection;
    section.lock();
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (mgr->m_Bits[6].IsSet(no)) {
            if (mgr->SendPacketBatch(no, NULL, NULL, 0, PACKET_PLAYER_REMOVED, 0, 0, 0)) {
                mgr->m_Bits[6].Remove(no);
            }
        } else if (mgr->m_ExistingPlayers.IsSet(no)) {
            nn::pia::StationId stations[PLAYER_NO_MAX];
            mgr->m_AvatarMgr.GetStations(stations);
            const void* parts[] = {stations};
            const size_t partSizes[] = {STATIONS_SIZE};
            if (mgr->SendPacketBatch(no, parts, partSizes, 1, PACKET_PLAYERS, 0, 0, 0)) {
                mgr->m_ExistingPlayers.Remove(no);
            }
        } else if (mgr->m_Entry.IsSet(no)) {
            nn::pia::StationId stations[PLAYER_NO_MAX];
            mgr->m_AvatarMgr.GetStations(stations);
            const void* parts[] = {stations};
            const size_t partSizes[] = {STATIONS_SIZE};
            mgr->SendPacketBatch(no, parts, partSizes, 1, PACKET_PLAYERS_FULL, 0, 0, 1);
        } else if (mgr->m_Bits[4].IsSet(no)) {
            mgr->SendPacketBatch(no, NULL, NULL, 0, PACKET_PLAYER_ADDED, 0, 0, 1);
        }
    }
    section.unlock();
    StageMgr& stageMgr = mgr->m_StageMgr;
    if (mgr->GetPlayerCount() <= 1) {
        FinishFrame();
        return;
    }
    MachineBitTable targets;
    if (!mgr->CollectGuests(&targets, false, 1)) {
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            if (targets.IsSet(static_cast<PlayerNo>(i))) {
                m_Counter[i]++;
                m_Counter2[i]++;
            }
        }
        return;
    }
    bool restoresTransition = false;
    u32 savedTransitionSize = 0;
    u32 controller = PLAYER_NO_MAX;
    NetGameMgr::TransitionMgr& transition = mgr->m_TransitionMgr;
    if (transition.m_EnteringPlayer < PLAYER_NO_MAX || transition.m_ExitingPlayer < PLAYER_NO_MAX) {
        if (transition.m_State != 2) {
            restoresTransition = true;
            savedTransitionSize = transition.m_BufferSize;
        } else if (transition.m_EnteringPlayer < PLAYER_NO_MAX) {
            controller = transition.m_EnteringPlayer;
        } else if (transition.m_ExitingPlayer < PLAYER_NO_MAX) {
            PlayerNo owner = stageMgr.m_Stages.FindController(stageMgr.m_Stages.GetStage(myNo));
            if (owner != myNo && owner < PLAYER_NO_MAX) {
                controller = owner;
            }
        }
    }
    u32 finalSizes[PLAYER_NO_MAX] = {};
    bool handled[PLAYER_NO_MAX] = {};
    u8* starts[PLAYER_NO_MAX];
    u8* writes[PLAYER_NO_MAX];
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no == myNo) {
            starts[i] = NULL;
            writes[i] = NULL;
            continue;
        }
        if (mgr->IsAdmitted(no)) {
            mgr->m_SendMgr.Prepare(no);
            u8* buffer = mgr->m_SendMgr.GetBuffer(no);
            starts[i] = buffer;
            writes[i] = buffer;
            if (buffer != NULL) {
                writes[i] = buffer + sizeof(PacketHeader);
            }
        } else {
            starts[i] = NULL;
        }
    }
    u32 stateSize = 0;
    for (s32 id = 0; id < static_cast<s32>(STATE_NUM); id++) {
        if (m_pReceiveStateBuffer == NULL || m_pShouldSendStateUpdate == NULL || !m_pShouldSendStateUpdate[id]) {
            continue;
        }
        const u8* state = m_pReceiveStateBuffer + s_StateOffsets[id];
        if (state == NULL) {
            continue;
        }
        u32 size = s_StateSizes[id];
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            if (writes[i] != NULL) {
                *writes[i]++ = id;
                std::memcpy(writes[i], state, size);
                writes[i] += size;
            }
        }
        stateSize += size + 1;
    }
    u32 extraSizes[PLAYER_NO_MAX] = {};
    if (controller < PLAYER_NO_MAX) {
        u32 size = transition.m_BufferSize;
        if (size != 0 && writes[controller] != NULL && stateSize + size + sizeof(PacketHeader) <= SEND_LIMIT) {
            std::memcpy(writes[controller], transition.m_pBuffer, size);
            writes[controller] += size;
            extraSizes[controller] += size;
        }
    }
    u32 total = m_pFront == m_pBack ? m_pFront - m_pBuffer : 0;
    const u8* packet = m_pBuffer;
    u32 headerSize = stateSize + sizeof(PacketHeader);
    if (packet != NULL && total >= sizeof(PacketHeader)) {
        u32 offset = 0;
        do {
            PacketHeader header;
            std::memcpy(&header, packet, sizeof(header));
            u32 size = header.GetSize();
            if (size >= 0x40000 || header.target > 7 || header.sender > 3) {
                break;
            }
            u32 fullSize = size + sizeof(PacketHeader);
            switch (header.target) {
            case 0:
            case 1:
            case 2:
            case 3:
                AppendPacket(writes, extraSizes, header.target, packet, fullSize, headerSize);
                break;
            case PACKET_TARGET_ALL:
                for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                    AppendPacket(writes, extraSizes, i, packet, fullSize, headerSize);
                }
                break;
            case PACKET_TARGET_CONTROLLER: {
                stage::Name stage = stageMgr.m_Stages.GetStage(myNo);
                if (stage >= stage::NAME_MAX) {
                    break;
                }
                PlayerNo owner = stageMgr.m_Stages.FindController(stage);
                if (owner >= PLAYER_NO_MAX) {
                    break;
                }
                if (owner == myNo) {
                    if (transition.m_State == 1 && transition.m_ExitingPlayer < PLAYER_NO_MAX && mgr->IsStageControl()) {
                        transition.Add(packet, fullSize);
                    }
                } else if (writes[owner] != NULL) {
                    if (transition.m_ExitingPlayer < PLAYER_NO_MAX && transition.m_State != 2) {
                        transition.Add(packet, fullSize);
                    } else {
                        AppendPacket(writes, extraSizes, owner, packet, fullSize, headerSize);
                    }
                }
                break;
            }
            case PACKET_TARGET_SAME_STAGE: {
                if (!stageMgr.m_Stages.HasControl(myNo)) {
                    break;
                }
                stage::Name stage = stageMgr.m_Stages.GetStage(myNo);
                if (stage >= stage::NAME_MAX) {
                    break;
                }
                for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                    PlayerNo no = static_cast<PlayerNo>(i);
                    if (writes[i] == NULL) {
                        continue;
                    }
                    if (stageMgr.m_Stages.GetStage(no) == stage) {
                        if (stageMgr.unk_0x20 != no) {
                            AppendPacket(writes, extraSizes, i, packet, fullSize, headerSize);
                        }
                    } else if (transition.m_EnteringPlayer == no && transition.m_State != 2) {
                        transition.Add(packet, fullSize);
                    }
                }
                break;
            }
            default:
                break;
            }
            offset += fullSize;
            packet += fullSize;
        } while (offset + sizeof(PacketHeader) <= total);
    }
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        u8* start = starts[i];
        if (start == NULL) {
            continue;
        }
        PlayerNo no = static_cast<PlayerNo>(i);
        u32 size = stateSize + extraSizes[i];
        if (size != 0) {
            finalSizes[i] = size + sizeof(PacketHeader);
            handled[i] = mgr->WritePacketHead(&start, no, false, PACKET_FRAME);
            MakeParam(reinterpret_cast<u32*>(start), stateSize, extraSizes[i]);
        } else {
            finalSizes[i] = sizeof(NetGameMgr::PacketHead);
            handled[i] = mgr->WritePacketHead(&start, no, true, PACKET_FRAME);
        }
    }
    bool isBusy = false;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        if (starts[i] != NULL && finalSizes[i] > sizeof(NetGameMgr::PacketHead) && mgr->m_SendMgr.IsBusy(static_cast<PlayerNo>(i))) {
            isBusy = true;
            break;
        }
    }
    if (isBusy) {
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            if (starts[i] == NULL) {
                continue;
            }
            if (handled[i] || m_Counter2[i] >= 300) {
                if (mgr->SendPacketBatch(static_cast<PlayerNo>(i), NULL, NULL, 0, PACKET_FRAME, 1, 1, 0)) {
                    m_Counter2[i] = 0;
                }
            } else {
                m_Counter2[i]++;
            }
            m_Counter[i]++;
        }
        if (restoresTransition) {
            transition.m_BufferSize = savedTransitionSize;
        }
        return;
    }
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        if (starts[i] == NULL) {
            continue;
        }
        PlayerNo no = static_cast<PlayerNo>(i);
        if (finalSizes[i] <= sizeof(NetGameMgr::PacketHead) && !handled[i] && m_Counter2[i] < 300) {
            m_Counter2[i]++;
        } else if (mgr->m_AvatarMgr.Send(no, starts[i], finalSizes[i])) {
            mgr->m_SendCounters[i]++;
            m_Counter2[i] = 0;
        }
        m_Counter[i] = 0;
        mgr->m_Receiver.SetWasHandled(no, false);
        if (finalSizes[i] > sizeof(NetGameMgr::PacketHead)) {
            mgr->m_SendMgr.AddPending(no, 1);
        }
    }
    FinishFrame();
    if (controller < PLAYER_NO_MAX) {
        transition.Reset();
    } else if (transition.m_ExitingPlayer < PLAYER_NO_MAX && transition.m_State == 1) {
        transition.m_State = 2;
    }
}

DECOMP_ALWAYS_INLINE void netgame::PacketQueue::FinishFrame()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (m_pShouldSendStateUpdate != NULL) {
        std::memset(m_pShouldSendStateUpdate, 0, STATE_NUM);
    }
    m_pFront = m_pBuffer;
    m_pBack = m_pBuffer;
    mgr->m_StageMgr.ProcessPackets();
    mgr->m_SaveDemoMgr.AdvanceAfterPackets();
    mgr->m_LeaveMgr.OnPlayerLeft();
    mgr->m_IslandMgr.OnPlayerGone();
    m_Tick = g_FrameCounter;
}
} // namespace netgame
