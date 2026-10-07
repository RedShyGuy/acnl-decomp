#include "nn/pia/session/session_LeaveWithHostMigrationJob.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshProtocol.h"
#include "nn/pia/session/session_ProcessHostMigrationJob.h"
#include "nn/pia/transport/transport_DisconnectStationJob.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationManager.h"

namespace nn {
namespace pia {
namespace session {
namespace {
const s32 TIMEOUT_MSEC = 5000;
// Mesh::m_HostMigrationMode with a ranking of the candidates
const u8 HOST_MIGRATION_MODE_MULTI = 2;
} // namespace

// 0x00442D40
nn::pia::common::ExecuteResult nn::pia::session::LeaveWithHostMigrationJob::CleanupMesh()
{
    for (transport::Station** it = transport::StationManager::s_pInstance->m_ActiveStations.Begin();
         it != transport::StationManager::s_pInstance->m_ActiveStations.End(); it++) {
        (*it)->m_pDisconnectStationJob->OnDisconnected(*it);
    }
    Mesh::s_pInstance->CleanupStationsJobs();
    Mesh::s_pInstance->m_DisconnectReason = Mesh::DISCONNECT_REASON_LEAVE;
    Mesh::s_pInstance->CleanupStatus();
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalSuccess(nn::Result());
        }
        m_pCallContext = nullptr;
    }
    CleanupStatus();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00442DF4 slot 0x1C | fefates:bytes
void nn::pia::session::LeaveWithHostMigrationJob::CleanupStatus()
{
    m_pCallContext = nullptr;
    m_IsRunning = false;
    m_NewHostStationIndex = STATION_INDEX_UNIDENTIFIED;
    memset(m_IsWaitingResponses, 0, sizeof(m_IsWaitingResponses));
    m_IsWaitingResponse = false;
}

// 0x00442E20
nn::pia::common::ExecuteResult nn::pia::session::LeaveWithHostMigrationJob::WaitMigrationResponse()
{
    for (int i = 0; i < STATION_INDEX_MAX + 1; i++) {
        if (m_IsWaitingResponses[i]) {
            if (common::Scheduler::s_pInstance->m_DispatchTime < m_Deadline) {
                return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
            }
            break;
        }
    }
    m_IsWaitingResponse = false;
    SetStep(&LeaveWithHostMigrationJob::CleanupMesh, "LeaveWithHostMigrationJob::CleanupMesh");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00442EE8 | fefates:bytes [tier B]
void nn::pia::session::LeaveWithHostMigrationJob::ReceiveMigrationResponse(nn::pia::StationIndex stationIndex)
{
    if (stationIndex <= STATION_INDEX_MAX) {
        m_IsWaitingResponses[stationIndex] = false;
    }
}

// 0x00442F00
nn::pia::common::ExecuteResult nn::pia::session::LeaveWithHostMigrationJob::WaitHostMigrationProcess()
{
    if (Mesh::s_pInstance->m_pProcessHostMigrationJob->m_IsRunning) {
        // a migration of the stations runs already
        if (common::Scheduler::s_pInstance->m_DispatchTime < m_Deadline) {
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        SetStep(&LeaveWithHostMigrationJob::CleanupMesh, "LeaveWithHostMigrationJob::CleanupMesh");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&LeaveWithHostMigrationJob::SendStartMigrationMessage, "LeaveWithHostMigrationJob::SendStartMigrationMessage");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0044302C
nn::pia::common::ExecuteResult nn::pia::session::LeaveWithHostMigrationJob::CheckHostMigrationProcess()
{
    bool isMigrationRunning = Mesh::s_pInstance->m_pProcessHostMigrationJob->m_IsRunning;
    bool isMulti = Mesh::s_pInstance->m_HostMigrationMode == HOST_MIGRATION_MODE_MULTI;
    if (isMigrationRunning) {
        if (isMulti) {
            SetStep(&LeaveWithHostMigrationJob::CleanupMesh, "LeaveWithHostMigrationJob::CleanupMesh");
        } else {
            SetStep(&LeaveWithHostMigrationJob::WaitHostMigrationProcess, "LeaveWithHostMigrationJob::WaitHostMigrationProcess");
        }
    } else {
        if (isMulti) {
            SetStep(&LeaveWithHostMigrationJob::SendStartMultiMigrationMessage, "LeaveWithHostMigrationJob::SendStartMultiMigrationMessage");
        } else {
            SetStep(&LeaveWithHostMigrationJob::SendStartMigrationMessage, "LeaveWithHostMigrationJob::SendStartMigrationMessage");
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x004431D8
nn::pia::common::ExecuteResult nn::pia::session::LeaveWithHostMigrationJob::SendStartMigrationMessage()
{
    m_NewHostStationIndex = DecideNextHost();
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (m_NewHostStationIndex > STATION_INDEX_MAX || m_NewHostStationIndex == localStationIndex) {
        SetStep(&LeaveWithHostMigrationJob::CleanupMesh, "LeaveWithHostMigrationJob::CleanupMesh");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    // tell every station the next host
    for (int i = 0; i <= STATION_INDEX_MAX; i++) {
        StationIndex stationIndex = static_cast<StationIndex>(i);
        if (stationIndex == localStationIndex || !Mesh::s_pInstance->CheckStationIndexIsValid(stationIndex)) {
            m_IsWaitingResponses[i] = false;
            continue;
        }
        Mesh::s_pInstance->m_pMeshProtocol->SendMigrationRequest(stationIndex, m_NewHostStationIndex);
        m_IsWaitingResponses[i] = true;
    }
    m_IsWaitingResponse = true;
    SetStep(&LeaveWithHostMigrationJob::WaitMigrationResponse, "LeaveWithHostMigrationJob::WaitMigrationResponse");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00443334
nn::pia::common::ExecuteResult nn::pia::session::LeaveWithHostMigrationJob::SendStartMultiMigrationMessage()
{
    if (Mesh::s_pInstance->m_pMeshProtocol->SendMultiMigrationRanking(m_IsWaitingResponses)) {
        m_IsWaitingResponse = true;
        SetStep(&LeaveWithHostMigrationJob::WaitMigrationResponse, "LeaveWithHostMigrationJob::WaitMigrationResponse");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&LeaveWithHostMigrationJob::CleanupMesh, "LeaveWithHostMigrationJob::CleanupMesh");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00443420 | fefates:bytes [tier B]
void nn::pia::session::LeaveWithHostMigrationJob::Cleanup()
{
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
    }
    CleanupStatus();
    m_Deadline = common::Time();
}

// 0x00443468 (name is ours)
bool nn::pia::session::LeaveWithHostMigrationJob::Startup(nn::pia::common::CallContext* pCallContext)
{
    if (m_IsRunning) {
        return false;
    }
    common::Time now;
    now.SetNow();
    m_Deadline = now + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * m_TimeoutMSec);
    if (pCallContext != nullptr) {
        m_pCallContext = pCallContext;
        pCallContext->InitiateCall();
    }
    m_IsRunning = true;
    Mesh::s_pInstance->EndMonitoring(Mesh::DISCONNECT_REASON_LEAVE);
    Reset(true);
    SetStep(&LeaveWithHostMigrationJob::CheckHostMigrationProcess, "LeaveWithHostMigrationJob::CheckHostMigrationProcess");
    return true;
}

// 0x00443570 | fefates:bytes [tier B]
nn::pia::session::LeaveWithHostMigrationJob::LeaveWithHostMigrationJob() : m_Deadline(), m_TimeoutMSec(TIMEOUT_MSEC)
{
    m_pCallContext = nullptr;
    m_IsRunning = false;
    m_NewHostStationIndex = STATION_INDEX_UNIDENTIFIED;
    memset(m_IsWaitingResponses, 0, sizeof(m_IsWaitingResponses));
    m_IsWaitingResponse = false;
}

// 0x004435DC
// 0x004435C8 (deleting dtor)
nn::pia::session::LeaveWithHostMigrationJob::~LeaveWithHostMigrationJob()
{
    // empty (in the original too)
}

// 0x00734174 slot 0x14
void nn::pia::session::LeaveWithHostMigrationJob::Trace(unsigned long long) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
