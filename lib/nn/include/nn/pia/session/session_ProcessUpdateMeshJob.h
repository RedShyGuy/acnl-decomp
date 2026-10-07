#pragma once

#include "decomp.h"
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
// RTTI N2nn3pia7session20ProcessUpdateMeshJobE @ 0x008D0074
// vtable 0x00901A34 (vptr 0x00901A3C), offset_to_top 0, 6 entries
//
// Applies an update of the mesh from the host (the station data list, MeshProtocol): connects the
// new stations and disconnects the ones that left, then waits until all are connected. In a
// network with relay routes it first waits for the direct connections, reports them to the host
// (RelayRouteManageJob) and connects the rest over the relay routes the host sends back. The steps
// are from the strings of the binary; the layout is from the constructor, the member names and the
// names marked so are ours.
class ProcessUpdateMeshJob : public ::nn::pia::common::StepSequenceJob
{
public:
    // the results of Startup / UpdateStationDataList / SetStationDataList (names are ours)
    static const u32 UPDATE_STATE_INVALID = 0;
    static const u32 UPDATE_STATE_PART_PENDING = 1; // the other part of the list is missing
    static const u32 UPDATE_STATE_OLD = 2;          // an older version than the one being received
    static const u32 UPDATE_STATE_NOT_APPLIED = 3;  // the last list could not be applied
    static const u32 UPDATE_STATE_ACCEPTED = 4;

    ProcessUpdateMeshJob(); // 0x0043DE40
    virtual ~ProcessUpdateMeshJob(); // 0x0043E14C slot 0x00
    // 0x0043E13C slot 0x04 (deleting dtor)
    virtual void Trace(unsigned long long flag) const; // 0x007339E8 slot 0x14

    // the first list of an update: UPDATE_STATE_ACCEPTED starts the job (isSameVersion: the mesh has
    // this version already, but not this number of stations)
    u32 Startup(const unsigned char* pData, unsigned int size, bool isSameVersion); // 0x0043DC68 | fefates:bytes-fuzzy [tier B]
    // a newer list while the job runs
    u32 UpdateStationDataList(const unsigned char* pData, unsigned int size); // 0x0043D490 | fefates:bytes-fuzzy [tier B]
    void Cleanup(); // 0x0043DBB0
    DECOMP_NOINLINE void CalcTimeLimit(bool isSameVersion); // 0x0043B8E0 | fefates:bytes-fuzzy [tier B]
    // the call context of the connection to the station is free again
    void ClearStationIndex(nn::pia::StationIndex stationIndex); // 0x0043BAA8 | fefates:bytes [tier B]
    DECOMP_NOINLINE u32 SetStationDataList(const unsigned char* pData, unsigned int size); // 0x0043BF90 | fefates:bytes [tier B]
    // takes over the new list: disconnects the stations that are not in it any more
    DECOMP_NOINLINE void UpdateDataTakeover(unsigned int stationNum); // 0x0043C2F8 | fefates:bytes [tier B]
    // the host tells that the station could not connect to another one
    void SetConnectionFailureNotice(nn::pia::StationIndex stationIndex, unsigned char reason); // 0x0043D9CC | fefates:bytes [tier B]
    void SetMonitoringData(); // 0x0043BACC
    // (name is ours)
    void ClearMonitoringData(); // 0x0043DA14
    // the NAT of the station maps endpoint dependent
    bool CheckEdmByStationIndex(nn::pia::StationIndex stationIndex) const; // 0x00733930 | fefates:bytes [tier B]
    nn::pia::StationIndex GetStationIndexByPrincipalID(unsigned int principalId) const; // 0x00733990 | fefates:bytes [tier B]

    // the steps
    common::ExecuteResult WaitDirectConnection(); // 0x0043D030
    common::ExecuteResult SendConnectionReport(); // 0x0043C678
    common::ExecuteResult StartRelayConnection(); // 0x0043C88C
    common::ExecuteResult CheckConnectionAll(); // 0x0043BAE0
    common::ExecuteResult UpdateFailed(); // 0x0043B7BC
    common::ExecuteResult WaitLeaveMesh(); // 0x0043BA24
    common::ExecuteResult CleanupByProcessFailure(); // 0x0043D91C

    // (names are ours)
    // connects and disconnects the stations after the list; false if the update failed
    DECOMP_NOINLINE bool UpdateStations(); // 0x0043AD8C
    // the result of the join after the failure reason
    DECOMP_NOINLINE void SetJoinFailureResult(nn::pia::StationIndex stationIndex, u8 reason, bool isFromNotice); // 0x0043DA20

    // (inline; names are ours)
    void ReleaseCallContext(StationIndex stationIndex)
    {
        if (m_CallContextIndices[stationIndex] < m_StationNumMax) {
            m_CallContextIndices[stationIndex] = INVALID_CALL_CONTEXT_INDEX;
        }
    }
    // a call context that no connection uses (INVALID_CALL_CONTEXT_INDEX if there is none)
    u8 GetFreeCallContextIndex() const
    {
        u8 freeIndex = INVALID_CALL_CONTEXT_INDEX;
        for (u32 i = 0; i < m_StationNumMax; i++) {
            bool isUsed = false;
            for (int j = 0; j < STATION_INDEX_MAX + 1; j++) {
                if (m_CallContextIndices[j] == i) {
                    isUsed = true;
                    break;
                }
            }
            if (isUsed || m_pCallContexts[i].GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
                continue;
            }
            freeIndex = i;
            break;
        }
        return freeIndex;
    }
    void ClearPartData()
    {
        m_PartStationNum = 0;
        m_PartHostEntry = 0;
        m_PartVersion = 0;
        m_PartNum = 0;
        m_IsPartReceived[0] = false;
        m_IsPartReceived[1] = false;
    }

    static const u8 INVALID_CALL_CONTEXT_INDEX = 0xFF;

    common::Time m_Deadline;                                // 0x40, of the update
    s32 m_TimeLimitMSec;                                    // 0x48 (25000)
    // the time limit of a list with the same version, also added to the one of a new version
    s32 m_ShortTimeLimitMSec;                               // 0x4C (20000)
    // until then WaitDirectConnection waits for missing stations; then the time of the next
    // connection checks of SendConnectionReport
    common::Time m_DirectConnectionDeadline;                // 0x50
    s32 m_DirectConnectionTimeLimitMSec;                    // 0x58
    common::CallContext* m_pCallContexts;                   // 0x5C, m_StationNumMax of them
    u8 m_CallContextIndices[STATION_INDEX_MAX + 1];         // 0x60, of the connection to each station
    u32 m_StationNumMax;                                    // 0x6C (Mesh)
    transport::StationConnectionInfo* m_pStationConnectionInfos; // 0x70, of the station data list
    StationIndex* m_pStationIndices;                        // 0x74, of the station data list
    transport::StationConnectionInfo* m_pNewStationConnectionInfos; // 0x78, of a newer list
    StationIndex* m_pNewStationIndices;                     // 0x7C, of a newer list
    // a list in two parts (bytes 8 to 11 of the list)
    bool m_IsPartReceived[2];                               // 0x80
    u8 m_PartStationNum;                                    // 0x82
    u8 m_PartHostEntry;                                     // 0x83
    u8 m_PartVersion;                                       // 0x84 (only the low byte)
    u8 m_PartNum;                                           // 0x85
    u32 m_StationNum;                                       // 0x88, in the station data list
    u8* m_pConnectionFailureReasons;                        // 0x8C, per entry (SetConnectionFailureNotice)
    common::Time m_NextFailureNoticeTime;                   // 0x90
    u32 m_ReportSequence;                                   // 0x98, of the connection reports
    u32 m_DirectionsVersion;                                // 0x9C, of the relay route directions taken
    u32 m_Version;                                          // 0xA0, of the station data list
    bool m_IsProcessing;                                    // 0xA4, an update runs
    bool m_IsSameVersion;                                   // 0xA5
    bool m_IsJoining;                                       // 0xA6, set while JoinMeshJob runs
    u32 m_SameVersionUpdateNum;                             // 0xA8 (SetMonitoringData)
    bool m_IsApplied;                                       // 0xAC, the last list was applied
    common::CallContext m_CallContext;                      // 0xB0, of the leaving
};
ASSERT_OFFSET(ProcessUpdateMeshJob, m_pCallContexts, 0x5C);
ASSERT_OFFSET(ProcessUpdateMeshJob, m_StationNumMax, 0x6C);
ASSERT_OFFSET(ProcessUpdateMeshJob, m_IsPartReceived, 0x80);
ASSERT_OFFSET(ProcessUpdateMeshJob, m_StationNum, 0x88);
ASSERT_OFFSET(ProcessUpdateMeshJob, m_Version, 0xA0);
ASSERT_OFFSET(ProcessUpdateMeshJob, m_IsJoining, 0xA6);
ASSERT_OFFSET(ProcessUpdateMeshJob, m_IsApplied, 0xAC);
ASSERT_OFFSET(ProcessUpdateMeshJob, m_CallContext, 0xB0);
ASSERT_SIZE(ProcessUpdateMeshJob, 0xC8);
} // namespace session
} // namespace pia
} // namespace nn
