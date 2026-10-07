#include "nn/pia/session/session_DestroyMeshJob.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshProtocol.h"
#include "nn/pia/transport/transport_DisconnectStationJob.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationManager.h"
#include <string.h>

namespace nn {
namespace pia {
namespace session {
// 0x004310F0
nn::pia::common::ExecuteResult nn::pia::session::DestroyMeshJob::CleanupMesh()
{
    for (transport::Station** it = transport::StationManager::s_pInstance->m_ActiveStations.Begin();
         it != transport::StationManager::s_pInstance->m_ActiveStations.End(); it++) {
        (*it)->m_pDisconnectStationJob->OnDisconnected(*it);
    }
    Mesh::s_pInstance->CleanupStationsJobs();
    if (!m_IsSystem) {
        Mesh::s_pInstance->m_DisconnectReason = Mesh::DISCONNECT_REASON_LEAVE;
    }
    Mesh::s_pInstance->CleanupStatus();
    if (m_pCallContext != nullptr) {
        m_pCallContext->SignalSuccess(nn::Result());
        m_pCallContext = nullptr;
    }
    memset(m_IsWaitingStation, 0, sizeof(m_IsWaitingStation));
    m_IsWaitingResponse = false;
    m_IsRunning = false;
    m_IsSystem = false;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x004311B4
nn::pia::common::ExecuteResult nn::pia::session::DestroyMeshJob::SendDestroyMesh()
{
    StationIndex localStationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    if (localStationIndex > STATION_INDEX_MAX) {
        SetStep(&DestroyMeshJob::CleanupMesh, "DestroyMeshJob::CleanupMesh");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    for (int i = 0; i <= STATION_INDEX_MAX; i++) {
        StationIndex stationIndex = static_cast<StationIndex>(i);
        if (stationIndex != localStationIndex && Mesh::s_pInstance->CheckStationIndexIsValid(stationIndex)) {
            Mesh::s_pInstance->m_pMeshProtocol->SendDestroyMesh(stationIndex);
            m_IsWaitingStation[i] = true;
        } else {
            m_IsWaitingStation[i] = false;
        }
    }
    m_IsWaitingResponse = true;
    SetStep(&DestroyMeshJob::WaitDestroyResponse, "DestroyMeshJob::WaitDestroyResponse");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004312D4 | fefates:bytes [tier B]
bool nn::pia::session::DestroyMeshJob::AssociateSystemWith(nn::pia::common::CallContext* pCallContext)
{
    if (!m_IsRunning || !m_IsSystem) {
        return false;
    }
    if (pCallContext != nullptr) {
        m_pCallContext = pCallContext;
        pCallContext->InitiateCall();
    }
    m_IsSystem = false;
    return true;
}

// 0x00431318 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::session::DestroyMeshJob::WaitDestroyResponse()
{
    for (int i = 0; i < 12; i++) {
        if (m_IsWaitingStation[i]) {
            if (common::Scheduler::s_pInstance->m_DispatchTime < m_Deadline) {
                return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
            }
            break;
        }
    }
    m_IsWaitingResponse = false;
    SetStep(&DestroyMeshJob::CleanupMesh, "DestroyMeshJob::CleanupMesh");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x004313D4 | fefates:bytes [tier B]
void nn::pia::session::DestroyMeshJob::ReceiveDestroyResponse(nn::pia::StationIndex stationIndex)
{
    if (stationIndex <= STATION_INDEX_MAX) {
        m_IsWaitingStation[stationIndex] = false;
    }
}

// 0x004313EC | fefates:bytes [tier B]
void nn::pia::session::DestroyMeshJob::Cleanup()
{
    m_Deadline = common::Time();
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalSuccess(nn::Result());
        }
        m_pCallContext = nullptr;
    }
    memset(m_IsWaitingStation, 0, sizeof(m_IsWaitingStation));
    m_IsWaitingResponse = false;
    m_IsRunning = false;
    m_IsSystem = false;
}

// 0x00431444 | fefates:bytes-fuzzy [tier B]
bool nn::pia::session::DestroyMeshJob::Startup(nn::pia::common::CallContext* pCallContext, bool isSystem)
{
    if (m_IsRunning) {
        return false;
    }
    if (pCallContext != nullptr) {
        m_pCallContext = pCallContext;
        pCallContext->InitiateCall();
    }
    common::Time now;
    now.SetNow();
    m_Deadline = now + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * m_TimeoutMSec);
    m_IsRunning = true;
    m_IsSystem = isSystem;
    Mesh::s_pInstance->EndMonitoring(isSystem ? Mesh::s_pInstance->m_DisconnectReason : Mesh::DISCONNECT_REASON_LEAVE);
    Reset(true);
    SetStep(&DestroyMeshJob::SendDestroyMesh, "DestroyMeshJob::SendDestroyMesh");
    return true;
}

// 0x00431540 | fefates:bytes [tier B]
nn::pia::session::DestroyMeshJob::DestroyMeshJob()
    : m_pCallContext(nullptr), m_Deadline(), m_TimeoutMSec(5000), m_IsWaitingResponse(false), m_IsRunning(false), m_IsSystem(false)
{
    memset(m_IsWaitingStation, 0, sizeof(m_IsWaitingStation));
}

// 0x0043159C
// 0x0043158C (deleting dtor)
nn::pia::session::DestroyMeshJob::~DestroyMeshJob()
{
    // empty (in the original too)
}

// 0x00733870 slot 0x14
void nn::pia::session::DestroyMeshJob::Trace(unsigned long long) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
