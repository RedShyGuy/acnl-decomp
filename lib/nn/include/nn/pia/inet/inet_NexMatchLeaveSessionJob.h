#pragma once

#include "decomp.h"
#include "nn/pia/session/session_LeaveSessionJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet23NexMatchLeaveSessionJobE @ 0x008CF9B4
// vtable 0x00900490 (vptr 0x00900498), offset_to_top 0, 16 entries
//
// Leaves the session through NexMatchmakeSession: first the owners of the other sessions of the
// stations are asked (StationIdStatusTable), the host of a joint session passes the ownership of
// the other matchmake session to the stations of the current one, and the sessions that the
// notifications still know are left too. The step names are from the strings; the member names
// are ours.
class NexMatchLeaveSessionJob : public ::nn::pia::session::LeaveSessionJob
{
public:
    static const u32 SESSION_NUM_MAX = 12;
    static const s32 HOST_MIGRATION_WAIT_MSEC = 15000;

    NexMatchLeaveSessionJob(); // 0x00404A54
    // (armlink placed it in front of the destructor of the base)
    virtual ~NexMatchLeaveSessionJob(); // 0x00434238 slot 0x00
    // 0x00404AB4 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072F528 slot 0x14
    virtual void vf_0x18(); // 0x0040498C slot 0x18
    virtual void vf_0x1C(); // 0x004042A4 slot 0x1C
    virtual void vf_0x20(); // 0x00404840 slot 0x20
    virtual void vf_0x24(); // 0x00404104 slot 0x24
    virtual bool vf_0x2C(); // 0x00404818 slot 0x2C
    virtual void vf_0x30(); // 0x004047B8 slot 0x30
    virtual s32 GetHostMigrationWaitMSec(); // 0x0072F51C slot 0x34
    // (armlink placed it far in front, before __ARM_common_memclr4_12)
    virtual void vf_0x3C(); // 0x00134CE4 slot 0x3C

    // the steps
    common::ExecuteResult CompleteProcess(); // 0x00404080
    common::ExecuteResult GetMatchmakeSessionOwners(); // 0x00404158
    common::ExecuteResult MigrateMatchmakeSessionOwner(); // 0x004043FC
    common::ExecuteResult WaitGetMatchmakeSessionOwners(); // 0x004045E0
    common::ExecuteResult WaitMigrateMatchmakeSessionOwner(); // 0x0040470C

    // the next matchmake session to leave: one the notifications still know becomes the other one
    // (inline in vf_0x1C / vf_0x20)
    DECOMP_ALWAYS_INLINE bool SetJoinedSessionAsOther();

    u32 m_SessionIds[SESSION_NUM_MAX];      // 0x78, of the stations (StationIdStatusTable)
    u8 m_SessionNum;                         // 0xA8
    bool m_IsOwnerAsked[SESSION_NUM_MAX];    // 0xA9
};
ASSERT_OFFSET(NexMatchLeaveSessionJob, m_SessionIds, 0x78);
ASSERT_OFFSET(NexMatchLeaveSessionJob, m_SessionNum, 0xA8);
ASSERT_SIZE(NexMatchLeaveSessionJob, 0xB8);
} // namespace inet
} // namespace pia
} // namespace nn
