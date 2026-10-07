#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/pia_Types.h"

namespace nn {
namespace pia {
namespace session {
class CommonMatchmakeSession;
// RTTI N2nn3pia7session26ConfigParticipationJobBaseE @ 0x008D0110
// vtable 0x00901C78 (vptr 0x00901C80), offset_to_top 0, 18 entries
//
// Opens or closes the participation of a joint session together with the other stations (the
// SessionProtocol messages 12 to 16, 23 and 24): the host asks the stations to start (SendStart,
// WaitStart), changes the participation of the matchmake session (Open/CloseParticipation) and
// tells them to finish (SendFinish, WaitFinishHost, SendEndConfigParticipation); the other
// stations answer (ClientStart, WaitFinishClient, WaitEndConfigParticipation). A station that
// becomes the host on the way takes over. The step names are from the strings; the layout is from
// the constructor, the member and slot names are ours.
class ConfigParticipationJobBase : public ::nn::pia::common::StepSequenceJob
{
public:
    // what the job does (m_Mode; names are ours)
    enum Mode : u8
    {
        MODE_NONE = 0,
        MODE_CLOSE_PARTICIPATION = 1,
        MODE_OPEN_PARTICIPATION = 2,
    };
    // the times of the steps (names are ours)
    static const s32 START_TIMEOUT_MSEC = 60000;
    static const s32 FINISH_TIMEOUT_MSEC = 40000;
    static const s32 P2P_STABLE_WAIT_MSEC = 1000;
    static const s32 END_REQUEST_INTERVAL_MSEC = 10000;

    ConfigParticipationJobBase(); // 0x004462AC
    virtual ~ConfigParticipationJobBase(); // 0x004463AC slot 0x00
    // 0x00446388 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0073420C slot 0x14
    // (names are ours)
    virtual nn::Result Startup(nn::pia::common::CallContext* pCallContext, u32 sessionId, nn::pia::session::CommonMatchmakeSession* pSession,
                               u8 mode); // 0x004459F0 slot 0x18
    // message 12: the host asks the station to start
    virtual nn::Result ReceiveStartRequest(const nn::pia::StationId& hostStationId, u32 sessionId, u8 mode, u8 counter); // 0x00444D40 slot 0x1C
    // message 14: the host tells the station to finish
    virtual void ReceiveFinish(u8 value); // 0x00444D20 slot 0x20
    // the value of message 14 (base: false)
    virtual bool vf_0x24(u32 sessionId); // 0x00734178 slot 0x24
    virtual void vf_0x28(); // 0x00444D10 slot 0x28
    virtual void vf_0x2C(); // 0x00444D0C slot 0x2C
    virtual void Cleanup(); // 0x0044598C slot 0x30
    // the state of the station for the job as bits: 1/2 (target or not) << 4, vf_0x24 << 2, in a
    // session
    virtual u8 vf_0x34(const nn::pia::StationId& stationId, u32 sessionId); // 0x00734180 slot 0x34
    virtual void vf_0x38(); // 0x00445984 slot 0x38
    virtual void vf_0x3C(); // 0x00445988 slot 0x3C
    // the target stations that left are removed
    virtual void RemoveInvalidTargetStations(); // 0x0044453C slot 0x40
    virtual void ClearState(); // 0x00445C64 slot 0x44

    // the messages of SessionProtocol (names are ours)
    // the station answered (it is one of the stations the job waits for)
    void AddRespondedStation(const nn::pia::StationId& stationId); // 0x00444494
    // the same for message 16
    void AddRespondedStation2(const nn::pia::StationId& stationId); // 0x00444A48
    // a station joined: the host starts again (MeshEventListenerForSession)
    void RequestRestart(); // 0x00444D14
    // messages 15 and 24: the host ended the job
    void SetEndReceived(); // 0x00444D34

    // (names are ours)
    // the valid stations of the station id table are the targets
    void SetupTargetStations(); // 0x004443D8
    // the job ends with the result
    DECOMP_NOINLINE void Finish(const nn::Result& result); // 0x00444154
    // all target stations answered
    bool IsAllResponded(); // 0x00444AF0
    // the local station is the host of the session (of both sessions of a joint session; inline)
    DECOMP_ALWAYS_INLINE static bool IsSessionHost();
    // the job ends after a timeout (inline)
    DECOMP_ALWAYS_INLINE void FinishByTimeout();

    // the steps
    common::ExecuteResult SendFinish(); // 0x004435E0
    common::ExecuteResult ClientStart(); // 0x004439D0
    common::ExecuteResult WaitP2PStable(); // 0x00443F14
    common::ExecuteResult WaitFinishHost(); // 0x004441D8
    common::ExecuteResult WaitFinishClient(); // 0x004445F8
    common::ExecuteResult OpenParticipation(); // 0x004448E0
    common::ExecuteResult CloseParticipation(); // 0x00444BD8
    common::ExecuteResult WaitOpenParticipation(); // 0x00444EF8
    common::ExecuteResult WaitCloseParticipation(); // 0x00445078
    common::ExecuteResult SendEndConfigParticipation(); // 0x00445204
    common::ExecuteResult WaitEndConfigParticipation(); // 0x00445378
    common::ExecuteResult StartupPassiveProcessFailure(); // 0x00445958
    common::ExecuteResult SendStart(); // 0x00445D24
    common::ExecuteResult WaitStart(); // 0x00445F74

    common::CallContext* m_pCallContext;          // 0x40, of the caller
    common::CallContext m_CallContext;            // 0x44, of the matchmake session
    u8 m_Mode;                                    // 0x58, Mode
    u32 m_SessionId;                              // 0x5C
    CommonMatchmakeSession* m_pSession;           // 0x60
    bool m_IsRequestedByHost;                     // 0x64, message 12 arrived
    bool m_IsFinishReceived;                      // 0x65, message 14 arrived
    u8 m_Unknown0x66;                             // 0x66
    StationId m_TargetStationIds[12];             // 0x68
    u32 m_TargetStationNum;                       // 0xC8
    StationId m_RespondedStationIds[12];          // 0xCC
    u32 m_RespondedStationNum;                    // 0x12C
    StationId m_HostStationId;                    // 0x130, of the job
    nn::Result m_Result;                          // 0x138
    u8 m_Unknown0x13C;                            // 0x13C
    u8 m_Unknown0x13D;                            // 0x13D
    common::Time m_Deadline;                      // 0x140
    common::Time m_StartTime;                     // 0x148, of WaitP2PStable / WaitEndConfigParticipation
    bool m_IsRestartRequested;                    // 0x150
    bool m_IsRestarted;                           // 0x151, SendFinish follows the answers
    bool m_IsEndReceived;                         // 0x152
    bool m_IsPendingRequest;                      // 0x153, a start request arrived while waiting
    u8 m_PendingMode;                             // 0x154, of that request
    u8 m_Counter;                                 // 0x155, of the start requests (0..9)
    u8 m_Phase;                                   // 0x156, the number of the current step
    u8 m_Unknown0x157;                            // 0x157
    u8 m_Unknown0x158;                            // 0x158
};
ASSERT_OFFSET(ConfigParticipationJobBase, m_Mode, 0x58);
ASSERT_OFFSET(ConfigParticipationJobBase, m_TargetStationIds, 0x68);
ASSERT_OFFSET(ConfigParticipationJobBase, m_RespondedStationIds, 0xCC);
ASSERT_OFFSET(ConfigParticipationJobBase, m_HostStationId, 0x130);
ASSERT_OFFSET(ConfigParticipationJobBase, m_Deadline, 0x140);
ASSERT_OFFSET(ConfigParticipationJobBase, m_IsRestartRequested, 0x150);
ASSERT_OFFSET(ConfigParticipationJobBase, m_Phase, 0x156);
ASSERT_SIZE(ConfigParticipationJobBase, 0x160);
} // namespace session
} // namespace pia
} // namespace nn
