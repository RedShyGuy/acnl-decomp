#pragma once

#include "decomp.h"
#include "netgame/dMachineBitTable.h"
#include "netgame/netgame_Api.h"

namespace netgame {
// Locks the game of all players for a command (entry, leaving, saving, tours) (names after libgarden; the members
// of the telop and the helpers are ours)
class LockMgr
{
public:
    enum State : u8
    {
        STATE_UNLOCKED,
        STATE_LOCKED,
        STATE_LOCK_REQUESTED,
        STATE_LOCK_PENDING,
    };
    enum Cmd : u8
    {
        CMD_FRIEND_MATCH_ENTRY,
        CMD_FRIEND_MATCH_LEAVE,
        CMD_SAVE_CONTINUE,
        CMD_SAVE_INTERRUPT,
        CMD_TOUR_END,
        CMD_RANDOM_MATCH_ENTRY,
        CMD_RANDOM_MATCH_LEAVE,
        CMD_RANDOM_MATCH_START_TOUR,
        CMD_COUNT,
        CMD_NONE = CMD_COUNT,
    };

    enum Action : u8
    {
        ACTION_NONE,
        ACTION_BEGIN,
        ACTION_WAIT,
        ACTION_FAIL,
        ACTION_ACCEPT,
        ACTION_CANCEL,
        ACTION_WAIT_DELAY,
        ACTION_EXEC_CMD,
    };
    enum RequestIssueState : u8
    {
        REQUEST_ISSUE_STATE_NONE,
        REQUEST_ISSUE_STATE_BEGIN,
        REQUEST_ISSUE_STATE_WAIT,
        REQUEST_ISSUE_STATE_FAIL,
        REQUEST_ISSUE_STATE_SUCCESS,
    };
    enum RequestHandleState : u8
    {
        REQUEST_HANDLE_STATE_NONE,
        REQUEST_HANDLE_STATE_BEGIN,
        REQUEST_HANDLE_STATE_LOCK_PLAYER,
        REQUEST_HANDLE_STATE_BEGIN_STEP,
        REQUEST_HANDLE_STATE_DO_CANCEL,
        REQUEST_HANDLE_STATE_CANCELING,
    };

    LockMgr(); // 0x0061B1F4
    void Reset(); // 0x0061B17C
    // the lock for the command, if nothing else is running
    bool Request(Cmd cmd)
    {
        if (m_State != STATE_UNLOCKED && m_State != STATE_LOCKED) {
            return false;
        }
        m_State = STATE_LOCK_REQUESTED;
        m_NextCmdReq = cmd;
        return true;
    }
    void Process(); // 0x00619EA8 | libgarden
    void ProcessEnd(); // 0x0061AEF4
    bool HandleRequestResult(PlayerNo player, bool success); // 0x0061AFA8 | libgarden
    void SetNextCmd(PlayerNo player, Cmd cmd); // 0x0061B15C
    void SetRequestHandleState(PlayerNo player, RequestHandleState state); // 0x0061B16C
    void RemovePlayer(PlayerNo player); // 0x006255BC
    bool IsWaitingForCmd(Cmd cmd) const; // 0x0075F074

private:
    PlayerNo GetCurrentPlayerNo() const;
    PlayerNo GetNextRequestIssuer() const;
    void ClearRequestHandleStates();
    void ProcessLockRequest(PlayerNo myNo);
    void HandleAction(PlayerNo myNo);
    void ExecCmd(PlayerNo myNo);
    void ProcessRequest();
    void StartHandleNextRequest();
    void ShowTelop();

public:
    Cmd m_SuccessfulCmd;     // 0x00
    PlayerNo m_SourcePlayer; // 0x01
    RequestIssueState m_ReqIssueState[4]; // 0x02
    MachineBitTable m_ReqCmdBit; // 0x06
    Cmd m_NextCmds[4];       // 0x08
    State m_State;           // 0x0C
    Cmd m_NextCmdReq;        // 0x0D
    RequestHandleState m_ReqHandleState[4]; // 0x0E
    Action m_Action;         // 0x12
    u8 unk_0x13;
    s32 m_WaitPlayerReadyTimer; // 0x14
    Cmd m_Cmd;               // 0x18
    PlayerNo m_CmdIssuePlayer; // 0x19
    s32 m_MinimumDelayTimer; // 0x1C
    s32 m_CancelTelopTimer;  // 0x20
    MachineBitTable m_HandleCmdBit; // 0x24
    bool m_IsActionInProgress; // 0x26
    bool m_Unlocked;         // 0x27
    bool m_HandleCmdInSinglePlayer; // 0x28 (used to still go to the tour result stage alone)
    u8 unk_0x29;
    bool m_ShowingTelop;     // 0x2A
    bool m_RequestFailGateClosed; // 0x2B
    bool m_RequestFailNotBestFriends; // 0x2C
    u8 unk_0x2D[3];
};
ASSERT_SIZE(LockMgr, 0x30);
} // namespace netgame
