#include "nn/pia/session/session_RelayRouteManageJob.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshProtocol.h"
#include "nn/pia/transport/transport_NetworkRttManager.h"
#include "nn/pia/transport/transport_RelayRouteManager.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_Transport.h"
#include "nn/nstd/nstd_String.h"
#include <string.h>

namespace nn {
namespace pia {
namespace session {
namespace {
const u64 TRACE_FLAG = 0x10000000;
// the header of a connection report (MeshProtocol::SendConnectionReport), then the row
const u32 REPORT_HEADER_SIZE = 16;
// byte 12 of the reports
const u8 REPORT_FORMAT = 50;
// a station without a round trip time is checked this often
const s32 CONNECTION_CHECK_INTERVAL_MSEC = 300;
} // namespace

// 0x0043A094 | fefates:bytes [tier B]
void nn::pia::session::RelayRouteManageJob::UpdateConnectionReport(nn::pia::StationIndex stationIndex, const unsigned char* pReport, unsigned int size)
{
    if (stationIndex >= m_StationNum) {
        return;
    }
    u32 version = common::deserializeU32(pReport + 4);
    u32 sequence = common::deserializeU32(pReport + 8);
    if (version == 0) {
        return;
    }
    if (m_Version == version) {
        if (m_pReportSequences[stationIndex] >= sequence) {
            return;
        }
    } else {
        if (m_Version > version) {
            return;
        }
        // a new version: the reports start again
        m_Version = version;
        memset(m_pRttTable, 0, m_StationNum * m_StationNum);
        memset(m_pReportFormats, 0, m_StationNum);
        memset(m_pReportSequences, 0, m_StationNum * sizeof(u32));
        m_DirectionsVersion = 1;
    }
    m_pReportFormats[stationIndex] = pReport[12];
    nnnstdMemCpy(m_pRttTable + m_StationNum * stationIndex, pReport + REPORT_HEADER_SIZE, size - REPORT_HEADER_SIZE);
    m_pReportSequences[stationIndex] = sequence;
}

// 0x0043A164
nn::pia::common::ExecuteResult nn::pia::session::RelayRouteManageJob::SendRelayRouteDirections()
{
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX) {
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    Trace(TRACE_FLAG);
    transport::RelayRouteManager* pRelayRouteManager = transport::Transport::s_pInstance->m_pRelayRouteManager;
    // (a trace call of the RelayRouteManager was removed by the linker here and below)
    if (pRelayRouteManager->UpdateRttBetweenRelayNodeData(m_pReportFormats, m_StationNum).IsFailure()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (pRelayRouteManager
            ->UpdateDirectConnectionData(m_pRttTable, m_StationNum * m_StationNum, m_StationBitmap, &m_KickoutStationBitmap, localStationIndex,
                                         m_pKickoutReasons)
            .IsFailure()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    pRelayRouteManager->m_DirectionsVersionHigh = m_Version;
    pRelayRouteManager->m_DirectionsVersionLow = m_DirectionsVersion;
    m_StationBitmap = 0;
    if (Mesh::s_pInstance->m_pMeshProtocol->SendRelayRouteDirections()) {
        m_DirectionsVersion++;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&RelayRouteManageJob::WaitAllDirectConnectionReport, "RelayRouteManageJob::WaitAllDirectConnectionReport");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0043A2E4 | fefates:bytes [tier B]
void nn::pia::session::RelayRouteManageJob::PrepareForBecomingNewHost()
{
    transport::Transport::s_pInstance->m_pRelayRouteManager->UpdateRelayTable(m_StationBitmap);
    m_StationBitmap = 0;
}

// 0x0043A310
nn::pia::common::ExecuteResult nn::pia::session::RelayRouteManageJob::WaitAllDirectConnectionReport()
{
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX) {
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    // a report of every station of the mesh and none of the others
    bool isComplete = true;
    for (u32 i = 0; i < m_StationNum; i++) {
        if (i == localStationIndex) {
            continue;
        }
        if (Mesh::s_pInstance->CheckStationIndexIsValid(static_cast<StationIndex>(i))) {
            if (m_pReportSequences[i] == 0) {
                isComplete = false;
            }
        } else if (m_pReportSequences[i] != 0) {
            isComplete = false;
        }
    }
    if (!isComplete) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    // the row of the host
    bool isCheckTime = common::Scheduler::s_pInstance->m_DispatchTime >= m_NextCheckTime;
    u32 offset = m_StationNum * localStationIndex;
    for (u32 i = 0; i < m_StationNum; i++) {
        u16 rtt = 0;
        transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(static_cast<StationIndex>(i));
        if (pStation != nullptr && pStation->m_State == transport::Station::STATION_STATE_CONNECTED) {
            rtt = transport::NetworkRttManager::s_pInstance->GetAverage(pStation->m_StationAddress);
        }
        u8 value;
        if (i != localStationIndex && Mesh::s_pInstance->CheckStationIndexIsValid(static_cast<StationIndex>(i))) {
            if (rtt != 0) {
                if (pStation->IsConnectionRouteRelay()) {
                    rtt = 0;
                    value = 0;
                } else {
                    value = (rtt >> 2) == 0 ? 1 : ((rtt >> 2) >= 0xFF ? 0xFF : rtt >> 2);
                }
            } else {
                if (!pStation->IsConnectionRouteRelay()) {
                    // no round trip time yet: ask the station now and then
                    if (isCheckTime) {
                        Mesh::s_pInstance->m_pMeshProtocol->SendConnectionCheck(static_cast<StationIndex>(i));
                        m_NextCheckTime = common::Scheduler::s_pInstance->m_DispatchTime +
                                          common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * CONNECTION_CHECK_INTERVAL_MSEC);
                    }
                    isComplete = false;
                }
                value = 0;
            }
        } else if (rtt != 0) {
            value = (rtt >> 2) == 0 ? 1 : ((rtt >> 2) >= 0xFF ? 0xFF : rtt >> 2);
        } else {
            value = 0;
        }
        m_pRttTable[offset] = value;
        offset++;
    }
    m_pReportFormats[localStationIndex] = REPORT_FORMAT;
    if (!isComplete) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&RelayRouteManageJob::SendRelayRouteDirections, "RelayRouteManageJob::SendRelayRouteDirections");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0043A5B0 | fefates:bytes [tier B]
void nn::pia::session::RelayRouteManageJob::Cleanup()
{
    m_Version = 0;
    memset(m_pRttTable, 0, m_StationNum * m_StationNum);
    memset(m_pReportSequences, 0, m_StationNum * sizeof(u32));
    m_StationBitmap = 0;
    m_KickoutStationBitmap = 0;
}

// 0x0043A5EC | fefates:bytes [tier B]
bool nn::pia::session::RelayRouteManageJob::Startup(nn::pia::StationIndex stationIndex, unsigned int version, unsigned int sequence)
{
    if (IsRunning() || stationIndex >= m_StationNum || version == 0 || m_Version > version) {
        return false;
    }
    if (m_Version == version && m_pReportSequences[stationIndex] >= sequence) {
        return false;
    }
    m_NextCheckTime = common::Scheduler::s_pInstance->m_DispatchTime;
    Reset(true);
    SetStep(&RelayRouteManageJob::WaitAllDirectConnectionReport, "RelayRouteManageJob::WaitAllDirectConnectionReport");
    return true;
}

// 0x0043A6C4 | fefates:bytes [tier B]
nn::pia::session::RelayRouteManageJob::RelayRouteManageJob()
    : m_Version(0), m_DirectionsVersion(0), m_NextCheckTime()
{
    m_StationNum = transport::Transport::s_pInstance->m_StationNum;
    m_pRttTable = common::NewArray<u8>(m_StationNum * m_StationNum);
    m_pReportFormats = common::NewArray<u8>(m_StationNum);
    m_pReportSequences = common::NewArray<u32>(m_StationNum);
    memset(m_pRttTable, 0, m_StationNum * m_StationNum);
    memset(m_pReportFormats, 0, m_StationNum);
    memset(m_pReportSequences, 0, m_StationNum * sizeof(u32));
    m_StationBitmap = 0;
    m_KickoutStationBitmap = 0;
    m_pKickoutReasons = common::NewArray<u8>(m_StationNum);
    // (the reasons stay as they are: the formats are cleared once more)
    memset(m_pReportFormats, 0, m_StationNum);
}

// 0x0043A904
// 0x0043A860 (deleting dtor)
nn::pia::session::RelayRouteManageJob::~RelayRouteManageJob()
{
    if (m_pRttTable != nullptr) {
        common::DeleteArray(m_pRttTable);
    }
    if (m_pReportFormats != nullptr) {
        common::DeleteArray(m_pReportFormats);
    }
    if (m_pReportSequences != nullptr) {
        common::DeleteArray(m_pReportSequences);
    }
    if (m_pKickoutReasons != nullptr) {
        common::DeleteArray(m_pKickoutReasons);
    }
}

// 0x00733928 slot 0x14
void nn::pia::session::RelayRouteManageJob::Trace(unsigned long long) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
