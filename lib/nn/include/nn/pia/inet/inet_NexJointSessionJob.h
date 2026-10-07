#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/session/session_JointSessionJob.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet18NexJointSessionJobE @ 0x008CF8EC
// vtable 0x00900150 (vptr 0x00900158), offset_to_top 0, 25 entries
//
// The joint session of inet: the leader and the companion station of the two sessions agree over
// SessionProtocol on a matchmake session (created, joined or found by random matchmaking), the
// stations leave their mesh and join the mesh of the new session; a failure or a destroy leaves
// the session again. The step names are from the strings (the PMF table is at 0x008B0D2C); the
// member names are ours.
class NexJointSessionJob : public ::nn::pia::session::JointSessionJob
{
public:
    NexJointSessionJob(); // 0x003F8334
    virtual ~NexJointSessionJob(); // 0x003F8400 slot 0x00
    // 0x003F83DC slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072F154 slot 0x14
    virtual u8 GetPhase() const; // 0x003E9EE0 slot 0x18
    virtual nn::Result vf_0x24(const nn::pia::session::CreateSessionSetting* pCreateSetting, const nn::pia::session::SessionSearchCriteria* pCriteria, u32 criteriaNum); // 0x003F6F08 slot 0x24
    virtual nn::Result vf_0x28(const nn::pia::session::CreateSessionSetting* pSetting); // 0x003F5060 slot 0x28
    virtual nn::Result vf_0x2C(const nn::pia::session::JoinSessionSetting* pSetting); // 0x003F3468 slot 0x2C
    virtual nn::Result vf_0x30(); // 0x003F4588 slot 0x30
    virtual nn::Result vf_0x34(); // 0x003F55E8 slot 0x34
    virtual nn::Result vf_0x38(u8 phase, u8 messageType, const nn::pia::StationId* pStationIds, u32 stationNum, const nn::pia::StationId& stationId); // 0x003F1F24 slot 0x38
    virtual nn::Result vf_0x3C(u8 phase, u32 sessionId); // 0x003F09CC slot 0x3C
    virtual nn::Result vf_0x40(u8 phase, const nn::pia::StationId* pStationIds, u32 stationNum, const nn::pia::StationId& stationId); // 0x003F27E8 slot 0x40
    virtual nn::Result vf_0x44(u8 phase, const nn::pia::StationId& stationId); // 0x003F55D0 slot 0x44
    virtual nn::Result vf_0x48(u8 phase, const nn::pia::StationId& stationId); // 0x003F5050 slot 0x48
    virtual void vf_0x4C(const nn::pia::StationId& stationId); // 0x003F1E18 slot 0x4C
    virtual void vf_0x50(u32 sessionId, u32 principalId); // 0x003F2770 slot 0x50
    virtual void vf_0x54(u32 sessionId, u32 value); // 0x003F1ECC slot 0x54
    virtual void vf_0x58(u32 sessionId, u32 principalId); // 0x003ED7A4 slot 0x58
    virtual void vf_0x5C(u32 sessionId); // 0x003F1E5C slot 0x5C
    virtual void vf_0x60(); // 0x003E9248 slot 0x60

    // the steps (in the order of the PMF table)
    common::ExecuteResult CallSessionEvent(); // 0x003E9B0C
    common::ExecuteResult CallDestroySessionEvent(); // 0x003EF430
    common::ExecuteResult ProcessFailure(); // 0x003E9468
    common::ExecuteResult StartLeaveMesh(); // 0x003E9520
    common::ExecuteResult CloseMatchmakeSession(); // 0x003ED83C
    common::ExecuteResult WaitCloseMatchmakeSession(); // 0x003F1354
    common::ExecuteResult CloseJointSessionParticipation(); // 0x003F5350
    common::ExecuteResult SendInvitationAsCompanion(); // 0x003F09D4
    common::ExecuteResult WaitCloseJointSessionParticipation(); // 0x003F7490
    common::ExecuteResult SendDestroyInvitation(); // 0x003EDAA0
    common::ExecuteResult WaitForAnswerToDestroyInvitation(); // 0x003F6504
    common::ExecuteResult ResendDestroyInvitation(); // 0x003EF74C
    common::ExecuteResult WaitForInvitationAsCompanion(); // 0x003F3C98
    common::ExecuteResult SendAnswerToInvitation(); // 0x003EE500
    common::ExecuteResult SendAnswerToDestroyInvitation(); // 0x003F4828
    common::ExecuteResult WaitForAnswerToInvitation(); // 0x003F1650
    common::ExecuteResult ResendInvitationAsCompanion(); // 0x003F2BDC
    common::ExecuteResult StartCreateMatchmakeSession(); // 0x003F3074
    common::ExecuteResult StartJoinMatchmakeSession(); // 0x003F0F40
    common::ExecuteResult StartRandomMatchmake(); // 0x003ECC3C
    common::ExecuteResult SendLeaveMeshCompanion(); // 0x003EE854
    common::ExecuteResult WaitRandomMatchmake(); // 0x003EC930
    common::ExecuteResult WaitNotification(); // 0x003EA700
    common::ExecuteResult WaitCreateMatchmakeSession(); // 0x003F252C
    common::ExecuteResult WaitJoinMatchmakeSession(); // 0x003F0654
    common::ExecuteResult SendNextSessionId(); // 0x003EA8EC
    common::ExecuteResult SendPreparedForMigrateSession(); // 0x003F4B68
    common::ExecuteResult WaitCompanionStationPrepared(); // 0x003F37B0
    common::ExecuteResult ResendNextSessionId(); // 0x003EC1F8
    common::ExecuteResult WaitUntilCompanionLeavesMesh(); // 0x003F4328
    common::ExecuteResult StartLeavePreviousMesh(); // 0x003EEE18
    common::ExecuteResult WaitNextSessionId(); // 0x003EB9E4
    common::ExecuteResult WaitUntilLeaderLeavesMesh(); // 0x003F1C38
    common::ExecuteResult GetNextMatchmakeSessionInfo(); // 0x003F2970
    common::ExecuteResult WaitGetNextMatchmakeSessionInfo(); // 0x003F5E24
    common::ExecuteResult StartLeavePreviousMatchmakeSession(); // 0x003F7218
    common::ExecuteResult WaitMeshRestart(); // 0x003E9970
    common::ExecuteResult WaitLeavePreviousMesh(); // 0x003EE044
    common::ExecuteResult WaitLeavePreviousMatchmakeSession(); // 0x003F6CFC
    common::ExecuteResult MeshRestart(); // 0x003E8910
    common::ExecuteResult WaitStartRetryJoinMesh(); // 0x003EF130
    common::ExecuteResult StartGetNextMeshHostStationConnectionInfo(); // 0x003F80B8
    common::ExecuteResult StartCreateNextMesh(); // 0x003EC740
    common::ExecuteResult WaitCreateNextMesh(); // 0x003EBE40
    common::ExecuteResult WaitCompanionStation(); // 0x003ED028
    common::ExecuteResult WaitGetNextMeshHostStationConnectionInfo(); // 0x003F7AD4
    common::ExecuteResult StartJoinNextMesh(); // 0x003EAF2C
    common::ExecuteResult WaitJoinNextMesh(); // 0x003E9EE8
    common::ExecuteResult WaitHostStationId(); // 0x003EB114
    common::ExecuteResult WaitUntilCompanionCompletion(); // 0x003F40F4
    common::ExecuteResult SendCompletionForMigrateSession(); // 0x003F5858
    common::ExecuteResult SendCompletionInvitation(); // 0x003EFD9C
    common::ExecuteResult WaitForAnswerToCompletionInvitation(); // 0x003F7788
    common::ExecuteResult WaitCompletionInvitation(); // 0x003F01A8
    common::ExecuteResult ProcessComplete(); // 0x003E9728
    common::ExecuteResult StartLeaveCurrentMatchmakeSession(); // 0x003F6AF8
    common::ExecuteResult WaitLeaveMesh(); // 0x003E92C4
    common::ExecuteResult WaitLeaveBufferMatchmakeSession(); // 0x003F601C
    common::ExecuteResult StartLeaveBufferMatchmakeSession(); // 0x003F61DC
    common::ExecuteResult WaitLeaveCurrentMatchmakeSession(); // 0x003F6968

    // (names are ours)
    // the joint session parts of the monitoring data are cleared
    void ResetMonitoringContent(); // 0x003EE314
    // the station list is set to the valid stations of the current session
    void SetupStationIdList(); // 0x003EFC14
    // m_Unknown0x1AD: all participants of the session (notifications) are in the station list
    void UpdateUnknown0x1AD(); // 0x003F08EC

    transport::StationConnectionInfo m_ConnectionInfo; // 0x0C4, of the host of the next mesh
    common::Time m_Time0x118;                          // 0x118
    common::Time m_Time0x120;                          // 0x120
    common::Time m_RestartTime;                        // 0x128, of MeshRestart
    common::Time m_Time0x130;                          // 0x130
    common::Time m_Time0x138;                          // 0x138
    common::Time m_Time0x140;                          // 0x140
    common::Time m_RetryDeadline;                      // 0x148
    common::Time m_Time0x150;                          // 0x150
    common::Time m_Time0x158;                          // 0x158
    common::Time m_Time0x160;                          // 0x160
    common::Time m_MeshRestartDeadline;                // 0x168
    common::Time m_Time0x170;                          // 0x170
    nn::Result m_Result;                               // 0x178, RESULT_NOT_SET at first
    u8 m_JobPhase;                                     // 0x17C, GetPhase
    u8 m_Unknown0x17D;                                 // 0x17D
    StationId m_StationId0x180;                        // 0x180
    u8 m_Unknown0x188;                                 // 0x188
    u32 m_Unknown0x18C;                                // 0x18C
    u8 m_Unknown0x190;                                 // 0x190
    u32 m_Unknown0x194;                                // 0x194
    // set by vf_0x54 for the current / the other session
    u8 m_Unknown0x198;                                 // 0x198
    u32 m_Unknown0x19C;                                // 0x19C
    u8 m_Unknown0x1A0;                                 // 0x1A0
    u32 m_Unknown0x1A4;                                // 0x1A4
    u8 m_Unknown0x1A8;                                 // 0x1A8
    u8 m_RetryCount;                                   // 0x1A9
    u8 m_Unknown0x1AA;                                 // 0x1AA
    u8 m_IsUnregistering;                              // 0x1AB, the buffer session is unregistered instead of left
    u8 m_Unknown0x1AC;                                 // 0x1AC
    bool m_Unknown0x1AD;                               // 0x1AD
};
ASSERT_OFFSET(NexJointSessionJob, m_ConnectionInfo, 0xC4);
ASSERT_OFFSET(NexJointSessionJob, m_Time0x118, 0x118);
ASSERT_OFFSET(NexJointSessionJob, m_Result, 0x178);
ASSERT_OFFSET(NexJointSessionJob, m_StationId0x180, 0x180);
ASSERT_OFFSET(NexJointSessionJob, m_Unknown0x1A8, 0x1A8);
ASSERT_SIZE(NexJointSessionJob, 0x1B0);
} // namespace inet
} // namespace pia
} // namespace nn
