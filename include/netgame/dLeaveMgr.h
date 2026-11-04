#pragma once

#include "decomp.h"
#include "netgame/dConfirmMachine.h"
#include "netgame/dSaveBlock.h"
#include "netgame/netgame_Api.h"

namespace netgame {
// A player leaves the village (names after libgarden: LeaveMgr, StartLeave, m_State; the others are ours)
class LeaveMgr
{
public:
    LeaveMgr(); // 0x0062468C
    ~LeaveMgr(); // 0x00624700
    void Reset(); // 0x006244D4
    void Process(); // 0x00623C6C
    bool ProcessLeave(); // 0x006240A4
    void ProcessEnd(); // 0x0062413C
    void RemovePlayer(PlayerNo player); // 0x0062427C
    // the leaving player is out of the session
    void OnPlayerLeft(); // 0x006242C4
    void StartLeave(bool isLocked); // 0x00624544 | libgarden

    // the steps of a leaving player of the island (the argument is the leaving player; the calls behind the first
    // and the others are gone in the original)
    bool CheckLeaveA(PlayerNo player); // 0x00624330
    bool CheckLeaveB(PlayerNo player); // 0x0062436C
    void NotifyLeaveFinished(PlayerNo player); // 0x006243A8
    bool CheckLeaveC(PlayerNo player); // 0x006243E4
    void NotifySessionEnd(PlayerNo player); // 0x00624420
    void NotifyEventA(PlayerNo player); // 0x0062445C
    void NotifyEventB(PlayerNo player); // 0x00624498

    u8 m_State;                // 0x0000
    PlayerNo m_LeavingPlayer;  // 0x0001
    u8 unk_0x2[2];
    ConfirmMachine m_Confirm;  // 0x0004
    ConfirmMachine m_GuestConfirm; // 0x000C
    ConfirmMachine m_HostConfirm;  // 0x0014
    SaveBlock m_SaveBlock;     // 0x001C
    bool unk_0x253C;
    bool unk_0x253D;
    bool unk_0x253E;
    bool unk_0x253F;
};
ASSERT_SIZE(LeaveMgr, 0x2540);
} // namespace netgame
