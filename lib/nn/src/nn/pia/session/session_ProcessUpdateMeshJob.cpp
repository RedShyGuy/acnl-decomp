#include "nn/pia/session/session_ProcessUpdateMeshJob.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_JoinMeshJob.h"
#include "nn/pia/session/session_LeaveMeshJob.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshProtocol.h"
#include "nn/pia/session/session_ProcessHostMigrationJob.h"
#include "nn/pia/transport/transport_ConnectStationJob.h"
#include "nn/pia/transport/transport_DisconnectStationJob.h"
#include "nn/pia/transport/transport_IdentificationInfoTable.h"
#include "nn/pia/transport/transport_MissingStationHandler.h"
#include "nn/pia/transport/transport_ProcessConnectionRequestJob.h"
#include "nn/pia/transport/transport_RelayRouteManager.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_StationProtocol.h"
#include "nn/pia/transport/transport_Transport.h"
#include <string.h>

namespace nn {
namespace pia {
namespace session {
namespace {
const u64 TRACE_FLAG = 0x200000000ULL;
// the header of the station data list (MeshProtocol::SendStationDataList)
const u32 STATION_DATA_LIST_HEADER_SIZE = 12;
// the time limits of the update
const s32 TIME_LIMIT_MSEC = 25000;
const s32 SHORT_TIME_LIMIT_MSEC = 20000;
const s32 TIME_LIMIT_8_STATIONS_MSEC = 40000;
const s32 TIME_LIMIT_12_STATIONS_MSEC = 50000;
const s32 TIME_LIMIT_MANY_STATIONS_MSEC = 60000;
const s32 RELAY_EXTRA_TIME_LIMIT_MSEC = 10000;
// the interval of the failure notices and of the connection checks
const s32 FAILURE_NOTICE_INTERVAL_MSEC = 500;
const s32 CONNECTION_CHECK_INTERVAL_MSEC = 300;
// Mesh::m_RelayMode
const u8 RELAY_MODE_DIRECT_REPORT = 1;
const u8 RELAY_MODE_RELAY = 2;
// the mapping of a NAT that maps endpoint dependent (StationLocation::m_NatMapping)
const u8 NAT_MAPPING_EDM = 2;
// the failure reasons of the connections (MeshProtocol::SendConnectionFailureNotice): 1 to 9, 16
// more in a network with relay routes
const u8 FAILURE_REASON_RELAY_OFFSET = 16;

// (inline; names are ours)
DECOMP_ALWAYS_INLINE u8 GetConnectionFailureReason(const nn::Result& result)
{
    if (result == common::RESULT_CANCELED) {
        return 1;
    } else if (result == common::RESULT_STATION_CONNECTION_FAILED_E7) {
        return 2;
    } else if (result == common::RESULT_STATION_CONNECTION_FAILED_EA) {
        return 3;
    } else if (result == common::RESULT_STATION_CONNECTION_FAILED_EB) {
        return 5;
    } else if (result == common::RESULT_STATION_CONNECTION_FAILED_EC) {
        return 6;
    } else if (result == common::RESULT_STATION_CONNECTION_FAILED_ED) {
        return 7;
    } else if (result == common::RESULT_STATION_CONNECTION_FAILED_F2) {
        return 8;
    } else if (result == common::RESULT_TIMEOUT) {
        return 9;
    }
    return 1;
}

DECOMP_ALWAYS_INLINE void DestroyStation(transport::Station* pStation)
{
    pStation->m_pDisconnectStationJob->OnDisconnected(pStation);
    transport::StationConnectionInfoTable::s_pInstance->EraseFromTable(pStation);
    transport::IdentificationInfoTable::s_pInstance->EraseFromTable(pStation);
    pStation->Cleanup();
    pStation->CleanupJobs();
    transport::StationManager::s_pInstance->DestroyStation(pStation);
}

DECOMP_ALWAYS_INLINE void DecrementStationNum()
{
    if (Mesh::s_pInstance->m_StationNum != 0) {
        Mesh::s_pInstance->m_StationNum--;
    }
}

DECOMP_ALWAYS_INLINE common::Time GetTimeAfter(s32 msec)
{
    return common::Scheduler::s_pInstance->m_DispatchTime + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * msec);
}
} // namespace

// 0x0043AD8C (name is ours)
bool nn::pia::session::ProcessUpdateMeshJob::UpdateStations()
{
    if (common::Scheduler::s_pInstance->m_DispatchTime >= m_Deadline) {
        return false;
    }
    Mesh* pMesh = Mesh::s_pInstance;
    StationIndex localStationIndex = pMesh->m_LocalStationIndex;
    StationIndex hostStationIndex = pMesh->m_HostStationIndex;
    if (localStationIndex > STATION_INDEX_MAX || hostStationIndex > STATION_INDEX_MAX) {
        return false;
    }
    // we connect to the stations with a lower index
    bool isConnecting = pMesh->m_RelayMode != RELAY_MODE_DIRECT_REPORT && !m_IsSameVersion;
    if (transport::StationManager::s_pInstance->m_pLocalStation == nullptr) {
        return false;
    }
    transport::StationConnectionInfoTable* pTable = transport::StationConnectionInfoTable::s_pInstance;
    u32 listBitmap = 0;
    bool isChanged = false;
    transport::StationConnectionInfo info;
    for (u32 i = 0; i < m_StationNum; i++) {
        listBitmap |= 1 << m_pStationIndices[i];
        // the station with the index now
        if (pMesh->CheckStationIndexIsValid(m_pStationIndices[i])) {
            transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(m_pStationIndices[i]);
            if (pStation != nullptr) {
                if (pTable->GetStationConnectionInfo(pStation, &info).IsFailure()) {
                    pStation->Trace(TRACE_FLAG);
                } else {
                    if (info == m_pStationConnectionInfos[i]) {
                        continue;
                    }
                    // another station has the index now
                    if (m_pStationIndices[i] == localStationIndex) {
                        if (!m_IsJoining) {
                            return false;
                        }
                        continue;
                    }
                    ReleaseCallContext(m_pStationIndices[i]);
                    pMesh->UnfixDisconnectedId(m_pStationIndices[i]);
                    DecrementStationNum();
                    DestroyStation(pStation);
                    isChanged = true;
                }
            }
        }

        // the station of the entry
        transport::Station* pStation = pTable->GetStation(m_pStationConnectionInfos[i]);
        if (pStation == nullptr && m_pStationConnectionInfos[i].m_PublicLocation.m_StationAddress.IsValid()) {
            pStation = transport::StationManager::s_pInstance->GetStation(m_pStationConnectionInfos[i].m_PublicLocation.m_StationAddress);
            if (pStation == nullptr ||
                (pStation == transport::StationManager::s_pInstance->m_pLocalStation && m_pStationIndices[i] != Mesh::s_pInstance->m_LocalStationIndex) ||
                pTable->GetStationConnectionInfo(pStation, &info).IsFailure()) {
                pStation = pTable->GetStationPartialMatch(m_pStationConnectionInfos[i].m_PublicLocation,
                                                          transport::StationConnectionInfoTable::PARTIAL_MATCH_MODE_STATION_KEY);
                if (pStation != nullptr && (pStation->m_State == transport::Station::STATION_STATE_CONNECTED ||
                                            pStation->m_State == transport::Station::STATION_STATE_DISCONNECTED)) {
                    // an old connection of it
                    pStation->Trace(TRACE_FLAG);
                    ReleaseCallContext(m_pStationIndices[i]);
                    if (pStation->m_StationIndex != m_pStationIndices[i] && pStation->m_StationIndex <= STATION_INDEX_MAX) {
                        pMesh->UnfixDisconnectedId(pStation->m_StationIndex);
                        DecrementStationNum();
                    }
                    DestroyStation(pStation);
                    isChanged = true;
                    pStation = nullptr;
                }
            }
        }
        if (pStation != nullptr) {
            StationIndex stationIndex = pStation->m_StationIndex;
            if (stationIndex == m_pStationIndices[i] || stationIndex > STATION_INDEX_MAX) {
                if (pStation->m_State == transport::Station::STATION_STATE_CONNECTED && stationIndex > STATION_INDEX_MAX) {
                    isChanged = true;
                }
                pStation->m_StationIndex = m_pStationIndices[i];
                continue;
            }
            // it had another index
            if (pMesh->CheckStationIndexIsValid(stationIndex)) {
                if (pStation->m_StationIndex == localStationIndex) {
                    return false;
                }
                pMesh->UnfixDisconnectedId(pStation->m_StationIndex);
                DecrementStationNum();
            }
            DestroyStation(pStation);
            isChanged = true;
        }

        // a new station
        if (m_pStationIndices[i] == localStationIndex) {
            return false;
        }
        if (m_pStationIndices[i] == hostStationIndex &&
            (!Mesh::s_pInstance->m_IsHostMigrationEnabled || !Mesh::s_pInstance->m_pProcessHostMigrationJob->m_IsRunning)) {
            return false;
        }
        if (m_pStationIndices[i] < localStationIndex) {
            if (!isConnecting) {
                continue;
            }
            if (transport::StationManager::s_pInstance->m_ActiveStations.GetNum() >= transport::Transport::s_pInstance->m_StationNum) {
                return false;
            }
            transport::Station* pNewStation = transport::StationManager::s_pInstance->CreateStation();
            pNewStation->Startup(pMesh->m_pStationProtocol, m_pStationIndices[i], m_pStationConnectionInfos[i].m_PublicLocation.m_StationAddress);
            transport::ConnectStationJob* pJob = pNewStation->m_pConnectStationJob;
            u8 callContextIndex = GetFreeCallContextIndex();
            if (pJob->Startup(&m_pCallContexts[callContextIndex], pNewStation, m_pStationConnectionInfos[i], false,
                              pNewStation->m_pStationProtocol->m_ProcessTimeoutMSec)
                    .IsFailure()) {
                return false;
            }
            pTable->AddToTable(pNewStation, m_pStationConnectionInfos[i]);
            pNewStation->m_State = transport::Station::STATION_STATE_2;
            m_CallContextIndices[m_pStationIndices[i]] = callContextIndex;
            pJob->Ready(false);
        }
        // (for the stations with a higher index, which connect to us, the original computes the
        // longest time to wait for them here, 2500 ms per index of ours behind a NAT with an
        // endpoint dependent mapping, else 5000 ms, but never uses it)
    }

    // the stations not in the list any more
    u32 lostBitmap = pMesh->m_StationBitmap & ~listBitmap;
    if (lostBitmap != 0) {
        u32 bit = 1;
        for (u8 i = 0; i <= STATION_INDEX_MAX; i++, bit <<= 1) {
            StationIndex stationIndex = static_cast<StationIndex>(i);
            if ((lostBitmap & bit) == 0) {
                continue;
            }
            if (pMesh->m_LocalStationIndex == stationIndex) {
                if (!m_IsJoining) {
                    return false;
                }
                continue;
            }
            pMesh->UnfixDisconnectedId(stationIndex);
            if (Mesh::s_pInstance->m_StationNum != 0) {
                pMesh->m_StationNum--;
            }
            transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(stationIndex);
            if (pStation != nullptr) {
                DestroyStation(pStation);
            }
            if (transport::Transport::s_pInstance->m_pRelayRouteManager != nullptr) {
                transport::Transport::s_pInstance->m_pRelayRouteManager->SearchRefugeRelayRoute(stationIndex);
            }
        }
        isChanged = true;
    }

    // the connections to stations that are not in the list
    transport::Station* pOldStations[STATION_INDEX_MAX + 1] = {};
    u32 oldStationNum = 0;
    transport::StationManager* pManager = transport::StationManager::s_pInstance;
    for (transport::Station** it = pManager->m_ActiveStations.Begin(); it != pManager->m_ActiveStations.End(); it++) {
        if (*it == pManager->m_pLocalStation || (*it)->m_State == transport::Station::STATION_STATE_DISCONNECTED) {
            continue;
        }
        if (pTable->GetStationConnectionInfo(*it, &info).IsSuccess()) {
            bool isInList = false;
            for (u32 i = 0; i < m_StationNum; i++) {
                if (info == m_pStationConnectionInfos[i]) {
                    isInList = true;
                    break;
                }
            }
            if (isInList) {
                continue;
            }
        }
        pOldStations[oldStationNum++] = *it;
    }
    for (u32 i = 0; i < oldStationNum; i++) {
        pOldStations[i]->Trace(TRACE_FLAG);
        if (pOldStations[i]->m_pConnectStationJob->IsRunning() || pOldStations[i]->m_pProcessConnectionRequestJob->IsRunning() ||
            pOldStations[i]->m_State == transport::Station::STATION_STATE_CONNECTED) {
            continue;
        }
        StationIndex stationIndex = pOldStations[i]->m_StationIndex;
        pOldStations[i]->m_pDisconnectStationJob->OnDisconnected(pOldStations[i]);
        transport::StationConnectionInfoTable::s_pInstance->EraseFromTable(pOldStations[i]);
        transport::IdentificationInfoTable::s_pInstance->EraseFromTable(pOldStations[i]);
        pOldStations[i]->Cleanup();
        pOldStations[i]->CleanupJobs();
        pManager->DestroyStation(pOldStations[i]);
        transport::Station* pStation = pManager->GetStation(stationIndex);
        if (stationIndex <= STATION_INDEX_MAX && (pStation == nullptr || pStation->m_State == transport::Station::STATION_STATE_CONNECTED)) {
            ReleaseCallContext(stationIndex);
        }
        isChanged = true;
    }
    if (isChanged) {
        transport::Transport::s_pInstance->OutputStreamUpdateEvent();
    }
    return true;
}

// 0x0043B7BC
nn::pia::common::ExecuteResult nn::pia::session::ProcessUpdateMeshJob::UpdateFailed()
{
    Mesh* pMesh = Mesh::s_pInstance;
    if (m_IsJoining) {
        // the join fails: leave the mesh
        if (m_CallContext.GetState() != common::CallContext::STATE_CALL_IN_PROGRESS && pMesh->m_pLeaveMeshJob->Startup(&m_CallContext)) {
            Mesh::s_pInstance->m_pLeaveMeshJob->Ready(false);
            SetStep(&ProcessUpdateMeshJob::WaitLeaveMesh, "ProcessUpdateMeshJob::WaitLeaveMesh");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        Mesh::s_pInstance->m_DisconnectReason = Mesh::DISCONNECT_REASON_LEAVE;
        Mesh::s_pInstance->EndMonitoring(Mesh::DISCONNECT_REASON_LEAVE);
    } else {
        pMesh->m_DisconnectReason = Mesh::DISCONNECT_REASON_6;
        pMesh->EndMonitoring(Mesh::DISCONNECT_REASON_6);
    }
    SetStep(&ProcessUpdateMeshJob::CleanupByProcessFailure, "ProcessUpdateMeshJob::CleanupByProcessFailure");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0043B8E0 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::ProcessUpdateMeshJob::CalcTimeLimit(bool isSameVersion)
{
    if (isSameVersion) {
        m_TimeLimitMSec = m_ShortTimeLimitMSec;
        m_DirectConnectionTimeLimitMSec = m_ShortTimeLimitMSec;
    } else {
        s32 timeLimit;
        if (m_StationNum <= 8) {
            timeLimit = TIME_LIMIT_8_STATIONS_MSEC;
        } else if (m_StationNum <= 12) {
            timeLimit = TIME_LIMIT_12_STATIONS_MSEC;
        } else {
            timeLimit = TIME_LIMIT_MANY_STATIONS_MSEC;
        }
        m_TimeLimitMSec = timeLimit;
        m_DirectConnectionTimeLimitMSec = timeLimit;
        if (Mesh::s_pInstance->m_RelayMode == RELAY_MODE_RELAY) {
            m_TimeLimitMSec += RELAY_EXTRA_TIME_LIMIT_MSEC;
        }
    }
    m_DirectConnectionDeadline = GetTimeAfter(m_DirectConnectionTimeLimitMSec);
    m_Deadline = GetTimeAfter(m_TimeLimitMSec);
    if (!isSameVersion && !m_IsJoining) {
        m_Deadline += common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * m_ShortTimeLimitMSec);
    }
}

// 0x0043BA24
nn::pia::common::ExecuteResult nn::pia::session::ProcessUpdateMeshJob::WaitLeaveMesh()
{
    if (m_CallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&ProcessUpdateMeshJob::CleanupByProcessFailure, "ProcessUpdateMeshJob::CleanupByProcessFailure");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0043BAA8 | fefates:bytes [tier B]
void nn::pia::session::ProcessUpdateMeshJob::ClearStationIndex(nn::pia::StationIndex stationIndex)
{
    if (stationIndex <= STATION_INDEX_MAX) {
        ReleaseCallContext(stationIndex);
    }
}

// 0x0043BACC
void nn::pia::session::ProcessUpdateMeshJob::SetMonitoringData()
{
    common::g_SessionStateMonitoringContent.m_Unknown0x344 = m_SameVersionUpdateNum;
}

// 0x0043BAE0
nn::pia::common::ExecuteResult nn::pia::session::ProcessUpdateMeshJob::CheckConnectionAll()
{
    common::Time now = common::Scheduler::s_pInstance->m_DispatchTime;
    Mesh* pMesh = Mesh::s_pInstance;
    if (now >= m_Deadline) {
        pMesh->m_pJoinMeshJob->m_Result = common::RESULT_TIMEOUT;
        SetStep(&ProcessUpdateMeshJob::UpdateFailed, "ProcessUpdateMeshJob::UpdateFailed");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    StationIndex localStationIndex = pMesh->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX) {
        SetStep(&ProcessUpdateMeshJob::UpdateFailed, "ProcessUpdateMeshJob::UpdateFailed");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    bool isNoticeTime = m_NextFailureNoticeTime < now;
    bool isChanged = false;
    bool isWaiting = false;
    bool isFailed = false;
    for (u32 i = 0; i < m_StationNum; i++) {
        if (m_pStationIndices[i] == localStationIndex) {
            continue;
        }
        u8 reason = m_pConnectionFailureReasons[i];
        if (reason != 0 && reason <= FAILURE_REASON_RELAY_OFFSET) {
            // the host told that it could not connect
            SetJoinFailureResult(m_pStationIndices[i], reason, true);
            isWaiting = true;
            isFailed = true;
            continue;
        }
        transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(m_pStationIndices[i]);
        if (pStation == nullptr) {
            pStation = transport::StationConnectionInfoTable::s_pInstance->GetStation(m_pStationConnectionInfos[i]);
            if (pStation != nullptr) {
                if (pStation->m_State == transport::Station::STATION_STATE_CONNECTED && pStation->m_StationIndex > STATION_INDEX_MAX) {
                    isChanged = true;
                }
                pStation->m_StationIndex = m_pStationIndices[i];
            }
        }
        u8 callContextIndex = m_CallContextIndices[m_pStationIndices[i]];
        if (callContextIndex < m_StationNumMax) {
            common::CallContext* pCallContext = &m_pCallContexts[callContextIndex];
            if (!pCallContext->IsFinished()) {
                isWaiting = true;
            } else if (pCallContext->m_Result.IsSuccess()) {
                m_CallContextIndices[m_pStationIndices[i]] = INVALID_CALL_CONTEXT_INDEX;
                if (pStation == nullptr) {
                    isWaiting = true;
                }
            } else {
                reason = GetConnectionFailureReason(pCallContext->m_Result);
                SetJoinFailureResult(m_pStationIndices[i], reason, false);
                isWaiting = true;
                isFailed = true;
                if (isNoticeTime) {
                    pMesh->m_pMeshProtocol->SendConnectionFailureNotice(pMesh->m_HostStationIndex, m_pStationIndices[i], localStationIndex, reason, m_Version);
                    m_NextFailureNoticeTime = GetTimeAfter(FAILURE_NOTICE_INTERVAL_MSEC);
                }
            }
        } else if (pStation == nullptr || pStation->m_State != transport::Station::STATION_STATE_CONNECTED) {
            isWaiting = true;
        }
    }
    if (m_IsJoining && isFailed) {
        SetStep(&ProcessUpdateMeshJob::UpdateFailed, "ProcessUpdateMeshJob::UpdateFailed");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (isWaiting) {
        if (isChanged) {
            transport::Transport::s_pInstance->OutputStreamUpdateEvent();
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    // all are connected
    for (u32 i = 0; i < m_StationNum; i++) {
        if (!Mesh::s_pInstance->CheckStationIndexIsValid(m_pStationIndices[i])) {
            Mesh::s_pInstance->FixConnectedId(m_pStationIndices[i]);
        }
    }
    Mesh::s_pInstance->m_StationNum = m_StationNum;
    m_Version = 0;
    m_IsProcessing = false;
    ClearPartData();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0043BF90 | fefates:bytes [tier B]
u32 nn::pia::session::ProcessUpdateMeshJob::SetStationDataList(const unsigned char* pData, unsigned int)
{
    u32 infoSize = m_pStationConnectionInfos->GetSerializedSize();
    // the info, the index, aligned to 4 bytes
    u32 entrySize = infoSize + (3 - (infoSize & 3)) + 1;
    u8 hostEntry = pData[2];
    u8 stationNum = pData[1];
    u32 version = common::deserializeU32(pData + 4);
    if (stationNum > m_StationNumMax) {
        return UPDATE_STATE_INVALID;
    }
    const u8* pEntries = pData + STATION_DATA_LIST_HEADER_SIZE;
    u8 partNum = pData[8];
    if (partNum == 1) {
        if (m_IsPartReceived[0] || m_IsPartReceived[1]) {
            if (m_PartVersion > version) {
                return UPDATE_STATE_OLD;
            }
            ClearPartData();
        }
        for (int i = 0; i < stationNum; i++) {
            const u8* pEntry = pEntries + entrySize * i;
            StationIndex stationIndex = static_cast<StationIndex>(pEntry[infoSize]);
            m_pStationConnectionInfos[i].Deserialize(pEntry);
            m_pStationIndices[i] = stationIndex;
        }
        m_Version = version;
        m_StationNum = stationNum;
    } else if (partNum == 2) {
        u8 partIndex = pData[9];
        if (m_IsPartReceived[0] || m_IsPartReceived[1]) {
            if (m_PartStationNum != stationNum || m_PartHostEntry != hostEntry || m_PartVersion != version || m_PartNum != partNum) {
                // another list
                if (m_PartVersion >= version) {
                    return UPDATE_STATE_OLD;
                }
                m_PartVersion = version;
                m_IsPartReceived[0] = false;
                m_IsPartReceived[1] = false;
                m_PartStationNum = stationNum;
                m_PartHostEntry = hostEntry;
                m_PartNum = partNum;
            }
            if (m_IsPartReceived[partIndex]) {
                return UPDATE_STATE_PART_PENDING;
            }
        } else {
            m_PartStationNum = stationNum;
            m_PartVersion = version;
            m_PartHostEntry = hostEntry;
            m_PartNum = partNum;
        }
        u8 entryNum = pData[10];
        u8 firstEntry = pData[11];
        for (int i = 0; i < entryNum; i++) {
            const u8* pEntry = pEntries + entrySize * i;
            m_pStationConnectionInfos[firstEntry + i].Deserialize(pEntry);
            m_pStationIndices[firstEntry + i] = static_cast<StationIndex>(pEntry[infoSize]);
        }
        m_IsPartReceived[partIndex] = true;
        if (!m_IsPartReceived[0] || !m_IsPartReceived[1]) {
            return UPDATE_STATE_PART_PENDING;
        }
        m_Version = m_PartVersion;
        m_StationNum = m_PartStationNum;
        ClearPartData();
    } else {
        return UPDATE_STATE_INVALID;
    }
    for (u32 i = 0; i < m_StationNumMax; i++) {
        m_pConnectionFailureReasons[i] = 0;
    }
    m_NextFailureNoticeTime = common::Scheduler::s_pInstance->m_DispatchTime;
    return UPDATE_STATE_ACCEPTED;
}

// 0x0043C2F8 | fefates:bytes [tier B]
void nn::pia::session::ProcessUpdateMeshJob::UpdateDataTakeover(unsigned int stationNum)
{
    u32 oldStationNum = m_StationNum;
    m_StationNum = stationNum;
    // (not used)
    transport::StationConnectionInfo info;
    StationIndex oldStationIndices[STATION_INDEX_MAX + 1];
    for (u32 i = 0; i < m_StationNumMax; i++) {
        oldStationIndices[i] = m_pStationIndices[i];
        m_pStationIndices[i] = m_pNewStationIndices[i];
    }
    for (u32 i = 0; i < oldStationNum; i++) {
        bool isKept = false;
        for (u32 j = 0; j < m_StationNum; j++) {
            if (oldStationIndices[i] == m_pNewStationIndices[j]) {
                isKept = m_pStationConnectionInfos[i] == m_pNewStationConnectionInfos[j];
                break;
            }
        }
        if (isKept) {
            // the same station: its finished connection is not needed any more
            transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(oldStationIndices[i]);
            if (pStation == nullptr) {
                pStation = transport::StationConnectionInfoTable::s_pInstance->GetStation(m_pStationConnectionInfos[i]);
            }
            if (pStation == nullptr || pStation->m_State == transport::Station::STATION_STATE_DISCONNECTED) {
                u8 callContextIndex = m_CallContextIndices[oldStationIndices[i]];
                if (callContextIndex < m_StationNumMax && m_pCallContexts[callContextIndex].IsFinished()) {
                    m_CallContextIndices[oldStationIndices[i]] = INVALID_CALL_CONTEXT_INDEX;
                }
            }
            continue;
        }
        if (Mesh::s_pInstance->CheckStationIndexIsValid(oldStationIndices[i])) {
            ReleaseCallContext(oldStationIndices[i]);
            continue;
        }
        // a station that is gone: stop connecting to it
        u8 callContextIndex = m_CallContextIndices[oldStationIndices[i]];
        transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(oldStationIndices[i]);
        if (pStation == nullptr) {
            pStation = transport::StationConnectionInfoTable::s_pInstance->GetStation(m_pStationConnectionInfos[i]);
            if (pStation == nullptr) {
                if (callContextIndex < m_StationNumMax) {
                    m_CallContextIndices[oldStationIndices[i]] = INVALID_CALL_CONTEXT_INDEX;
                    transport::MissingStationHandler* pHandler = Mesh::s_pInstance->m_pMissingStationHandler;
                    if (pHandler != nullptr) {
                        pHandler->Execute(&m_pStationConnectionInfos[i]);
                    }
                }
                continue;
            }
        }
        if (callContextIndex < m_StationNumMax) {
            m_pCallContexts[callContextIndex].Cancel();
            m_CallContextIndices[oldStationIndices[i]] = INVALID_CALL_CONTEXT_INDEX;
        }
        DestroyStation(pStation);
        transport::Transport::s_pInstance->OutputStreamUpdateEvent();
    }
    for (u32 i = 0; i < m_StationNum; i++) {
        m_pStationConnectionInfos[i] = m_pNewStationConnectionInfos[i];
    }
}

// 0x0043C678
nn::pia::common::ExecuteResult nn::pia::session::ProcessUpdateMeshJob::SendConnectionReport()
{
    Mesh* pMesh = Mesh::s_pInstance;
    if (common::Scheduler::s_pInstance->m_DispatchTime >= m_Deadline) {
        pMesh->m_pJoinMeshJob->m_Result = common::RESULT_TIMEOUT;
        SetStep(&ProcessUpdateMeshJob::UpdateFailed, "ProcessUpdateMeshJob::UpdateFailed");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (pMesh->m_LocalStationIndex > STATION_INDEX_MAX || pMesh->m_HostStationIndex > STATION_INDEX_MAX) {
        SetStep(&ProcessUpdateMeshJob::UpdateFailed, "ProcessUpdateMeshJob::UpdateFailed");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    u32 noRouteBitmap = 0;
    u32 sequence = m_ReportSequence + 1;
    if (pMesh->m_pMeshProtocol->SendConnectionReport(pMesh->m_HostStationIndex, m_Version, sequence, &noRouteBitmap)) {
        m_ReportSequence = sequence;
        SetStep(&ProcessUpdateMeshJob::StartRelayConnection, "ProcessUpdateMeshJob::StartRelayConnection");
    } else if (noRouteBitmap != 0 && common::Scheduler::s_pInstance->m_DispatchTime >= m_DirectConnectionDeadline) {
        // the stations without a round trip time are asked now and then
        for (int i = 0; i <= STATION_INDEX_MAX; i++) {
            if (noRouteBitmap & 1) {
                Mesh::s_pInstance->m_pMeshProtocol->SendConnectionCheck(static_cast<StationIndex>(i));
            }
            noRouteBitmap >>= 1;
            if (noRouteBitmap == 0) {
                break;
            }
        }
        m_DirectConnectionDeadline = GetTimeAfter(CONNECTION_CHECK_INTERVAL_MSEC);
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0043C88C
nn::pia::common::ExecuteResult nn::pia::session::ProcessUpdateMeshJob::StartRelayConnection()
{
    common::Time now = common::Scheduler::s_pInstance->m_DispatchTime;
    Mesh* pMesh = Mesh::s_pInstance;
    if (now >= m_Deadline) {
        pMesh->m_pJoinMeshJob->m_Result = common::RESULT_TIMEOUT;
        SetStep(&ProcessUpdateMeshJob::UpdateFailed, "ProcessUpdateMeshJob::UpdateFailed");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    StationIndex localStationIndex = pMesh->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX) {
        SetStep(&ProcessUpdateMeshJob::UpdateFailed, "ProcessUpdateMeshJob::UpdateFailed");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    transport::RelayRouteManager* pRelayRouteManager = transport::Transport::s_pInstance->m_pRelayRouteManager;
    if (pRelayRouteManager->m_DirectionsVersionHigh != m_Version || pRelayRouteManager->m_DirectionsVersionLow <= m_DirectionsVersion) {
        // no new directions of the host yet
        bool isNoticeTime = m_NextFailureNoticeTime < now;
        for (u32 i = 0; i < m_StationNum; i++) {
            if (m_pStationIndices[i] == localStationIndex) {
                continue;
            }
            u8 callContextIndex = m_CallContextIndices[m_pStationIndices[i]];
            if (callContextIndex >= m_StationNumMax) {
                continue;
            }
            common::CallContext* pCallContext = &m_pCallContexts[callContextIndex];
            if (!pCallContext->IsFinished() || pCallContext->m_Result.IsSuccess()) {
                continue;
            }
            // (no case for the timeout here)
            u8 reason = FAILURE_REASON_RELAY_OFFSET + 1;
            if (pCallContext->m_Result == common::RESULT_STATION_CONNECTION_FAILED_E7) {
                reason = FAILURE_REASON_RELAY_OFFSET + 2;
            } else if (pCallContext->m_Result == common::RESULT_STATION_CONNECTION_FAILED_EA) {
                reason = FAILURE_REASON_RELAY_OFFSET + 3;
            } else if (pCallContext->m_Result == common::RESULT_STATION_CONNECTION_FAILED_EB) {
                reason = FAILURE_REASON_RELAY_OFFSET + 5;
            } else if (pCallContext->m_Result == common::RESULT_STATION_CONNECTION_FAILED_EC) {
                reason = FAILURE_REASON_RELAY_OFFSET + 6;
            } else if (pCallContext->m_Result == common::RESULT_STATION_CONNECTION_FAILED_ED) {
                reason = FAILURE_REASON_RELAY_OFFSET + 7;
            } else if (pCallContext->m_Result == common::RESULT_STATION_CONNECTION_FAILED_F2) {
                reason = FAILURE_REASON_RELAY_OFFSET + 8;
            }
            if (isNoticeTime) {
                Mesh::s_pInstance->m_pMeshProtocol->SendConnectionFailureNotice(Mesh::s_pInstance->m_HostStationIndex, m_pStationIndices[i],
                                                                                localStationIndex, reason, m_Version);
                m_NextFailureNoticeTime = GetTimeAfter(FAILURE_NOTICE_INTERVAL_MSEC);
            }
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }

    // the finished connections are not needed any more
    for (u32 i = 0; i < m_StationNum; i++) {
        if (m_pStationIndices[i] == localStationIndex) {
            continue;
        }
        u8 callContextIndex = m_CallContextIndices[m_pStationIndices[i]];
        if (callContextIndex < m_StationNumMax && m_pCallContexts[callContextIndex].IsFinished()) {
            m_CallContextIndices[m_pStationIndices[i]] = INVALID_CALL_CONTEXT_INDEX;
        }
    }
    // connect to the stations without a direct connection over their relay route
    bool isChanged = false;
    for (u32 i = 0; i < m_StationNum; i++) {
        if (m_pStationIndices[i] == localStationIndex) {
            continue;
        }
        transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(m_pStationIndices[i]);
        if (pStation == nullptr) {
            pStation = transport::StationConnectionInfoTable::s_pInstance->GetStation(m_pStationConnectionInfos[i]);
            if (pStation != nullptr) {
                pStation->Trace(TRACE_FLAG);
                if (pStation->m_State == transport::Station::STATION_STATE_DISCONNECTED) {
                    DestroyStation(pStation);
                    isChanged = true;
                    pStation = nullptr;
                } else {
                    if (pStation->m_State == transport::Station::STATION_STATE_CONNECTED && pStation->m_StationIndex > STATION_INDEX_MAX) {
                        isChanged = true;
                    }
                    pStation->m_StationIndex = m_pStationIndices[i];
                }
            }
        }
        StationIndex relayStationIndex;
        if (pRelayRouteManager->GetRelayRoute(localStationIndex, m_pStationIndices[i], &relayStationIndex).IsFailure()) {
            if (pStation != nullptr) {
                pStation->Trace(TRACE_FLAG);
            }
            continue;
        }
        if (relayStationIndex == m_pStationIndices[i] || relayStationIndex == localStationIndex) {
            // a direct route
            continue;
        }
        if (pStation != nullptr && pStation->m_State == transport::Station::STATION_STATE_CONNECTED) {
            if (pStation->IsConnectionRouteRelay()) {
                continue;
            }
            DestroyStation(pStation);
            isChanged = true;
            pStation = nullptr;
        }
        if (m_pStationIndices[i] >= localStationIndex) {
            continue;
        }
        if (pStation == nullptr) {
            if (transport::StationManager::s_pInstance->m_ActiveStations.GetNum() >= transport::Transport::s_pInstance->m_StationNum) {
                continue;
            }
            pStation = transport::StationManager::s_pInstance->CreateStation();
            pStation->Startup(Mesh::s_pInstance->m_pStationProtocol, m_pStationIndices[i], m_pStationConnectionInfos[i].m_PublicLocation.m_StationAddress);
        }
        u8 callContextIndex = m_CallContextIndices[m_pStationIndices[i]];
        if (callContextIndex < m_StationNumMax && !m_pCallContexts[callContextIndex].IsFinished()) {
            if (pStation->m_State == transport::Station::STATION_STATE_2 || pStation->m_State == transport::Station::STATION_STATE_CONNECTING ||
                pStation->m_State == transport::Station::STATION_STATE_CONNECTED) {
                continue;
            }
            m_pCallContexts[callContextIndex].SignalCancel();
            m_pCallContexts[callContextIndex].Reset();
        }
        transport::ConnectStationJob* pJob = pStation->m_pConnectStationJob;
        u8 newCallContextIndex = GetFreeCallContextIndex();
        if (pJob->StartupRelayConnection(&m_pCallContexts[newCallContextIndex], pStation, m_pStationConnectionInfos[i], false,
                                         pStation->m_pStationProtocol->m_ProcessTimeoutMSec)
                .IsFailure()) {
            continue;
        }
        transport::StationConnectionInfoTable::s_pInstance->AddToTable(pStation, m_pStationConnectionInfos[i]);
        pStation->m_State = transport::Station::STATION_STATE_2;
        m_CallContextIndices[m_pStationIndices[i]] = newCallContextIndex;
        pJob->Ready(false);
    }
    if (isChanged) {
        transport::Transport::s_pInstance->OutputStreamUpdateEvent();
    }
    m_DirectionsVersion = pRelayRouteManager->m_DirectionsVersionLow;
    Mesh::s_pInstance->m_pJoinMeshJob->m_HostPrincipalId = 0xFFFFFFFF;
    SetStep(&ProcessUpdateMeshJob::CheckConnectionAll, "ProcessUpdateMeshJob::CheckConnectionAll");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0043D030
nn::pia::common::ExecuteResult nn::pia::session::ProcessUpdateMeshJob::WaitDirectConnection()
{
    common::Time now = common::Scheduler::s_pInstance->m_DispatchTime;
    Mesh* pMesh = Mesh::s_pInstance;
    if (now >= m_Deadline) {
        pMesh->m_pJoinMeshJob->m_Result = common::RESULT_TIMEOUT;
        SetStep(&ProcessUpdateMeshJob::UpdateFailed, "ProcessUpdateMeshJob::UpdateFailed");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    StationIndex localStationIndex = pMesh->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX) {
        SetStep(&ProcessUpdateMeshJob::UpdateFailed, "ProcessUpdateMeshJob::UpdateFailed");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    bool isNoticeTime = m_NextFailureNoticeTime < now;
    bool isChanged = false;
    bool isWaiting = false;
    bool isMissing = false;
    for (u32 i = 0; i < m_StationNum; i++) {
        if (m_pStationIndices[i] == localStationIndex) {
            continue;
        }
        u8 reason = m_pConnectionFailureReasons[i];
        if (reason != 0) {
            // the host told that it could not connect
            SetJoinFailureResult(m_pStationIndices[i], reason, true);
            continue;
        }
        transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(m_pStationIndices[i]);
        if (pStation == nullptr) {
            pStation = transport::StationConnectionInfoTable::s_pInstance->GetStation(m_pStationConnectionInfos[i]);
            if (pStation != nullptr) {
                if (pStation->m_State == transport::Station::STATION_STATE_CONNECTED && pStation->m_StationIndex > STATION_INDEX_MAX) {
                    isChanged = true;
                }
                pStation->m_StationIndex = m_pStationIndices[i];
            }
        }
        u8 callContextIndex = m_CallContextIndices[m_pStationIndices[i]];
        if (callContextIndex < m_StationNumMax) {
            common::CallContext* pCallContext = &m_pCallContexts[callContextIndex];
            if (!pCallContext->IsFinished()) {
                isWaiting = true;
            } else if (pCallContext->m_Result.IsSuccess()) {
                m_CallContextIndices[m_pStationIndices[i]] = INVALID_CALL_CONTEXT_INDEX;
            } else {
                reason = FAILURE_REASON_RELAY_OFFSET + GetConnectionFailureReason(pCallContext->m_Result);
                SetJoinFailureResult(m_pStationIndices[i], reason, false);
                if (isNoticeTime) {
                    Mesh::s_pInstance->m_pMeshProtocol->SendConnectionFailureNotice(Mesh::s_pInstance->m_HostStationIndex, m_pStationIndices[i],
                                                                                    localStationIndex, reason, m_Version);
                    m_NextFailureNoticeTime = GetTimeAfter(FAILURE_NOTICE_INTERVAL_MSEC);
                }
            }
        } else if (pStation == nullptr) {
            isMissing = true;
        } else if (pStation->m_State != transport::Station::STATION_STATE_CONNECTED) {
            isWaiting = true;
        }
    }
    if (isChanged) {
        transport::Transport::s_pInstance->OutputStreamUpdateEvent();
    }
    if (isWaiting || (isMissing && common::Scheduler::s_pInstance->m_DispatchTime < m_DirectConnectionDeadline)) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    m_DirectConnectionDeadline = common::Scheduler::s_pInstance->m_DispatchTime;
    SetStep(&ProcessUpdateMeshJob::SendConnectionReport, "ProcessUpdateMeshJob::SendConnectionReport");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0043D490 | fefates:bytes-fuzzy [tier B]
u32 nn::pia::session::ProcessUpdateMeshJob::UpdateStationDataList(const unsigned char* pData, unsigned int)
{
    u32 infoSize = m_pStationConnectionInfos->GetSerializedSize();
    u32 entrySize = infoSize + (3 - (infoSize & 3)) + 1;
    u8 hostEntry = pData[2];
    u8 stationNum = pData[1];
    u32 version = common::deserializeU32(pData + 4);
    if (m_StationNumMax < stationNum || version <= m_Version) {
        return UPDATE_STATE_INVALID;
    }
    const u8* pEntries = pData + STATION_DATA_LIST_HEADER_SIZE;
    u8 partNum = pData[8];
    if (partNum == 1) {
        if (m_IsPartReceived[0] || m_IsPartReceived[1]) {
            if (m_PartVersion > version) {
                return UPDATE_STATE_OLD;
            }
            ClearPartData();
        }
        for (int i = 0; i < stationNum; i++) {
            const u8* pEntry = pEntries + entrySize * i;
            StationIndex stationIndex = static_cast<StationIndex>(pEntry[infoSize]);
            m_pNewStationConnectionInfos[i].Deserialize(pEntry);
            m_pNewStationIndices[i] = stationIndex;
        }
    } else if (partNum == 2) {
        u8 partIndex = pData[9];
        if (m_IsPartReceived[0] || m_IsPartReceived[1]) {
            if (m_PartStationNum != stationNum || m_PartHostEntry != hostEntry || m_PartVersion != version || m_PartNum != partNum) {
                if (m_PartVersion >= version) {
                    return UPDATE_STATE_OLD;
                }
                m_PartVersion = version;
                m_IsPartReceived[0] = false;
                m_IsPartReceived[1] = false;
                m_PartStationNum = stationNum;
                m_PartHostEntry = hostEntry;
                m_PartNum = partNum;
            }
            if (m_IsPartReceived[partIndex]) {
                return UPDATE_STATE_PART_PENDING;
            }
        } else {
            m_PartStationNum = stationNum;
            m_PartVersion = version;
            m_PartHostEntry = hostEntry;
            m_PartNum = partNum;
        }
        u8 entryNum = pData[10];
        u8 firstEntry = pData[11];
        for (int i = 0; i < entryNum; i++) {
            const u8* pEntry = pEntries + entrySize * i;
            m_pNewStationConnectionInfos[firstEntry + i].Deserialize(pEntry);
            m_pNewStationIndices[firstEntry + i] = static_cast<StationIndex>(pEntry[infoSize]);
        }
        m_IsPartReceived[partIndex] = true;
        if (!m_IsPartReceived[0] || !m_IsPartReceived[1]) {
            return UPDATE_STATE_PART_PENDING;
        }
        ClearPartData();
    } else {
        return UPDATE_STATE_INVALID;
    }
    if (!m_IsApplied) {
        return UPDATE_STATE_NOT_APPLIED;
    }
    UpdateDataTakeover(stationNum);
    m_IsApplied = UpdateStations();
    CalcTimeLimit(false);
    m_ReportSequence = 0;
    m_DirectionsVersion = 0;
    m_Version = version;
    for (u32 i = 0; i < m_StationNumMax; i++) {
        m_pConnectionFailureReasons[i] = 0;
    }
    m_NextFailureNoticeTime = common::Scheduler::s_pInstance->m_DispatchTime;
    m_IsSameVersion = false;
    if (m_IsApplied) {
        if (Mesh::s_pInstance->m_RelayMode == RELAY_MODE_RELAY) {
            SetStep(&ProcessUpdateMeshJob::WaitDirectConnection, "ProcessUpdateMeshJob::WaitDirectConnection");
        } else if (Mesh::s_pInstance->m_RelayMode == RELAY_MODE_DIRECT_REPORT) {
            SetStep(&ProcessUpdateMeshJob::SendConnectionReport, "ProcessUpdateMeshJob::SendConnectionReport");
        } else {
            SetStep(&ProcessUpdateMeshJob::CheckConnectionAll, "ProcessUpdateMeshJob::CheckConnectionAll");
        }
    } else {
        SetStep(&ProcessUpdateMeshJob::UpdateFailed, "ProcessUpdateMeshJob::UpdateFailed");
    }
    return UPDATE_STATE_ACCEPTED;
}

// 0x0043D91C
nn::pia::common::ExecuteResult nn::pia::session::ProcessUpdateMeshJob::CleanupByProcessFailure()
{
    Mesh::s_pInstance->CleanupStationsJobs();
    Mesh::s_pInstance->CleanupStatus();
    m_Version = 0;
    m_IsProcessing = false;
    ClearPartData();
    for (u32 i = 0; i < m_StationNumMax; i++) {
        if (m_pCallContexts[i].GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContexts[i].SignalCancel();
        }
        m_pCallContexts[i].Reset();
    }
    if (m_CallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_CallContext.SignalCancel();
    }
    m_CallContext.Reset();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0043D9CC | fefates:bytes [tier B]
void nn::pia::session::ProcessUpdateMeshJob::SetConnectionFailureNotice(nn::pia::StationIndex stationIndex, unsigned char reason)
{
    for (u32 i = 0; i < m_StationNumMax; i++) {
        if (m_pStationIndices[i] == stationIndex) {
            m_pConnectionFailureReasons[i] = reason;
            return;
        }
    }
}

// 0x0043DA14 (name is ours)
void nn::pia::session::ProcessUpdateMeshJob::ClearMonitoringData()
{
    m_SameVersionUpdateNum = 0;
}

// 0x0043DA20 (name is ours)
void nn::pia::session::ProcessUpdateMeshJob::SetJoinFailureResult(nn::pia::StationIndex stationIndex, u8 reason, bool isFromNotice)
{
    JoinMeshJob* pJob = Mesh::s_pInstance->m_pJoinMeshJob;
    nn::Result current = pJob->m_Result;
    // a result only replaces the less important ones
    if (reason == 2 || reason == FAILURE_REASON_RELAY_OFFSET + 2) {
        if (current != common::RESULT_STATION_CONNECTION_FAILED_EA && current != common::RESULT_STATION_CONNECTION_FAILED_ED &&
            current != common::RESULT_STATION_CONNECTION_FAILED_F2 && current != common::RESULT_STATION_CONNECTION_FAILED_EB &&
            current != common::RESULT_STATION_CONNECTION_FAILED_EC) {
            pJob->m_Result = common::RESULT_STATION_CONNECTION_FAILED_E7;
        }
    } else if (reason == 3 || reason == FAILURE_REASON_RELAY_OFFSET + 3) {
        if (current != common::RESULT_STATION_CONNECTION_FAILED_ED && current != common::RESULT_STATION_CONNECTION_FAILED_F2 &&
            current != common::RESULT_STATION_CONNECTION_FAILED_EB && current != common::RESULT_STATION_CONNECTION_FAILED_EC) {
            pJob->m_Result = common::RESULT_STATION_CONNECTION_FAILED_EA;
        }
    } else if (reason == 4 || reason == FAILURE_REASON_RELAY_OFFSET + 4) {
        if (current != common::RESULT_STATION_CONNECTION_FAILED_F2) {
            pJob->m_Result = common::RESULT_STATION_CONNECTION_FAILED_ED;
        }
    } else if (reason == 5 || reason == FAILURE_REASON_RELAY_OFFSET + 5) {
        if (current != common::RESULT_STATION_CONNECTION_FAILED_ED && current != common::RESULT_STATION_CONNECTION_FAILED_F2) {
            pJob->m_Result = isFromNotice ? common::RESULT_STATION_CONNECTION_FAILED_EC : common::RESULT_STATION_CONNECTION_FAILED_EB;
        }
    } else if (reason == 6 || reason == FAILURE_REASON_RELAY_OFFSET + 6) {
        if (current != common::RESULT_STATION_CONNECTION_FAILED_ED && current != common::RESULT_STATION_CONNECTION_FAILED_F2) {
            pJob->m_Result = isFromNotice ? common::RESULT_STATION_CONNECTION_FAILED_EB : common::RESULT_STATION_CONNECTION_FAILED_EC;
        }
    } else if (reason == 7 || reason == FAILURE_REASON_RELAY_OFFSET + 7) {
        if (current != common::RESULT_STATION_CONNECTION_FAILED_F2) {
            pJob->m_Result = common::RESULT_STATION_CONNECTION_FAILED_ED;
        }
    } else if (reason == 8 || reason == FAILURE_REASON_RELAY_OFFSET + 8) {
        pJob->m_Result = common::RESULT_STATION_CONNECTION_FAILED_F2;
    } else if (reason == 9 || reason == FAILURE_REASON_RELAY_OFFSET + 9) {
        pJob->m_Result = common::RESULT_TIMEOUT;
    }
    transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(stationIndex);
    if (pStation != nullptr) {
        Mesh::s_pInstance->m_pJoinMeshJob->m_HostPrincipalId = transport::StationConnectionInfoTable::s_pInstance->GetPrincipalIdByStation(pStation);
    } else {
        Mesh::s_pInstance->m_pJoinMeshJob->m_HostPrincipalId = 0xFFFFFFFF;
    }
}

// 0x0043DBB0
void nn::pia::session::ProcessUpdateMeshJob::Cleanup()
{
    m_StationNum = 0;
    m_ReportSequence = 0;
    m_DirectionsVersion = 0;
    m_Version = 0;
    m_IsProcessing = false;
    m_IsPartReceived[0] = false;
    m_IsPartReceived[1] = false;
    m_PartStationNum = 0;
    m_PartHostEntry = 0;
    m_PartVersion = 0;
    m_PartNum = 0;
    for (u32 i = 0; i < m_StationNumMax; i++) {
        if (m_pCallContexts[i].GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContexts[i].SignalCancel();
        }
        m_pCallContexts[i].Reset();
    }
    if (m_CallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_CallContext.SignalCancel();
    }
    m_CallContext.Reset();
    memset(m_CallContextIndices, INVALID_CALL_CONTEXT_INDEX, sizeof(m_CallContextIndices));
    m_IsJoining = false;
    m_Deadline = common::Time();
}

// 0x0043DC68 | fefates:bytes-fuzzy [tier B]
u32 nn::pia::session::ProcessUpdateMeshJob::Startup(const unsigned char* pData, unsigned int size, bool isSameVersion)
{
    u32 state = SetStationDataList(pData, size);
    if (state != UPDATE_STATE_ACCEPTED) {
        return state;
    }
    if (isSameVersion) {
        CalcTimeLimit(true);
        m_SameVersionUpdateNum++;
    } else {
        CalcTimeLimit(false);
        m_ReportSequence = 0;
        m_DirectionsVersion = 0;
    }
    m_IsSameVersion = isSameVersion;
    memset(m_CallContextIndices, INVALID_CALL_CONTEXT_INDEX, sizeof(m_CallContextIndices));
    m_IsProcessing = true;
    Reset(true);
    m_IsApplied = UpdateStations();
    if (m_IsApplied) {
        if (Mesh::s_pInstance->m_RelayMode == RELAY_MODE_RELAY) {
            SetStep(&ProcessUpdateMeshJob::WaitDirectConnection, "ProcessUpdateMeshJob::WaitDirectConnection");
        } else if (Mesh::s_pInstance->m_RelayMode == RELAY_MODE_DIRECT_REPORT) {
            SetStep(&ProcessUpdateMeshJob::SendConnectionReport, "ProcessUpdateMeshJob::SendConnectionReport");
        } else {
            SetStep(&ProcessUpdateMeshJob::CheckConnectionAll, "ProcessUpdateMeshJob::CheckConnectionAll");
        }
    } else {
        SetStep(&ProcessUpdateMeshJob::UpdateFailed, "ProcessUpdateMeshJob::UpdateFailed");
    }
    return UPDATE_STATE_ACCEPTED;
}

// 0x0043DE40
nn::pia::session::ProcessUpdateMeshJob::ProcessUpdateMeshJob()
    : m_Deadline(), m_TimeLimitMSec(TIME_LIMIT_MSEC), m_ShortTimeLimitMSec(SHORT_TIME_LIMIT_MSEC), m_DirectConnectionDeadline(),
      m_NextFailureNoticeTime(), m_CallContext()
{
    m_DirectConnectionTimeLimitMSec = m_TimeLimitMSec - m_ShortTimeLimitMSec / 2;
    memset(m_CallContextIndices, INVALID_CALL_CONTEXT_INDEX, sizeof(m_CallContextIndices));
    m_ReportSequence = 0;
    m_DirectionsVersion = 0;
    m_Version = 0;
    m_IsProcessing = false;
    m_IsPartReceived[0] = false;
    m_IsPartReceived[1] = false;
    m_PartStationNum = 0;
    m_PartHostEntry = 0;
    m_PartVersion = 0;
    m_PartNum = 0;
    m_StationNumMax = Mesh::s_pInstance->m_StationNumMax;
    m_StationNum = 0;
    m_pCallContexts = common::NewArray<common::CallContext>(m_StationNumMax);
    m_pStationConnectionInfos = common::NewArray<transport::StationConnectionInfo>(m_StationNumMax);
    m_pStationIndices = common::NewArray<StationIndex>(m_StationNumMax);
    m_pNewStationConnectionInfos = common::NewArray<transport::StationConnectionInfo>(m_StationNumMax);
    m_pNewStationIndices = common::NewArray<StationIndex>(m_StationNumMax);
    m_pConnectionFailureReasons = common::NewArray<u8>(m_StationNumMax);
    for (u32 i = 0; i < m_StationNumMax; i++) {
        m_pConnectionFailureReasons[i] = 0;
    }
    m_IsSameVersion = false;
    m_IsJoining = false;
    m_SameVersionUpdateNum = 0;
    m_IsApplied = true;
    m_CallContext.Reset();
}

// 0x0043E14C
// 0x0043E13C (deleting dtor)
nn::pia::session::ProcessUpdateMeshJob::~ProcessUpdateMeshJob()
{
    if (m_pCallContexts != nullptr) {
        common::DeleteArray(m_pCallContexts);
    }
    if (m_pStationConnectionInfos != nullptr) {
        common::DeleteArray(m_pStationConnectionInfos);
    }
    if (m_pStationIndices != nullptr) {
        common::DeleteArray(m_pStationIndices);
    }
    if (m_pNewStationConnectionInfos != nullptr) {
        common::DeleteArray(m_pNewStationConnectionInfos);
    }
    if (m_pNewStationIndices != nullptr) {
        common::DeleteArray(m_pNewStationIndices);
    }
    if (m_pConnectionFailureReasons != nullptr) {
        common::DeleteArray(m_pConnectionFailureReasons);
    }
}

// 0x00733930 | fefates:bytes [tier B]
bool nn::pia::session::ProcessUpdateMeshJob::CheckEdmByStationIndex(nn::pia::StationIndex stationIndex) const
{
    for (u32 i = 0; i < m_StationNum; i++) {
        if (m_pStationIndices[i] == stationIndex) {
            return m_pStationConnectionInfos[i].m_PublicLocation.m_NatMapping == NAT_MAPPING_EDM;
        }
    }
    return false;
}

// 0x00733990 | fefates:bytes [tier B]
nn::pia::StationIndex nn::pia::session::ProcessUpdateMeshJob::GetStationIndexByPrincipalID(unsigned int principalId) const
{
    for (u32 i = 0; i < m_StationNum; i++) {
        if (m_pStationConnectionInfos[i].m_PublicLocation.m_PrincipalId == principalId) {
            return m_pStationIndices[i];
        }
    }
    return STATION_INDEX_UNIDENTIFIED;
}

// 0x007339E8 slot 0x14
void nn::pia::session::ProcessUpdateMeshJob::Trace(unsigned long long) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
