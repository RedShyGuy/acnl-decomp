#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/session/session_JoinSessionJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet22NexMatchJoinSessionJobE @ 0x008CF990
// vtable 0x0090040C (vptr 0x00900414), offset_to_top 0, 17 entries
//
// Joins the matchmake session on the server: first the sessions that the notifications still
// know are left, then the session is joined (and the joint session it belongs to), its own join
// notification is awaited (10 seconds), the connection info of the host is asked and the NAT
// session starts before the mesh (JoinSessionJob::MeshStartup). A failed mesh join is retried
// once (CleanupForRetryJoinMesh), if needed after the owner of the session changed. On failure the
// sessions are left. The step names are from the strings except WaitGetJoinedSessions (no step
// sets it); the member names are ours.
class NexMatchJoinSessionJob : public ::nn::pia::session::JoinSessionJob
{
public:
    static const u32 JOINED_SESSION_NUM_MAX = 4;
    static const s64 NOTIFICATION_TIMEOUT_MSEC = 10000;
    static const s64 OWNER_CHANGE_TIMEOUT_MSEC = 30000;
    static const s64 OWNER_CHANGE_TIMEOUT_MSEC_19 = 60000;
    static const s64 OWNER_CHANGE_RETRY_MSEC = 10000;
    static const s64 OWNER_CHANGE_RETRY_MSEC_20 = 30000;
    static const u8 RETRY_COUNT_MAX = 2;

    NexMatchJoinSessionJob(); // 0x00403DC8
    // (armlink placed it in front of the destructor of the base)
    virtual ~NexMatchJoinSessionJob(); // 0x004320C8 slot 0x00
    // 0x00403E08 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072F1D4 slot 0x14
    virtual void vf_0x18(u32 sessionId, u32 principalId); // 0x004027AC slot 0x18
    virtual void vf_0x1C(u32 sessionId); // 0x00402790 slot 0x1C
    virtual nn::Result vf_0x30(const nn::pia::session::JoinSessionSetting* pSetting); // 0x00401638 slot 0x30
    virtual void vf_0x34(); // 0x0040161C slot 0x34
    virtual nn::pia::common::ExecuteResult vf_0x38(nn::Result result); // 0x00403208 slot 0x38
    virtual nn::pia::common::ExecuteResult vf_0x3C(); // 0x00402FCC slot 0x3C
    virtual nn::pia::common::ExecuteResult vf_0x40(); // 0x00402F6C slot 0x40

    // the steps
    common::ExecuteResult StartNatSession(); // 0x0040176C
    common::ExecuteResult JoinJointSession(); // 0x004018A4
    common::ExecuteResult WaitNotification(); // 0x004019E0
    common::ExecuteResult WaitJoinMatchmake(); // 0x00401C40
    common::ExecuteResult WaitStartNatSession(); // 0x00401E04
    common::ExecuteResult JoinMatchmakeSession(); // 0x00402048
    common::ExecuteResult WaitJoinJointSession(); // 0x004021F8
    common::ExecuteResult LeaveMatchmakeSession(); // 0x00402418
    common::ExecuteResult CleanupForRetryJoinMesh(); // 0x00402914
    common::ExecuteResult GetStationConnectionInfo(); // 0x00402D54
    common::ExecuteResult LeaveJoinedMatchmakeSession(); // 0x00403030
    common::ExecuteResult WaitGetStationConnectionInfo(); // 0x004034F0
    // (name is ours)
    common::ExecuteResult WaitGetJoinedSessions(); // 0x004036B0
    common::ExecuteResult WaitLeaveBufferMatchmakeSession(); // 0x0040380C
    common::ExecuteResult WaitLeaveJoinedMatchmakeSession(); // 0x00403930
    common::ExecuteResult WaitLeaveCurrentMatchmakeSession(); // 0x00403A3C
    common::ExecuteResult WaitChangeOwnerOfMatchmakeSession(); // 0x00403B4C

    u32 m_HostPrincipalId;                          // 0xE0, of the connection info
    u32 m_OwnerPrincipalId;                         // 0xE4, of the session (notification)
    u32 m_JointOwnerPrincipalId;                    // 0xE8, of the joint session
    u32 m_JointSessionId;                           // 0xEC, the session belongs to it
    common::Time m_OwnerChangeDeadline;             // 0xF0, then the session is left
    common::Time m_OwnerChangeRetryTime;            // 0xF8, then the connection info is asked again
    common::Time m_NotificationDeadline;            // 0x100
    u32 m_JoinedSessionIds[JOINED_SESSION_NUM_MAX]; // 0x108, the sessions to leave
    u32 m_JoinedSessionNum;                         // 0x118
    bool m_IsWaitingForOwnerChange;                 // 0x11C
};
ASSERT_OFFSET(NexMatchJoinSessionJob, m_HostPrincipalId, 0xE0);
ASSERT_OFFSET(NexMatchJoinSessionJob, m_OwnerChangeDeadline, 0xF0);
ASSERT_OFFSET(NexMatchJoinSessionJob, m_JoinedSessionIds, 0x108);
ASSERT_OFFSET(NexMatchJoinSessionJob, m_IsWaitingForOwnerChange, 0x11C);
ASSERT_SIZE(NexMatchJoinSessionJob, 0x120);
} // namespace inet
} // namespace pia
} // namespace nn
