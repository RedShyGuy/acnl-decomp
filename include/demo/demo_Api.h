#pragma once

#include "decomp.h"

class DemoActor;

// The orders of the demos (namespace, OrderID and the libgarden functions after libgarden, all other names ours): the game
// queues orders by priority (OrderList); one order at a time is the current one and goes through its states with the
// functions of its kind (s_OrderFunctions). In a network game the guests ask the host to lock the actors they use
// (packet 57).
namespace demo {
class Order;

// the kinds of orders (after libgarden)
enum OrderID : u8
{
    ORDER_0 = 0,
    INTERACT = 1,
    DEFAULT_TALK = 2,
    ORDER_3 = 3,
    WARP = 4,
    ORDER_5 = 5,
    CREDITS = 6,
    PLAYER_THINK = 7,
    ISLAND_FORCE_LEAVE_THINK = 8,
    ORDER_9 = 9,
    SAVE = 10,
    ORDER_11 = 11,
    NETGAME_LOCK = 12,
    ORDER_13 = 13,
    REVOKED_PLAYER_CONTROL = 14, // CheckOrder: any order; a free order has this id
    ORDER_15 = 15,
    ORDER_ID_NUM = 14,
};

// the states of an order
enum OrderState : u8
{
    ORDER_STATE_FREE = 0,
    ORDER_STATE_QUEUED = 1,
    ORDER_STATE_STARTED = 2,  // OrderFunctions::update until it is true
    ORDER_STATE_RUNNING = 3,  // OrderFunctions::end until it is true
    ORDER_STATE_ENDING = 4,   // OrderFunctions::finish until it is true
};

// the functions of a kind of order
struct OrderFunctions
{
    bool (*start)(Order* order);  // the queued order may start
    bool (*update)(Order* order);
    bool (*end)(Order* order);
    bool (*finish)(Order* order);
};
extern const OrderFunctions s_OrderFunctions[ORDER_ID_NUM];

// the state of the orders
struct State
{
    enum Flag : u8
    {
        FLAG_0 = 1 << 0,
        FLAG_1 = 1 << 1,
        FLAG_2 = 1 << 2, // MakeOrder9 does nothing
    };

    u8 flags;             // 0x00
    u32 actorId;          // 0x04 the actor of the talk (talk partner if partnerId is 0)
    u32 partnerId;        // 0x08
    Order* pCurrent;      // 0x0C
    s32 counter;          // 0x10 counted down each frame
    u32 pendingActorId;   // 0x14 a talk that starts when the current order is finished
};
ASSERT_SIZE(State, 0x18);
extern State s_State;
// the answer of the host to a request: 0 none yet, 1 accepted, 2 refused
extern u8 s_NetResult;

// the current order is of the kind (REVOKED_PLAYER_CONTROL: any); checkState: only while it runs and its result is not 2
DECOMP_NOINLINE bool CheckOrder(OrderID id, bool checkState = false); // 0x00316DA4 | libgarden

void Initialize(bool all); // 0x0052C508
// the first order of the game: talk to the actor of Func241598 or order 0
void StartFirstOrder(); // 0x0052CB00
OrderID GetCurrentOrderId(); // 0x0052C4EC
bool ConsumeFlag1(); // 0x0052C550
bool IsFlag1(); // 0x0052D308
void SetFlag2(); // 0x0052C63C
void ClearFlag2(); // 0x0052C96C
void ClearFlag0(); // 0x0052D49C
bool IsCounterRunning(); // 0x0052CCF4
bool SetPendingActor(u32 actorId); // 0x0052D3A8
void SetPartner(DemoActor* actor); // 0x0052CED8
DemoActor* GetTalkActor(); // 0x0052C914
DemoActor* GetActor(); // 0x0052CF24
bool IsTalking(); // 0x0052E17C
bool IsOrder5Running(); // 0x0052C600
bool IsCreditsRunning(); // 0x0052C8A8
bool IsCreditsRunningEntry(); // 0x0052C8A4
u8 GetNetResult(); // 0x0052D4B4

// the orders (true: queued)
bool MakeOrder13(); // 0x0052C654
bool MakeOrderPlayerThink(u8 param); // 0x0052C6C8 | libgarden
bool MakeOrderWarp(); // 0x0052C750
bool MakeOrderSave(); // 0x0052C7D8
bool MakeOrderInteract(DemoActor* actor); // 0x0052C84C
bool MakeOrder11(); // 0x0052CA8C
bool MakeOrderDefaultTalk(DemoActor* actor); // 0x0052CB84
bool MakeOrderDefaultTalkEntry(DemoActor* actor); // 0x0052CB80
bool MakeOrder9(); // 0x0052CC60
bool MakeOrderNetgameLock(); // 0x0052CD74 | libgarden
bool MakeOrderDefaultTalk3(DemoActor* actor); // 0x0052CF70
bool MakeOrderIslandForceLeaveThink(); // 0x0052D020 | libgarden
bool MakeOrderDefaultTalk1(DemoActor* actor); // 0x0052D0FC
bool MakeOrderDefaultTalk2(DemoActor* actor); // 0x0052D178
bool MakeOrder5(bool force); // 0x0052D1F4
bool MakeOrderDefaultTalk4(DemoActor* actor); // 0x0052D28C
bool MakeOrderCredits(); // 0x0052D320
// the end of the current order of the kind (true: it is that kind)
bool EndOrder5(); // 0x0052C8E4
bool RevokeNetgameLockOrder(); // 0x0052CC00 | libgarden
bool EndCredits(); // 0x0052CC30
bool EndOrder13(); // 0x0052CD10
bool EndPlayerThink(); // 0x0052CD44
bool EndSave(); // 0x0052CE08
bool EndOrder11(); // 0x0052CFEC
bool EndOrder0Or3(); // 0x0052D0A8
bool EndIslandForceLeaveThink(u8 param); // 0x0052D3C8
// the actor ends the talk with it (in some stages always)
bool EndTalk(DemoActor* actor); // 0x0052CE3C
bool EndTalk3(DemoActor* actor); // 0x0052D404

// the steps of the orders each frame (demo::Proc::Calc)
void UpdateRunning(); // 0x0052E0F8
void UpdateEnding(); // 0x0052DFFC
void StartNextOrder(); // 0x0052C588
void UpdateStarted(); // 0x0052C498
void UpdateList(); // 0x0052E14C

// the locks of the actors in a network game
// the answers of the requests: 2 -> 1, 4 -> 3, 6 -> 5, 8 -> 7, else 1
u8 GetAnswer(u8 request); // 0x0052CEF0
void ResetLocks(); // 0x0052D4C4
// 1: locked, 2: refused, 0: asked the host
u8 RequestLock(u32 actorId); // 0x0052D564
void Unlock(); // 0x0052D788
bool IsGuestOutsideStage(); // 0x0052D764
void SendRequest(u32 actorId, u8 kind, u8 type); // 0x0052D7FC
bool AddRequest(u32 actorId, u32 value); // 0x0052D86C
u32 FindHolder(u32 actorId); // 0x0052D8CC
void RemoveHolder(u32 actorId); // 0x0052D920
bool Lock(u32 player, u32 actorId); // 0x0052DACC
void ProcessAnswerRequest(u32 player, const u8* packet); // 0x0052D61C
void ProcessPacket(u32 player, const u8* packet); // 0x0052D97C
void ProcessNet(); // 0x0052DB6C
} // namespace demo
