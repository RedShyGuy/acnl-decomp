#pragma once

#include "decomp.h"
#include "netgame/dConfirmMachine.h"
#include "netgame/dMachineBitTable.h"
#include "netgame/netgame_Api.h"
#include "stage/stage_Api.h"

namespace netgame {
// Which stage each player is in and who controls it (names after libgarden: StageMgr, PlayerStages,
// PlayerStageUpdatePacket, SetStage, UpdateStageControl, GetTransitioningPlayer, m_State, m_Stages; the others are ours)
class StageMgr
{
public:
    // a change of the stage of a player as packet
    struct PlayerStageUpdatePacket
    {
        void Set(PlayerNo player, bool updateStage, stage::Name stage, bool updateControl, bool control,
                 PlayerNo receiver = PLAYER_NO_INVALID); // 0x00626348 | libgarden

        u16 value; // 0x0
    };

    struct PlayerStages
    {
        PlayerStages(); // 0x00625468
        void SetStage(PlayerNo player, stage::Name stage); // 0x0062542C
        void SetControl(PlayerNo player, bool control); // 0x00625440
        stage::Name GetStage(PlayerNo player) const; // 0x0075F84C | libgarden
        u8 GetPlayerCount(stage::Name stage) const; // 0x0075F874 | libgarden
        bool HasControl(PlayerNo player) const; // 0x0075F85C
        // the player in the stage that has the control
        PlayerNo FindController(stage::Name stage) const; // 0x0075F8DC
        PlayerNo FindPlayerWithoutControl(stage::Name stage) const; // 0x0075F938

        stage::Name stages[PLAYER_NO_MAX]; // 0x0
        u8 ctrl;                           // 0x4
    };
    ASSERT_SIZE(PlayerStages, 5);

    // the host: the next stages of the players and their place in the line of transitions (0 = now)
    struct Transitions
    {
        DECOMP_NOINLINE Transitions(); // 0x00626754
        DECOMP_NOINLINE void Reset(); // 0x00626718
        DECOMP_NOINLINE void Clear(PlayerNo player); // 0x006266F8
        DECOMP_NOINLINE void SetNextStage(PlayerNo player, stage::Name stage); // 0x0062667C
        DECOMP_NOINLINE void SetSlot(PlayerNo player, u8 slot); // 0x006266B8
        DECOMP_NOINLINE stage::Name GetNextStage(PlayerNo player) const; // 0x0075FA60
        DECOMP_NOINLINE u8 GetSlot(PlayerNo player) const; // 0x0075FA98

        stage::Name nextStages[PLAYER_NO_MAX]; // 0x0
        u8 slots[PLAYER_NO_MAX];               // 0x4
    };
    ASSERT_SIZE(Transitions, 8);

    // the guests: the transition that the host announced
    struct CurrentTransition
    {
        DECOMP_NOINLINE CurrentTransition(); // 0x00626668
        DECOMP_NOINLINE void Reset(); // 0x00626628
        DECOMP_NOINLINE void Set(PlayerNo newPlayer, stage::Name newStage); // 0x0062663C
        DECOMP_NOINLINE void Assign(const CurrentTransition& other); // 0x0062665C

        stage::Name stage; // 0x0
        PlayerNo player;   // 0x1
    };
    ASSERT_SIZE(CurrentTransition, 2);

    // the request of a guest to go to a stage, or that it has arrived
    struct TransitionRequest
    {
        u16 isDone : 1;
        u16 stage : 8;
    };
    ASSERT_SIZE(TransitionRequest, 2);

    StageMgr(); // 0x00622BD0
    ~StageMgr(); // 0x00622C30
    void InitState(); // 0x006256BC
    void Reset(); // 0x006227CC
    void Process(); // 0x00621DC8
    void ProcessFrame(); // 0x00621980
    void ProcessEnd(); // 0x00621F9C
    // the part of the end of the frame after the packets
    void ProcessPackets(); // 0x00622320
    void OnArrive(); // 0x00621EA0
    void RemovePlayer(PlayerNo player); // 0x0062215C
    // the player has finished the transition: the next one in the line is announced
    void FinishTransition(PlayerNo player); // 0x006221A8
    // the host puts the transition of the player into the line
    void RequestTransition(PlayerNo player, stage::Name stage); // 0x00622670
    void UpdateStageControl(PlayerNo player, bool control); // 0x0062287C | libgarden
    DECOMP_NOINLINE void SetStage(PlayerNo player, stage::Name stage); // 0x006229E4 | libgarden
    void SetStages(const MachineBitTable& players, stage::Name stage); // 0x00622A94
    void AddPlayer(PlayerNo player, stage::Name stage); // 0x00621F78
    void ClearCurrentTransition(); // 0x00626620
    void SetCurrentTransition(const CurrentTransition& transition); // 0x00626654
    PlayerNo GetTransitioningPlayer() const; // 0x0075F318 | libgarden
    // whether all players are in the stage
    bool AreAllPlayersIn(stage::Name stage) const; // 0x0075F378
    stage::Name GetNextStage() const; // 0x0075F3A8
    bool AreAllPlayersConfirmed() const; // 0x0075F42C
    bool FUN_0075f464() const; // 0x0075F464 | libgarden

    u8 m_State;                 // 0x00
    PlayerStages m_Stages;      // 0x01
    Transitions m_Transitions;  // 0x06
    CurrentTransition m_CurrentTransition; // 0x0E
    ConfirmMachine m_ArriveConfirm; // 0x10
    ConfirmMachine m_LeaveConfirm;  // 0x18
    PlayerNo unk_0x20;
    MachineBitTable m_Players;  // 0x21
    u8 unk_0x23;
};
ASSERT_SIZE(StageMgr, 0x24);
} // namespace netgame
