#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/session/session_AutoMatchmakeJob.h"

namespace nn {
namespace pia {
namespace inet {
class NexMatchmakeSession;
// RTTI N2nn3pia4inet24NexMatchAutoMatchmakeJobE @ 0x008CF9D8
// vtable 0x00900524 (vptr 0x0090052C), offset_to_top 0, 16 entries
//
// The automatic matchmaking on the server: first the sessions that the notifications still know
// are left, then NexMatchmakeSession finds or creates a session, its own join notification is
// awaited (10 seconds), the joint session it belongs to is joined and the NAT session starts
// before the mesh (AutoMatchmakeJob::MeshStartup). A failed mesh join is retried once
// (CleanupForRetryJoinMesh), if needed after the owner of the session changed. On failure the
// sessions are left (a creator unregisters its session). The step names are from the strings
// except WaitGetJoinedSessions (no step sets it); the member names are ours.
class NexMatchAutoMatchmakeJob : public ::nn::pia::session::AutoMatchmakeJob
{
public:
    static const u32 CRITERIA_NUM_MAX = 2;
    static const u32 JOINED_SESSION_NUM_MAX = 4;
    static const s64 NOTIFICATION_TIMEOUT_MSEC = 10000;
    static const s64 OWNER_CHANGE_TIMEOUT_MSEC = 30000;
    static const s64 OWNER_CHANGE_TIMEOUT_MSEC_19 = 60000;
    static const s64 OWNER_CHANGE_RETRY_MSEC = 10000;
    static const s64 OWNER_CHANGE_RETRY_MSEC_20 = 30000;
    static const u8 RETRY_COUNT_MAX = 2;

    NexMatchAutoMatchmakeJob(); // 0x0040A0CC
    // (armlink placed it in front of the destructor of the base)
    virtual ~NexMatchAutoMatchmakeJob(); // 0x00437CC8 slot 0x00
    // 0x0040A118 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072F844 slot 0x14
    virtual void vf_0x20(u32 sessionId, u32 principalId); // 0x00408F30 slot 0x20
    virtual void vf_0x24(u32 sessionId); // 0x00408F14 slot 0x24
    virtual nn::Result vf_0x28(const nn::pia::session::CreateSessionSetting* pCreateSetting, const nn::pia::session::SessionSearchCriteria* pCriteria, u32 criteriaNum); // 0x00407DB4 slot 0x28
    virtual void vf_0x2C(); // 0x00407D7C slot 0x2C
    virtual void vf_0x30(); // 0x004099A4 slot 0x30
    virtual nn::pia::common::ExecuteResult vf_0x34(nn::Result result); // 0x004099F0 slot 0x34
    virtual nn::pia::common::ExecuteResult vf_0x38(nn::Result result); // 0x004096F0 slot 0x38
    virtual nn::pia::common::ExecuteResult vf_0x3C(); // 0x004094B0 slot 0x3C

    // the steps
    common::ExecuteResult AutoMatchmake(); // 0x00407F70
    common::ExecuteResult StartNatSession(); // 0x004080F4
    common::ExecuteResult JoinJointSession(); // 0x00408224
    common::ExecuteResult WaitNotification(); // 0x00408334
    common::ExecuteResult WaitAutoMatchmake(); // 0x00408580
    common::ExecuteResult WaitStartNatSession(); // 0x00408778
    common::ExecuteResult WaitJoinJointSession(); // 0x004089C0
    common::ExecuteResult LeaveMatchmakeSession(); // 0x00408BA4
    common::ExecuteResult CleanupForRetryJoinMesh(); // 0x004090A4
    common::ExecuteResult LeaveJoinedMatchmakeSession(); // 0x00409514
    // (name is ours)
    common::ExecuteResult WaitGetJoinedSessions(); // 0x00409A58
    common::ExecuteResult WaitLeaveBufferMatchmakeSession(); // 0x00409BC8
    common::ExecuteResult WaitLeaveJoinedMatchmakeSession(); // 0x00409CEC
    common::ExecuteResult WaitLeaveCurrentMatchmakeSession(); // 0x00409DFC
    common::ExecuteResult WaitChangeOwnerOfMatchmakeSession(); // 0x00409F10

    u32 m_JointSessionId;                           // 0xE8, the session belongs to it
    NexMatchmakeSession* m_pSession;                // 0xEC, the current matchmake session of Session
    u32 m_OwnerPrincipalId;                         // 0xF0, of the session (notification)
    u32 m_JointOwnerPrincipalId;                    // 0xF4, of the joint session
    common::Time m_OwnerChangeDeadline;             // 0xF8, then the session is left
    common::Time m_OwnerChangeRetryTime;            // 0x100, then the NAT session starts again
    common::Time m_NotificationDeadline;            // 0x108
    u32 m_JoinedSessionIds[JOINED_SESSION_NUM_MAX]; // 0x110, the sessions to leave
    u32 m_JoinedSessionNum;                         // 0x120
    bool m_IsWaitingForOwnerChange;                 // 0x124
    bool m_IsJoinable;                              // 0x125, of the create setting
};
ASSERT_OFFSET(NexMatchAutoMatchmakeJob, m_JointSessionId, 0xE8);
ASSERT_OFFSET(NexMatchAutoMatchmakeJob, m_OwnerChangeDeadline, 0xF8);
ASSERT_OFFSET(NexMatchAutoMatchmakeJob, m_JoinedSessionIds, 0x110);
ASSERT_OFFSET(NexMatchAutoMatchmakeJob, m_IsJoinable, 0x125);
ASSERT_SIZE(NexMatchAutoMatchmakeJob, 0x128);
} // namespace inet
} // namespace pia
} // namespace nn
