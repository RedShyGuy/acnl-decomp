#include "netgame/dStageMgr.h"

#include "netgame/dNetGameMgr.h"

namespace netgame {
namespace {
const u8 PACKET_PLAYER_STAGE = 1;
const u8 PACKET_TRANSITION = 3;
const u8 PACKET_TRANSITIONS_DONE = 4;
const u8 PACKET_TRANSITION_START = 5;
const u8 PACKET_NEEDS_CONTROLLER = 6;
const u8 PACKET_LEAVE_WITHOUT_CONTROL = 8;
const u8 PACKET_ASK_CONTROLLER = 10;
const u8 PACKET_ASK_HOST = 11;
const u8 PACKET_TRANSITION_REQUEST = 13;
const u8 PACKET_PAYLOAD_D = 14;
const u8 PACKET_PAYLOAD_E = 15;
const u8 PACKET_PAYLOAD_A = 16;
const u8 PACKET_PAYLOAD_C = 17;

const u8 SLOT_NONE = 4;

DECOMP_ALWAYS_INLINE void SendStageUpdate(const StageMgr::PlayerStageUpdatePacket& packet, PlayerNo target)
{
    PacketQueue& queue = NetGameMgr::s_pInstance->m_PacketQueue;
    queue.BeginPacket();
    queue.AddPacketData(&packet, sizeof(packet));
    queue.EndPacket(PACKET_PLAYER_STAGE, target);
}

DECOMP_ALWAYS_INLINE PlayerNo FindNextInLine(const StageMgr::Transitions& transitions)
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (transitions.GetSlot(no) == 0) {
            return no;
        }
    }
    return PLAYER_NO_INVALID;
}
} // namespace

// 0x00621980 (name is ours)
void netgame::StageMgr::ProcessFrame()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->m_PacketQueue.m_IsClosed || !mgr->IsMultiPlayer()) {
        return;
    }
    PlayerNo myNo = mgr->GetMyPlayerNo();
    PacketQueue& queue = mgr->m_PacketQueue;
    switch (m_State) {
    case 2: {
        if (GetTransitioningPlayer() != myNo || !m_ArriveConfirm.IsComplete()) {
            return;
        }
        stage::Name stage = m_Stages.GetStage(myNo);
        if (!m_Stages.HasControl(myNo)) {
            unk_0x23 = 0;
            queue.BeginPacket();
            queue.EndPacket(PACKET_LEAVE_WITHOUT_CONTROL, static_cast<PlayerNo>(5));
            m_State = 5;
            return;
        }
        PlayerNo other = m_Stages.FindPlayerWithoutControl(stage);
        if (other >= PLAYER_NO_MAX) {
            m_State = 3;
            return;
        }
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            PlayerNo no = static_cast<PlayerNo>(i);
            if (myNo != no && mgr->IsAdmitted(no) && m_Stages.GetStage(no) == stage) {
                m_Players.Remove(no);
            } else {
                m_Players.Add(no);
            }
        }
        mgr->m_PayloadA.m_Player = other;
        queue.BeginPacket();
        queue.EndPacket(PACKET_NEEDS_CONTROLLER, static_cast<PlayerNo>(6));
        m_State = 8;
        break;
    }
    case 11: {
        if (!m_LeaveConfirm.IsComplete()) {
            return;
        }
        stage::Name stage = GetNextStage();
        PlayerNo controller = m_Stages.FindController(stage);
        if (controller < PLAYER_NO_MAX) {
            queue.BeginPacket();
            queue.EndPacket(PACKET_ASK_CONTROLLER, controller);
            mgr->m_PayloadC.unk_0x9 = 0;
            m_State = 13;
            return;
        }
        if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST) {
            queue.BeginPacket();
            queue.EndPacket(PACKET_ASK_HOST, PLAYER_NO_HOST);
            mgr->m_PayloadB.unk_0x8 = 0;
        } else {
            mgr->m_PayloadB.Build(0, stage);
            mgr->m_PayloadB.unk_0x8 = 1;
        }
        m_State = 12;
        break;
    }
    case 12:
        if (mgr->m_PayloadB.unk_0x8) {
            mgr->m_PayloadB.Apply(myNo, GetNextStage());
            m_State = 14;
        }
        break;
    case 13:
        if (mgr->m_PayloadC.unk_0x9) {
            mgr->m_PayloadC.Apply(myNo, GetNextStage());
            m_State = 14;
        }
        break;
    case 16:
        if (m_LeaveConfirm.IsComplete()) {
            if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST) {
                TransitionRequest request;
                request.isDone = true;
                request.stage = stage::NAME_NONE;
                queue.BeginPacket();
                queue.AddPacketData(&request, sizeof(request));
                queue.EndPacket(PACKET_TRANSITION_REQUEST, PLAYER_NO_HOST);
            } else {
                FinishTransition(PLAYER_NO_HOST);
            }
            m_State = 17;
        }
        break;
    case 17:
        if (GetTransitioningPlayer() != myNo) {
            m_State = 0;
        }
        break;
    default:
        break;
    }
}

// 0x00621DC8 (name is ours)
void netgame::StageMgr::Process()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->m_PacketQueue.m_IsClosed || !mgr->IsMultiPlayer()) {
        return;
    }
    PlayerNo myNo = mgr->GetMyPlayerNo();
    if (m_State != 1) {
        return;
    }
    m_ArriveConfirm.Update(myNo, false);
    stage::Name next = stage::GetNext();
    if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST) {
        TransitionRequest request;
        request.isDone = false;
        request.stage = next;
        PacketQueue& queue = mgr->m_PacketQueue;
        queue.BeginPacket();
        queue.AddPacketData(&request, sizeof(request));
        queue.EndPacket(PACKET_TRANSITION_REQUEST, PLAYER_NO_HOST);
    } else {
        RequestTransition(myNo, next);
    }
    m_State = 2;
}

// 0x00621EA0 (name is ours)
void netgame::StageMgr::OnArrive()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    PlayerNo myNo = mgr->GetMyPlayerNo();
    m_LeaveConfirm.Update(myNo, true);
    stage::Name next = GetNextStage();
    bool isAlone = m_Stages.GetPlayerCount(next) == 0;
    SetStage(myNo, next);
    UpdateStageControl(myNo, isAlone);
    if (mgr->GetPlayerCount() > 1) {
        PlayerStageUpdatePacket packet;
        packet.Set(myNo, true, next, true, isAlone, myNo);
        SendStageUpdate(packet, PLAYER_NO_INVALID);
    }
}

// 0x00621F78 (name is ours)
void netgame::StageMgr::AddPlayer(PlayerNo player, stage::Name stage)
{
    SetStage(player, stage);
    UpdateStageControl(player, true);
}

// 0x00621F9C (name is ours)
void netgame::StageMgr::ProcessEnd()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->unk_0x132B4 || IsSaving() || mgr->m_PacketQueue.m_IsClosed) {
        return;
    }
    if (m_State == 6) {
        m_State = 7;
        return;
    }
    if (m_State == 9) {
        m_State = 10;
        return;
    }
    if (m_State != 4) {
        return;
    }
    PlayerNo myNo = mgr->GetMyPlayerNo();
    PayloadD& payload = mgr->m_PayloadD;
    PacketQueue& queue = mgr->m_PacketQueue;
    if (mgr->GetPlayerCount() > 1) {
        if (myNo != PLAYER_NO_HOST) {
            queue.BeginPacket();
            if (payload.m_Size != 0) {
                queue.AddPacketData(payload.m_pBuffer, payload.m_Size);
            }
            queue.EndPacket(PACKET_PAYLOAD_D, PLAYER_NO_HOST);
        } else {
            stage::Name stage = m_Stages.GetStage(PLAYER_NO_HOST);
            if (stage < stage::NAME_MAX) {
                payload.Apply(PLAYER_NO_HOST, stage);
            }
        }
    }
    m_LeaveConfirm.Update(myNo, true);
    SetStage(myNo, stage::NAME_NONE);
    UpdateStageControl(myNo, false);
    if (mgr->GetPlayerCount() > 1) {
        PlayerStageUpdatePacket packet;
        packet.Set(myNo, true, stage::NAME_NONE, true, false, myNo);
        SendStageUpdate(packet, PLAYER_NO_INVALID);
    }
    payload.Reset();
    m_State = 11;
}

// 0x0062215C (name is ours)
void netgame::StageMgr::RemovePlayer(PlayerNo player)
{
    if (player >= PLAYER_NO_MAX) {
        return;
    }
    m_Transitions.Clear(player);
    SetStage(player, stage::NAME_NONE);
    UpdateStageControl(player, false);
    m_Players.Remove(player);
}

// 0x006221A8 (name is ours)
void netgame::StageMgr::FinishTransition(PlayerNo player)
{
    m_Transitions.SetSlot(player, SLOT_NONE);
    m_Transitions.SetNextStage(player, stage::NAME_NONE);
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        u8 slot = m_Transitions.GetSlot(no) - 1;
        if (slot < 3) {
            m_Transitions.SetSlot(no, slot);
        }
    }
    PlayerNo next = GetTransitioningPlayer();
    PacketQueue& queue = NetGameMgr::s_pInstance->m_PacketQueue;
    if (next >= PLAYER_NO_MAX) {
        queue.BeginPacket();
        queue.EndPacket(PACKET_TRANSITIONS_DONE, PLAYER_NO_INVALID);
        return;
    }
    CurrentTransition transition;
    transition.Set(next, m_Transitions.GetNextStage(next));
    queue.BeginPacket();
    queue.AddPacketData(&transition, sizeof(transition));
    queue.EndPacket(PACKET_TRANSITION, PLAYER_NO_INVALID);
    if (next != PLAYER_NO_HOST) {
        queue.BeginPacket();
        queue.EndPacket(PACKET_TRANSITION_START, next);
    } else {
        m_ArriveConfirm.Add(PLAYER_NO_HOST);
    }
}

// 0x00622320 (name is ours)
void netgame::StageMgr::ProcessPackets()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    PacketQueue& queue = mgr->m_PacketQueue;
    if (queue.m_IsClosed) {
        return;
    }
    u8 state = m_State;
    PlayerNo myNo = mgr->GetMyPlayerNo();
    if (state == 7) {
        if (mgr->GetPlayerCount() > 1) {
            m_LeaveConfirm.Update(myNo, true);
            PlayerNo controller = m_Stages.FindController(m_Stages.GetStage(myNo));
            PayloadE& payload = mgr->m_PayloadE;
            queue.BeginPacket();
            if (payload.m_Size != 0) {
                queue.AddPacketData(payload.m_pBuffer, payload.m_Size);
            }
            queue.EndPacket(PACKET_PAYLOAD_E, controller);
            payload.Reset();
            SetStage(myNo, stage::NAME_NONE);
            UpdateStageControl(myNo, false);
            PlayerStageUpdatePacket packet;
            packet.Set(myNo, true, stage::NAME_NONE, true, false, myNo);
            SendStageUpdate(packet, PLAYER_NO_INVALID);
            m_State = 11;
        }
    } else if (state == 10) {
        if (mgr->GetPlayerCount() > 1) {
            PayloadA& payload = mgr->m_PayloadA;
            PlayerNo other = payload.m_Player;
            m_LeaveConfirm.Update(myNo, true);
            queue.BeginPacket();
            if (payload.m_Size != 0) {
                queue.AddPacketData(payload.m_pBuffer, payload.m_Size);
            }
            queue.EndPacket(PACKET_PAYLOAD_A, other);
            payload.Reset();
            SetStage(myNo, stage::NAME_NONE);
            UpdateStageControl(myNo, false);
            UpdateStageControl(other, true);
            PlayerStageUpdatePacket packet;
            packet.Set(myNo, true, stage::NAME_NONE, true, false, PLAYER_NO_INVALID);
            SendStageUpdate(packet, other);
            packet.Set(other, false, stage::NAME_NONE, true, true, myNo);
            SendStageUpdate(packet, other);
            m_State = 11;
        }
    }
    PayloadC& payload = mgr->m_PayloadC;
    PlayerNo player = payload.m_Player;
    if (player >= PLAYER_NO_MAX) {
        return;
    }
    stage::Name next = GetNextStage();
    if (next >= stage::NAME_MAX || m_Stages.GetStage(myNo) != next) {
        return;
    }
    payload.Build(player, next);
    queue.BeginPacket();
    if (payload.m_Size != 0) {
        queue.AddPacketData(payload.m_pBuffer, payload.m_Size);
    }
    queue.EndPacket(PACKET_PAYLOAD_C, player);
    mgr->m_TransitionMgr.m_EnteringPlayer = player;
    mgr->m_TransitionMgr.m_State = 0;
    payload.Reset();
}

// 0x00622670 (name is ours)
void netgame::StageMgr::RequestTransition(PlayerNo player, stage::Name stage)
{
    u8 slot = 0;
    if (GetTransitioningPlayer() < PLAYER_NO_MAX) {
        slot = 1;
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            u8 other = m_Transitions.GetSlot(static_cast<PlayerNo>(i));
            if (other < SLOT_NONE && other >= slot) {
                slot = other + 1;
            }
        }
    }
    m_Transitions.SetSlot(player, slot);
    m_Transitions.SetNextStage(player, stage);
    if (slot != 0) {
        return;
    }
    CurrentTransition transition;
    transition.Set(player, stage);
    PacketQueue& queue = NetGameMgr::s_pInstance->m_PacketQueue;
    queue.BeginPacket();
    queue.AddPacketData(&transition, sizeof(transition));
    queue.EndPacket(PACKET_TRANSITION, PLAYER_NO_INVALID);
    if (player != PLAYER_NO_HOST) {
        queue.BeginPacket();
        queue.EndPacket(PACKET_TRANSITION_START, player);
    } else {
        m_ArriveConfirm.Add(PLAYER_NO_HOST);
    }
}

// 0x006227CC (name is ours)
void netgame::StageMgr::Reset()
{
    InitState();
    m_Players.Reset();
    SetStage(PLAYER_NO_HOST, stage::GetCurrent());
    UpdateStageControl(PLAYER_NO_HOST, true);
}

// 0x0062287C (name after libgarden)
void netgame::StageMgr::UpdateStageControl(PlayerNo player, bool control)
{
    m_Stages.SetControl(player, control);
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    NetGameMgr::TransitionMgr& transition = mgr->m_TransitionMgr;
    PlayerNo exiting = transition.m_ExitingPlayer;
    if (exiting >= PLAYER_NO_MAX || transition.m_State != 0) {
        return;
    }
    PlayerNo myNo = mgr->GetMyPlayerNo();
    PlayerNo controller = m_Stages.FindController(m_Stages.GetStage(myNo));
    if (controller >= PLAYER_NO_MAX || controller == exiting) {
        return;
    }
    if (controller != myNo) {
        transition.m_State = 2;
        return;
    }
    transition.m_State = 1;
    PlayerStageUpdatePacket exitingPacket;
    exitingPacket.Set(exiting, true, stage::NAME_NONE, true, false, PLAYER_NO_INVALID);
    PlayerStageUpdatePacket myPacket;
    myPacket.Set(myNo, false, stage::NAME_NONE, true, true, exiting);
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no != exiting && no != myNo && mgr->IsAdmitted(no)) {
            SendStageUpdate(exitingPacket, no);
            SendStageUpdate(myPacket, no);
        }
    }
}

// 0x006229E4 (name after libgarden)
void netgame::StageMgr::SetStage(PlayerNo player, stage::Name stage)
{
    if (player >= PLAYER_NO_MAX) {
        return;
    }
    NetGameMgr::TransitionMgr& transition = NetGameMgr::s_pInstance->m_TransitionMgr;
    PlayerNo watched = unk_0x20;
    PlayerNo entering = transition.m_EnteringPlayer;
    bool isWatched = false;
    stage::Name old = stage::NAME_NONE;
    if (watched == player || entering == player) {
        isWatched = true;
        old = m_Stages.GetStage(player);
    }
    m_Stages.SetStage(player, stage);
    if (isWatched && m_Stages.GetStage(player) != old) {
        if (watched == player) {
            unk_0x20 = PLAYER_NO_INVALID;
        }
        if (entering == player && transition.m_State != 2) {
            transition.m_State = 2;
        }
    }
}

// 0x00622A94 (name is ours)
void netgame::StageMgr::SetStages(const MachineBitTable& players, stage::Name stage)
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (players.IsSet(no)) {
            SetStage(no, stage);
            UpdateStageControl(no, no == PLAYER_NO_HOST);
        } else {
            SetStage(no, stage::NAME_NONE);
            UpdateStageControl(no, false);
        }
    }
}

// 0x00622BD0 (name is ours)
netgame::StageMgr::StageMgr()
{
    m_State = 0;
    unk_0x20 = PLAYER_NO_INVALID;
    unk_0x23 = 0;
}

// 0x00622C30 (name is ours)
netgame::StageMgr::~StageMgr()
{
    // (only the members are destroyed)
}

// 0x006256BC (name is ours)
void netgame::StageMgr::InitState()
{
    m_State = 0;
    m_Transitions.Reset();
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        SetStage(no, stage::NAME_NONE);
        UpdateStageControl(no, false);
    }
    m_CurrentTransition.Reset();
    m_ArriveConfirm.Reset();
    m_LeaveConfirm.Reset();
    unk_0x20 = PLAYER_NO_INVALID;
    unk_0x23 = 0;
}

// 0x00626620 (name is ours)
void netgame::StageMgr::ClearCurrentTransition()
{
    m_CurrentTransition.Reset();
}

// 0x00626628 (name is ours)
void netgame::StageMgr::CurrentTransition::Reset()
{
    player = PLAYER_NO_INVALID;
    stage = stage::NAME_NONE;
}

// 0x0062663C (name is ours)
void netgame::StageMgr::CurrentTransition::Set(PlayerNo newPlayer, stage::Name newStage)
{
    if (newPlayer < PLAYER_NO_MAX && newStage < stage::NAME_MAX) {
        player = newPlayer;
        stage = newStage;
    }
}

// 0x00626654 (name is ours)
void netgame::StageMgr::SetCurrentTransition(const CurrentTransition& transition)
{
    m_CurrentTransition.Assign(transition);
}

// 0x0062665C (name is ours)
void netgame::StageMgr::CurrentTransition::Assign(const CurrentTransition& other)
{
    *this = other;
}

// 0x00626668 (name is ours)
netgame::StageMgr::CurrentTransition::CurrentTransition()
{
    player = PLAYER_NO_INVALID;
    stage = stage::NAME_NONE;
}

// 0x0062667C (name is ours)
void netgame::StageMgr::Transitions::SetNextStage(PlayerNo player, stage::Name stage)
{
    if (NetGameMgr::s_pInstance->GetMyPlayerNo() != PLAYER_NO_HOST) {
        return;
    }
    if (stage <= stage::NAME_NONE && player < PLAYER_NO_MAX) {
        nextStages[player] = stage;
    }
}

// 0x006266B8 (name is ours)
void netgame::StageMgr::Transitions::SetSlot(PlayerNo player, u8 slot)
{
    if (NetGameMgr::s_pInstance->GetMyPlayerNo() != PLAYER_NO_HOST) {
        return;
    }
    if (slot <= SLOT_NONE && player < PLAYER_NO_MAX) {
        slots[player] = slot;
    }
}

// 0x006266F8 (name is ours)
void netgame::StageMgr::Transitions::Clear(PlayerNo player)
{
    if (player < PLAYER_NO_MAX) {
        nextStages[player] = stage::NAME_NONE;
        slots[player] = SLOT_NONE;
    }
}

// 0x00626718 (name is ours)
void netgame::StageMgr::Transitions::Reset()
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        Clear(static_cast<PlayerNo>(i));
    }
}

// 0x00626754 (name is ours)
netgame::StageMgr::Transitions::Transitions()
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        Clear(static_cast<PlayerNo>(i));
    }
}

// 0x0075F318 (name after libgarden)
netgame::PlayerNo netgame::StageMgr::GetTransitioningPlayer() const
{
    if (NetGameMgr::s_pInstance->GetMyPlayerNo() != PLAYER_NO_HOST) {
        return m_CurrentTransition.player;
    }
    return FindNextInLine(m_Transitions);
}

// 0x0075F378 (name is ours)
bool netgame::StageMgr::AreAllPlayersIn(stage::Name stage) const
{
    return m_Stages.GetPlayerCount(stage) == NetGameMgr::s_pInstance->GetPlayerCount();
}

// 0x0075F3A8 (name is ours)
stage::Name netgame::StageMgr::GetNextStage() const
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    PlayerNo player = GetTransitioningPlayer();
    if (player >= PLAYER_NO_MAX) {
        return stage::NAME_NONE;
    }
    if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST) {
        return m_CurrentTransition.stage;
    }
    return m_Transitions.GetNextStage(player);
}

// 0x0075F42C (name is ours)
bool netgame::StageMgr::AreAllPlayersConfirmed() const
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        if (!m_Players.IsSet(static_cast<PlayerNo>(i))) {
            return false;
        }
    }
    return true;
}

// 0x0075F464 (name after libgarden)
bool netgame::StageMgr::FUN_0075f464() const
{
    if (NetGameMgr::s_pInstance->GetMyPlayerNo() != PLAYER_NO_HOST) {
        return false;
    }
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        if (m_Transitions.GetSlot(static_cast<PlayerNo>(i)) < SLOT_NONE) {
            return true;
        }
    }
    return false;
}

// 0x0075FA60 (name is ours)
stage::Name netgame::StageMgr::Transitions::GetNextStage(PlayerNo player) const
{
    if (NetGameMgr::s_pInstance->GetMyPlayerNo() == PLAYER_NO_HOST && player < PLAYER_NO_MAX) {
        return nextStages[player];
    }
    return stage::NAME_NONE;
}

// 0x0075FA98 (name is ours)
u8 netgame::StageMgr::Transitions::GetSlot(PlayerNo player) const
{
    if (NetGameMgr::s_pInstance->GetMyPlayerNo() == PLAYER_NO_HOST && player < PLAYER_NO_MAX) {
        return slots[player];
    }
    return SLOT_NONE;
}
} // namespace netgame
