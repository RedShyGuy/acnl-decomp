#include "nn/pia/session/session_ProcessDestroyMeshJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_JoinMeshJob.h"
#include "nn/pia/session/session_LeaveMeshJob.h"
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
const s32 TIMEOUT_MSEC = 1000;
} // namespace

// 0x0043F4F4
nn::pia::common::ExecuteResult nn::pia::session::ProcessDestroyMeshJob::CleanupMesh()
{
    for (transport::Station** it = transport::StationManager::s_pInstance->m_ActiveStations.Begin();
         it != transport::StationManager::s_pInstance->m_ActiveStations.End(); it++) {
        (*it)->m_pDisconnectStationJob->OnDisconnected(*it);
    }
    Mesh::s_pInstance->CleanupStationsJobs();
    Mesh::s_pInstance->m_DisconnectReason = Mesh::DISCONNECT_REASON_BY_HOST;
    Mesh::s_pInstance->CleanupStatus();
    m_IsRunning = false;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0043F57C
nn::pia::common::ExecuteResult nn::pia::session::ProcessDestroyMeshJob::SendDestroyResponse()
{
    Mesh* pMesh = Mesh::s_pInstance;
    if (pMesh->m_HostStationIndex > STATION_INDEX_MAX || pMesh->m_pMeshProtocol->SendDestroyResponse(pMesh->m_HostStationIndex) ||
        common::Scheduler::s_pInstance->m_DispatchTime >= m_Deadline) {
        SetStep(&ProcessDestroyMeshJob::CleanupMesh, "ProcessDestroyMeshJob::CleanupMesh");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0043F624 (name is ours)
void nn::pia::session::ProcessDestroyMeshJob::Cleanup()
{
    m_IsRunning = false;
}

// 0x0043F630 | fefates:bytes-fuzzy [tier B]
bool nn::pia::session::ProcessDestroyMeshJob::Startup()
{
    if (m_IsRunning) {
        return false;
    }
    // the jobs that could still change the mesh stop
    ProcessUpdateMeshJob* pUpdateJob = Mesh::s_pInstance->m_pProcessUpdateMeshJob;
    if (pUpdateJob->m_IsProcessing) {
        pUpdateJob->Cleanup();
        pUpdateJob->Reset(false);
    }
    ProcessHostMigrationJob* pMigrationJob = Mesh::s_pInstance->m_pProcessHostMigrationJob;
    if (pMigrationJob != nullptr && pMigrationJob->m_IsRunning) {
        pMigrationJob->Cleanup();
        pMigrationJob->Reset(false);
    }
    LeaveMeshJob* pLeaveJob = Mesh::s_pInstance->m_pLeaveMeshJob;
    if (pLeaveJob->GetState() == common::Job::EXECUTE_STATE_RUNNING || pLeaveJob->GetState() == common::Job::EXECUTE_STATE_WAITING) {
        return false;
    }
    JoinMeshJob* pJoinJob = Mesh::s_pInstance->m_pJoinMeshJob;
    if (pJoinJob->IsRunning()) {
        // the join fails
        pJoinJob->Cleanup(common::RESULT_JOIN_FAILED);
        pJoinJob->Reset(true);
        m_IsRunning = true;
        return false;
    }
    common::Time now;
    now.SetNow();
    m_Deadline = now + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * m_TimeoutMSec);
    m_IsRunning = true;
    Mesh::s_pInstance->EndMonitoring(Mesh::DISCONNECT_REASON_BY_HOST);
    Reset(true);
    SetStep(&ProcessDestroyMeshJob::SendDestroyResponse, "ProcessDestroyMeshJob::SendDestroyResponse");
    return true;
}

// 0x0043F7FC | fefates:bytes [tier B]
nn::pia::session::ProcessDestroyMeshJob::ProcessDestroyMeshJob() : m_Deadline(), m_TimeoutMSec(TIMEOUT_MSEC), m_IsRunning(false)
{
}

// 0x0043F844
// 0x0043F834 (deleting dtor)
nn::pia::session::ProcessDestroyMeshJob::~ProcessDestroyMeshJob()
{
    // empty (in the original too)
}

// 0x00734058 slot 0x14
void nn::pia::session::ProcessDestroyMeshJob::Trace(unsigned long long) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
