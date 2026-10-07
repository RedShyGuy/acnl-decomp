#include "nn/pia/session/session_CreateMeshJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_ProcessJoinRequestJob.h"
#include "nn/pia/session/session_SyncClockProtocol.h"
#include "nn/pia/transport/transport_IdentificationInfoTable.h"
#include "nn/pia/transport/transport_ProtocolId.h"
#include "nn/pia/transport/transport_ProtocolManager.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace session {
// 0x00430A34 slot 0x1C
void nn::pia::session::CreateMeshJob::CleanupImpl()
{
    // empty (in the original too)
}

// 0x00430A38 slot 0x18 | fefates:bytes
nn::Result nn::pia::session::CreateMeshJob::StartupImpl()
{
    Reset(true);
    SetStep(&CreateMeshJob::SetupSystemProtocols, "CreateMeshJob::SetupSystemProtocols");
    return nn::Result();
}

// 0x00430AA0
nn::pia::common::ExecuteResult nn::pia::session::CreateMeshJob::SetupMeshStatus()
{
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (m_pCallContext != nullptr && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        Cleanup();
        transport::StationConnectionInfoTable::s_pInstance->EraseFromTable(pLocalStation);
        transport::IdentificationInfoTable::s_pInstance->EraseFromTable(pLocalStation);
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    nn::Result result = transport::Transport::s_pInstance->m_ProtocolManager.StartupProtocols(Mesh::s_pInstance->m_LocalStationIndex);
    if (result.IsFailure()) {
        transport::StationConnectionInfoTable::s_pInstance->EraseFromTable(pLocalStation);
        transport::IdentificationInfoTable::s_pInstance->EraseFromTable(pLocalStation);
        if (m_pCallContext != nullptr) {
            m_pCallContext->SignalFailure(result);
            m_pCallContext = nullptr;
        }
        Cleanup();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    pLocalStation->m_State = transport::Station::STATION_STATE_2;
    Mesh::s_pInstance->m_StationNum = 1;
    Mesh::s_pInstance->m_Unknown0x70 = 0;
    if (!Mesh::s_pInstance->m_pProcessJoinRequestJob->Startup()) {
        transport::StationConnectionInfoTable::s_pInstance->EraseFromTable(pLocalStation);
        transport::IdentificationInfoTable::s_pInstance->EraseFromTable(pLocalStation);
        if (m_pCallContext != nullptr) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
            m_pCallContext = nullptr;
        }
        Cleanup();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    Mesh::s_pInstance->m_pProcessJoinRequestJob->Ready(false);
    // the local station is the host
    transport::StationConnectionInfoTable::s_pInstance->m_HostInfo = transport::StationConnectionInfoTable::s_pInstance->m_LocalInfo;
    pLocalStation->m_State = transport::Station::STATION_STATE_CONNECTED;
    Mesh::s_pInstance->SetJoined(true);
    Mesh* pMesh = Mesh::s_pInstance;
    pMesh->m_DisconnectReason = Mesh::DISCONNECT_REASON_NONE;
    pMesh->m_IsJoinable = true;
    pMesh->NoticeMeshEvent(Mesh::EVENT_TYPE_JOIN, pMesh->m_LocalStationIndex);
    SyncClockProtocol* pSyncClockProtocol = transport::Transport::s_pInstance->m_ProtocolManager.GetProtocol<SyncClockProtocol>(
        Mesh::s_pInstance->m_SyncClockProtocolId, transport::PROTOCOL_TYPE_SYNC_CLOCK);
    common::Time now;
    now.SetNow();
    pSyncClockProtocol->m_SyncClock.SetBaseTime(now);
    Mesh::s_pInstance->m_MonitoringStartTime = now;
    Mesh::s_pInstance->m_IsMonitoring = true;
    if (m_pCallContext != nullptr) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalSuccess(nn::Result());
        }
        m_pCallContext = nullptr;
    }
    common::g_SessionBeginMonitoringContent.m_JoinResult = 0xFFFFFFFF;
    common::g_SessionBeginMonitoringContent.m_JoinStationNum = Mesh::s_pInstance->m_StationNum;
    common::g_SessionBeginMonitoringContent.m_HostPrincipalId = 0xFFFFFFFF;
    Mesh::s_pInstance->SetSessionBeginMonitoringData();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00430D18
nn::pia::common::ExecuteResult nn::pia::session::CreateMeshJob::SetupLocalStation()
{
    if (m_pCallContext != nullptr && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        Cleanup();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->CreateLocalStation();
    if (!pLocalStation->Startup(Mesh::s_pInstance->m_pStationProtocol)) {
        if (m_pCallContext != nullptr) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
            m_pCallContext = nullptr;
        }
        Cleanup();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    Mesh::s_pInstance->ClearStationBitmap();
    StationIndex stationIndex = Mesh::s_pInstance->GetFreeStationIndex();
    Mesh* pMesh = Mesh::s_pInstance;
    pMesh->m_LocalStationIndex = stationIndex;
    pMesh->m_HostStationIndex = stationIndex;
    pMesh->StartUse(stationIndex);
    pLocalStation->m_StationIndex = Mesh::s_pInstance->m_LocalStationIndex;
    transport::StationConnectionInfoTable* pTable = transport::StationConnectionInfoTable::s_pInstance;
    pLocalStation->m_StationAddress = pTable->m_LocalInfo.m_PublicLocation.m_StationAddress;
    pTable->AddToTable(pLocalStation, pTable->m_LocalInfo);
    transport::Station::IdentificationInfo info;
    transport::IdentificationInfoTable::s_pInstance->GetLocalIdentificationInfo(&info);
    if (Mesh::s_pInstance->m_IdentificationCallback == nullptr) {
        info.m_Unknown0x48 = 0;
    } else {
        info.m_Unknown0x48 = Mesh::s_pInstance->m_IdentificationCallback();
    }
    transport::IdentificationInfoTable::s_pInstance->SetLocalIdentificationInfo(&info);
    transport::IdentificationInfoTable::s_pInstance->AddToTable(pLocalStation, &info);
    transport::StationManager::s_pInstance->m_Unknown0xA8 = Mesh::s_pInstance->m_HostStationIndex;
    SetStep(&CreateMeshJob::SetupMeshStatus, "CreateMeshJob::SetupMeshStatus");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE_FOREGROUND);
}

// 0x00430EEC | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::session::CreateMeshJob::SetupSystemProtocols()
{
    if (m_pCallContext != nullptr && m_pCallContext->IsCancelRequested()) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
        Cleanup();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    nn::Result result = Mesh::s_pInstance->SetupProtocols();
    if (result.IsFailure()) {
        if (m_pCallContext != nullptr) {
            m_pCallContext->SignalFailure(result);
            m_pCallContext = nullptr;
        }
        Cleanup();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&CreateMeshJob::SetupLocalStation, "CreateMeshJob::SetupLocalStation");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE_FOREGROUND);
}

// 0x00430FE0 slot 0x20 (name is ours)
void nn::pia::session::CreateMeshJob::SetupMonitoringData()
{
    // empty (in the original too)
}

// 0x00430FE4 | fefates:bytes [tier B]
void nn::pia::session::CreateMeshJob::Cleanup()
{
    CleanupImpl();
    if (m_pCallContext != nullptr) {
        if (!m_pCallContext->IsFinished()) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
    }
}

// 0x0043102C | fefates:callgraph
nn::Result nn::pia::session::CreateMeshJob::Startup(nn::pia::common::CallContext* pCallContext)
{
    if (common::IsValidPointer(pCallContext)) {
        m_pCallContext = pCallContext;
        pCallContext->InitiateCall();
    }
    nn::Result result = StartupImpl();
    if (result.IsSuccess()) {
        Mesh::s_pInstance->ClearSessionBeginMonitoringData();
        SetupMonitoringData();
    }
    return result;
}

// 0x00431098
nn::pia::session::CreateMeshJob::CreateMeshJob() : m_pCallContext(nullptr)
{
}

// 0x004310CC
// 0x004310B8 (deleting dtor)
nn::pia::session::CreateMeshJob::~CreateMeshJob()
{
    // empty (in the original too)
}

// 0x0073386C slot 0x14
void nn::pia::session::CreateMeshJob::Trace(unsigned long long) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
