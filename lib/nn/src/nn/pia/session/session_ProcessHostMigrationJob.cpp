#include "nn/pia/session/session_ProcessHostMigrationJob.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_JoinMeshJob.h"
#include "nn/pia/session/session_LeaveMeshJob.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshEventListener.h"
#include "nn/pia/session/session_MeshProtocol.h"
#include "nn/pia/session/session_ProcessDestroyMeshJob.h"
#include "nn/pia/session/session_ProcessJoinRequestJob.h"
#include "nn/pia/session/session_ProcessUpdateMeshJob.h"
#include "nn/pia/session/session_RelayRouteManageJob.h"
#include "nn/pia/transport/transport_DisconnectStationJob.h"
#include "nn/pia/transport/transport_IdentificationInfoTable.h"
#include "nn/pia/transport/transport_RelayRouteManager.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_Transport.h"
#include "nn/nstd/nstd_String.h"

namespace nn {
namespace pia {
namespace session {
namespace {
const u64 TRACE_FLAG = 0x400000000ULL;
const s32 TIMEOUT_MSEC = 10000;
// the interval of the greetings; the new host waits that much less for the greeting time limit
const s32 GREETING_INTERVAL_MSEC = 500;
// the fitness of a host candidate (MakeHostCandidateRanking; a lower value is better)
const u32 RANK_EDM = 0x800000;                // its NAT maps endpoint dependent
const u32 RANK_NOT_RELAYED_BY_TARGET = 0x100; // its route to the excluded station is not over it
const int RANK_DIRECT_SHIFT = 12;             // the stations it has no direct connection to
const int RANK_APPLICATION_SHIFT = 24;        // Mesh::m_HostCandidateCallback
const u32 RANK_NONE = 0xFFFFFFFF;
// StationLocation::m_NatMapping (see ProcessUpdateMeshJob::CheckEdmByStationIndex)
const u8 NAT_MAPPING_EDM = 2;
// the rank decision of a station (ReceiveRankDecision)
const u8 RANK_DECISION_LOWER = 1;
const u8 RANK_DECISION_HIGHER = 2;
// the result of DecideNextHostCommonProc
const u32 NEXT_HOST_NONE = 0;
const u32 NEXT_HOST_LOCAL = 1;
const u32 NEXT_HOST_OTHER = 2;
// Mesh::m_HostMigrationMode with a ranking
const u8 HOST_MIGRATION_MODE_MULTI = 2;
} // namespace

// (inline; name is ours)
DECOMP_ALWAYS_INLINE void nn::pia::session::ProcessHostMigrationJob::StartMonitoring()
{
    transport::RelayRouteManager* pRelayRouteManager = transport::Transport::s_pInstance->m_pRelayRouteManager;
    if (pRelayRouteManager != nullptr && Mesh::s_pInstance->m_HostStationIndex <= STATION_INDEX_MAX) {
        pRelayRouteManager->SearchRefugeRelayRoute(Mesh::s_pInstance->m_HostStationIndex);
    }
    common::Time now;
    now.SetNow();
    m_StartTime = now;
    m_Duration = 0;
    m_StartStationNum = static_cast<u8>(Mesh::s_pInstance->m_StationNum);
    m_EndStationNum = 0xFF;
    m_MigrationNum++;
    m_Unknown0x6E = 0;
    m_IsRunning = true;
}

// 0x00441030 slot 0x30
void nn::pia::session::ProcessHostMigrationJob::CleanupImpl()
{
    // empty (in the original too)
}

// 0x00441034
bool nn::pia::session::ProcessHostMigrationJob::StartupMulti(bool isFromMessage, const unsigned char* pRanking, unsigned int meshVersion, unsigned int directionsVersion)
{
    if (m_IsRunning) {
        return false;
    }
    if (Mesh::s_pInstance->m_pLeaveMeshJob->IsRunning() || Mesh::s_pInstance->m_pProcessDestroyMeshJob->m_IsRunning) {
        return false;
    }
    if (Mesh::s_pInstance->m_pJoinMeshJob->IsRunning() && Mesh::s_pInstance->GetJoinMeshJobPhase() < 3) {
        // the join cannot go on
        if (Mesh::s_pInstance->m_pEventListener != nullptr) {
            Mesh::Event event;
            event.m_Type = Mesh::EVENT_TYPE_19;
            event.m_StationIndex = STATION_INDEX_UNIDENTIFIED;
            event.m_Unknown0x4 = 0;
            Mesh::s_pInstance->m_pEventListener->OnEvent(event);
        }
        Mesh::s_pInstance->m_pJoinMeshJob->Cleanup(common::RESULT_JOIN_FAILED);
        Mesh::s_pInstance->m_pJoinMeshJob->Reset(true);
        Mesh::s_pInstance->m_pProcessUpdateMeshJob->Cleanup();
        Mesh::s_pInstance->m_pProcessUpdateMeshJob->Reset(true);
        return false;
    }
    if (Mesh::s_pInstance->m_LocalStationIndex > STATION_INDEX_MAX) {
        return false;
    }
    // the stations not connected now leave the mesh
    transport::StationManager* pManager = transport::StationManager::s_pInstance;
    for (transport::Station** it = pManager->m_ActiveStations.Begin(); it != pManager->m_ActiveStations.End(); it++) {
        if ((*it == pManager->m_pLocalStation || (*it)->m_State == transport::Station::STATION_STATE_CONNECTED) &&
            Mesh::s_pInstance->CheckStationIndexIsValid((*it)->m_StationIndex)) {
            continue;
        }
        if (Mesh::s_pInstance->CheckStationIndexIsValid((*it)->m_StationIndex)) {
            Mesh::s_pInstance->UnfixDisconnectedId((*it)->m_StationIndex);
            if (Mesh::s_pInstance->m_StationNum != 0) {
                Mesh::s_pInstance->m_StationNum--;
            }
        }
        (*it)->m_State = transport::Station::STATION_STATE_DISCONNECTED;
    }
    ClearGreetingStatus();
    if (pRanking == nullptr) {
        m_MeshVersion = 0;
        m_DirectionsVersion = 0;
    } else {
        nnnstdMemCpy(m_Ranking, pRanking, Mesh::s_pInstance->m_StationNumMax);
        m_MeshVersion = meshVersion;
        m_DirectionsVersion = directionsVersion;
    }
    Reset(true);
    if (!StartupMultiImpl(isFromMessage, Mesh::s_pInstance->m_StationNumMax)) {
        return false;
    }
    StartMonitoring();
    return true;
}

// 0x00441320 slot 0x34
void nn::pia::session::ProcessHostMigrationJob::CleanupStatus()
{
    m_IsRunning = false;
    ClearGreetingStatus();
    m_MeshVersion = 0;
    m_DirectionsVersion = 0;
    memset(m_RankDecisions, 0, sizeof(m_RankDecisions));
}

// 0x00441370 (name is ours)
void nn::pia::session::ProcessHostMigrationJob::ReceiveMigrationFinish(bool isAllGreetingAnswered)
{
    m_IsHostAllGreetingAnswered = isAllGreetingAnswered;
    m_IsWaitingMigrationFinish = false;
}

// 0x00441380 (name is ours)
void nn::pia::session::ProcessHostMigrationJob::ReceiveGreeting(nn::pia::StationIndex stationIndex)
{
    m_GreetingStationIndex = stationIndex;
}

// 0x00441388 (name is ours)
void nn::pia::session::ProcessHostMigrationJob::ReceiveRankDecision(nn::pia::StationIndex stationIndex, bool isHigher)
{
    if (stationIndex < Mesh::s_pInstance->m_StationNumMax) {
        m_RankDecisions[stationIndex] = isHigher ? RANK_DECISION_HIGHER : RANK_DECISION_LOWER;
    }
}

// 0x004413B8 slot 0x44
bool nn::pia::session::ProcessHostMigrationJob::StartupMultiImpl(bool, unsigned short)
{
    return false;
}

// 0x004413C0 slot 0x40
void nn::pia::session::ProcessHostMigrationJob::DisconnectStation(nn::pia::StationIndex stationIndex)
{
    if (!Mesh::s_pInstance->CheckStationIndexIsValid(stationIndex)) {
        return;
    }
    Mesh::s_pInstance->UnfixDisconnectedId(stationIndex);
    transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(stationIndex);
    if (pStation != nullptr) {
        pStation->m_State = transport::Station::STATION_STATE_DISCONNECTED;
        pStation->m_StationIndex = STATION_INDEX_UNIDENTIFIED;
    }
    if (Mesh::s_pInstance->m_StationNum != 0) {
        Mesh::s_pInstance->m_StationNum--;
    }
}

// 0x00441430 slot 0x1C
bool nn::pia::session::ProcessHostMigrationJob::IsFatalErrorOccur()
{
    return false;
}

// 0x00441438
void nn::pia::session::ProcessHostMigrationJob::SetMonitoringData()
{
    common::g_SessionStateMonitoringContent.m_Unknown0x346 = m_MigrationNum;
    u32 durationMSec = m_Duration / common::TimeSpan::GetTicksPerMSec().GetTick();
    common::g_SessionStateMonitoringContent.m_Unknown0x348 = durationMSec == 0 ? 0xFFFFFFFF : durationMSec;
    common::g_SessionStateMonitoringContent.m_Unknown0x34C = m_StartStationNum;
    common::g_SessionStateMonitoringContent.m_Unknown0x34D = m_EndStationNum;
    common::g_SessionStateMonitoringContent.m_Unknown0x3D9 = m_Unknown0x6E;
}

// 0x00441490
nn::pia::common::ExecuteResult nn::pia::session::ProcessHostMigrationJob::SendGreetingMessage()
{
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (pLocalStation != nullptr) {
        transport::StationConnectionInfoTable* pTable = transport::StationConnectionInfoTable::s_pInstance;
        transport::StationConnectionInfo info;
        if (pTable->GetStationConnectionInfo(pLocalStation, &info).IsFailure()) {
            pLocalStation->Trace(TRACE_FLAG);
        } else {
            // we are the host now
            pTable->m_HostInfo = info;
            pTable->m_HostAddress = pLocalStation->m_StationAddress;
            transport::Station* pOldHostStation = transport::StationManager::s_pInstance->GetStation(m_OldHostStationIndex);
            if (pOldHostStation != nullptr) {
                if (!Mesh::s_pInstance->CheckStationIndexIsValid(m_OldHostStationIndex)) {
                    pOldHostStation->m_State = transport::Station::STATION_STATE_DISCONNECTED;
                } else {
                    DisconnectStation(m_OldHostStationIndex);
                }
            }
            Mesh::s_pInstance->NoticeMeshEvent(Mesh::EVENT_TYPE_HOST_CHANGED, localStationIndex);

            // greet all stations
            StationIndex lostStationIndices[STATION_INDEX_MAX + 1] = {};
            int lostStationNum = 0;
            for (int i = 0; i <= STATION_INDEX_MAX; i++) {
                StationIndex stationIndex = static_cast<StationIndex>(i);
                if (stationIndex == localStationIndex || !Mesh::s_pInstance->CheckStationIndexIsValid(stationIndex)) {
                    m_IsGreetingSent[i] = false;
                    continue;
                }
                bool isSent = Mesh::s_pInstance->m_pMeshProtocol->SendGreeting(stationIndex);
                m_IsGreetingSent[i] = true;
                if (!isSent && transport::StationManager::s_pInstance->GetStation(stationIndex) == nullptr) {
                    lostStationIndices[lostStationNum++] = stationIndex;
                    m_IsGreetingSent[i] = false;
                }
            }
            for (int i = 0; i < lostStationNum; i++) {
                Mesh::s_pInstance->UnfixDisconnectedId(lostStationIndices[i]);
                if (Mesh::s_pInstance->m_StationNum != 0) {
                    Mesh::s_pInstance->m_StationNum--;
                }
                transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(lostStationIndices[i]);
                if (pStation != nullptr) {
                    pStation->m_pDisconnectStationJob->OnDisconnected(pStation);
                    transport::StationConnectionInfoTable::s_pInstance->EraseFromTable(pStation);
                    transport::IdentificationInfoTable::s_pInstance->EraseFromTable(pStation);
                    pStation->CleanupJobs();
                    pStation->Cleanup();
                    transport::StationManager::s_pInstance->DestroyStation(pStation);
                    transport::Transport::s_pInstance->OutputStreamUpdateEvent();
                }
            }
            m_GreetingDeadline = common::Scheduler::s_pInstance->m_DispatchTime +
                                 common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * GREETING_INTERVAL_MSEC);
            m_IsWaitingGreetingResponse = true;
            SetStep(&ProcessHostMigrationJob::WaitGreetingResponse, "ProcessHostMigrationJob::WaitGreetingResponse");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
    }
    SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "ProcessHostMigrationJob::HostMigrationFailure");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x004417FC
nn::pia::common::ExecuteResult nn::pia::session::ProcessHostMigrationJob::SendMigrationFinish()
{
    Mesh::s_pInstance->m_pMeshProtocol->SendMigrationFinish(m_IsAllGreetingAnswered);
    m_IsWaitingGreetingResponse = false;
    Mesh::s_pInstance->m_IsJoinable = true;
    Mesh::s_pInstance->m_pMeshProtocol->SendStationDataList(true);
    if (CallUpdateSessionHost(0)) {
        SetStep(&ProcessHostMigrationJob::WaitUpdateSessionHost, "ProcessHostMigrationJob::WaitUpdateSessionHost");
    } else {
        SetStep(&ProcessHostMigrationJob::HostMigrationSuccess, "ProcessHostMigrationJob::HostMigrationSuccess");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x004418F8
nn::pia::common::ExecuteResult nn::pia::session::ProcessHostMigrationJob::WaitNewHostFinished()
{
    if (Mesh::s_pInstance->CheckJoined() == common::RESULT_NOT_JOINED) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "ProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (m_IsWaitingMigrationFinish) {
        if (common::Scheduler::s_pInstance->m_DispatchTime < m_Deadline) {
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (!CheckWhetherReselectNewHost()) {
            SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "ProcessHostMigrationJob::HostMigrationFailure");
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    m_IsHostAllGreetingAnswered = false;
    SetStep(&ProcessHostMigrationJob::HostMigrationSuccess, "ProcessHostMigrationJob::HostMigrationSuccess");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00441A64
nn::pia::common::ExecuteResult nn::pia::session::ProcessHostMigrationJob::WaitNewHostGreeting()
{
    if (Mesh::s_pInstance->CheckJoined() == common::RESULT_NOT_JOINED) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "ProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    StationIndex greetingStationIndex = m_GreetingStationIndex;
    if (greetingStationIndex == STATION_INDEX_UNIDENTIFIED) {
        if (common::Scheduler::s_pInstance->m_DispatchTime < m_Deadline) {
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (!CheckWhetherReselectNewHost()) {
            SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "ProcessHostMigrationJob::HostMigrationFailure");
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (m_IsMultiMigration) {
        // the station that greets is the new host
        m_NewHostStationIndex = greetingStationIndex;
    } else if (greetingStationIndex != m_NewHostStationIndex) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "ProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    Mesh::s_pInstance->m_HostStationIndex = m_NewHostStationIndex;
    transport::StationManager::s_pInstance->m_Unknown0xA8 = m_NewHostStationIndex;
    transport::Station* pHostStation = transport::StationManager::s_pInstance->GetStation(m_NewHostStationIndex);
    if (pHostStation == nullptr) {
        SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "ProcessHostMigrationJob::HostMigrationFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    {
        transport::StationConnectionInfoTable* pTable = transport::StationConnectionInfoTable::s_pInstance;
        transport::StationConnectionInfo info;
        if (pTable->GetStationConnectionInfo(pHostStation, &info).IsFailure()) {
            pHostStation->Trace(TRACE_FLAG);
            SetStep(&ProcessHostMigrationJob::HostMigrationFailure, "ProcessHostMigrationJob::HostMigrationFailure");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        pTable->m_HostInfo = info;
        pTable->m_HostAddress = pHostStation->m_StationAddress;
    }
    if (m_OldHostStationIndex != m_NewHostStationIndex) {
        if (transport::StationManager::s_pInstance->GetStation(m_OldHostStationIndex) != nullptr) {
            DisconnectStation(m_OldHostStationIndex);
        }
        Mesh::s_pInstance->NoticeMeshEvent(Mesh::EVENT_TYPE_HOST_CHANGED, m_NewHostStationIndex);
    }
    Mesh::s_pInstance->m_pMeshProtocol->SendGreetingResponse(m_NewHostStationIndex);
    m_IsWaitingGreeting = false;
    m_IsWaitingMigrationFinish = true;
    SetStep(&ProcessHostMigrationJob::WaitNewHostFinished, "ProcessHostMigrationJob::WaitNewHostFinished");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00441D5C
nn::pia::common::ExecuteResult nn::pia::session::ProcessHostMigrationJob::HostMigrationFailure()
{
    Mesh::s_pInstance->NoticeMeshEvent(Mesh::EVENT_TYPE_HOST_MIGRATION_FAILED, STATION_INDEX_UNIDENTIFIED);
    common::Time now;
    now.SetNow();
    m_Duration = (now - m_StartTime).GetTick();
    Mesh::s_pInstance->m_DisconnectReason = Mesh::DISCONNECT_REASON_HOST_MIGRATION_FAILED;
    Mesh::s_pInstance->EndMonitoring(Mesh::DISCONNECT_REASON_HOST_MIGRATION_FAILED);
    Mesh::s_pInstance->CleanupStatus();
    CleanupStatus();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00441DE8
nn::pia::common::ExecuteResult nn::pia::session::ProcessHostMigrationJob::HostMigrationSuccess()
{
    common::Time now;
    now.SetNow();
    m_Duration = (now - m_StartTime).GetTick();
    m_EndStationNum = static_cast<u8>(Mesh::s_pInstance->m_StationNum);
    CleanupStatus();
    Mesh::s_pInstance->UpdateMonitoringData();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00441E60
nn::pia::common::ExecuteResult nn::pia::session::ProcessHostMigrationJob::WaitGreetingResponse()
{
    bool isWaiting = false;
    for (int i = 0; i < STATION_INDEX_MAX + 1; i++) {
        if (!m_IsGreetingSent[i]) {
            continue;
        }
        isWaiting = true;
        common::Time now = common::Scheduler::s_pInstance->m_DispatchTime;
        if (now >= m_Deadline) {
            break;
        }
        if (m_GreetingDeadline < now) {
            // greet the ones without a response again
            for (int j = 0; j <= STATION_INDEX_MAX; j++) {
                if (!m_IsGreetingSent[j]) {
                    continue;
                }
                StationIndex stationIndex = static_cast<StationIndex>(j);
                if (Mesh::s_pInstance->m_pMeshProtocol->SendGreeting(stationIndex)) {
                    continue;
                }
                if (Mesh::s_pInstance->CheckStationIndexIsValid(stationIndex)) {
                    if (transport::StationManager::s_pInstance->GetStation(stationIndex) != nullptr) {
                        continue;
                    }
                    Mesh::s_pInstance->UnfixDisconnectedId(stationIndex);
                    if (Mesh::s_pInstance->m_StationNum != 0) {
                        Mesh::s_pInstance->m_StationNum--;
                    }
                }
                m_IsGreetingSent[j] = false;
            }
            m_GreetingDeadline = now + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * GREETING_INTERVAL_MSEC);
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (CheckWhetherSendMigrationFinish()) {
        SetStep(&ProcessHostMigrationJob::SendMigrationFinish, "ProcessHostMigrationJob::SendMigrationFinish");
    }
    m_IsAllGreetingAnswered = !isWaiting;
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0044202C slot 0x38
bool nn::pia::session::ProcessHostMigrationJob::CallUpdateSessionHost(unsigned int)
{
    return false;
}

// 0x00442034 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::session::ProcessHostMigrationJob::WaitUpdateSessionHost()
{
    if (IsCompletedUpdateSessionHost()) {
        SetStep(&ProcessHostMigrationJob::HostMigrationSuccess, "ProcessHostMigrationJob::HostMigrationSuccess");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004420CC (name is ours)
void nn::pia::session::ProcessHostMigrationJob::ReceiveGreetingResponse(nn::pia::StationIndex stationIndex)
{
    if (stationIndex <= STATION_INDEX_MAX) {
        m_IsGreetingSent[stationIndex] = false;
    }
}

// 0x004420E4
u32 nn::pia::session::ProcessHostMigrationJob::DecideNextHostCommonProc()
{
    m_OldHostStationIndex = Mesh::s_pInstance->m_HostStationIndex;
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX || m_NewHostStationIndex > STATION_INDEX_MAX) {
        return NEXT_HOST_NONE;
    }
    if (m_NewHostStationIndex == localStationIndex) {
        common::Time now;
        now.SetNow();
        m_Deadline = now + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * (m_TimeoutMSec - GREETING_INTERVAL_MSEC));
        return NEXT_HOST_LOCAL;
    }
    m_IsWaitingGreeting = true;
    m_GreetingStationIndex = STATION_INDEX_UNIDENTIFIED;
    common::Time now;
    now.SetNow();
    m_Deadline = now + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * m_TimeoutMSec);
    return NEXT_HOST_OTHER;
}

// 0x004421DC | fefates:bytes-fuzzy [tier B]
nn::Result nn::pia::session::ProcessHostMigrationJob::MakeHostCandidateRanking(nn::pia::StationIndex stationIndex, unsigned char* pRanking, unsigned int* pMeshVersion, unsigned int* pDirectionsVersion, bool isFromConnectionInfo)
{
    transport::RelayRouteManager* pRelayRouteManager = transport::Transport::s_pInstance->m_pRelayRouteManager;
    Mesh* pMesh = Mesh::s_pInstance;
    u32 ranks[STATION_INDEX_MAX + 1] = {};
    if (isFromConnectionInfo) {
        transport::StationConnectionInfoTable* pTable = transport::StationConnectionInfoTable::s_pInstance;
        transport::StationConnectionInfo info;
        for (int i = 0; i < pMesh->m_StationNumMax; i++) {
            ranks[i] = RANK_NONE;
            StationIndex candidate = static_cast<StationIndex>(i);
            if (candidate == stationIndex || !pMesh->CheckStationIndexIsValid(candidate)) {
                continue;
            }
            transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(candidate);
            if (pStation == nullptr || pTable->GetStationConnectionInfo(pStation, &info).IsFailure()) {
                continue;
            }
            u32 directNum = 0;
            bool isRelayedByTarget = true;
            if (common::IsValidPointer(pRelayRouteManager)) {
                StationIndex relayStationIndex;
                if (pRelayRouteManager->GetOriginalRelayRoute(candidate, stationIndex, &relayStationIndex).IsSuccess()) {
                    isRelayedByTarget = stationIndex == relayStationIndex;
                }
                u32 directBitmap;
                if (pRelayRouteManager->GetOriginalDirectStationList(candidate, &directBitmap).IsSuccess()) {
                    while (directBitmap != 0) {
                        if (directBitmap & 0x80000000) {
                            directNum++;
                        }
                        directBitmap <<= 1;
                    }
                }
                if (isRelayedByTarget) {
                    directNum--;
                }
            }
            u32 applicationRank = 0;
            if (pMesh->m_HostCandidateCallback != nullptr) {
                applicationRank = pMesh->m_HostCandidateCallback(candidate, true);
            }
            ranks[i] = (info.m_PublicLocation.m_NatMapping == NAT_MAPPING_EDM ? RANK_EDM : 0) + (isRelayedByTarget ? 0 : RANK_NOT_RELAYED_BY_TARGET) +
                       ((pMesh->m_StationNumMax - directNum) << RANK_DIRECT_SHIFT) + (applicationRank << RANK_APPLICATION_SHIFT) + i + 1;
        }
    } else {
        for (int i = 0; i < pMesh->m_StationNumMax; i++) {
            ranks[i] = RANK_NONE;
            StationIndex candidate = static_cast<StationIndex>(i);
            if (candidate == stationIndex || !pMesh->CheckStationIndexIsValid(candidate)) {
                continue;
            }
            bool isRelayedByTarget = true;
            u32 directNum = 0;
            if (common::IsValidPointer(pRelayRouteManager)) {
                StationIndex relayStationIndex;
                if (pRelayRouteManager->GetOriginalRelayRoute(candidate, stationIndex, &relayStationIndex).IsSuccess()) {
                    isRelayedByTarget = stationIndex == relayStationIndex;
                }
                u32 directBitmap;
                if (pRelayRouteManager->GetOriginalDirectStationList(candidate, &directBitmap).IsSuccess()) {
                    while (directBitmap != 0) {
                        if (directBitmap & 0x80000000) {
                            directNum++;
                        }
                        directBitmap <<= 1;
                    }
                }
                if (isRelayedByTarget) {
                    directNum--;
                }
            }
            u32 applicationRank = 0;
            if (pMesh->m_HostCandidateCallback != nullptr) {
                applicationRank = pMesh->m_HostCandidateCallback(candidate, false);
            }
            bool isEdm = pMesh->m_pProcessUpdateMeshJob->CheckEdmByStationIndex(candidate);
            ranks[i] = (isEdm ? RANK_EDM : 0) + (isRelayedByTarget ? 0 : RANK_NOT_RELAYED_BY_TARGET) +
                       ((pMesh->m_StationNumMax - directNum) << RANK_DIRECT_SHIFT) + (applicationRank << RANK_APPLICATION_SHIFT) + i + 1;
        }
    }

    // sort them: each time the lowest rank above the last one
    u32 rankNum = 0;
    u32 lastRank = 0;
    for (; rankNum < pMesh->m_StationNumMax; rankNum++) {
        u32 bestRank = RANK_NONE;
        u32 bestIndex;
        for (u32 i = 0; i < pMesh->m_StationNumMax; i++) {
            if (ranks[i] > lastRank && ranks[i] < bestRank) {
                bestIndex = i;
                bestRank = ranks[i];
            }
        }
        if (bestRank == RANK_NONE) {
            break;
        }
        lastRank = bestRank;
        pRanking[rankNum] = bestIndex;
    }
    for (; static_cast<int>(rankNum) < pMesh->m_StationNumMax; rankNum++) {
        pRanking[rankNum] = STATION_INDEX_UNIDENTIFIED;
    }
    *pMeshVersion = pMesh->m_Unknown0x70;
    if (common::IsValidPointer(pRelayRouteManager)) {
        *pDirectionsVersion = pRelayRouteManager->m_DirectionsVersionLow;
    } else {
        *pDirectionsVersion = 0;
    }
    return nn::Result();
}

// 0x00442638 slot 0x48
bool nn::pia::session::ProcessHostMigrationJob::CheckWhetherReselectNewHost()
{
    return false;
}

// 0x00442640 (name is ours)
void nn::pia::session::ProcessHostMigrationJob::ClearMonitoringData()
{
    m_Duration = 0;
    m_MigrationNum = 0;
    m_Unknown0x6E = 0;
    m_StartStationNum = 0xFF;
    m_EndStationNum = 0xFF;
}

// 0x00442664 slot 0x20
void nn::pia::session::ProcessHostMigrationJob::vf_0x20()
{
    // empty (in the original too)
}

// 0x00442668 slot 0x18
bool nn::pia::session::ProcessHostMigrationJob::CleanupOldHostInfoCommonProc()
{
    Mesh::s_pInstance->m_pMeshProtocol->SendMigrationResponse(m_OldHostStationIndex);
    DisconnectStation(m_OldHostStationIndex);
    return true;
}

// 0x004426A4 (name is ours)
nn::pia::StationIndex nn::pia::session::ProcessHostMigrationJob::GetStationIndexByPrincipalId(unsigned int principalId) const
{
    return Mesh::s_pInstance->m_pProcessUpdateMeshJob->GetStationIndexByPrincipalID(principalId);
}

// 0x004426B8 slot 0x3C
bool nn::pia::session::ProcessHostMigrationJob::IsCompletedUpdateSessionHost()
{
    return true;
}

// 0x004426C0 slot 0x4C
bool nn::pia::session::ProcessHostMigrationJob::CheckWhetherSendMigrationFinish()
{
    return true;
}

// 0x004426C8 | fefates:bytes-fuzzy [tier B]
bool nn::pia::session::ProcessHostMigrationJob::PrepareForBecomingHostCommonProc()
{
    if (Mesh::s_pInstance->GetTime() == -1) {
        return false;
    }
    Mesh::s_pInstance->m_IsJoinable = false;
    if (!Mesh::s_pInstance->m_pProcessJoinRequestJob->Startup()) {
        return false;
    }
    Mesh::s_pInstance->m_pProcessJoinRequestJob->Ready(false);
    ProcessUpdateMeshJob* pUpdateJob = Mesh::s_pInstance->m_pProcessUpdateMeshJob;
    if (pUpdateJob->m_IsProcessing) {
        pUpdateJob->Cleanup();
        Mesh::s_pInstance->m_pProcessUpdateMeshJob->Reset(false);
    }
    // the connected stations keep their index, the others are gone
    transport::StationManager* pManager = transport::StationManager::s_pInstance;
    for (transport::Station** it = pManager->m_ActiveStations.Begin(); it != pManager->m_ActiveStations.End(); it++) {
        if ((*it == pManager->m_pLocalStation || (*it)->m_State == transport::Station::STATION_STATE_CONNECTED) &&
            (*it)->m_StationIndex < STATION_INDEX_MAX) {
            if (!Mesh::s_pInstance->CheckStationIndexIsValid((*it)->m_StationIndex)) {
                Mesh::s_pInstance->FixConnectedId((*it)->m_StationIndex);
            }
            continue;
        }
        (*it)->m_State = transport::Station::STATION_STATE_DISCONNECTED;
    }
    if (Mesh::s_pInstance->m_pRelayRouteManageJob != nullptr) {
        Mesh::s_pInstance->m_pRelayRouteManageJob->PrepareForBecomingNewHost();
    }
    return true;
}

// 0x004427F0 slot 0x28
void nn::pia::session::ProcessHostMigrationJob::UpdateHostStationIndexByLocalStationIndex()
{
    Mesh::s_pInstance->m_HostStationIndex = m_NewHostStationIndex;
    transport::StationManager::s_pInstance->m_Unknown0xA8 = m_NewHostStationIndex;
}

// 0x0044281C | fefates:bytes [tier B]
void nn::pia::session::ProcessHostMigrationJob::Cleanup()
{
    CleanupStatus();
    CleanupImpl();
}

// 0x00442848
bool nn::pia::session::ProcessHostMigrationJob::Startup(bool isFromMessage, nn::pia::StationIndex stationIndex)
{
    if (m_IsRunning) {
        return false;
    }
    if (Mesh::s_pInstance->m_pLeaveMeshJob->IsRunning() || Mesh::s_pInstance->m_pProcessDestroyMeshJob->m_IsRunning) {
        return false;
    }
    if (Mesh::s_pInstance->m_pJoinMeshJob->IsRunning()) {
        // the join cannot go on
        Mesh::s_pInstance->m_pJoinMeshJob->Cleanup(common::RESULT_JOIN_FAILED);
        Mesh::s_pInstance->m_pJoinMeshJob->Reset(true);
        Mesh::s_pInstance->m_pProcessUpdateMeshJob->Cleanup();
        Mesh::s_pInstance->m_pProcessUpdateMeshJob->Reset(true);
        return false;
    }
    if (Mesh::s_pInstance->m_LocalStationIndex > STATION_INDEX_MAX) {
        return false;
    }
    ClearGreetingStatus();
    Reset(true);
    if (!StartupImpl(isFromMessage, stationIndex)) {
        return false;
    }
    StartMonitoring();
    return true;
}

// 0x00442A04
nn::pia::session::ProcessHostMigrationJob::ProcessHostMigrationJob()
    : m_Deadline(), m_TimeoutMSec(TIMEOUT_MSEC), m_GreetingDeadline(), m_StartTime(), m_Duration(0)
{
    m_MigrationNum = 0;
    m_StartStationNum = 0;
    m_EndStationNum = 0;
    m_IsRunning = false;
    ClearGreetingStatus();
    m_IsMultiMigration = Mesh::s_pInstance->m_HostMigrationMode == HOST_MIGRATION_MODE_MULTI;
    m_MeshVersion = 0;
    m_DirectionsVersion = 0;
    memset(m_RankDecisions, 0, sizeof(m_RankDecisions));
    memset(m_Ranking, STATION_INDEX_UNIDENTIFIED, sizeof(m_Ranking));
}

// 0x00442AF0
// 0x00442ADC (deleting dtor)
nn::pia::session::ProcessHostMigrationJob::~ProcessHostMigrationJob()
{
    // empty (in the original too)
}

// 0x00734080 (name is ours)
nn::Result nn::pia::session::ProcessHostMigrationJob::CompareRank(nn::pia::StationIndex stationIndex1, nn::pia::StationIndex stationIndex2, bool* pIsHigher, unsigned int* pValue1, unsigned int* pValue2) const
{
    if (m_MeshVersion == 0) {
        return common::RESULT_INVALID_STATE;
    }
    if (stationIndex1 == STATION_INDEX_UNIDENTIFIED || stationIndex2 == STATION_INDEX_UNIDENTIFIED || !common::IsValidPointer(pIsHigher)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    u32 rank1 = STATION_INDEX_MAX + 1;
    u32 rank2 = STATION_INDEX_MAX + 1;
    for (int i = 0; i < Mesh::s_pInstance->m_StationNumMax; i++) {
        if (m_Ranking[i] == stationIndex1) {
            rank1 = i;
        }
        if (m_Ranking[i] == stationIndex2) {
            rank2 = i;
        }
    }
    if (rank1 == STATION_INDEX_MAX + 1 && rank2 == STATION_INDEX_MAX + 1) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    *pIsHigher = rank1 < rank2;
    *pValue1 = m_MeshVersion;
    *pValue2 = m_DirectionsVersion;
    return nn::Result();
}

// 0x00734154 slot 0x14
void nn::pia::session::ProcessHostMigrationJob::Trace(unsigned long long) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
