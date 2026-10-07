#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session15LeaveSessionJobE @ 0x008CFF90
// vtable 0x009017B0 (vptr 0x009017B8), offset_to_top 0, 16 entries
//
// Leaves the session (Session::LeaveSessionAsync): it waits for the joint session job, leaves the
// mesh (as the host with host migration, then it waits until the host migrated), leaves the
// matchmake sessions (the other one of a joint session first) and cleans up the mesh. The
// network decides the order with its slots. The step names are from the strings; the layout is
// from the constructor, the member and slot names are ours.
class LeaveSessionJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LeaveSessionJob(); // 0x004341C8
    virtual ~LeaveSessionJob(); // 0x0043423C slot 0x00
    // 0x00434214 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007338D8 slot 0x14
    // sets the step LeaveMesh
    virtual void vf_0x18(); // 0x00433E1C slot 0x18
    // the mesh is left: the next step of the network
    virtual void vf_0x1C() = 0; // slot 0x1C
    // the current matchmake session is left
    virtual void vf_0x20() = 0; // slot 0x20
    // the mesh is cleaned up: the last step of the network
    virtual void vf_0x24() = 0; // slot 0x24
    // sets the step MeshCleanup
    virtual void vf_0x28(); // 0x00433DD4 slot 0x28
    // the current matchmake session does not need to be left
    virtual bool vf_0x2C() = 0; // slot 0x2C
    // the host migration did not end in time
    virtual void vf_0x30(); // 0x00433CB4 slot 0x30
    // how long the host waits for the migration after it left (ms)
    virtual s32 GetHostMigrationWaitMSec(); // 0x007338D0 slot 0x34
    virtual void vf_0x38(); // 0x00433CA4 slot 0x38
    // the cleanup of the network (Cleanup calls it first)
    virtual void vf_0x3C(); // 0x0043341C slot 0x3C

    // (names are ours)
    nn::Result Startup(nn::pia::common::CallContext* pCallContext); // 0x00433EC4
    void Cleanup(); // 0x00433E60

    // the steps
    common::ExecuteResult MeshCleanup(); // 0x00433360
    common::ExecuteResult WaitLeaveMesh(); // 0x00433420
    common::ExecuteResult WaitHostMigrated(); // 0x004334B8
    common::ExecuteResult LeaveBufferMatchmakeSession(); // 0x004336CC
    common::ExecuteResult LeaveCurrentMatchmakeSession(); // 0x00433868
    common::ExecuteResult WaitLeaveMeshWithHostMigration(); // 0x004339A8
    common::ExecuteResult WaitLeaveBufferMatchmakeSession(); // 0x00433AE4
    common::ExecuteResult WaitLeaveCurrentMatchmakeSession(); // 0x00433BD0
    common::ExecuteResult WaitForcedTerminatingOfJointSessionJob(); // 0x00433CC0
    common::ExecuteResult LeaveMesh(); // 0x00434014

    common::CallContext* m_pCallContext; // 0x40, of Session
    common::CallContext m_CallContext;   // 0x44, of the mesh and the matchmake sessions
    nn::Result m_Result;                 // 0x58
    common::Time m_HostMigrationDeadline; // 0x60
    bool m_IsLastStation;                // 0x68, the host left as the only station
    common::Time m_StartTime;            // 0x70 (the monitoring data)
};
ASSERT_OFFSET(LeaveSessionJob, m_Result, 0x58);
ASSERT_OFFSET(LeaveSessionJob, m_IsLastStation, 0x68);
ASSERT_OFFSET(LeaveSessionJob, m_StartTime, 0x70);
ASSERT_SIZE(LeaveSessionJob, 0x78);
} // namespace session
} // namespace pia
} // namespace nn
