#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/pia_Types.h"

namespace nn {
namespace pia {
namespace transport {
class StationConnectionInfo;
} // namespace transport
namespace session {
// RTTI N2nn3pia7session11JoinMeshJobE @ 0x008CFF30
// vtable 0x00901630 (vptr 0x00901638), offset_to_top 0, 10 entries
//
// Joins the mesh of a host: connects to the host station, sends the join request (resent by
// ResendingMessageManager until it is acked), waits for the join response (MeshProtocol hands it
// to ParseJoinResponse) and then for the connections to the other stations of the mesh. The
// monitoring data of the session begin gets the result. The steps are from the strings of the
// binary; the layout is from the constructor, the member names and the names marked so are ours.
class JoinMeshJob : public ::nn::pia::common::StepSequenceJob
{
public:
    // the phase of the join (m_Phase; the monitoring data has it)
    enum Phase : u8
    {
        PHASE_NONE = 0,
        PHASE_CONNECTING_TO_HOST = 1,
        PHASE_CONNECTING_TO_STATIONS = 2,
        // the bandwidth check of inet::JoinMeshJob
        PHASE_BANDWIDTH_CHECK = 3,
        PHASE_COMPLETED = 4,
    };

    // byte 4 of the join response (m_ResponseCode, the reason of a rejection; names are ours)
    enum ResponseCode : u8
    {
        RESPONSE_CODE_REFUSED = 0,
        RESPONSE_CODE_DENIED = 1,
        RESPONSE_CODE_2 = 2,
        RESPONSE_CODE_INVALID = 3, // set here for a response that cannot be used
    };

    // the join response comes in at most this many parts
    static const u32 RESPONSE_PART_NUM_MAX = 3;

    JoinMeshJob(); // 0x0042C030
    virtual ~JoinMeshJob(); // 0x0042C304 slot 0x00
    // 0x0042C1FC slot 0x04 (deleting dtor)
    virtual void Trace(unsigned long long flag) const; // 0x00733834 slot 0x14
    virtual bool StartupImpl(); // 0x0042A19C slot 0x18 | fefates:bytes-fuzzy
    virtual void CleanupImpl(); // 0x0042A198 slot 0x1C
    virtual void SetHostInfoToMonitoringData(const nn::pia::transport::StationConnectionInfo& info); // 0x0042BA68 slot 0x20
    // the step after all stations are connected (name is ours)
    virtual common::ExecuteResult ProceedToCompleteProcess(); // 0x0042BC08 slot 0x24

    bool Startup(const nn::pia::transport::StationConnectionInfo& info, nn::pia::common::CallContext* pCallContext); // 0x0042BE9C | fefates:callgraph
    void Cleanup(nn::Result result); // 0x0042BD24 | fefates:callgraph
    // the join response (from MeshProtocol); false if it does not fit the request (name is ours)
    DECOMP_NOINLINE bool ParseJoinResponse(const u8* pData, u32 size); // 0x0042ACA4
    // (name is ours)
    Phase GetPhase() const; // 0x0042B28C

    // the steps
    common::ExecuteResult StartConnectingToHost(); // 0x0042B294
    common::ExecuteResult WaitUntilConnectToHost(); // 0x0042B580
    common::ExecuteResult SendJoinRequest(); // 0x0042A4D4
    common::ExecuteResult WaitRequestAck(); // 0x0042A284
    common::ExecuteResult WaitJoinResponse(); // 0x0042A7AC | fefates:bytes [tier B]
    common::ExecuteResult AnalyzeJoinResponse(); // 0x0042B08C
    common::ExecuteResult WaitAllConnection(); // 0x0042A88C
    common::ExecuteResult CompleteProcess(); // 0x0042A414
    common::ExecuteResult WaitLeaveMesh(); // 0x0042A1FC
    common::ExecuteResult CompleteCancelWithLeaveMesh(); // 0x0042B9E0
    // steps of inet::JoinMeshJob (names are ours)
    common::ExecuteResult LeaveMeshWithHostMigration(); // 0x0042B910
    common::ExecuteResult LeaveMesh(); // 0x0042BFC8

    // true (with the call context signaled) if the call was canceled
    DECOMP_NOINLINE bool CheckContextCallCanncelled(); // 0x0042B884 | fefates:bytes-fuzzy [tier B]
    // true (with the call context signaled) if the transport or the host station failed
    DECOMP_NOINLINE bool CheckTransportConnectionStatus(); // 0x0042BB60 | fefates:callgraph [tier C]
    DECOMP_NOINLINE bool CheckConnectionStateWithHostStation(); // 0x0042BC60 | fefates:callgraph [tier C]
    // the indices from the join response
    DECOMP_NOINLINE bool UpdateLocalAndHostInformation(nn::pia::StationIndex localStationIndex, nn::pia::StationIndex hostStationIndex); // 0x0042BA6C | fefates:callgraph [tier C]
    // the time since the start (name is ours)
    DECOMP_NOINLINE void SetElapsedTimeToMonitoringData(); // 0x00733580
    // the stations connected through a relay
    DECOMP_NOINLINE void CheckRelayConnectionForMonitoring() const; // 0x007335D8 | fefates:callgraph [tier C]

    // (inline; names are ours)
    // the call context of the caller fails with the result
    void SignalFailureToCaller(nn::Result result);
    // the connection to the host is given up
    void CancelConnectingToHost()
    {
        m_pConnectCallContext->SignalCancel();
        m_pConnectCallContext->Reset();
    }
    void ClearWaitingFlags()
    {
        m_IsWaitingResponse = false;
        for (u32 i = 0; i < RESPONSE_PART_NUM_MAX; i++) {
            m_IsPartPending[i] = false;
        }
    }
    void ClearSkipFlags()
    {
        for (u32 i = 0; i < m_StationNumMax; i++) {
            m_pIsSkipped[i] = false;
        }
    }

    common::CallContext* m_pCallContext;                    // 0x40, of the caller (null when signaled)
    common::CallContext* m_pConnectCallContext;             // 0x44, of the ConnectStationJob of the host
    u32 m_AckId;                                            // 0x48, of the join request (0: none)
    u32 m_StationNum;                                       // 0x4C, in the join response (0: rejected)
    u32 m_StationNumMax;                                    // 0x50 (Mesh)
    transport::StationConnectionInfo* m_pStationConnectionInfos; // 0x54, of the stations of the response
    StationIndex* m_pStationIndices;                        // 0x58, of the stations of the response
    bool m_IsWaitingResponse;                               // 0x5C, cleared by ParseJoinResponse
    bool m_IsPartPending[RESPONSE_PART_NUM_MAX];            // 0x5D
    ResponseCode m_ResponseCode;                            // 0x60
    // the first part of a split response (the following ones must fit it)
    u8 m_PartStationNum;                                    // 0x61
    u8 m_PartLocalEntry;                                    // 0x62
    u8 m_PartHostEntry;                                     // 0x63
    u8 m_PartNum;                                           // 0x64
    u32 m_UpdateCount;                                      // 0x68, of the last mesh update looked at
    bool* m_pIsSkipped;                                     // 0x6C, the stations not waited for
    Phase m_Phase;                                          // 0x70
    nn::Result m_Result;                                    // 0x74, set by the mesh update
    u32 m_HostPrincipalId;                                  // 0x78
    common::Time m_StartTime;                               // 0x80
    common::CallContext m_CallContext;                      // 0x88, of the leaving of inet::JoinMeshJob
};
ASSERT_OFFSET(JoinMeshJob, m_pCallContext, 0x40);
ASSERT_OFFSET(JoinMeshJob, m_IsWaitingResponse, 0x5C);
ASSERT_OFFSET(JoinMeshJob, m_ResponseCode, 0x60);
ASSERT_OFFSET(JoinMeshJob, m_UpdateCount, 0x68);
ASSERT_OFFSET(JoinMeshJob, m_Phase, 0x70);
ASSERT_OFFSET(JoinMeshJob, m_StartTime, 0x80);
ASSERT_OFFSET(JoinMeshJob, m_CallContext, 0x88);
ASSERT_SIZE(JoinMeshJob, 0xA0);
} // namespace session
} // namespace pia
} // namespace nn
