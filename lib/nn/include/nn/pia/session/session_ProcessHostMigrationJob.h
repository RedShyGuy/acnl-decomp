#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/pia_Types.h"
#include <string.h>

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session23ProcessHostMigrationJobE @ 0x008D00D4
// vtable 0x00901B90 (vptr 0x00901B98), offset_to_top 0, 20 entries
//
// The host migration when the host is gone: the network specific jobs (inet::NexProcessHost-
// MigrationJob, local::LocalProcessHostMigrationJobNew) decide the new host and use the steps here.
// The new host greets all stations (SendGreetingMessage, WaitGreetingResponse) and tells them the
// end (SendMigrationFinish); the others wait for its greeting (WaitNewHostGreeting,
// WaitNewHostFinished). The steps are from the strings of the binary (two of them from the
// strings of inet::NexProcessHostMigrationJob, which uses them); the layout is from the
// constructor, the member names and the names marked so are ours.
class ProcessHostMigrationJob : public ::nn::pia::common::StepSequenceJob
{
public:
    ProcessHostMigrationJob(); // 0x00442A04
    virtual ~ProcessHostMigrationJob(); // 0x00442AF0 slot 0x00
    // 0x00442ADC slot 0x04 (deleting dtor)
    virtual void Trace(unsigned long long flag) const; // 0x00734154 slot 0x14
    virtual bool CleanupOldHostInfoCommonProc(); // 0x00442668 slot 0x18
    virtual bool IsFatalErrorOccur(); // 0x00441430 slot 0x1C
    virtual void vf_0x20(); // 0x00442664 slot 0x20
    virtual bool IsValidHostMigrationSetting() = 0; // slot 0x24
    virtual void UpdateHostStationIndexByLocalStationIndex(); // 0x004427F0 slot 0x28
    virtual bool StartupImpl(bool isFromMessage, nn::pia::StationIndex stationIndex) = 0; // slot 0x2C
    virtual void CleanupImpl(); // 0x00441030 slot 0x30
    virtual void CleanupStatus(); // 0x00441320 slot 0x34
    // true if the job has to wait for it (WaitUpdateSessionHost)
    virtual bool CallUpdateSessionHost(unsigned int value); // 0x0044202C slot 0x38
    virtual bool IsCompletedUpdateSessionHost(); // 0x004426B8 slot 0x3C
    virtual void DisconnectStation(nn::pia::StationIndex stationIndex); // 0x004413C0 slot 0x40
    virtual bool StartupMultiImpl(bool isFromMessage, unsigned short stationNumMax); // 0x004413B8 slot 0x44
    virtual bool CheckWhetherReselectNewHost(); // 0x00442638 slot 0x48
    virtual bool CheckWhetherSendMigrationFinish(); // 0x004426C0 slot 0x4C

    bool Startup(bool isFromMessage, nn::pia::StationIndex stationIndex); // 0x00442848
    // the migration of a mesh with host migration mode 2: the ranking of the candidates for the host
    bool StartupMulti(bool isFromMessage, const unsigned char* pRanking, unsigned int meshVersion, unsigned int directionsVersion); // 0x00441034
    void Cleanup(); // 0x0044281C | fefates:bytes [tier B]
    void SetMonitoringData(); // 0x00441438
    // 0: the local station is gone, 1: it is the new host, 2: it waits for the greeting of the new
    // host
    u32 DecideNextHostCommonProc(); // 0x004420E4
    // the stations sorted by their fitness as the host (STATION_INDEX_UNIDENTIFIED after the last)
    nn::Result MakeHostCandidateRanking(nn::pia::StationIndex stationIndex, unsigned char* pRanking, unsigned int* pMeshVersion, unsigned int* pDirectionsVersion, bool isFromConnectionInfo); // 0x004421DC | fefates:bytes-fuzzy [tier B]
    bool PrepareForBecomingHostCommonProc(); // 0x004426C8 | fefates:bytes-fuzzy [tier B]

    // the steps
    common::ExecuteResult SendGreetingMessage(); // 0x00441490
    common::ExecuteResult WaitGreetingResponse(); // 0x00441E60
    common::ExecuteResult SendMigrationFinish(); // 0x004417FC
    common::ExecuteResult WaitUpdateSessionHost(); // 0x00442034 | fefates:bytes [tier B]
    common::ExecuteResult WaitNewHostGreeting(); // 0x00441A64
    common::ExecuteResult WaitNewHostFinished(); // 0x004418F8
    common::ExecuteResult HostMigrationSuccess(); // 0x00441DE8
    common::ExecuteResult HostMigrationFailure(); // 0x00441D5C

    // (names are ours)
    void ReceiveMigrationFinish(bool isAllGreetingAnswered); // 0x00441370
    void ReceiveGreeting(nn::pia::StationIndex stationIndex); // 0x00441380
    void ReceiveGreetingResponse(nn::pia::StationIndex stationIndex); // 0x004420CC
    void ReceiveRankDecision(nn::pia::StationIndex stationIndex, bool isHigher); // 0x00441388
    void ClearMonitoringData(); // 0x00442640
    nn::pia::StationIndex GetStationIndexByPrincipalId(unsigned int principalId) const; // 0x004426A4
    nn::Result CompareRank(nn::pia::StationIndex stationIndex1, nn::pia::StationIndex stationIndex2, bool* pIsHigher, unsigned int* pValue1, unsigned int* pValue2) const; // 0x00734080

    // (inline; names are ours)
    void ClearGreetingStatus()
    {
        memset(m_IsGreetingSent, 0, sizeof(m_IsGreetingSent));
        m_IsWaitingGreetingResponse = false;
        m_IsWaitingGreeting = false;
        m_GreetingStationIndex = STATION_INDEX_UNIDENTIFIED;
        m_IsWaitingMigrationFinish = false;
        m_IsHostAllGreetingAnswered = false;
        m_NewHostStationIndex = STATION_INDEX_UNIDENTIFIED;
        m_OldHostStationIndex = STATION_INDEX_UNIDENTIFIED;
    }
    void StartMonitoring();

    common::Time m_Deadline;               // 0x40
    s32 m_TimeoutMSec;                     // 0x48 (10000)
    common::Time m_GreetingDeadline;       // 0x50, of the next greetings
    common::Time m_StartTime;              // 0x58, of the migration (monitoring)
    s64 m_Duration;                        // 0x60, in ticks (monitoring)
    u32 m_MigrationNum;                    // 0x68 (monitoring)
    u8 m_StartStationNum;                  // 0x6C, the stations of the mesh at the start (monitoring)
    u8 m_EndStationNum;                    // 0x6D, the ones at the success (monitoring)
    u8 m_Unknown0x6E;                      // 0x6E (monitoring)
    bool m_IsRunning;                      // 0x6F, the migration runs (LeaveMeshJob cleans it up)
    StationIndex m_OldHostStationIndex;    // 0x70 (253)
    StationIndex m_NewHostStationIndex;    // 0x71 (253)
    bool m_IsGreetingSent[STATION_INDEX_MAX + 1]; // 0x72, no greeting response yet
    bool m_IsWaitingGreetingResponse;      // 0x7E
    bool m_IsWaitingGreeting;              // 0x7F, of the new host
    StationIndex m_GreetingStationIndex;   // 0x80 (253)
    bool m_IsWaitingMigrationFinish;       // 0x81
    bool m_IsHostAllGreetingAnswered;      // 0x82, received with the migration finish
    bool m_IsAllGreetingAnswered;          // 0x83, sent with the migration finish
    bool m_IsMultiMigration;               // 0x84, Mesh::m_HostMigrationMode is 2
    u8 m_Ranking[STATION_INDEX_MAX + 1];   // 0x85 (253)
    u32 m_MeshVersion;                     // 0x94, of the ranking
    u32 m_DirectionsVersion;               // 0x98, of the ranking
    u8 m_RankDecisions[STATION_INDEX_MAX + 1]; // 0x9C, per station (ReceiveRankDecision)
};
ASSERT_OFFSET(ProcessHostMigrationJob, m_Duration, 0x60);
ASSERT_OFFSET(ProcessHostMigrationJob, m_IsRunning, 0x6F);
ASSERT_OFFSET(ProcessHostMigrationJob, m_IsGreetingSent, 0x72);
ASSERT_OFFSET(ProcessHostMigrationJob, m_IsMultiMigration, 0x84);
ASSERT_OFFSET(ProcessHostMigrationJob, m_MeshVersion, 0x94);
ASSERT_OFFSET(ProcessHostMigrationJob, m_RankDecisions, 0x9C);
ASSERT_SIZE(ProcessHostMigrationJob, 0xA8);
} // namespace session
} // namespace pia
} // namespace nn
