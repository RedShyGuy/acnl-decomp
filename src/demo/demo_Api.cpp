#include "demo/demo_Api.h"
#include "Ac/dAcPlayer.h"
#include "Bs/dBsDemoCancelMgr.h"
#include "Other/dDemoActor.h"
#include "demo/dOrderList.h"
#include "demo/demo_Extern.h"
#include "netgame/netgame_Api.h"
#include "stage/stage_Api.h"

#include <cstring>

namespace demo {
// 0x009526B0
State s_State;
// 0x009526C8
u8 s_NetResult;

namespace {
// the packet of the requests between the players
const u8 PACKET_DEMO = 57;
// the kinds of the packet (the first byte)
enum PacketKind : u8
{
    PACKET_LOCK = 0,          // lock the actor (guest -> host)
    PACKET_UNLOCK = 1,
    PACKET_REQUEST = 2,       // ask the actor (SendRequest)
    PACKET_ANSWER = 3,        // the answer to the actor
    PACKET_ACCEPTED = 4,
    PACKET_REFUSED = 5,
};
} // namespace

// an actor and a value (names are ours)
struct Entry
{
    u32 actorId;
    u32 value;
};
// the actors held by the players of the host and the requests to them
struct HolderTable
{
    DECOMP_NOINLINE void ProcessRequests(); 

    Entry holders[6];  // 0x00
    Entry requests[6]; // 0x30
};
// the packets that arrived for actors that were not there yet
struct PacketBuffer
{
    struct Packet
    {
        u8 data[6];
        u8 isSet;
    };

    DECOMP_NOINLINE void Process(); 

    Packet packets[netgame::PLAYER_NO_MAX];
};

// the actor each player has locked
// 0x009AC588
u32 s_LockedIds[netgame::PLAYER_NO_MAX];
namespace {
// 0x009AC598
HolderTable s_HolderTable;
// 0x009AC5F8
PacketBuffer s_PacketBuffer;

inline bool IsHostSide()
{
    return !netgame::IsMultiplePlayer() || netgame::IsHost();
}

inline bool IsGuestSide()
{
    return netgame::IsMultiplePlayer() && !netgame::IsHost();
}

inline u32 ReadId(const u8* data)
{
    u32 id;
    std::memcpy(&id, data, sizeof(id));
    return id;
}

inline void SendAnswer(u32 player, u8 kind)
{
    u8 data = kind;
    netgame::EnqueuePacket(PACKET_DEMO, static_cast<netgame::PlayerNo>(player), &data, 1);
}

// a new order in the list
inline bool MakeOrder(OrderID id, u8 priority, u8 param, u32 partnerId, u32 actorId)
{
    Order* order = s_OrderList.Alloc();
    if (order == NULL) {
        return false;
    }
    order->m_PartnerId = partnerId;
    order->m_ActorId = actorId;
    order->m_State = ORDER_STATE_QUEUED;
    order->m_Id = id;
    order->m_Priority = priority;
    order->m_Param = param;
    s_OrderList.InsertByPriority(order);
    return true;
}

// an order for the player
inline bool MakePlayerOrder(OrderID id, u8 priority, u8 param)
{
    AcPlayer* player = PlayerGetActorEntry(netgame::PLAYER_NO_MY_NO, false);
    if (player == NULL) {
        return false;
    }
    return MakeOrder(id, priority, param, 0, static_cast<DemoActor*>(player)->GetId());
}

// an order of the player with another actor
inline bool MakeTalkOrder(OrderID id, u8 priority, u8 param, DemoActor* actor)
{
    AcPlayer* player = PlayerGetActorEntry(netgame::PLAYER_NO_MY_NO, false);
    if (player == NULL) {
        return false;
    }
    return MakeOrder(id, priority, param, static_cast<DemoActor*>(player)->GetId(), actor->GetId());
}

// the end of the current order of the kind
inline bool EndOrder(OrderID id)
{
    if (!CheckOrder(id)) {
        return false;
    }
    s_State.pCurrent->m_Result = 1;
    return true;
}
} // namespace

// 0x00316DA4 (name after libgarden)
bool CheckOrder(OrderID id, bool checkState)
{
    Order* order = s_State.pCurrent;
    if (order == NULL) {
        return false;
    }
    if (checkState && (order->m_State != ORDER_STATE_RUNNING || order->m_Result == 2)) {
        return false;
    }
    if (id == REVOKED_PLAYER_CONTROL || order->m_Id == id) {
        return true;
    }
    return false;
}

// 0x0052C498 (name is ours)
void UpdateStarted()
{
    Order* order = s_State.pCurrent;
    if (order != NULL && order->m_State == ORDER_STATE_STARTED && s_OrderFunctions[order->m_Id].update(order)) {
        s_State.pCurrent->m_State = ORDER_STATE_RUNNING;
    }
}

// 0x0052C4EC (name is ours)
OrderID GetCurrentOrderId()
{
    if (s_State.pCurrent == NULL) {
        return REVOKED_PLAYER_CONTROL;
    }
    return static_cast<OrderID>(s_State.pCurrent->m_Id);
}

// 0x0052C508 (name is ours)
void Initialize(bool all)
{
    s_OrderList.Reset();
    s_State.pCurrent = NULL;
    s_State.actorId = 0;
    s_State.partnerId = 0;
    s_State.counter = 0;
    if (all) {
        s_State.flags = 0;
        s_State.pendingActorId = 0;
    }
}

// 0x0052C550 (name is ours)
bool ConsumeFlag1()
{
    if ((s_State.flags & State::FLAG_1) == 0) {
        return false;
    }
    s_State.flags |= State::FLAG_0;
    s_State.flags &= ~State::FLAG_1;
    return true;
}

// 0x0052C588 (name is ours)
void StartNextOrder()
{
    if (s_State.pCurrent != NULL) {
        return;
    }
    for (Order* order = static_cast<Order*>(s_OrderList.GetFirst()); order != NULL;
         order = static_cast<Order*>(order->m_pNext)) {
        if (s_OrderFunctions[order->m_Id].start(order)) {
            s_OrderList.Erase(order);
            s_State.pCurrent = order;
            order->m_State = ORDER_STATE_STARTED;
            return;
        }
    }
}

// 0x0052C600 (name is ours)
bool IsOrder5Running()
{
    return CheckOrder(ORDER_5) && s_State.pCurrent->m_Result != 1;
}

// 0x0052C63C (name is ours)
void SetFlag2()
{
    s_State.flags |= State::FLAG_2;
}

// 0x0052C654 (name is ours)
bool MakeOrder13()
{
    return MakePlayerOrder(ORDER_13, 4, 0);
}

// 0x0052C6C8 (name after libgarden)
bool MakeOrderPlayerThink(u8 param)
{
    if (CheckOrder(REVOKED_PLAYER_CONTROL)) {
        return false;
    }
    return MakePlayerOrder(PLAYER_THINK, 4, param);
}

// 0x0052C750 (name is ours)
bool MakeOrderWarp()
{
    if (CheckOrder(REVOKED_PLAYER_CONTROL)) {
        return false;
    }
    return MakePlayerOrder(WARP, 0, 0);
}

// 0x0052C7D8 (name is ours)
bool MakeOrderSave()
{
    return MakePlayerOrder(SAVE, 4, 0);
}

// 0x0052C84C (name is ours)
bool MakeOrderInteract(DemoActor* actor)
{
    return MakeOrder(INTERACT, 3, 0, actor->GetId(), 0);
}

// 0x0052C8A8 (name is ours)
bool IsCreditsRunning()
{
    return CheckOrder(CREDITS) && s_State.pCurrent->m_Result != 1;
}

// 0x0052C8A4 (name is ours)
bool IsCreditsRunningEntry()
{
    // (a removed call in front of IsCreditsRunning, which follows)
    return IsCreditsRunning();
}

// 0x0052C8E4 (name is ours)
bool EndOrder5()
{
    return EndOrder(ORDER_5);
}

// 0x0052C914 (name is ours)
DemoActor* GetTalkActor()
{
    if (!CheckOrder(INTERACT) && !CheckOrder(DEFAULT_TALK)) {
        return NULL;
    }
    if (s_State.partnerId != 0) {
        return FindDemoActor(s_State.partnerId);
    }
    return FindDemoActor(s_State.actorId);
}

// 0x0052C96C (name is ours)
void ClearFlag2()
{
    s_State.flags &= ~State::FLAG_2;
}

// 0x0052CA8C (name is ours)
bool MakeOrder11()
{
    return MakePlayerOrder(ORDER_11, 4, 0);
}

// 0x0052CB00 (name is ours)
void StartFirstOrder()
{
    ResetLocks();
    s_OrderList.Reset();
    s_State.pCurrent = NULL;
    s_State.actorId = 0;
    s_State.partnerId = 0;
    s_State.counter = 0;
    Order* order = s_OrderList.Alloc();
    order->m_ActorId = 0;
    order->m_State = ORDER_STATE_RUNNING;
    order->m_Result = 4;
    order->m_Priority = 2;
    order->m_PartnerId = Func241598();
    if (order->m_PartnerId != 0) {
        order->m_Id = ORDER_3;
        order->m_Result = 8;
    } else {
        order->m_Id = ORDER_0;
        order->m_Result = 5;
    }
    s_State.pCurrent = order;
}

// 0x0052CB80 (name is ours)
bool MakeOrderDefaultTalkEntry(DemoActor* actor)
{
    // (a removed call in front of MakeOrderDefaultTalk, which follows)
    return MakeOrderDefaultTalk(actor);
}

// 0x0052CB84 (name is ours)
bool MakeOrderDefaultTalk(DemoActor* actor)
{
    return MakeTalkOrder(DEFAULT_TALK, 3, 0, actor);
}

// 0x0052CC00 (name after libgarden)
bool RevokeNetgameLockOrder()
{
    return EndOrder(NETGAME_LOCK);
}

// 0x0052CC30 (name is ours)
bool EndCredits()
{
    return EndOrder(CREDITS);
}

// 0x0052CC60 (name is ours)
bool MakeOrder9()
{
    if ((s_State.flags & State::FLAG_2) != 0) {
        return false;
    }
    Func5C3998();
    return MakePlayerOrder(ORDER_9, 5, 0);
}

// 0x0052CCF4 (name is ours)
bool IsCounterRunning()
{
    return s_State.counter > 0;
}

// 0x0052CD10 (name is ours)
bool EndOrder13()
{
    return EndOrder(ORDER_13);
}

// 0x0052CD44 (name is ours)
bool EndPlayerThink()
{
    return EndOrder(PLAYER_THINK);
}

// 0x0052CD74 (name after libgarden)
bool MakeOrderNetgameLock()
{
    if (PlayerGetActorEntry(netgame::PLAYER_NO_MY_NO, false) == NULL) {
        return false;
    }
    if (CheckOrder(REVOKED_PLAYER_CONTROL)) {
        return false;
    }
    if (!Func5C32D8()) {
        return false;
    }
    Order* order = s_OrderList.Alloc();
    if (order == NULL) {
        return false;
    }
    // (the current order at once, not queued)
    order->m_PartnerId = 0;
    order->m_ActorId = 0;
    order->m_Id = NETGAME_LOCK;
    order->m_Priority = 0;
    order->m_Param = 0;
    order->m_State = ORDER_STATE_RUNNING;
    order->m_Result = 4;
    s_State.pCurrent = order;
    Func5C2CC0();
    return true;
}

// 0x0052CE08 (name is ours)
bool EndSave()
{
    return EndOrder(SAVE);
}

// 0x0052CE3C (name is ours)
bool EndTalk(DemoActor* actor)
{
    if (stage::IsKind(stage::KIND_SCREEN, stage::GetCurrent()) ||
        stage::IsKind(stage::KIND_MINI_GAME, stage::GetCurrent())) {
        return true;
    }
    if (!CheckOrder(INTERACT) && !CheckOrder(DEFAULT_TALK)) {
        return false;
    }
    Order* order = s_State.pCurrent;
    if (actor->GetId() != order->m_ActorId) {
        return false;
    }
    order->m_Result = 1;
    return true;
}

// 0x0052CED8 (name is ours)
void SetPartner(DemoActor* actor)
{
    s_State.partnerId = actor->GetId();
}

// 0x0052CEF0 (name is ours)
u8 GetAnswer(u8 request)
{
    if (request == 2) {
        return 1;
    }
    if (request == 4) {
        return 3;
    }
    if (request == 6) {
        return 5;
    }
    if (request == 8) {
        return 7;
    }
    return 1;
}

// 0x0052CF24 (name is ours)
DemoActor* GetActor()
{
    if (!CheckOrder(INTERACT) && !CheckOrder(DEFAULT_TALK)) {
        return NULL;
    }
    if (s_State.actorId == 0) {
        return NULL;
    }
    return FindDemoActor(s_State.actorId);
}

// 0x0052CF70 (name is ours)
bool MakeOrderDefaultTalk3(DemoActor* actor)
{
    return MakeTalkOrder(DEFAULT_TALK, 3, 3, actor);
}

// 0x0052CFEC (name is ours)
bool EndOrder11()
{
    return EndOrder(ORDER_11);
}

// 0x0052D020 (name after libgarden)
bool MakeOrderIslandForceLeaveThink()
{
    if (CheckOrder(REVOKED_PLAYER_CONTROL)) {
        return false;
    }
    return MakePlayerOrder(ISLAND_FORCE_LEAVE_THINK, 4, 0);
}

// 0x0052D0A8 (name is ours)
bool EndOrder0Or3()
{
    if (!CheckOrder(ORDER_0) && !CheckOrder(ORDER_3)) {
        return false;
    }
    Order* order = s_State.pCurrent;
    if (order->m_Result != 5) {
        return false;
    }
    order->m_Result = 1;
    return true;
}

// 0x0052D0FC (name is ours)
bool MakeOrderDefaultTalk1(DemoActor* actor)
{
    return MakeTalkOrder(DEFAULT_TALK, 3, 1, actor);
}

// 0x0052D178 (name is ours)
bool MakeOrderDefaultTalk2(DemoActor* actor)
{
    return MakeTalkOrder(DEFAULT_TALK, 3, 2, actor);
}

// 0x0052D1F4 (name is ours)
bool MakeOrder5(bool force)
{
    if (!force && CheckOrder(REVOKED_PLAYER_CONTROL)) {
        return false;
    }
    return MakePlayerOrder(ORDER_5, 1, 0);
}

// 0x0052D28C (name is ours)
bool MakeOrderDefaultTalk4(DemoActor* actor)
{
    return MakeTalkOrder(DEFAULT_TALK, 3, 4, actor);
}

// 0x0052D308 (name is ours)
bool IsFlag1()
{
    return (s_State.flags & State::FLAG_1) != 0;
}

// 0x0052D320 (name is ours)
bool MakeOrderCredits()
{
    if (CheckOrder(REVOKED_PLAYER_CONTROL)) {
        return false;
    }
    return MakePlayerOrder(CREDITS, 1, 0);
}

// 0x0052D3A8 (name is ours)
bool SetPendingActor(u32 actorId)
{
    if (s_State.pendingActorId != 0) {
        return false;
    }
    s_State.pendingActorId = actorId;
    return true;
}

// 0x0052D3C8 (name is ours)
bool EndIslandForceLeaveThink(u8 param)
{
    if (!CheckOrder(ISLAND_FORCE_LEAVE_THINK)) {
        return false;
    }
    Order* order = s_State.pCurrent;
    order->m_Result = 1;
    order->m_Param = param;
    return true;
}

// 0x0052D404 (name is ours)
bool EndTalk3(DemoActor* actor)
{
    if (stage::IsKind(stage::KIND_SCREEN, stage::GetCurrent())) {
        return true;
    }
    if (!CheckOrder(INTERACT) && !CheckOrder(DEFAULT_TALK)) {
        return false;
    }
    if (actor->GetId() != s_State.pCurrent->m_ActorId || netgame::IsMultiPlayer()) {
        return false;
    }
    s_State.pCurrent->m_Result = 3;
    return true;
}

// 0x0052D49C (name is ours)
void ClearFlag0()
{
    s_State.flags &= ~State::FLAG_0;
}

// 0x0052D4B4 (name is ours)
u8 GetNetResult()
{
    return s_NetResult;
}

// 0x0052D4C4 (name is ours)
void ResetLocks()
{
    for (u32 i = 0; i < 6; i++) {
        s_HolderTable.requests[i].actorId = 0;
        s_HolderTable.requests[i].value = 0;
    }
    if (IsHostSide()) {
        s_LockedIds[0] = 0;
    } else if (IsGuestSide()) {
        SendAnswer(0, PACKET_UNLOCK);
    }
}

// 0x0052D564 (name is ours)
u8 RequestLock(u32 actorId)
{
    if (IsHostSide()) {
        return Lock(0, actorId) ? 1 : 2;
    }
    if (IsGuestSide()) {
        s_NetResult = 0;
        u8 data[5];
        data[0] = PACKET_LOCK;
        std::memcpy(&data[1], &actorId, sizeof(actorId));
        netgame::EnqueuePacket(PACKET_DEMO, static_cast<netgame::PlayerNo>(0), data, sizeof(data));
    }
    return 0;
}

// 0x0052D61C (name is ours)
void ProcessAnswerRequest(u32 player, const u8* packet)
{
    u32 actorId = ReadId(&packet[1]);
    u8 kind = packet[5];
    AcPlayer* playerActor = PlayerGetActorEntry(static_cast<netgame::PlayerNo>(player), false);
    DemoActor* actor = FindDemoActor(actorId);
    if (playerActor != NULL && actor != NULL) {
        bool isAccepted;
        if (kind == 2) {
            isAccepted = actor->vf_0x48(static_cast<DemoActor*>(playerActor));
        } else if (kind == 4) {
            isAccepted = actor->vf_0x70(static_cast<DemoActor*>(playerActor));
        } else {
            isAccepted = false;
        }
        if (isAccepted) {
            u8 answer = GetAnswer(kind);
            if (netgame::GetMyPlayerNo() == player) {
                s_NetResult = 1;
            } else {
                actor->vf_0x50(answer, static_cast<netgame::PlayerNo>(player));
                SendAnswer(player, PACKET_ACCEPTED);
            }
            return;
        }
    }
    if (netgame::GetMyPlayerNo() == player) {
        s_NetResult = 2;
        return;
    }
    SendAnswer(player, PACKET_REFUSED);
}

// 0x0052D764 (name is ours)
bool IsGuestOutsideStage()
{
    return netgame::IsMultiplePlayer() && !netgame::IsStageControl();
}

// 0x0052D788 (name is ours)
void Unlock()
{
    if (IsHostSide()) {
        s_LockedIds[0] = 0;
    } else if (IsGuestSide()) {
        SendAnswer(0, PACKET_UNLOCK);
    }
}

// 0x0052D7FC (name is ours)
void SendRequest(u32 actorId, u8 kind, u8 type)
{
    if (netgame::IsMultiplePlayer()) {
        s_NetResult = 0;
        u8 data[6];
        data[0] = type;
        std::memcpy(&data[1], &actorId, sizeof(actorId));
        data[5] = kind;
        netgame::EnqueuePacket(PACKET_DEMO, static_cast<netgame::PlayerNo>(5), data, sizeof(data));
    }
}

// 0x0052D86C (name is ours)
bool AddRequest(u32 actorId, u32 value)
{
    for (u32 i = 0; i < 6; i++) {
        if (s_HolderTable.requests[i].actorId == 0) {
            s_HolderTable.requests[i].actorId = actorId;
            s_HolderTable.requests[i].value = value;
            return true;
        }
    }
    return false;
}

// 0x0052D8CC (name is ours)
u32 FindHolder(u32 actorId)
{
    for (s32 i = 0; i < 6; i++) {
        if (s_HolderTable.holders[i].actorId == actorId) {
            return s_HolderTable.holders[i].value;
        }
    }
    return 0;
}

// 0x0052D920 (name is ours)
void RemoveHolder(u32 actorId)
{
    if (!IsHostSide()) {
        return;
    }
    for (u32 i = 0; i < 6; i++) {
        if (s_HolderTable.holders[i].actorId == actorId) {
            s_HolderTable.holders[i].actorId = 0;
            s_HolderTable.holders[i].value = 0;
            return;
        }
    }
}

namespace {
// (inline in PacketBuffer::Process)
DECOMP_ALWAYS_INLINE void ProcessPacketImpl(u32 player, const u8* packet)
{
    switch (packet[0]) {
    case PACKET_LOCK:
        SendAnswer(player, Lock(player, ReadId(&packet[1])) ? PACKET_ACCEPTED : PACKET_REFUSED);
        break;
    case PACKET_UNLOCK:
        s_LockedIds[player] = 0;
        break;
    case PACKET_REQUEST:
        ProcessAnswerRequest(player, packet);
        break;
    case PACKET_ANSWER: {
        DemoActor* actor = FindDemoActor(ReadId(&packet[1]));
        if (actor != NULL) {
            actor->vf_0x50(packet[5], static_cast<netgame::PlayerNo>(player));
            break;
        }
        PacketBuffer::Packet& stored = s_PacketBuffer.packets[player];
        std::memset(stored.data, 0, sizeof(stored.data));
        stored.isSet = true;
        std::memcpy(stored.data, packet, sizeof(stored.data));
        break;
    }
    case PACKET_ACCEPTED:
        s_NetResult = 1;
        break;
    case PACKET_REFUSED:
        s_NetResult = 2;
        break;
    }
}
} // namespace

// 0x0052D97C (name is ours)
void ProcessPacket(u32 player, const u8* packet)
{
    ProcessPacketImpl(player, packet);
}

// 0x0052DACC (name is ours)
bool Lock(u32 player, u32 actorId)
{
    if (s_LockedIds[player] != 0) {
        return false;
    }
    for (s32 i = 0; i < netgame::PLAYER_NO_MAX; i++) {
        if (s_LockedIds[i] == actorId) {
            return false;
        }
    }
    for (s32 i = 0; i < 6; i++) {
        if (s_HolderTable.holders[i].value == actorId) {
            return false;
        }
    }
    s_LockedIds[player] = actorId;
    return true;
}

// 0x0052DB6C (name is ours)
DECOMP_NOINLINE void ProcessNet()
{
    s_HolderTable.ProcessRequests();
    s_PacketBuffer.Process();
}

// 0x0052DC5C (name is ours)
void HolderTable::ProcessRequests()
{
    for (u32 i = 0; i < 6; i++) {
        Entry& request = requests[i];
        if (request.actorId == 0) {
            continue;
        }
        DemoActor* actor = FindDemoActor(request.actorId);
        if (actor != NULL && actor->vf_0x68(request.value)) {
            u32 value = request.value;
            u32 actorId = request.actorId;
            // 1: held now, 2: refused
            s32 result;
            if (IsGuestSide()) {
                result = 2;
            } else {
                result = 1;
                for (s32 j = 0; j < netgame::PLAYER_NO_MAX; j++) {
                    if (s_LockedIds[j] == value) {
                        result = 2;
                    }
                }
                for (s32 j = 0; j < 6 && result != 2; j++) {
                    if (holders[j].value == value) {
                        result = 2;
                    }
                }
                if (result != 2) {
                    for (u32 j = 0; j < 6; j++) {
                        if (holders[j].actorId == 0) {
                            holders[j].actorId = actorId;
                            holders[j].value = value;
                            break;
                        }
                    }
                }
            }
            actor->vf_0x50(10, netgame::PLAYER_NO_MAX);
            if (result == 1) {
                actor->vf_0x50(11, netgame::PLAYER_NO_MAX);
            } else if (result == 2) {
                actor->vf_0x50(12, netgame::PLAYER_NO_MAX);
            }
        }
        request.actorId = 0;
        request.value = 0;
    }
}

// 0x0052DE08 (name is ours)
void PacketBuffer::Process()
{
    for (u32 i = 0; i < netgame::PLAYER_NO_MAX; i++) {
        Packet& stored = packets[i];
        if (!stored.isSet) {
            continue;
        }
        u8 packet[6];
        std::memcpy(packet, stored.data, sizeof(packet));
        stored.isSet = false;
        std::memset(stored.data, 0, sizeof(stored.data));
        if (netgame::IsStageControl()) {
            ProcessPacketImpl(i, packet);
        } else {
            netgame::EnqueuePacket(PACKET_DEMO, static_cast<netgame::PlayerNo>(5), packet, sizeof(packet));
        }
    }
}

// 0x0052DFFC (name is ours)
void UpdateEnding()
{
    Order* order = s_State.pCurrent;
    if (order == NULL || order->m_State != ORDER_STATE_ENDING || !s_OrderFunctions[order->m_Id].finish(order)) {
        return;
    }
    if (s_State.pendingActorId != 0) {
        Order* talk = s_OrderList.Alloc();
        AcPlayer* player = PlayerGetActorEntry(netgame::PLAYER_NO_MY_NO, false);
        if (player != NULL) {
            talk->m_PartnerId = static_cast<DemoActor*>(player)->GetId();
            talk->m_ActorId = s_State.pendingActorId;
            talk->m_Id = DEFAULT_TALK;
            talk->m_State = ORDER_STATE_QUEUED;
            if (s_OrderFunctions[DEFAULT_TALK].start(talk)) {
                s_State.pCurrent->Reset();
                s_State.pCurrent = talk;
                talk->m_State = ORDER_STATE_STARTED;
                s_State.pendingActorId = 0;
                return;
            }
            talk->Reset();
            return;
        }
    }
    s_State.pCurrent->Reset();
    s_State.pCurrent = NULL;
    if (BsDemoCancelMgr::s_pInstance != NULL) {
        BsDemoCancelMgr::s_pInstance->Reset();
    }
}

// 0x0052E0F8 (name is ours)
void UpdateRunning()
{
    Order* order = s_State.pCurrent;
    if (order != NULL && order->m_State == ORDER_STATE_RUNNING && s_OrderFunctions[order->m_Id].end(order)) {
        s_State.pCurrent->m_State = ORDER_STATE_ENDING;
    }
}

// 0x0052E14C (name is ours)
void UpdateList()
{
    s_OrderList.Clear();
    if (s_State.counter > 0) {
        s_State.counter--;
    }
    ProcessNet();
}

// 0x0052E17C (name is ours)
bool IsTalking()
{
    return CheckOrder(INTERACT) || CheckOrder(DEFAULT_TALK);
}
} // namespace demo
