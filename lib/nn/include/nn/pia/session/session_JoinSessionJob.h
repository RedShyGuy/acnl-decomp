#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"

namespace nn {
namespace pia {
namespace session {
class JoinSessionSetting;

// RTTI N2nn3pia7session14JoinSessionJobE @ 0x008CFF78
// vtable 0x009016F8 (vptr 0x00901700), offset_to_top 0, 17 entries
//
// Joins a session (Session::JoinSessionAsync): the network joins it on its side (slot 0x30 and
// its steps) and fills m_ConnectionInfo with the host, MeshStartup starts the mesh and JoinMesh
// joins it; on a failure the mesh is left again. The step names are from the strings; the
// layout is from the constructor and Cleanup, the member and slot names are ours.
class JoinSessionJob : public ::nn::pia::common::StepSequenceJob
{
public:
    JoinSessionJob(); // 0x00432030
    virtual ~JoinSessionJob(); // 0x004320CC slot 0x00
    // 0x0043209C slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007338C8 slot 0x14
    // the owner of a session changed (Session::UpdateSessionOwner)
    virtual void vf_0x18(u32 sessionId, u32 principalId); // 0x00431CD8 slot 0x18
    // (Session::OnUnknownNotification)
    virtual void vf_0x1C(u32 value); // 0x00431CD4 slot 0x1C
    virtual void CancelCall(); // 0x00431CDC slot 0x20
    // the caller asked to cancel the join
    virtual bool IsCancelRequested(); // 0x00431CF8 slot 0x24
    virtual void Cleanup(); // 0x00431D14 slot 0x28
    virtual u8 GetPhase() const; // 0x00733874 slot 0x2C
    // the network takes the setting and sets its first step
    virtual nn::Result vf_0x30(const nn::pia::session::JoinSessionSetting* pSetting) = 0; // slot 0x30
    // the cleanup of the network (Cleanup calls it first)
    virtual void vf_0x34(); // 0x004315A0 slot 0x34
    // the join failed with the result
    virtual nn::pia::common::ExecuteResult vf_0x38(nn::Result result) = 0; // slot 0x38
    // the mesh is left again (after a failure)
    virtual nn::pia::common::ExecuteResult vf_0x3C() = 0; // slot 0x3C
    // the mesh is cleaned up
    virtual nn::pia::common::ExecuteResult vf_0x40() = 0; // slot 0x40

    // (name is ours)
    nn::Result Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::session::JoinSessionSetting* pSetting); // 0x00431DB8

    // the steps
    common::ExecuteResult MeshCleanup(); // 0x004315A4
    common::ExecuteResult MeshStartup(); // 0x004315DC
    common::ExecuteResult WaitJoinMesh(); // 0x00431704
    common::ExecuteResult WaitLeaveMesh(); // 0x0043186C
    common::ExecuteResult CompleteFailure(); // 0x004318CC
    common::ExecuteResult CompleteProcess(); // 0x00431A24
    common::ExecuteResult JoinMesh(); // 0x00431EB0
    common::ExecuteResult LeaveMesh(); // 0x00431FC8

    // the state of Session (for the derived classes; names are ours)
    void SetSessionState(u8 state); // 0x00431CAC
    void SetSessionDisconnectState(u8 state); // 0x00431CC0
    // the time of the join for the monitoring data (armlink placed it at the end of the code;
    // CompleteFailure has it inline)
    void SetJoinTimeToMonitoringContent(); // 0x0073387C

    common::CallContext* m_pCallContext;               // 0x40, of Session
    common::CallContext m_CallContext;                 // 0x44, of the network
    transport::StationConnectionInfo m_ConnectionInfo; // 0x58, of the host
    u8 m_Phase;                                        // 0xAC, 2: the mesh is joined (GetPhase)
    u8 m_Unknown0xAD[3];                               // 0xAD
    u32 m_SessionId;                                   // 0xB0, to join (the network sets it)
    bool m_IsJoined;                                   // 0xB4, the join is done (CompleteProcess)
    bool m_Unknown0xB5;                                // 0xB5
    bool m_IsMeshEvent19;                              // 0xB6, Mesh::EVENT_TYPE_19 (MeshEventListenerForSession)
    bool m_IsMeshEvent20;                              // 0xB7, Mesh::EVENT_TYPE_20
    bool m_IsHostLeft;                                 // 0xB8 (Session::SetUnknownFlagOfJoiningJobs; also when the join
                                                       // response has another station number)
    bool m_Unknown0xB9;                                // 0xB9, the join fails like with m_IsHostLeft
    bool m_IsConnectionFailed;                         // 0xBA, Mesh::EVENT_TYPE_CONNECTION_FAILED
    u8 m_RetryCount;                                   // 0xBB, of the network
    nn::Result m_Result;                               // 0xBC
    common::Time m_Time0xC0;                           // 0xC0
    common::Time m_Time0xC8;                           // 0xC8
    common::Time m_Time0xD0;                           // 0xD0
    common::Time m_StartTime;                          // 0xD8 (the monitoring data)
};
ASSERT_OFFSET(JoinSessionJob, m_ConnectionInfo, 0x58);
ASSERT_OFFSET(JoinSessionJob, m_SessionId, 0xB0);
ASSERT_OFFSET(JoinSessionJob, m_IsHostLeft, 0xB8);
ASSERT_OFFSET(JoinSessionJob, m_Result, 0xBC);
ASSERT_SIZE(JoinSessionJob, 0xE0);
} // namespace session
} // namespace pia
} // namespace nn
