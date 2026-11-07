#include "applet/dErrEulaMgr.h"
#include "netgame/dLockMgr.h"

#include "Bs/dBsSvMgr.h"
#include "Bs/dBsTelop.h"
#include "Bs/dBsTourMgr.h"
#include "demo/demo_Api.h"
#include "netgame/dGameGlobals.h"
#include "netgame/dGateMgr.h"
#include "netgame/dGameFunctions.h"
#include "netgame/dNetGameMgr.h"
#include "stage/dTransitionMgr.h"

namespace netgame {


namespace {
// the packets of the lock
const u8 PACKET_LOCK_RESULT_RANDOM_MATCH = 8;
const u8 PACKET_LOCK_REQUEST_RANDOM_MATCH = 7;
const u8 PACKET_LOCK_REQUEST = 18;
const u8 PACKET_LOCK_ANSWER = 19;
const u8 PACKET_LOCK_CMD = 20;
const u8 PACKET_LOCK_CANCELED = 21;
const u8 PACKET_LOCK_CANCEL = 26;
const u8 PACKET_LOCK_RESULT = 27;
const u8 PACKET_LOCK_EXEC = 28;
const u8 PACKET_LOCK_DONE = 29;

const s32 WAIT_PLAYER_READY_FRAMES = 300;

// the bits of the result of a request of a station that is not admitted yet
const u16 RESULT_SUCCESS = 0x8000;
const u16 RESULT_GATE_CLOSED = 0x4000;
const u16 RESULT_NOT_BEST_FRIENDS = 0x2000;

DECOMP_ALWAYS_INLINE bool IsEntryCmd(LockMgr::Cmd cmd)
{
    return cmd == LockMgr::CMD_FRIEND_MATCH_ENTRY || cmd == LockMgr::CMD_RANDOM_MATCH_ENTRY;
}
} // namespace

DECOMP_ALWAYS_INLINE PlayerNo netgame::LockMgr::GetCurrentPlayerNo() const
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    return m_HandleCmdInSinglePlayer ? mgr->GetMyPlayerNo() : mgr->m_MyPlayerNo;
}

DECOMP_ALWAYS_INLINE PlayerNo netgame::LockMgr::GetNextRequestIssuer() const
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        if (m_ReqIssueState[i] != REQUEST_ISSUE_STATE_NONE) {
            return static_cast<PlayerNo>(i);
        }
    }
    return PLAYER_NO_INVALID;
}

DECOMP_ALWAYS_INLINE void netgame::LockMgr::ClearRequestHandleStates()
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        m_ReqHandleState[i] = REQUEST_HANDLE_STATE_NONE;
    }
}

DECOMP_ALWAYS_INLINE void netgame::LockMgr::ProcessLockRequest(PlayerNo myNo)
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    PacketQueue& queue = mgr->m_PacketQueue;
    if (m_SourcePlayer < PLAYER_NO_MAX) {
        m_State = STATE_UNLOCKED;
        return;
    }
    if (myNo == PLAYER_NO_HOST) {
        m_ReqCmdBit.Add(PLAYER_NO_HOST);
        m_NextCmds[PLAYER_NO_HOST] = m_NextCmdReq;
        m_State = STATE_LOCK_PENDING;
        return;
    }
    if (mgr->IsAdmitted(myNo)) {
        u8 cmd = m_NextCmdReq;
        queue.BeginPacket();
        queue.AddPacketData(&cmd, sizeof(cmd));
        queue.EndPacket(PACKET_LOCK_REQUEST, PLAYER_NO_HOST);
        m_State = STATE_LOCK_PENDING;
        return;
    }
    if (!mgr->IsInSession(PLAYER_NO_HOST, true)) {
        m_State = STATE_UNLOCKED;
        return;
    }
    u8 isRandomMatch = mgr->m_IsConnectRandomMatch ? 1 : 0;
    const void* parts[] = {&isRandomMatch};
    const size_t partSizes[] = {sizeof(isRandomMatch)};
    if (mgr->SendPacketBatch(PLAYER_NO_HOST, parts, partSizes, 1, PACKET_LOCK_REQUEST_RANDOM_MATCH, 0, 1, 0)) {
        m_State = STATE_LOCK_PENDING;
    }
}

DECOMP_ALWAYS_INLINE void netgame::LockMgr::ExecCmd(PlayerNo myNo)
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (m_Cmd != CMD_SAVE_INTERRUPT && m_Cmd != CMD_TOUR_END &&
        (m_Cmd != CMD_RANDOM_MATCH_START_TOUR || !mgr->m_IslandMgr.m_IsSelfDepartingForTour)) {
        stage::GetTransitionMgr()->SetReturnParam();
    }
    switch (m_Cmd) {
    case CMD_FRIEND_MATCH_LEAVE:
        stage::Transition(stage::NAME_STATION_RETIRE_DEMO, 1, 0, 1, 1);
        break;
    case CMD_TOUR_END:
        StageTransitionTourResult();
        break;
    case CMD_RANDOM_MATCH_ENTRY:
        stage::GetTransitionMgr()->Transition(stage::NAME_RANDOM_MATCHING_ENTRY, 1, 0, 1, 1);
        break;
    case CMD_RANDOM_MATCH_LEAVE:
        stage::GetTransitionMgr()->Transition(stage::NAME_RANDOM_MATCHING_RETIRE, 1, 0, 1, 1);
        break;
    case CMD_RANDOM_MATCH_START_TOUR:
        stage::Transition(stage::NAME_PREPARE_TOUR, 1, 0, 1, 1);
        break;
    default:
        stage::Transition(stage::NAME_SAVE, 1, 0, 1, 1);
        break;
    }
    m_IsActionInProgress = false;
    m_MinimumDelayTimer = 0;
    m_CancelTelopTimer = 0;
    switch (m_Cmd) {
    case CMD_FRIEND_MATCH_ENTRY:
        if (myNo != PLAYER_NO_HOST) {
            BsSvMgr::SetSaveConnectNetVst();
        } else {
            BsSvMgr::SetSaveConnectNetHst();
        }
        mgr->m_JoinMgr.StartEntry();
        break;
    case CMD_FRIEND_MATCH_LEAVE:
        mgr->m_LeaveMgr.StartLeave(true);
        break;
    case CMD_SAVE_CONTINUE:
        if (myNo != PLAYER_NO_HOST) {
            BsSvMgr::SetSaveContinueNetVst();
            mgr->m_SaveDemoMgr.OnStartSave(false);
        }
        break;
    case CMD_SAVE_INTERRUPT:
        if (myNo != PLAYER_NO_HOST) {
            BsSvMgr::SetSaveInterruptNetVst();
            mgr->m_SaveDemoMgr.OnStartSave(true);
        }
        break;
    case CMD_TOUR_END:
        if (myNo == PLAYER_NO_HOST) {
            mgr->m_IslandMgr.StartTourDeskArrivalAfterTour();
        }
        BsTourMgr::SetState(static_cast<BsTourMgr::State>(6));
        break;
    case CMD_RANDOM_MATCH_ENTRY:
        if (myNo != PLAYER_NO_HOST) {
            BsSvMgr::SetSaveConnectRandomMatchNetVst();
        } else {
            BsSvMgr::SetSaveConnectRandomMatchNetHst();
        }
        mgr->m_JoinMgr.StartEntry();
        break;
    case CMD_RANDOM_MATCH_LEAVE:
        if (myNo == PLAYER_NO_HOST) {
            BsSvMgr::SetSaveSeeOffRandomMatchNetHst();
        } else if (m_SourcePlayer == PLAYER_NO_HOST) {
            BsSvMgr::SetSaveSeeOffNetHostRandomMatchNetVst();
        } else {
            BsSvMgr::SetSaveSeeOffRandomMatchNetVst();
        }
        mgr->m_IslandMgr.LeaveRandomMatch(IslandMgr::LEAVE_RANDOM_MATCH_MODE_SEE_OFF);
        break;
    case CMD_RANDOM_MATCH_START_TOUR:
        if (myNo != PLAYER_NO_HOST) {
            BsSvMgr::SetSaveStartTourRandomMatchNetVst();
        } else {
            BsSvMgr::SetSaveStartTourRandomMatchNetHst();
        }
        mgr->m_IslandMgr.StartTourRandomMatch();
        break;
    default:
        break;
    }
    m_HandleCmdInSinglePlayer = false;
    m_Action = ACTION_NONE;
}

DECOMP_ALWAYS_INLINE void netgame::LockMgr::HandleAction(PlayerNo myNo)
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    PacketQueue& queue = mgr->m_PacketQueue;
    switch (m_Action) {
    case ACTION_BEGIN:
        m_IsActionInProgress = true;
        m_MinimumDelayTimer = m_Cmd == CMD_RANDOM_MATCH_START_TOUR ? 180 : 90;
        m_WaitPlayerReadyTimer = 0;
        m_CancelTelopTimer = 0;
        m_Action = ACTION_WAIT;
        break;
    case ACTION_WAIT: {
        Cmd cmd = m_Cmd;
        bool fails = false;
        if (cmd != CMD_TOUR_END && cmd != CMD_RANDOM_MATCH_START_TOUR && ++m_WaitPlayerReadyTimer > WAIT_PLAYER_READY_FRAMES) {
            fails = true;
        } else if (cmd == CMD_FRIEND_MATCH_ENTRY || cmd == CMD_FRIEND_MATCH_LEAVE || cmd == CMD_RANDOM_MATCH_ENTRY ||
                   cmd == CMD_RANDOM_MATCH_START_TOUR) {
            if (mgr->m_IsSessionClosedForIsland) {
                fails = true;
            } else if (cmd == CMD_RANDOM_MATCH_ENTRY &&
                       (g_TourStatus.state == 1 || g_TourStatus.state == 2 || g_TourStatus.unk_0x4)) {
                fails = true;
            }
        }
        if (fails) {
            m_IsActionInProgress = false;
            m_Action = ACTION_FAIL;
            break;
        }
        if ((cmd | myNo) == 0) {
            PlayerNo issuer = GetNextRequestIssuer();
            if ((s_GateParam & 1) != 0 && issuer < PLAYER_NO_MAX) {
                const AvatarMgr::AvatarInfo* info = mgr->m_AvatarMgr.GetAvatarInfo(issuer);
                if (info != NULL) {
                    u32 principalId = info->principalId;
                    if (mgr->m_FriendListMgr.FindBestFriend(principalId, info->avatarId) < 0) {
                        m_IsActionInProgress = false;
                        m_Action = ACTION_FAIL;
                        m_RequestFailNotBestFriends = true;
                        break;
                    }
                }
            }
            if ((m_Cmd | myNo) == 0 && !mgr->m_GateMgr.IsOpen()) {
                m_IsActionInProgress = false;
                m_Action = ACTION_FAIL;
                m_RequestFailGateClosed = true;
                break;
            }
        }
        if (demo::MakeOrderNetgameLock()) {
            m_Action = ACTION_ACCEPT;
        }
        break;
    }
    case ACTION_FAIL:
    case ACTION_ACCEPT:
        if (myNo != PLAYER_NO_HOST) {
            u8 isAccepted = m_Action != ACTION_FAIL;
            queue.BeginPacket();
            queue.AddPacketData(&isAccepted, sizeof(isAccepted));
            queue.EndPacket(PACKET_LOCK_ANSWER, PLAYER_NO_HOST);
        } else {
            m_ReqHandleState[PLAYER_NO_HOST] = m_Action != ACTION_FAIL ? REQUEST_HANDLE_STATE_BEGIN : REQUEST_HANDLE_STATE_NONE;
        }
        m_Action = ACTION_NONE;
        break;
    case ACTION_CANCEL:
        demo::RevokeNetgameLockOrder();
        m_CancelTelopTimer = 150;
        if (myNo != PLAYER_NO_HOST) {
            queue.BeginPacket();
            queue.EndPacket(PACKET_LOCK_CANCELED, PLAYER_NO_HOST);
        } else {
            m_ReqHandleState[PLAYER_NO_HOST] = REQUEST_HANDLE_STATE_NONE;
        }
        m_Action = ACTION_NONE;
        break;
    case ACTION_WAIT_DELAY:
        if (m_MinimumDelayTimer == 0) {
            m_Action = ACTION_EXEC_CMD;
        }
        break;
    case ACTION_EXEC_CMD:
        ExecCmd(myNo);
        break;
    default:
        break;
    }
}

DECOMP_ALWAYS_INLINE void netgame::LockMgr::ProcessRequest()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    PacketQueue& queue = mgr->m_PacketQueue;
    PlayerNo issuer = GetNextRequestIssuer();
    if (issuer >= PLAYER_NO_MAX) {
        return;
    }
    Cmd nextCmd = m_NextCmds[issuer];
    if (m_ReqIssueState[issuer] == REQUEST_ISSUE_STATE_BEGIN) {
        bool canStart = true;
        if (nextCmd == CMD_FRIEND_MATCH_ENTRY) {
            if (!mgr->m_GateMgr.IsOpen()) {
                m_ReqIssueState[issuer] = REQUEST_ISSUE_STATE_FAIL;
                ClearRequestHandleStates();
                m_RequestFailGateClosed = true;
                canStart = false;
            } else if ((s_GateParam & 1) != 0) {
                const AvatarMgr::AvatarInfo* info = mgr->m_AvatarMgr.GetAvatarInfo(issuer);
                if (info != NULL) {
                    u32 principalId = info->principalId;
                    if (mgr->m_FriendListMgr.FindBestFriend(principalId, info->avatarId) < 0) {
                        m_ReqIssueState[issuer] = REQUEST_ISSUE_STATE_FAIL;
                        ClearRequestHandleStates();
                        m_RequestFailNotBestFriends = true;
                        canStart = false;
                    }
                }
            }
        }
        if (canStart) {
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if (m_ReqHandleState[no] != REQUEST_HANDLE_STATE_LOCK_PLAYER) {
                    continue;
                }
                if (no == PLAYER_NO_HOST) {
                    m_Action = ACTION_BEGIN;
                    m_Cmd = nextCmd;
                    m_CmdIssuePlayer = issuer;
                } else {
                    u8 param = (nextCmd & 0x1F) | (issuer << 5);
                    queue.BeginPacket();
                    queue.AddPacketData(&param, sizeof(param));
                    queue.EndPacket(PACKET_LOCK_CMD, no);
                }
                m_ReqHandleState[no] = REQUEST_HANDLE_STATE_BEGIN_STEP;
            }
        }
        m_ReqIssueState[issuer] = REQUEST_ISSUE_STATE_WAIT;
    }
    if (m_ReqIssueState[issuer] == REQUEST_ISSUE_STATE_WAIT) {
        bool isFinished = true;
        bool isAccepted = true;
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            RequestHandleState state = m_ReqHandleState[i];
            if (state == REQUEST_HANDLE_STATE_BEGIN) {
                continue;
            }
            if (state != REQUEST_HANDLE_STATE_NONE) {
                isFinished = false;
                break;
            }
            isAccepted = false;
        }
        if (IsEntryCmd(nextCmd) && !mgr->IsInSession(issuer, true)) {
            isAccepted = false;
        }
        if ((mgr->IsMultiPlayer() || m_HandleCmdInSinglePlayer) && mgr->GetMyPlayerNo() == PLAYER_NO_HOST) {
            // no transition of the stage may be running
            stage::Name current = stage::GetCurrent();
            stage::Name next = stage::GetNext();
            if (current >= stage::NAME_MAX || next < stage::NAME_MAX || mgr->m_StageMgr.m_State != 0 ||
                mgr->m_StageMgr.FUN_0075f464()) {
                isAccepted = false;
            }
        }
        if (isFinished) {
            if (isAccepted) {
                if (IsEntryCmd(nextCmd)) {
                    mgr->m_Bits[2].Add(issuer);
                    mgr->m_Bits[0].Add(issuer);
                    mgr->m_Mgr8.FUN_00617b70();
                }
                m_ReqIssueState[issuer] = REQUEST_ISSUE_STATE_SUCCESS;
            } else {
                m_ReqIssueState[issuer] = REQUEST_ISSUE_STATE_FAIL;
                for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                    PlayerNo no = static_cast<PlayerNo>(i);
                    if (m_ReqHandleState[no] != REQUEST_HANDLE_STATE_BEGIN) {
                        continue;
                    }
                    if (no != issuer && mgr->IsAdmitted(no)) {
                        m_ReqHandleState[no] = REQUEST_HANDLE_STATE_DO_CANCEL;
                    } else {
                        m_ReqHandleState[no] = REQUEST_HANDLE_STATE_NONE;
                    }
                }
            }
        }
    }
    if (m_ReqIssueState[issuer] == REQUEST_ISSUE_STATE_FAIL) {
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            PlayerNo no = static_cast<PlayerNo>(i);
            if (m_ReqHandleState[no] != REQUEST_HANDLE_STATE_DO_CANCEL) {
                continue;
            }
            if (no == PLAYER_NO_HOST) {
                m_Action = ACTION_CANCEL;
            } else {
                queue.BeginPacket();
                queue.EndPacket(PACKET_LOCK_CANCEL, no);
            }
            m_ReqHandleState[no] = REQUEST_HANDLE_STATE_CANCELING;
        }
    }
    if (m_ReqIssueState[issuer] == REQUEST_ISSUE_STATE_FAIL) {
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            if (m_ReqHandleState[i] != REQUEST_HANDLE_STATE_NONE) {
                return;
            }
        }
        if (HandleRequestResult(issuer, false)) {
            m_ReqIssueState[issuer] = REQUEST_ISSUE_STATE_NONE;
            m_ReqCmdBit.Remove(issuer);
            m_HandleCmdBit.Reset();
            m_RequestFailGateClosed = false;
            m_RequestFailNotBestFriends = false;
        }
    } else if (m_ReqIssueState[issuer] == REQUEST_ISSUE_STATE_SUCCESS && HandleRequestResult(issuer, true)) {
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            PlayerNo no = static_cast<PlayerNo>(i);
            if (m_HandleCmdInSinglePlayer && no == issuer) {
                if (no == PLAYER_NO_HOST) {
                    m_Action = ACTION_WAIT_DELAY;
                }
                continue;
            }
            if (no == issuer && nextCmd != CMD_TOUR_END) {
                continue;
            }
            if (!mgr->IsAdmitted(no)) {
                continue;
            }
            if (no == PLAYER_NO_HOST) {
                m_Action = ACTION_WAIT_DELAY;
            } else {
                queue.BeginPacket();
                queue.EndPacket(PACKET_LOCK_EXEC, no);
            }
        }
        m_SuccessfulCmd = nextCmd;
        m_SourcePlayer = issuer;
        m_ReqIssueState[issuer] = REQUEST_ISSUE_STATE_NONE;
        m_ReqCmdBit.Remove(issuer);
    }
}

DECOMP_ALWAYS_INLINE void netgame::LockMgr::StartHandleNextRequest()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (GetNextRequestIssuer() < PLAYER_NO_MAX) {
        return;
    }
    PlayerNo issuer = PLAYER_NO_INVALID;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        if (m_ReqCmdBit.IsSet(static_cast<PlayerNo>(i))) {
            issuer = static_cast<PlayerNo>(i);
            break;
        }
    }
    if (issuer >= PLAYER_NO_MAX) {
        return;
    }
    m_ReqIssueState[issuer] = REQUEST_ISSUE_STATE_BEGIN;
    Cmd cmd = m_NextCmds[issuer];
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no == issuer) {
            if (cmd != CMD_FRIEND_MATCH_LEAVE && cmd != CMD_RANDOM_MATCH_LEAVE) {
                m_HandleCmdBit.Add(no);
            }
        } else if (mgr->IsAdmitted(no)) {
            m_HandleCmdBit.Add(no);
        }
    }
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        RequestHandleState state = REQUEST_HANDLE_STATE_BEGIN;
        if (m_HandleCmdInSinglePlayer && no == issuer) {
            state = REQUEST_HANDLE_STATE_LOCK_PLAYER;
        } else if ((no != issuer || cmd == CMD_TOUR_END) && mgr->IsAdmitted(no)) {
            state = REQUEST_HANDLE_STATE_LOCK_PLAYER;
        }
        m_ReqHandleState[no] = state;
    }
}

DECOMP_ALWAYS_INLINE void netgame::LockMgr::ShowTelop()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if ((!mgr->IsMultiPlayer() && !m_HandleCmdInSinglePlayer) || !m_IsActionInProgress) {
        return;
    }
    if (m_CancelTelopTimer != 0) {
        BsTelop::ShowTelop(static_cast<BsTelop::TelopName>(8));
        if (--m_CancelTelopTimer == 0) {
            m_IsActionInProgress = false;
        }
        return;
    }
    switch (m_Cmd) {
    case CMD_FRIEND_MATCH_ENTRY:
        BsTelop::ShowTelop(static_cast<BsTelop::TelopName>(0));
        break;
    case CMD_FRIEND_MATCH_LEAVE:
        BsTelop::ShowTelop(static_cast<BsTelop::TelopName>(1));
        break;
    case CMD_SAVE_CONTINUE:
        BsTelop::ShowTelop(static_cast<BsTelop::TelopName>(2));
        break;
    case CMD_SAVE_INTERRUPT:
        BsTelop::ShowTelop(static_cast<BsTelop::TelopName>(3));
        break;
    case CMD_TOUR_END:
        BsTelop::ShowTelop(static_cast<BsTelop::TelopName>(30));
        break;
    case CMD_RANDOM_MATCH_ENTRY:
        BsTelop::ShowTelop(static_cast<BsTelop::TelopName>(4));
        break;
    case CMD_RANDOM_MATCH_LEAVE:
        BsTelop::ShowTelop(static_cast<BsTelop::TelopName>(5));
        break;
    case CMD_RANDOM_MATCH_START_TOUR: {
        TourName tourName = static_cast<TourName>(g_TourStatus.tourName);
        BsTelop::TelopName telop = static_cast<BsTelop::TelopName>(mgr->m_IslandMgr.m_PlayersThatDepartForTour.IsSet(GetCurrentPlayerNo()) ? 21 : 23);
        BsTelop::ShowTourTelop(telop, tourName);
        break;
    }
    default:
        return;
    }
    if (m_MinimumDelayTimer != 0) {
        m_MinimumDelayTimer--;
    }
    m_ShowingTelop = true;
}

// 0x00619EA8 (name after libgarden)
void netgame::LockMgr::Process()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (!mgr->m_PacketQueue.m_IsClosed && ((mgr->m_IsInNetSession && mgr->m_IsPlayerAssigned) || m_HandleCmdInSinglePlayer)) {
        PlayerNo myNo = GetCurrentPlayerNo();
        if (myNo < PLAYER_NO_MAX) {
            if (m_State == STATE_LOCK_REQUESTED) {
                ProcessLockRequest(myNo);
            }
            HandleAction(myNo);
        }
        if (GetCurrentPlayerNo() == PLAYER_NO_HOST) {
            ProcessRequest();
            if (!m_Unlocked) {
                StartHandleNextRequest();
            } else {
                PlayerNo issuer = GetNextRequestIssuer();
                for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                    PlayerNo no = static_cast<PlayerNo>(i);
                    if (m_ReqCmdBit.IsSet(no) && no != issuer && HandleRequestResult(no, false)) {
                        m_ReqCmdBit.Remove(no);
                    }
                }
            }
        }
    }
    ShowTelop();
}

// 0x0061AEF4 (name is ours)
void netgame::LockMgr::ProcessEnd()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (!mgr->m_PacketQueue.m_IsClosed && !applet::ErrEulaMgr::IsInputBlocked() && !mgr->unk_0x132B4 && m_Unlocked) {
        if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST) {
            PacketQueue& queue = mgr->m_PacketQueue;
            queue.BeginPacket();
            queue.EndPacket(PACKET_LOCK_DONE, PLAYER_NO_HOST);
        } else {
            m_HandleCmdBit.Remove(PLAYER_NO_HOST);
            mgr->m_AvatarMgr.m_HasJoiner = false;
        }
        m_Unlocked = false;
    }
    if (m_ShowingTelop) {
        BsTelop::EndTelop();
        m_ShowingTelop = false;
    }
}

// 0x0061AFA8 (name after libgarden)
bool netgame::LockMgr::HandleRequestResult(PlayerNo player, bool success)
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (player == GetCurrentPlayerNo()) {
        m_State = success ? STATE_LOCKED : STATE_UNLOCKED;
        return true;
    }
    if (mgr->IsAdmitted(player)) {
        u8 result = success;
        PacketQueue& queue = mgr->m_PacketQueue;
        queue.BeginPacket();
        queue.AddPacketData(&result, sizeof(result));
        queue.EndPacket(PACKET_LOCK_RESULT, player);
        return true;
    }
    if (mgr->IsInSession(player, true)) {
        u16 result = 0;
        const void* parts[] = {&result};
        const size_t partSizes[] = {sizeof(result)};
        if (success) {
            result = RESULT_SUCCESS;
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if (mgr->IsAdmitted(no) && mgr->m_Bits[1].IsSet(no)) {
                    result |= GetMask(no);
                }
            }
        } else {
            result = m_RequestFailGateClosed ? RESULT_GATE_CLOSED : 0;
            result |= m_RequestFailNotBestFriends ? RESULT_NOT_BEST_FRIENDS : 0;
        }
        return mgr->SendPacketBatch(player, parts, partSizes, 1, PACKET_LOCK_RESULT_RANDOM_MATCH, 0, 1, 0);
    }
    return true;
}

// 0x0061B15C (name is ours)
void netgame::LockMgr::SetNextCmd(PlayerNo player, Cmd cmd)
{
    if (player < PLAYER_NO_MAX) {
        m_NextCmds[player] = cmd;
    }
}

// 0x0061B16C (name is ours)
void netgame::LockMgr::SetRequestHandleState(PlayerNo player, RequestHandleState state)
{
    if (player < PLAYER_NO_MAX) {
        m_ReqHandleState[player] = state;
    }
}

// 0x0061B17C (name is ours)
void netgame::LockMgr::Reset()
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        m_ReqIssueState[i] = REQUEST_ISSUE_STATE_NONE;
    }
    ClearRequestHandleStates();
    m_State = STATE_UNLOCKED;
    m_NextCmdReq = CMD_NONE;
    m_Action = ACTION_NONE;
    m_WaitPlayerReadyTimer = 0;
    m_Cmd = CMD_NONE;
    m_CmdIssuePlayer = PLAYER_NO_INVALID;
    m_ReqCmdBit.Reset();
    m_ReqCmdBit.Reset();
    m_SuccessfulCmd = CMD_NONE;
    m_SourcePlayer = PLAYER_NO_INVALID;
    m_IsActionInProgress = false;
    m_MinimumDelayTimer = 0;
    m_CancelTelopTimer = 0;
    m_HandleCmdBit.Reset();
    m_Unlocked = false;
    m_HandleCmdInSinglePlayer = false;
    m_ShowingTelop = false;
    m_RequestFailGateClosed = false;
    m_RequestFailNotBestFriends = false;
}

// 0x0061B1F4 (name is ours)
netgame::LockMgr::LockMgr()
{
    m_SuccessfulCmd = CMD_NONE;
    m_SourcePlayer = PLAYER_NO_INVALID;
    m_State = STATE_UNLOCKED;
    m_Action = ACTION_NONE;
    m_WaitPlayerReadyTimer = 0;
    m_Cmd = CMD_NONE;
    m_CmdIssuePlayer = PLAYER_NO_INVALID;
    m_MinimumDelayTimer = 0;
    m_CancelTelopTimer = 0;
    m_IsActionInProgress = false;
    m_Unlocked = false;
    m_HandleCmdInSinglePlayer = false;
    unk_0x29 = 0;
    m_ShowingTelop = false;
    m_RequestFailGateClosed = false;
    m_RequestFailNotBestFriends = false;
}

// 0x006255BC (name is ours)
void netgame::LockMgr::RemovePlayer(PlayerNo player)
{
    if (player >= PLAYER_NO_MAX) {
        return;
    }
    m_ReqIssueState[player] = REQUEST_ISSUE_STATE_NONE;
    m_ReqHandleState[player] = REQUEST_HANDLE_STATE_NONE;
    m_ReqCmdBit.Remove(player);
    m_HandleCmdBit.Remove(player);
}
} // namespace netgame
