#include "nn/pia/session/session_LeaveMeshJob.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshProtocol.h"
#include "nn/pia/session/session_ProcessHostMigrationJob.h"
#include "nn/pia/session/session_ProcessUpdateMeshJob.h"
#include "nn/pia/transport/transport_DisconnectStationJob.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationManager.h"

namespace nn {
namespace pia {
namespace session {
namespace {
// the flag of the trace of a station that could not be disconnected
const u64 TRACE_FLAG = 0x100000000ULL;
} // namespace

// 0x0042C3FC
nn::pia::common::ExecuteResult nn::pia::session::LeaveMeshJob::LeaveSuccess()
{
    for (transport::Station** it = transport::StationManager::s_pInstance->m_ActiveStations.Begin();
         it != transport::StationManager::s_pInstance->m_ActiveStations.End(); it++) {
        (*it)->m_pDisconnectStationJob->OnDisconnected(*it);
    }
    Mesh::s_pInstance->CleanupStationsJobs();
    Mesh::s_pInstance->m_DisconnectReason = Mesh::DISCONNECT_REASON_LEAVE;
    Mesh::s_pInstance->CleanupStatus();
    SignalSuccessToCallers();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0042C4C0 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::session::LeaveMeshJob::SendLeaveRequest()
{
    Mesh::s_pInstance->m_pMeshProtocol->SendLeaveRequest();
    Mesh::s_pInstance->m_pMeshProtocol->m_pLeaveMeshJob = this;
    m_IsWaitingResponse = true;
    SetStep(&LeaveMeshJob::WaitLeaveResponse, "LeaveMeshJob::WaitLeaveResponse");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0042C544 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::session::LeaveMeshJob::WaitLeaveResponse()
{
    if (m_IsWaitingResponse) {
        if (common::Scheduler::s_pInstance->m_DispatchTime < m_Deadline && transport::StationManager::s_pInstance->m_pLocalStation != nullptr) {
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        // no response: leave anyway
        m_IsWaitingResponse = false;
        SetStep(&LeaveMeshJob::LeaveSuccess, "LeaveMeshJob::LeaveSuccess");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&LeaveMeshJob::StartDisconnectStations, "LeaveMeshJob::StartDisconnectStations");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0042C660 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::session::LeaveMeshJob::WaitLeavingProcess()
{
    transport::StationManager* pManager = transport::StationManager::s_pInstance;
    for (transport::Station** it = pManager->m_ActiveStations.Begin(); it != pManager->m_ActiveStations.End(); it++) {
        if ((*it)->m_State == transport::Station::STATION_STATE_CONNECTED && *it != pManager->m_pLocalStation) {
            if (common::Scheduler::s_pInstance->m_DispatchTime < m_Deadline) {
                return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
            }
            break;
        }
    }
    SetStep(&LeaveMeshJob::LeaveSuccess, "LeaveMeshJob::LeaveSuccess");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0042C744 | fefates:bytes [tier B]
bool nn::pia::session::LeaveMeshJob::RegisterExtraCallback(nn::pia::common::CallContext* pCallContext)
{
    if (m_pExtraCallContext != nullptr) {
        return false;
    }
    m_pExtraCallContext = pCallContext;
    pCallContext->InitiateCall();
    return true;
}

// 0x0042C76C | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::session::LeaveMeshJob::StartDisconnectStations()
{
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (pLocalStation == nullptr) {
        SetStep(&LeaveMeshJob::LeaveSuccess, "LeaveMeshJob::LeaveSuccess");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    for (transport::Station** it = transport::StationManager::s_pInstance->m_ActiveStations.Begin();
         it != transport::StationManager::s_pInstance->m_ActiveStations.End(); it++) {
        if (*it == pLocalStation) {
            continue;
        }
        if (Mesh::s_pInstance->CheckStationIndexIsValid((*it)->m_StationIndex)) {
            Mesh::s_pInstance->UnfixDisconnectedId((*it)->m_StationIndex);
        }
        transport::DisconnectStationJob* pJob = (*it)->m_pDisconnectStationJob;
        if (pJob != nullptr && pJob->Startup(*it).IsSuccess()) {
            pJob->Ready(false);
        } else {
            (*it)->Trace(TRACE_FLAG);
        }
    }
    SetStep(&LeaveMeshJob::WaitLeavingProcess, "LeaveMeshJob::WaitLeavingProcess");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0042C8EC | fefates:bytes [tier B]
void nn::pia::session::LeaveMeshJob::Cleanup()
{
    m_IsWaitingResponse = false;
    m_Deadline = common::Time();
    SignalSuccessToCallers();
}

// 0x0042C950 (name is ours)
bool nn::pia::session::LeaveMeshJob::Startup(nn::pia::common::CallContext* pCallContext)
{
    if (GetState() == EXECUTE_STATE_RUNNING || GetState() == EXECUTE_STATE_WAITING || GetState() == EXECUTE_STATE_READY) {
        return false;
    }
    if (pCallContext != nullptr) {
        m_pCallContext = pCallContext;
        pCallContext->InitiateCall();
    }
    common::Time now;
    now.SetNow();
    m_Deadline = now + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * m_TimeoutMSec);
    Mesh::s_pInstance->EndMonitoring(Mesh::DISCONNECT_REASON_LEAVE);
    Reset(true);
    SetStep(&LeaveMeshJob::SendLeaveRequest, "LeaveMeshJob::SendLeaveRequest");
    // the jobs that could still change the mesh stop
    ProcessUpdateMeshJob* pUpdateJob = Mesh::s_pInstance->m_pProcessUpdateMeshJob;
    if (pUpdateJob->m_IsProcessing) {
        pUpdateJob->Cleanup();
        pUpdateJob->m_IsCancelRequested = true;
    }
    ProcessHostMigrationJob* pMigrationJob = Mesh::s_pInstance->m_pProcessHostMigrationJob;
    if (pMigrationJob != nullptr && pMigrationJob->m_IsRunning) {
        pMigrationJob->Cleanup();
        pMigrationJob->m_IsCancelRequested = true;
    }
    return true;
}

// 0x0042CAA8 | fefates:bytes [tier B]
nn::pia::session::LeaveMeshJob::LeaveMeshJob()
    : m_Deadline(), m_TimeoutMSec(5000), m_pCallContext(nullptr), m_pExtraCallContext(nullptr), m_IsWaitingResponse(false)
{
}

// 0x0042CAFC
// 0x0042CAEC (deleting dtor)
nn::pia::session::LeaveMeshJob::~LeaveMeshJob()
{
    // empty (in the original too)
}

// 0x00733838 slot 0x14
void nn::pia::session::LeaveMeshJob::Trace(unsigned long long) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
