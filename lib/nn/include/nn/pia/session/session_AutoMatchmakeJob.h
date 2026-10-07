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
class CreateSessionSetting;
class SessionSearchCriteria;
// RTTI N2nn3pia7session16AutoMatchmakeJobE @ 0x008CFFC0
// vtable 0x0090187C (vptr 0x00901884), offset_to_top 0, 16 entries
//
// Session::AutoMatchmakeAsync: the network (inet) searches a session or creates one in its
// steps (slot 0x28 and its steps). A creator starts the mesh as its host (CreateMesh), the others
// get the connection info of the host from the matchmake session and join the mesh like
// JoinSessionJob. The step names are from the strings; the layout is from the constructor and
// Cleanup, the member and slot names are ours.
class AutoMatchmakeJob : public ::nn::pia::common::StepSequenceJob
{
public:
    AutoMatchmakeJob(); // 0x00437C24
    virtual ~AutoMatchmakeJob(); // 0x00437CCC slot 0x00
    // 0x00437C9C slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007338F0 slot 0x14
    virtual void Cleanup(); // 0x004378B0 slot 0x18
    virtual u8 GetPhase() const; // 0x007338E8 slot 0x1C
    // the owner of a session changed (Session::UpdateSessionOwner)
    virtual void vf_0x20(u32 sessionId, u32 principalId); // 0x00437768 slot 0x20
    // (Session::OnUnknownNotification)
    virtual void vf_0x24(u32 value); // 0x00437764 slot 0x24
    // the network takes the settings and sets its first step
    virtual nn::Result vf_0x28(const nn::pia::session::CreateSessionSetting* pCreateSetting, const nn::pia::session::SessionSearchCriteria* pCriteria, u32 criteriaNum) = 0; // slot 0x28
    // the cleanup of the network (Cleanup calls it first)
    virtual void vf_0x2C(); // 0x00436D60 slot 0x2C
    // the mesh is created (WaitCreateMesh)
    virtual void vf_0x30() = 0; // slot 0x30
    // the creation failed with the result
    virtual nn::pia::common::ExecuteResult vf_0x34(nn::Result result) = 0; // slot 0x34
    // the join failed with the result
    virtual nn::pia::common::ExecuteResult vf_0x38(nn::Result result) = 0; // slot 0x38
    // the mesh is left again (after a failure)
    virtual nn::pia::common::ExecuteResult vf_0x3C() = 0; // slot 0x3C

    // (names are ours)
    nn::Result Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::session::CreateSessionSetting* pCreateSetting,
                       const nn::pia::session::SessionSearchCriteria* pCriteria, u32 criteriaNum); // 0x00437960
    // the caller asked to cancel the matchmaking
    bool IsCancelRequested() const; // 0x00437894

    // the steps
    common::ExecuteResult CreateMesh(); // 0x00436CA8
    common::ExecuteResult MeshStartup(); // 0x00436D64
    common::ExecuteResult WaitJoinMesh(); // 0x00436F30
    common::ExecuteResult WaitLeaveMesh(); // 0x0043709C
    common::ExecuteResult WaitCreateMesh(); // 0x004370E0
    common::ExecuteResult CompleteFailure(); // 0x004371EC
    common::ExecuteResult CompleteProcess(); // 0x00437348
    common::ExecuteResult GetStationConnection(); // 0x004375FC
    common::ExecuteResult WaitGetStationConnection(); // 0x0043776C
    common::ExecuteResult JoinMesh(); // 0x00437A84
    common::ExecuteResult LeaveMesh(); // 0x00437B78

    // the state of Session (for the derived classes; names are ours)
    void SetSessionState(u8 state); // 0x004375D4
    void SetSessionDisconnectState(u8 state); // 0x004375E8

    common::CallContext* m_pCallContext;                 // 0x40, of Session
    u8 m_Phase;                                          // 0x44, 2: the mesh is joined (GetPhase)
    u32 m_SessionId;                                     // 0x48, found or created (the network sets it)
    bool m_IsCreator;                                    // 0x4C, the local station creates the session
    bool m_IsJoined;                                     // 0x4D, the join is done (CompleteProcess)
    nn::Result m_Result;                                 // 0x50
    u8 m_Unknown0x54;                                    // 0x54
    bool m_IsMeshEvent19;                                // 0x55, Mesh::EVENT_TYPE_19 (MeshEventListenerForSession)
    bool m_IsMeshEvent20;                                // 0x56, Mesh::EVENT_TYPE_20
    bool m_IsHostLeft;                                   // 0x57 (Session::SetUnknownFlagOfJoiningJobs)
    bool m_Unknown0x58;                                  // 0x58, the join fails like with m_IsHostLeft
    bool m_IsConnectionFailed;                           // 0x59, Mesh::EVENT_TYPE_CONNECTION_FAILED
    u8 m_RetryCount;                                     // 0x5A, of the network
    u32 m_HostPrincipalId;                               // 0x5C, of m_ConnectionInfo
    transport::StationConnectionInfo m_ConnectionInfo;   // 0x60, of the host
    common::CallContext m_CallContext;                   // 0xB4, of the matchmake session and the mesh
    common::Time m_Time0xC8;                             // 0xC8
    common::Time m_Time0xD0;                             // 0xD0
    common::Time m_Time0xD8;                             // 0xD8
    common::Time m_StartTime;                            // 0xE0 (the monitoring data)
};
ASSERT_OFFSET(AutoMatchmakeJob, m_Result, 0x50);
ASSERT_OFFSET(AutoMatchmakeJob, m_HostPrincipalId, 0x5C);
ASSERT_OFFSET(AutoMatchmakeJob, m_ConnectionInfo, 0x60);
ASSERT_OFFSET(AutoMatchmakeJob, m_CallContext, 0xB4);
ASSERT_SIZE(AutoMatchmakeJob, 0xE8);
} // namespace session
} // namespace pia
} // namespace nn
