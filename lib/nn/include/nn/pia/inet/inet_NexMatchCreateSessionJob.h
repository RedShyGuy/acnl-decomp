#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/session/session_CreateSessionJob.h"

namespace nn {
namespace pia {
namespace inet {
class NexMatchmakeSession;
// RTTI N2nn3pia4inet24NexMatchCreateSessionJobE @ 0x008CF9E4
// vtable 0x0090056C (vptr 0x00900574), offset_to_top 0, 10 entries
//
// Creates the matchmake session on the server: first the sessions that the notifications still
// know are left, then the session is created, its own join notification is awaited (10 seconds)
// and the NAT session starts before the mesh (CreateSessionJob::MeshStartup). On failure the new
// session is unregistered. The step names are from the strings except WaitGetJoinedSessions (no
// step sets it); the member names are ours.
class NexMatchCreateSessionJob : public ::nn::pia::session::CreateSessionJob
{
public:
    static const s64 NOTIFICATION_TIMEOUT_MSEC = 10000;

    NexMatchCreateSessionJob(); // 0x0040B25C
    // (armlink placed it in front of the destructor of the base)
    virtual ~NexMatchCreateSessionJob(); // 0x004382A0 slot 0x00
    // 0x0040B288 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072F848 slot 0x14
    virtual void Cleanup(); // 0x0040B240 slot 0x18
    virtual nn::Result vf_0x1C(const nn::pia::session::CreateSessionSetting* pSetting); // 0x0040A128 slot 0x1C
    virtual void vf_0x20(); // 0x0040AF14 slot 0x20
    virtual nn::pia::common::ExecuteResult vf_0x24(nn::Result result); // 0x0040AF60 slot 0x24

    // the steps
    common::ExecuteResult StartNatSession(); // 0x0040A2CC
    common::ExecuteResult WaitNotification(); // 0x0040A3B8
    common::ExecuteResult UnregisterGathering(); // 0x0040A58C
    common::ExecuteResult WaitCreateMatchmake(); // 0x0040A6C0
    common::ExecuteResult WaitStartNatSession(); // 0x0040A87C
    common::ExecuteResult CreateMatchmakeSession(); // 0x0040AAB4
    common::ExecuteResult WaitUnregisterGathering(); // 0x0040ABDC
    common::ExecuteResult LeaveJoinedMatchmakeSession(); // 0x0040AD2C
    // (name is ours)
    common::ExecuteResult WaitGetJoinedSessions(); // 0x0040AFC8
    common::ExecuteResult WaitLeaveJoinedMatchmakeSession(); // 0x0040B130

    static const u32 JOINED_SESSION_NUM_MAX = 4;

    NexMatchmakeSession* m_pSession;                    // 0x5C, the current matchmake session of Session
    u32 m_JoinedSessionIds[JOINED_SESSION_NUM_MAX];     // 0x60, the sessions to leave
    u32 m_JoinedSessionNum;                             // 0x70
    common::Time m_NotificationDeadline;                // 0x78
    bool m_IsJoinable;                                  // 0x80, of the setting
};
ASSERT_OFFSET(NexMatchCreateSessionJob, m_pSession, 0x5C);
ASSERT_OFFSET(NexMatchCreateSessionJob, m_NotificationDeadline, 0x78);
ASSERT_OFFSET(NexMatchCreateSessionJob, m_IsJoinable, 0x80);
ASSERT_SIZE(NexMatchCreateSessionJob, 0x88);
} // namespace inet
} // namespace pia
} // namespace nn
