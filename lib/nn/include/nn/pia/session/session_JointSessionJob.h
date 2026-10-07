#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_ObjList.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/pia_Types.h"

namespace nn {
namespace pia {
namespace session {
class CreateSessionSetting;
class JoinSessionSetting;
class SessionSearchCriteria;

// RTTI N2nn3pia7session15JointSessionJobE @ 0x008CFF84
// vtable 0x00901744 (vptr 0x0090174C), offset_to_top 0, 25 entries
//
// Joins the stations of two sessions to a joint session (the SessionProtocol messages 3 and 6 to
// 11, 19 to 22). The base checks the messages and keeps the state; the network (inet) does the
// work in its steps (slots 0x24 to 0x60). The phases 6 to 10 are the kinds of joint sessions
// (Session::EVENT_TYPE_JOINT_SESSION_*). The layout is from the constructor and Cleanup, the member
// and slot names are ours.
class JointSessionJob : public ::nn::pia::common::StepSequenceJob
{
public:
    JointSessionJob(); // 0x00433094
    virtual ~JointSessionJob(); // 0x004332C0 slot 0x00
    // 0x00433214 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007338CC slot 0x14
    virtual u8 GetPhase() const = 0; // slot 0x18
    // the buffer of m_StationIdList (a node per station of the transport)
    virtual void AllocateStationIdList(); // 0x00432178 slot 0x1C
    virtual void FreeStationIdList(); // 0x004320F4 slot 0x20
    // the random joint session (a new session of the setting or one that matches the criteria) is
    // started
    virtual nn::Result vf_0x24(const nn::pia::session::CreateSessionSetting* pCreateSetting, const nn::pia::session::SessionSearchCriteria* pCriteria, u32 criteriaNum) = 0; // slot 0x24
    // the joint session with a new session of the setting (inet: NexCreateSessionSetting) is started
    virtual nn::Result vf_0x28(const nn::pia::session::CreateSessionSetting* pSetting) = 0; // slot 0x28
    // the joint session with the session of the setting (inet: NexJoinSessionSetting) is started
    virtual nn::Result vf_0x2C(const nn::pia::session::JoinSessionSetting* pSetting) = 0; // slot 0x2C
    // the joint session with the stations of the current session is started
    virtual nn::Result vf_0x30() = 0; // slot 0x30
    // the destruction of the joint session (the stations of the current session) is started
    virtual nn::Result vf_0x34() = 0; // slot 0x34
    // the network part of Start, ReceiveSessionInfo, Restart, ReceiveAck10 and ReceiveAck20
    virtual nn::Result vf_0x38(u8 phase, u8 messageType, const nn::pia::StationId* pStationIds, u32 stationNum, const nn::pia::StationId& stationId) = 0; // slot 0x38
    virtual nn::Result vf_0x3C(u8 phase, u32 sessionId) = 0; // slot 0x3C
    virtual nn::Result vf_0x40(u8 phase, const nn::pia::StationId* pStationIds, u32 stationNum, const nn::pia::StationId& stationId) = 0; // slot 0x40
    virtual nn::Result vf_0x44(u8 phase, const nn::pia::StationId& stationId) = 0; // slot 0x44
    virtual nn::Result vf_0x48(u8 phase, const nn::pia::StationId& stationId) = 0; // slot 0x48
    // the host changed (MeshEventListenerForSession)
    virtual void vf_0x4C(const nn::pia::StationId& stationId); // 0x004326D0 slot 0x4C
    virtual void vf_0x50(u32 sessionId, u32 principalId); // 0x00432740 slot 0x50
    virtual void vf_0x54(u32 value1, u32 value2); // 0x004326D8 slot 0x54
    virtual void vf_0x58(u32 sessionId, u32 principalId); // 0x00432388 slot 0x58
    virtual void vf_0x5C(u32 value); // 0x004326D4 slot 0x5C
    // the cleanup of the network (Cleanup calls it first)
    virtual void vf_0x60() = 0; // slot 0x60

    // (names are ours)
    void Cleanup(); // 0x00432F88
    // the messages of SessionProtocol
    nn::Result ReceiveSessionInfo(u8 phase, u32 sessionId, u32 ownerPrincipalId, u8 signatureMode, const u8* pSignatureKey, u8 signatureKeySize,
                                  const StationId& stationId); // 0x00432284
    nn::Result Start(u8 phase, u8 messageType, const StationId* pStationIds, u32 stationNum, const StationId& stationId); // 0x0043238C
    nn::Result Restart(u8 phase, const StationId* pStationIds, u32 stationNum, const StationId& stationId); // 0x00432594
    nn::Result ReceiveAck20(u8 phase, const StationId& stationId); // 0x0043268C
    nn::Result ReceiveAck10(u8 phase, const StationId& stationId); // 0x004326DC
    // the station the job works with left: true then (MeshEventListenerForSession)
    bool MarkStationLeft(const StationId& stationId); // 0x0043224C
    // the stations of the station id table that are not in m_StationIdList and that the
    // application does not know are removed from the tables
    void RemoveUnknownStations(); // 0x00432744
    // the joint session begins: the stations of the other session leave (for the application)
    void BeginJointSession(); // 0x00432870
    // the joint session is formed: false if the host is not known yet
    bool CompleteJointSession(); // 0x00432B98
    // the joint session failed
    void FailJointSession(); // 0x00432ECC
    // the phase of the joint session to the monitoring data
    static void UpdateMonitoringPhase(); // 0x00432F64

    common::CallContext* m_pCallContext;                 // 0x40
    common::CallContext m_CallContext;                   // 0x44
    u8 m_Phase;                                          // 0x58, 6..10 while the job runs
    u8 m_Unknown0x59;                                    // 0x59
    u8 m_MessageType;                                    // 0x5A, of the SessionProtocol messages it waits for
    StationId m_StationId;                               // 0x5C, the station it works with
    common::ObjList<StationId> m_StationIdList;          // 0x64, the stations of the joint session
    u64* m_pStationIdNodeBuffer;                         // 0x90
    u32 m_JointSessionId;                                // 0x94, of the other session (ReceiveSessionInfo)
    u8 m_Unknown0x98;                                    // 0x98
    bool m_Unknown0x99;                                  // 0x99
    u8 m_Unknown0x9A;                                    // 0x9A, the station it works with left (MarkStationLeft)
    u8 m_Unknown0x9B;                                    // 0x9B
    u8 m_Unknown0x9C;                                    // 0x9C
    u8 m_Unknown0x9D;                                    // 0x9D
    bool m_IsFailed;                                     // 0x9E (Session sets it with m_FailureResult)
    bool m_Unknown0x9F;                                  // 0x9F, the phases 9 and 10 (Start)
    nn::Result m_FailureResult;                          // 0xA0
    bool m_IsMeshEvent19;                                // 0xA4, Mesh::EVENT_TYPE_19 (MeshEventListenerForSession)
    bool m_IsMeshEvent20;                                // 0xA5, Mesh::EVENT_TYPE_20
    common::Time m_Time0xA8;                             // 0xA8
    common::Time m_StartTime;                            // 0xB0, of Start (the monitoring data)
    common::Time m_Time0xB8;                             // 0xB8
    u8 m_Unknown0xC0;                                    // 0xC0 (the monitoring data)
};
ASSERT_OFFSET(JointSessionJob, m_StationIdList, 0x64);
ASSERT_OFFSET(JointSessionJob, m_JointSessionId, 0x94);
ASSERT_OFFSET(JointSessionJob, m_IsFailed, 0x9E);
ASSERT_OFFSET(JointSessionJob, m_FailureResult, 0xA0);
ASSERT_OFFSET(JointSessionJob, m_StartTime, 0xB0);
ASSERT_OFFSET(JointSessionJob, m_Unknown0xC0, 0xC0);
ASSERT_SIZE(JointSessionJob, 0xC8);
} // namespace session
} // namespace pia
} // namespace nn
