#pragma once

#include "decomp.h"
#include "netgame/dConfirmMachine.h"
#include "netgame/dMachineBitTable.h"
#include "netgame/dSaveBlock.h"
#include "netgame/netgame_Api.h"

namespace sead {
class Heap;
} // namespace sead

namespace netgame {
// The trips to the island and the tours (names after libgarden)
// the frames of the telop in front of a tour
extern const u32 s_TourTelopFrames;

class IslandMgr
{
public:
    enum TourDeskSelection : u8
    {
        TOUR_DESK_SELECTION_TOUR,
        TOUR_DESK_SELECTION_LEAVE_ISLAND,
        TOUR_DESK_SELECTION_NONE,
    };
    enum LeaveRandomMatchMode : u8
    {
        LEAVE_RANDOM_MATCH_MODE_RETIRE,  // we are leaving
        LEAVE_RANDOM_MATCH_MODE_SEE_OFF, // someone else is leaving
    };

    IslandMgr(); // 0x0061FF98
    ~IslandMgr(); // 0x006200B8
    void Reset(); // 0x0061FED0
    void AllocBuffers(sead::Heap* heap); // 0x0061FECC
    void Process(); // 0x0061B388
    bool ProcessLeave(); // 0x0061E4B8
    void ProcessEnd(); // 0x0061E5BC
    void RemovePlayer(PlayerNo player); // 0x0061E84C
    void OnStationJoined(); // 0x0061F938
    void AddPrincipal(PlayerNo player, const u32* principalId, bool countsAsEntered); // 0x0061F94C
    void ConfirmTourGuest(); // 0x0061E360
    bool StartFindRandomMatch(); // 0x0061E3B0 | libgarden
    bool EndSeaDepartureDemo(bool stageChanged); // 0x0061E3E8 | libgarden
    static void ApplyTourData(); // 0x0061E494
    void StartTourDeskArrivalAfterTour(); // 0x0061E568 | libgarden
    void StartTelopTimer(); // 0x0061E5A8
    bool DepartForIsland(); // 0x0061E8BC | libgarden
    bool RetireFromIsland(); // 0x0061E954 | libgarden
    bool EndSeaReturnDemo(); // 0x0061E9C8
    void UpdatePlayerTourSplit(u8 depart, u8 stay); // 0x0061EA30 | libgarden
    DECOMP_NOINLINE void LeaveRandomMatch(LeaveRandomMatchMode mode); // 0x0061EBF4 | libgarden
    void OnPlayerGone(); // 0x0061EF60
    void ConfirmLeaveSelf(); // 0x0061F018 | libgarden
    void StartTourTransition(); // 0x0061F04C
    bool RequestTourData(PlayerNo player); // 0x0061F168
    static void StartIslandReturn(); // 0x0061F250
    static void StartIslandReturn(bool isLeaving); // 0x0061F258
    void StartTourRandomMatch(); // 0x0061F760 | libgarden
    bool ContinueLeaving(); // 0x0061F800
    static void TransitionToTourDesk(); // 0x0061F818
    bool CancelTrip(); // 0x0061F8E8 | libgarden
    bool RequestRandomMatchSeeOff(); // 0x0061FD78 | libgarden
    bool ChooseTour(TourName tour); // 0x0061FDC0 | libgarden
    bool ChooseLeaveIsland(); // 0x0061FE4C
    s32 GetRandomMatchingResult() const; // 0x0075F09C | libgarden
    bool CanDoTrip() const; // 0x0075F0DC
    bool IsDepartingInRandomMatch() const; // 0x0075F104
    bool IsRetiringRandomMatch(); // 0x0075F140 | libgarden
    bool CanRetireRandomMatch(); // 0x0075F1C4 | libgarden
    s32 GetRetireLockResult(); // 0x0075F1FC

    bool IsSelfDepartingForTour() const { return m_IsSelfDepartingForTour; }

private:
    void StoreStayingPrincipals();
    s32 FindStayingPrincipal(u32 principalId) const;

public:
    u8 m_State;                    // 0x000
    TourDeskSelection m_ChosenTrip; // 0x001
    u8 m_TourName;                 // 0x002
    PlayerNo m_LeavingPlayer;      // 0x003
    PlayerNo m_TripRequestPlayer;  // 0x004
    MachineBitTable m_PlayersThatDepartForTour;      // 0x005
    MachineBitTable m_PlayersThatDoNotDepartForTour; // 0x007
    u8 unk_0x9[3];
    ConfirmMachine m_Confirms[7];  // 0x00C
    u8 m_LeaveKind;                // 0x044
    u8 unk_0x45[3];
    u32 m_RandomMatchParam;        // 0x048
    s32 m_Timer;                   // 0x04C
    u32 m_RetryTimer;              // 0x050
    u32 m_Timestamp;               // 0x054
    u32 m_RetryCount;              // 0x058
    u32 m_NewHostPrincipalId;      // 0x05C
    u32 m_StayingPrincipals[4];    // 0x060
    MachineBitTable m_StayingPlayers; // 0x070
    NpcInfo m_NpcInfos[4];         // 0x072
    u16 unk_0x13A;
    u8 m_StayingPlayerNum;         // 0x13C
    bool m_IsNewHost;              // 0x13D
    bool unk_0x13E;
    bool m_IsRejoining;            // 0x13F
    bool unk_0x140;
    bool unk_0x141;
    bool unk_0x142;
    bool unk_0x143;
    bool m_IsSelfDepartingForTour;               // 0x144 (the own player goes on the tour)
    bool unk_0x145;
    bool m_ShowingTelop;           // 0x146
    bool unk_0x147;
    bool m_ResetsClock;            // 0x148
    u8 unk_0x149[3];
};
ASSERT_SIZE(IslandMgr, 0x14C);
} // namespace netgame
