#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshEventListener.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_MonitoringDataSender.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_SignatureManager.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_Api.h"
#include "nn/pia/session/session_CreateMeshJob.h"
#include "nn/pia/session/session_DestroyMeshJob.h"
#include "nn/pia/session/session_JoinMeshJob.h"
#include "nn/pia/session/session_KickoutManageJob.h"
#include "nn/pia/session/session_LeaveMeshJob.h"
#include "nn/pia/session/session_LeaveWithHostMigrationJob.h"
#include "nn/pia/session/session_MeshProtocol.h"
#include "nn/pia/session/session_ProcessDestroyMeshJob.h"
#include "nn/pia/session/session_ProcessHostMigrationJob.h"
#include "nn/pia/session/session_ProcessJoinRequestJob.h"
#include "nn/pia/session/session_ProcessUpdateMeshJob.h"
#include "nn/pia/session/session_RelayRouteManageJob.h"
#include "nn/pia/session/session_SignatureSettingStorage.h"
#include "nn/pia/session/session_SyncClockProtocol.h"
#include "nn/pia/transport/transport_AttendanceTable.h"
#include "nn/pia/transport/transport_BandwidthCheckerProtocol.h"
#include "nn/pia/transport/transport_DisconnectStationJob.h"
#include "nn/pia/transport/transport_IdentificationInfoTable.h"
#include "nn/pia/transport/transport_MissingStationHandler.h"
#include "nn/pia/transport/transport_NetworkFactory.h"
#include "nn/pia/transport/transport_ProtocolEvent.h"
#include "nn/pia/transport/transport_RelayRouteManager.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationIdTable.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_StationProtocol.h"
#include "nn/pia/transport/transport_StationProtocolManager.h"
#include "nn/pia/transport/transport_ThreadStreamManager.h"
#include "nn/pia/transport/transport_Transport.h"
#include "nn/pia/transport/transport_TransportAnalyzer.h"
#include <string.h>

namespace nn {
namespace pia {
namespace session {
namespace {
// 0x0097E444
Mesh::GlobalSetting s_GlobalSetting;

const u64 TRACE_FLAG_ADDRESS = 0x100000000ULL;
const u64 TRACE_FLAG_IDENTIFICATION = 0x80000ULL;
// the port of the bandwidth checker protocol
const u16 BANDWIDTH_CHECKER_PORT = 2;
// Setting::m_RelayMode / the host migration mode of the factory: 1 and 2 use the jobs
const u8 MODE_1 = 1;
const u8 MODE_2 = 2;
const u32 STATION_NUM_MAX = STATION_INDEX_MAX + 1;
// the timeouts (Startup)
const u32 TIMEOUT_MIN_MSEC = 1000;
const u32 TIMEOUT_MAX_MSEC = 30000;
const s32 PROCESS_TIMEOUT_MSEC = 15000;
const s32 UPDATE_EXTRA_TIMEOUT_MSEC = 5000;
// the keep alive interval after Initialize
const s32 KEEP_ALIVE_INTERVAL_MSEC = 1000;
const u16 RELAY_RTT_LIMIT = 500;
// the identification info of the local station: the length of the name and the version
const u8 IDENTIFICATION_NAME_LENGTH_MAX = 16;
const u8 IDENTIFICATION_VERSION = 2;
// the host index while joining (not known yet)
const StationIndex HOST_STATION_INDEX_JOINING = static_cast<StationIndex>(254);
// CheckApprovalJoin
const u8 APPROVAL_FULL = 0;
const u8 APPROVAL_NOT_JOINABLE = 2;
const u8 APPROVAL_OK = 0xFF;
// the phases of MonitoringProcess
const u8 MONITORING_PHASE_BEGIN = 0;
const u8 MONITORING_PHASE_END = 1;
const u8 MONITORING_PHASE_UPDATE = 2;
// KickoutManageJob::m_Reason that gives DISCONNECT_REASON_KICKOUT_4
const u8 KICKOUT_REASON_2 = 2;

// the bit of the station (a loop in the original)
inline u32 GetStationBit(StationIndex stationIndex)
{
    u32 bit = 1;
    for (u32 i = 0; i < stationIndex; i++) {
        bit <<= 1;
    }
    return bit;
}
} // namespace

// 0x0097E448
nn::pia::session::Mesh* nn::pia::session::Mesh::s_pInstance;

// 0x00447BCC | fefates:callgraph
nn::Result nn::pia::session::Mesh::CreateMesh(nn::pia::common::CallContext* pCallContext)
{
    if ((pCallContext != nullptr && pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) || m_IsJoined ||
        transport::StationManager::s_pInstance->m_pLocalStation != nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_pCreateMeshJob->GetState() != common::Job::EXECUTE_STATE_IDLE && m_pCreateMeshJob->GetState() != common::Job::EXECUTE_STATE_FINISHED) {
        return common::RESULT_INVALID_STATE;
    }
    nn::Result result = m_pCreateMeshJob->Startup(pCallContext);
    if (result.IsSuccess()) {
        m_pCreateMeshJob->Ready(false);
    }
    return result;
}

// 0x00447C70 (name is ours)
nn::Result nn::pia::session::Mesh::Initialize(const Setting& setting, bool isRelayRouteNetwork)
{
    transport::Transport* pTransport = transport::Transport::s_pInstance;
    transport::NetworkFactory* pFactory = setting.m_pNetworkFactory;
    m_MeshProtocolId = pTransport->m_ProtocolManager.CreateProtocol<MeshProtocol>(transport::PROTOCOL_TYPE_MESH, 0);
    m_pStationProtocol = transport::StationProtocolManager::s_pInstance->GetStationProtocol();
    m_pMeshProtocol = pTransport->m_ProtocolManager.GetProtocol<MeshProtocol>(m_MeshProtocolId, transport::PROTOCOL_TYPE_MESH);
    // (a call was removed by the linker here)
    m_pMeshProtocol->Initialize();
    m_SyncClockProtocolId = pTransport->m_ProtocolManager.CreateProtocol<SyncClockProtocol>(transport::PROTOCOL_TYPE_SYNC_CLOCK, 0);
    m_IsBandwidthCheckEnabled = setting.m_IsBandwidthCheckEnabled;
    common::g_SessionBeginMonitoringContent.m_Unknown0x261 = setting.m_IsBandwidthCheckEnabled;
    if (m_IsBandwidthCheckEnabled) {
        m_BandwidthCheckerProtocolId =
            pTransport->m_ProtocolManager.CreateProtocol<transport::BandwidthCheckerProtocol>(transport::PROTOCOL_TYPE_BANDWIDTH_CHECKER, BANDWIDTH_CHECKER_PORT);
        m_pBandwidthCheckerProtocol =
            pTransport->m_ProtocolManager.GetProtocol<transport::BandwidthCheckerProtocol>(m_BandwidthCheckerProtocolId, transport::PROTOCOL_TYPE_BANDWIDTH_CHECKER);
        // (a call was removed by the linker here)
        m_pBandwidthCheckerProtocol->Initialize();
    }
    m_StationNumMax = static_cast<u16>(transport::Transport::s_pInstance->m_StationNum);
    if (m_StationNumMax > STATION_NUM_MAX) {
        m_StationNumMax = STATION_NUM_MAX;
    }

    // the jobs
    if (m_pCreateMeshJob == nullptr) {
        m_pCreateMeshJob = pFactory->CreateCreateMeshJob();
    }
    if (m_pJoinMeshJob == nullptr) {
        m_pJoinMeshJob = pFactory->CreateJoinMeshJob();
    }
    if (m_pLeaveMeshJob == nullptr) {
        m_pLeaveMeshJob = pFactory->CreateLeaveMeshJob();
    }
    if (m_pProcessJoinRequestJob == nullptr) {
        m_pProcessJoinRequestJob = new ProcessJoinRequestJob();
    }
    if (m_pProcessUpdateMeshJob == nullptr) {
        m_pProcessUpdateMeshJob = new ProcessUpdateMeshJob();
    }
    if (m_pDestroyMeshJob == nullptr) {
        m_pDestroyMeshJob = new DestroyMeshJob();
    }
    if (m_pProcessDestroyMeshJob == nullptr) {
        m_pProcessDestroyMeshJob = new ProcessDestroyMeshJob();
    }
    if (m_pSignatureSettingStorage == nullptr) {
        m_pSignatureSettingStorage = pFactory->CreateSignatureSettingStorage();
    }
    m_RelayMode = setting.m_RelayMode;
    if (m_RelayMode == MODE_1 || m_RelayMode == MODE_2) {
        transport::RelayRouteManager* pRelayRouteManager = transport::Transport::s_pInstance->m_pRelayRouteManager;
        if (pRelayRouteManager == nullptr) {
            pRelayRouteManager = new transport::RelayRouteManager();
            pRelayRouteManager->Initialize(m_StationNumMax, isRelayRouteNetwork);
            transport::Transport::s_pInstance->m_pRelayRouteManager = pRelayRouteManager;
        }
        pRelayRouteManager->m_Unknown0x2B = 0;
        pRelayRouteManager->SetRelayCountMax(m_StationNumMax * m_StationNumMax);
        pRelayRouteManager->SetRttLimit(RELAY_RTT_LIMIT);
        // (a call was removed by the linker here)
        m_pRelayRouteManageJob = new RelayRouteManageJob();
    } else {
        transport::Transport::s_pInstance->m_pRelayRouteManager = nullptr;
        m_pRelayRouteManageJob = nullptr;
    }
    m_HostMigrationMode = pFactory->GetHostMigrationMode();
    if (m_HostMigrationMode == MODE_1 || m_HostMigrationMode == MODE_2) {
        m_pProcessHostMigrationJob = pFactory->CreateProcessHostMigrationJob();
        m_pLeaveWithHostMigrationJob = pFactory->CreateLeaveWithHostMigrationJob();
    } else {
        m_pProcessHostMigrationJob = nullptr;
        m_pLeaveWithHostMigrationJob = nullptr;
    }
    if (m_pKickoutManageJob == nullptr) {
        m_pKickoutManageJob = pFactory->CreateKickoutManageJob();
    }
    if (m_pMissingStationHandler == nullptr) {
        m_pMissingStationHandler = pFactory->CreateMissingStationHandler();
    }
    pTransport->SetKeepAliveInterval(KEEP_ALIVE_INTERVAL_MSEC);
    if (m_pMonitoringDataSender == nullptr) {
        m_pMonitoringDataSender = pFactory->CreateMonitoringDataSender();
    }
    pTransport->SetState(nn::Result());
    pTransport->m_IsUsingStationIdTable = false;
    return nn::Result();
}

// 0x004480B0 (name is ours)
void nn::pia::session::Mesh::SendMonitoringData(bool isSessionEnd)
{
    if (common::IsValidPointer(m_pMonitoringDataSender)) {
        m_pMonitoringDataSender->Send(isSessionEnd);
    }
}

// 0x004480D8 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::Mesh::CleanupJobs()
{
    m_pCreateMeshJob->Cleanup();
    m_pCreateMeshJob->Reset(false);
    m_pJoinMeshJob->Cleanup(nn::Result());
    m_pJoinMeshJob->Reset(false);
    m_pLeaveMeshJob->Cleanup();
    m_pLeaveMeshJob->Reset(false);
    m_pProcessJoinRequestJob->Cleanup();
    m_pProcessJoinRequestJob->Reset(false);
    m_pProcessUpdateMeshJob->Cleanup();
    m_pProcessUpdateMeshJob->Reset(false);
    m_pDestroyMeshJob->Cleanup();
    m_pDestroyMeshJob->Reset(false);
    m_pProcessDestroyMeshJob->Cleanup();
    m_pProcessDestroyMeshJob->Reset(false);
    if (m_pProcessHostMigrationJob != nullptr) {
        m_pProcessHostMigrationJob->Cleanup();
        m_pProcessHostMigrationJob->Reset(false);
    }
    if (m_pLeaveWithHostMigrationJob != nullptr) {
        m_pLeaveWithHostMigrationJob->Cleanup();
        m_pLeaveWithHostMigrationJob->Reset(false);
    }
    if (m_pRelayRouteManageJob != nullptr) {
        m_pRelayRouteManageJob->Cleanup();
        m_pRelayRouteManageJob->Reset(false);
    }
    m_pKickoutManageJob->Cleanup();
    m_pKickoutManageJob->Reset(false);
    CleanupStationsJobs();
    m_CallContext.Reset();
    m_AsyncType = ASYNC_TYPE_NONE;
}

// 0x00448280 | fefates:callgraph
nn::Result nn::pia::session::Mesh::DestroyMesh(nn::pia::common::CallContext* pCallContext)
{
    if (m_LocalStationIndex == STATION_INDEX_UNIDENTIFIED || m_LocalStationIndex != m_HostStationIndex) {
        return common::RESULT_INVALID_STATE;
    }
    if (pCallContext != nullptr && pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_IsJoinable = false;
    if (m_StationNum > 1) {
        // tell the other stations
        if (m_pDestroyMeshJob->Startup(pCallContext, false)) {
            m_pDestroyMeshJob->Ready(false);
        } else {
            m_pDestroyMeshJob->AssociateSystemWith(pCallContext);
        }
    } else {
        m_DisconnectReason = DISCONNECT_REASON_LEAVE;
        MonitoringProcess(DISCONNECT_REASON_LEAVE, MONITORING_PHASE_END);
        Cleanup();
        if (pCallContext != nullptr) {
            pCallContext->InitiateCall();
            pCallContext->SignalSuccess(nn::Result());
        }
    }
    return nn::Result();
}

// 0x00448394 (name is ours)
void nn::pia::session::Mesh::SetJoined(bool isJoined)
{
    if (m_IsJoined == isJoined) {
        return;
    }
    m_IsJoined = isJoined;
    if (m_pEventListener != nullptr) {
        Event event;
        event.m_Type = isJoined == true ? EVENT_TYPE_MESH_JOINED : EVENT_TYPE_MESH_LEFT;
        event.m_StationIndex = STATION_INDEX_UNIDENTIFIED;
        event.m_Unknown0x4 = 0;
        m_pEventListener->OnEvent(event);
    }
}

// 0x004483F4 | fefates:callgraph
nn::Result nn::pia::session::Mesh::joinMeshCore(const nn::pia::transport::StationConnectionInfo& info, nn::pia::common::CallContext* pCallContext)
{
    nn::Result result = common::RESULT_INVALID_STATE;
    if (m_IsJoined || transport::StationManager::s_pInstance->m_pLocalStation != nullptr) {
        return result;
    }
    if (pCallContext != nullptr && pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return result;
    }
    SetupProtocols();
    // (a call was removed by the linker here)
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->CreateLocalStation();
    if (!pLocalStation->Startup(m_pStationProtocol)) {
        return result;
    }
    m_StationBitmap = 0;
    transport::StationConnectionInfoTable* pTable = transport::StationConnectionInfoTable::s_pInstance;
    pLocalStation->m_StationAddress = pTable->m_LocalInfo.m_PublicLocation.m_StationAddress;
    pTable->AddToTable(pLocalStation, pTable->m_LocalInfo);
    transport::Station::IdentificationInfo identificationInfo;
    transport::IdentificationInfoTable::s_pInstance->GetLocalIdentificationInfo(&identificationInfo);
    if (s_pInstance->m_IdentificationCallback == nullptr) {
        identificationInfo.m_Unknown0x48 = 0;
    } else {
        identificationInfo.m_Unknown0x48 = s_pInstance->m_IdentificationCallback();
    }
    transport::IdentificationInfoTable::s_pInstance->SetLocalIdentificationInfo(&identificationInfo);
    result = transport::IdentificationInfoTable::s_pInstance->AddToTable(pLocalStation, &identificationInfo);
    if (result.IsFailure()) {
        transport::IdentificationInfoTable::s_pInstance->Trace(TRACE_FLAG_IDENTIFICATION);
    }
    m_HostStationIndex = HOST_STATION_INDEX_JOINING;
    m_Unknown0x70 = 0;
    if (m_pJoinMeshJob->GetState() != common::Job::EXECUTE_STATE_IDLE && m_pJoinMeshJob->GetState() != common::Job::EXECUTE_STATE_FINISHED) {
        m_pJoinMeshJob->Reset(true);
    }
    if (m_pJoinMeshJob->Startup(info, pCallContext)) {
        m_pJoinMeshJob->Ready(false);
    }
    return result;
}

// 0x004485A4 | fefates:callgraph
void nn::pia::session::Mesh::CleanupStatus()
{
    m_StationNum = 0;
    // the others leave
    for (u16 i = 0; i <= STATION_INDEX_MAX; i++) {
        StationIndex stationIndex = static_cast<StationIndex>(i);
        if (stationIndex != m_LocalStationIndex && CheckStationIndexIsValid(stationIndex)) {
            UnfixDisconnectedId(stationIndex);
        }
    }
    if (m_LocalStationIndex <= STATION_INDEX_MAX) {
        NoticeMeshEvent(EVENT_TYPE_LEAVE, m_LocalStationIndex);
    }
    m_LocalStationIndex = STATION_INDEX_UNIDENTIFIED;
    m_HostStationIndex = STATION_INDEX_UNIDENTIFIED;
    m_StationBitmap = 0;
    transport::Transport::s_pInstance->m_ProtocolManager.CleanupProtocols();
    transport::StationConnectionInfoTable::s_pInstance->ClearTable();
    transport::IdentificationInfoTable::s_pInstance->ClearTable();
    transport::StationManager* pManager = transport::StationManager::s_pInstance;
    while (pManager->m_ActiveStations.Begin() != pManager->m_ActiveStations.End()) {
        transport::Station* pStation = *pManager->m_ActiveStations.Begin();
        pStation->Cleanup();
        pStation->CleanupJobs();
        pManager->DestroyStation(pStation);
    }
    transport::Transport::s_pInstance->OutputStreamUpdateEvent();
    m_Unknown0x70 = 0xFFFF;
    SetJoined(false);
    SyncClockProtocol* pSyncClockProtocol =
        transport::Transport::s_pInstance->m_ProtocolManager.GetProtocol<SyncClockProtocol>(m_SyncClockProtocolId, transport::PROTOCOL_TYPE_SYNC_CLOCK);
    if (common::IsValidPointer(pSyncClockProtocol)) {
        pSyncClockProtocol->m_SyncClock.ClearBaseTime();
    }
    m_IsHostMigrationEnabled = true;
    if (!m_Unknown0xA5) {
        common::g_SessionBeginMonitoringContent.Cleanup();
        common::g_SessionStateMonitoringContent.Cleanup();
    }
    m_IsMonitoringDataSent = false;
    if (common::IsValidPointer(m_pMonitoringDataSender)) {
        m_pMonitoringDataSender->vf_0x08();
    }
    m_Unknown0x61 = false;
}

// 0x00448878 (name is ours)
nn::Result nn::pia::session::Mesh::JoinMeshAsync(const nn::pia::transport::StationConnectionInfo& info)
{
    if (m_CallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_AsyncType != ASYNC_TYPE_NONE) {
        m_CallContext.Reset();
        m_AsyncType = ASYNC_TYPE_NONE;
    }
    nn::Result result = joinMeshCore(info, &m_CallContext);
    if (result.IsSuccess()) {
        m_AsyncType = ASYNC_TYPE_JOIN;
    }
    return result;
}

// 0x004488D8 (name is ours)
void nn::pia::session::Mesh::Disconnect(DisconnectReason reason)
{
    if (reason != DISCONNECT_REASON_9) {
        s_pInstance->m_DisconnectReason = reason;
        s_pInstance->MonitoringProcess(reason, MONITORING_PHASE_END);
        s_pInstance->CleanupStatus();
        return;
    }
    s_pInstance->SetJoined(false);
}

// 0x00448964 (name is ours)
nn::Result nn::pia::session::Mesh::CreateInstance(const Setting& setting)
{
    if (!session::IsInitialized()) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!session::IsInSetupMode()) {
        return common::RESULT_INVALID_STATE;
    }
    if (transport::Transport::s_pInstance == nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    if (!common::IsValidPointer(setting.m_pNetworkFactory)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (setting.m_RelayMode != 0 && setting.m_RelayMode != MODE_1 && setting.m_RelayMode != MODE_2) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    // a network with relay routes only with relay mode 2
    bool isValid = setting.m_RelayMode == MODE_2 || !s_GlobalSetting.m_IsRelayRouteNetwork;
    if (!setting.m_pNetworkFactory->IsRelayRouteSupported() && setting.m_RelayMode != 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (!isValid) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (s_pInstance != nullptr) {
        return common::RESULT_ALREADY_EXISTS;
    }
    s_pInstance = new Mesh();
    nn::Result result = s_pInstance->Initialize(setting, s_GlobalSetting.m_IsRelayRouteNetwork);
    if (result.IsFailure()) {
        if (s_pInstance != nullptr) {
            delete s_pInstance;
        }
        s_pInstance = nullptr;
        return result;
    }
    common::g_SessionBeginMonitoringContent.m_Unknown0xA4 = setting.m_RelayMode;
    return nn::Result();
}

// 0x00448AB4 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::Mesh::FixConnectedId(nn::pia::StationIndex stationIndex)
{
    transport::AttendanceTable::s_pInstance->Update(true, stationIndex);
    transport::ProtocolEvent event(transport::ProtocolEvent::TYPE_JOIN, stationIndex);
    if (transport::Transport::s_pInstance->m_ProtocolManager.UpdateProtocolEvent(event).IsFailure()) {
        return;
    }
    transport::Transport::s_pInstance->OutputStreamUpdateEvent();
    StartUse(stationIndex);
    NoticeMeshEvent(EVENT_TYPE_JOIN, stationIndex);
}

// 0x00448B88 | fefates:callgraph
nn::Result nn::pia::session::Mesh::LeaveMeshAsync()
{
    if (m_CallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_AsyncType != ASYNC_TYPE_NONE) {
        m_CallContext.Reset();
        m_AsyncType = ASYNC_TYPE_NONE;
    }
    nn::Result result = LeaveMesh(&m_CallContext);
    if (result.IsSuccess()) {
        m_AsyncType = ASYNC_TYPE_LEAVE;
    }
    return result;
}

// 0x00448BE0 | fefates:callgraph
nn::Result nn::pia::session::Mesh::SetupProtocols()
{
    m_pMeshProtocol->Startup();
    // (a call with the station protocol was removed by the linker here)
    return nn::Result();
}

// 0x00448C00 | fefates:callgraph
nn::Result nn::pia::session::Mesh::CreateMeshAsync()
{
    if (m_CallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_AsyncType != ASYNC_TYPE_NONE) {
        m_CallContext.Reset();
        m_AsyncType = ASYNC_TYPE_NONE;
    }
    nn::Result result = CreateMesh(&m_CallContext);
    if (result.IsSuccess()) {
        m_AsyncType = ASYNC_TYPE_CREATE;
    }
    return result;
}

// 0x00448C58 | fefates:bytes
void nn::pia::session::Mesh::DestroyInstance()
{
    if (s_pInstance == nullptr) {
        return;
    }
    s_pInstance->Cleanup();
    if (s_pInstance != nullptr) {
        delete s_pInstance;
    }
    s_pInstance = nullptr;
}

// 0x00448CD8 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::Mesh::NoticeMeshEvent(nn::pia::session::Mesh::EventType type, nn::pia::StationIndex stationIndex)
{
    if (type == EVENT_TYPE_JOIN) {
        transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(stationIndex);
        if (common::IsValidPointer(pStation)) {
            if (!transport::Transport::s_pInstance->m_IsUsingStationIdTable) {
                StationId stationId;
                transport::Transport::s_pInstance->ConvertToStationId(&stationId, stationIndex);
                pStation->SetStationId(stationId);
            }
            pStation->m_Unknown0x69 = true;
        }
    } else if (type == EVENT_TYPE_LEAVE) {
        transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(stationIndex);
        if (common::IsValidPointer(pStation)) {
            pStation->m_Unknown0x69 = false;
        }
    }
    if (m_EventCallback != nullptr) {
        StationId stationId;
        transport::Transport::s_pInstance->ConvertToStationId(&stationId, stationIndex);
        m_EventCallback(type, stationId);
    }
    if (m_pEventListener != nullptr) {
        Event event;
        event.m_Type = type;
        event.m_StationIndex = stationIndex;
        event.m_Unknown0x4 = 0;
        m_pEventListener->OnEvent(event);
    }
}

// 0x00448DF0 | fefates:callgraph
nn::Result nn::pia::session::Mesh::DestroyMeshAsync()
{
    if (m_CallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_AsyncType != ASYNC_TYPE_NONE) {
        m_CallContext.Reset();
        m_AsyncType = ASYNC_TYPE_NONE;
    }
    nn::Result result = DestroyMesh(&m_CallContext);
    if (result.IsSuccess()) {
        m_AsyncType = ASYNC_TYPE_DESTROY;
    }
    return result;
}

// 0x00448E48 | fefates:callgraph
u8 nn::pia::session::Mesh::CheckApprovalJoin(nn::pia::transport::Station* pStation)
{
    if (!m_IsJoinable || m_Unknown0x61) {
        return APPROVAL_NOT_JOINABLE;
    }
    if (m_StationNum >= m_StationNumMax || GetFreeStationIndex() == STATION_INDEX_UNIDENTIFIED) {
        return APPROVAL_FULL;
    }
    transport::Transport* pTransport = transport::Transport::s_pInstance;
    if (!pTransport->m_IsUsingStationIdTable) {
        return APPROVAL_OK;
    }
    // the station id table must have room for it
    transport::StationConnectionInfo info;
    if (transport::StationConnectionInfoTable::s_pInstance->GetStationConnectionInfo(pStation, &info).IsFailure()) {
        return APPROVAL_NOT_JOINABLE;
    }
    u32 principalId = info.m_PublicLocation.m_PrincipalId;
    if (principalId == 0) {
        principalId = info.m_PrivateLocation.m_PrincipalId;
    }
    if (principalId != 0 && transport::Transport::s_pInstance->m_pStationIdTable->FindCore(principalId) != nullptr) {
        return APPROVAL_OK;
    }
    if (static_cast<u32>(transport::Transport::s_pInstance->m_pStationIdTable->GetEntryNum()) >= transport::Transport::s_pInstance->m_StationNum) {
        return APPROVAL_FULL;
    }
    transport::StationIdTable* pTable = transport::Transport::s_pInstance->m_pStationIdTable;
    if (static_cast<u32>(pTable->GetEntryNum()) < pTable->m_EntryNumMax) {
        return APPROVAL_OK;
    }
    return APPROVAL_FULL;
}

// 0x00448FAC
nn::Result nn::pia::session::Mesh::GetJoinMeshAsyncResult() const
{
    if (m_AsyncType != ASYNC_TYPE_JOIN || !m_CallContext.IsFinished()) {
        return common::RESULT_INVALID_STATE;
    }
    return m_CallContext.m_Result;
}

// 0x00448FE0 (name is ours)
void nn::pia::session::Mesh::UpdateMonitoringData()
{
    MonitoringProcess(DISCONNECT_REASON_NONE, MONITORING_PHASE_UPDATE);
}

// 0x00448FEC | fefates:callgraph
void nn::pia::session::Mesh::MonitoringProcess(nn::pia::session::Mesh::DisconnectReason reason, unsigned char phase)
{
    if (!m_IsMonitoring) {
        return;
    }
    s64 elapsed = (common::Scheduler::s_pInstance->m_DispatchTime - m_MonitoringStartTime).GetTick();
    if (reason != DISCONNECT_REASON_1 && m_pKickoutManageJob->m_Reason != 0) {
        reason = m_pKickoutManageJob->m_Reason == KICKOUT_REASON_2 ? DISCONNECT_REASON_KICKOUT_4 : DISCONNECT_REASON_KICKOUT_5;
    }
    u8 stationNum = static_cast<u8>(m_StationNum);
    if (stationNum == 0) {
        stationNum = 0xFF;
    }
    u32 version = m_Unknown0x70;
    if (version == 0) {
        version = 0xFFFFFFFF;
    }
    u32 directionsVersion;
    transport::RelayRouteManager* pRelayRouteManager = transport::Transport::s_pInstance->m_pRelayRouteManager;
    if (common::IsValidPointer(pRelayRouteManager) && pRelayRouteManager->m_DirectionsVersionLow != 0) {
        directionsVersion = pRelayRouteManager->m_DirectionsVersionLow;
    } else {
        directionsVersion = 0xFFFFFFFF;
    }
    // (a call was removed by the linker here)
    common::g_SessionStateMonitoringContent.m_Unknown0x334 = elapsed / common::TimeSpan::GetTicksPerMSec().GetTick();
    common::g_SessionStateMonitoringContent.m_Unknown0x338 = reason;
    common::g_SessionStateMonitoringContent.m_Unknown0x339 = stationNum;
    common::g_SessionStateMonitoringContent.m_Unknown0x33C = version;
    common::g_SessionStateMonitoringContent.m_Unknown0x340 = directionsVersion;
    m_pProcessUpdateMeshJob->SetMonitoringData();
    if (common::IsValidPointer(m_pProcessHostMigrationJob)) {
        m_pProcessHostMigrationJob->SetMonitoringData();
    }
    if (m_pStationProtocol != nullptr) {
        common::g_SessionStateMonitoringContent.m_Unknown0x3C4 = m_pStationProtocol->m_AddressChangedNum;
    }
    common::Scheduler::s_pInstance->SetMonitoringData();
    transport::Transport::s_pInstance->SetMonitoringNetworkRtt(false);
    transport::TransportAnalyzer::s_pInstance->SetMonitoringData();
    transport::ThreadStreamManager::s_pInstance->SetMonitoringData();
    if (!m_IsMonitoringDataSent && common::IsValidPointer(m_pMonitoringDataSender)) {
        m_pMonitoringDataSender->Send(phase);
    }
    if (phase == MONITORING_PHASE_END) {
        m_IsMonitoring = false;
    }
}

// 0x00449174
nn::Result nn::pia::session::Mesh::GetLeaveMeshAsyncResult() const
{
    if (m_AsyncType != ASYNC_TYPE_LEAVE || !m_CallContext.IsFinished()) {
        return common::RESULT_INVALID_STATE;
    }
    return m_CallContext.m_Result;
}

// 0x004491A8 (name is ours)
nn::Result nn::pia::session::Mesh::CancelJoinMeshAsync()
{
    if (m_AsyncType != ASYNC_TYPE_JOIN || m_CallContext.GetState() != common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_CallContext.Cancel();
    return nn::Result();
}

// 0x004491D8 | fefates:bytes [tier B]
void nn::pia::session::Mesh::CleanupStationsJobs()
{
    for (transport::Station** it = transport::StationManager::s_pInstance->m_ActiveStations.Begin();
         it != transport::StationManager::s_pInstance->m_ActiveStations.End(); it++) {
        (*it)->CleanupJobs();
    }
}

// 0x0044921C
nn::Result nn::pia::session::Mesh::GetCreateMeshAsyncResult() const
{
    if (m_AsyncType != ASYNC_TYPE_CREATE || !m_CallContext.IsFinished()) {
        return common::RESULT_INVALID_STATE;
    }
    return m_CallContext.m_Result;
}

// 0x00449250 (name is ours)
u8 nn::pia::session::Mesh::GetJoinMeshJobPhase() const
{
    if (m_pJoinMeshJob == nullptr) {
        return 0;
    }
    return m_pJoinMeshJob->GetPhase();
}

// 0x00449264
bool nn::pia::session::Mesh::IsJoinMeshAsyncCompleted() const
{
    return m_AsyncType == ASYNC_TYPE_JOIN && m_CallContext.IsFinished();
}

// 0x00449294 (name is ours)
void nn::pia::session::Mesh::EndMonitoring(DisconnectReason reason)
{
    MonitoringProcess(reason, MONITORING_PHASE_END);
}

// 0x0044929C | fefates:bytes-fuzzy [tier B]
void nn::pia::session::Mesh::UnfixDisconnectedId(nn::pia::StationIndex stationIndex)
{
    transport::AttendanceTable::s_pInstance->Update(false, stationIndex);
    transport::ProtocolEvent event(transport::ProtocolEvent::TYPE_LEAVE, stationIndex);
    if (transport::Transport::s_pInstance->m_ProtocolManager.UpdateProtocolEvent(event).IsFailure()) {
        return;
    }
    m_pKickoutManageJob->SetLeaveEventStationIndex(stationIndex);
    NoticeMeshEvent(EVENT_TYPE_LEAVE, stationIndex);
    if (stationIndex <= STATION_INDEX_MAX) {
        u32 bit = GetStationBit(stationIndex);
        m_StationBitmap &= ~bit;
        if (m_pRelayRouteManageJob != nullptr) {
            m_pRelayRouteManageJob->m_StationBitmap &= ~bit;
        }
    }
}

// 0x00449374
nn::Result nn::pia::session::Mesh::GetDestroyMeshAsyncResult() const
{
    if (m_AsyncType != ASYNC_TYPE_DESTROY || !m_CallContext.IsFinished()) {
        return common::RESULT_INVALID_STATE;
    }
    return m_CallContext.m_Result;
}

// 0x004493A8
bool nn::pia::session::Mesh::IsLeaveMeshAsyncCompleted() const
{
    return m_AsyncType == ASYNC_TYPE_LEAVE && m_CallContext.IsFinished();
}

// 0x004493D8 (name is ours)
void nn::pia::session::Mesh::SetUnknown0xA5(bool value)
{
    m_Unknown0xA5 = value;
}

// 0x004493E0 (name is ours)
nn::pia::StationIndex nn::pia::session::Mesh::GetFreeStationIndex() const
{
    u32 bit = 1;
    for (u16 i = 0; i < STATION_NUM_MAX; i++) {
        if ((m_StationBitmap & bit) == 0) {
            return static_cast<StationIndex>(i);
        }
        bit <<= 1;
    }
    return STATION_INDEX_UNIDENTIFIED;
}

// 0x0044941C
bool nn::pia::session::Mesh::IsCreateMeshAsyncCompleted() const
{
    return m_AsyncType == ASYNC_TYPE_CREATE && m_CallContext.IsFinished();
}

// 0x0044944C (name is ours)
void nn::pia::session::Mesh::SetSessionBeginMonitoringData()
{
    if (m_pStationProtocol != nullptr) {
        common::g_SessionBeginMonitoringContent.m_Unknown0x258 = m_pStationProtocol->m_AddressChangedNum;
    }
    if (!m_IsMonitoringDataSent && common::IsValidPointer(m_pMonitoringDataSender)) {
        m_pMonitoringDataSender->Send(MONITORING_PHASE_BEGIN);
    }
}

// 0x004494A0
bool nn::pia::session::Mesh::IsDestroyMeshAsyncCompleted() const
{
    return m_AsyncType == ASYNC_TYPE_DESTROY && m_CallContext.IsFinished();
}

// 0x004494D0 (name is ours)
void nn::pia::session::Mesh::ClearStationBitmap()
{
    m_StationBitmap = 0;
}

// 0x004494DC (name is ours)
void nn::pia::session::Mesh::ClearEventListener()
{
    m_pEventListener = nullptr;
}

// 0x004494E8 | fefates:bytes [tier B]
void nn::pia::session::Mesh::NotifyLeaveStationAddress(const nn::pia::common::StationAddress& address)
{
    transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(address);
    address.Trace(TRACE_FLAG_ADDRESS);
    if (pStation != nullptr) {
        pStation->m_State = transport::Station::STATION_STATE_DISCONNECTED;
    }
}

// 0x00449534 | fefates:callgraph
void nn::pia::session::Mesh::SetHostMigrationStartFlag(bool flag)
{
    m_HostMigrationStartFlag = flag;
}

// 0x0044953C (name is ours)
void nn::pia::session::Mesh::SetSyncClockRequestInterval(s32 intervalMSec)
{
    transport::Transport* pTransport = transport::Transport::s_pInstance;
    if (!common::IsValidPointer(pTransport)) {
        return;
    }
    SyncClockProtocol* pProtocol = pTransport->m_ProtocolManager.GetProtocol<SyncClockProtocol>(m_SyncClockProtocolId, transport::PROTOCOL_TYPE_SYNC_CLOCK);
    if (common::IsValidPointer(pProtocol)) {
        pProtocol->m_RequestIntervalMSec = intervalMSec;
    }
}

// 0x00449584 (name is ours)
void nn::pia::session::Mesh::ClearSessionBeginMonitoringData()
{
    common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
    content.m_JoinStationNum = 0xFF;
    content.m_JoinResult = 0xFFFFFFFF;
    content.m_JoinElapsedMSec = 0xFFFFFFFF;
    content.m_RelayedStationNum = 0xFF;
    for (u32 i = 0; i < 23; i++) {
        content.m_RelayedStationPrincipalIdHashes[i] = 0xFFFFFFFF;
    }
    content.m_RelayStationNum = 0xFF;
    for (u32 i = 0; i < STATION_NUM_MAX; i++) {
        content.m_RelayStationPrincipalIdHashes[i] = 0xFFFFFFFF;
    }
    content.m_Unknown0x168 = 0xFFFFFFFF;
    content.m_HostPrincipalId = 0xFFFFFFFF;
    content.m_Unknown0x258 = 0xFFFFFFFF;
    content.m_JoinPhase = 0xFF;
    for (u32 i = 0; i < 23; i++) {
        content.m_Unknown0x274[i] = 0xFFFFFFFF;
        content.m_Unknown0x2D0[i] = 0xFFFFFFFF;
        content.m_Unknown0x32C[i] = 0xFFFFFFFF;
        content.m_Unknown0x388[i] = 0xFFFFFFFF;
        content.m_Unknown0x3E4[i] = 0xFFFFFFFF;
        content.m_Unknown0x440[i] = 0xFF;
        content.m_Unknown0x457[i] = 0xFF;
        content.m_Unknown0x46E[i] = 0xFF;
        content.m_Unknown0x485[i] = 0xFF;
    }
}

// 0x00449634 (name is ours)
nn::Result nn::pia::session::Mesh::LeaveMeshWithHostMigration(nn::pia::common::CallContext* pCallContext)
{
    if (!m_IsHostMigrationEnabled || m_LocalStationIndex == STATION_INDEX_UNIDENTIFIED || m_LocalStationIndex != m_HostStationIndex) {
        return common::RESULT_INVALID_STATE;
    }
    if (pCallContext != nullptr && pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_IsJoinable = false;
    if (m_StationNum > 1) {
        if (m_pLeaveWithHostMigrationJob->Startup(pCallContext)) {
            m_pLeaveWithHostMigrationJob->Ready(false);
        }
    } else {
        m_DisconnectReason = DISCONNECT_REASON_LEAVE;
        MonitoringProcess(DISCONNECT_REASON_LEAVE, MONITORING_PHASE_END);
        Cleanup();
        if (pCallContext != nullptr) {
            pCallContext->InitiateCall();
            pCallContext->SignalSuccess(nn::Result());
        }
    }
    return nn::Result();
}

// 0x0044973C (name is ours)
nn::Result nn::pia::session::Mesh::SetGlobalSetting(const GlobalSetting& setting)
{
    if (s_pInstance != nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    s_GlobalSetting = setting;
    return nn::Result();
}

// 0x00449764 (name is ours)
void nn::pia::session::Mesh::ClearIdentificationCallback()
{
    m_IdentificationCallback = nullptr;
}

// 0x00449770 (name is ours)
nn::Result nn::pia::session::Mesh::LeaveMeshWithHostMigrationAsync()
{
    if (m_CallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_AsyncType != ASYNC_TYPE_NONE) {
        m_CallContext.Reset();
        m_AsyncType = ASYNC_TYPE_NONE;
    }
    nn::Result result = LeaveMeshWithHostMigration(&m_CallContext);
    if (result.IsSuccess()) {
        m_AsyncType = ASYNC_TYPE_LEAVE_WITH_HOST_MIGRATION;
    }
    return result;
}

// 0x004497C8
nn::Result nn::pia::session::Mesh::GetLeaveMeshWithHostMigrationAsyncResult() const
{
    if (m_AsyncType != ASYNC_TYPE_LEAVE_WITH_HOST_MIGRATION || !m_CallContext.IsFinished()) {
        return common::RESULT_INVALID_STATE;
    }
    return m_CallContext.m_Result;
}

// 0x004497FC
bool nn::pia::session::Mesh::IsLeaveMeshWithHostMigrationAsyncCompleted() const
{
    return m_AsyncType == ASYNC_TYPE_LEAVE_WITH_HOST_MIGRATION && m_CallContext.IsFinished();
}

// 0x0044982C (name is ours)
void nn::pia::session::Mesh::ClearJoinApprovalCallback()
{
    m_JoinApprovalCallback = nullptr;
}

// 0x00449838 (name is ours)
nn::Result nn::pia::session::Mesh::SetHostCandidateCallback(HostCandidateCallback callback)
{
    if (m_HostMigrationMode != MODE_2) {
        return common::RESULT_INVALID_STATE;
    }
    m_HostCandidateCallback = callback;
    return nn::Result();
}

// 0x00449854 (name is ours)
void nn::pia::session::Mesh::ClearHostCandidateCallback()
{
    m_HostCandidateCallback = nullptr;
}

// 0x00449860 | fefates:callgraph
void nn::pia::session::Mesh::Cleanup()
{
    if (!m_IsStarted) {
        return;
    }
    CleanupJobs();
    CleanupStatus();
    transport::Transport::s_pInstance->Cleanup();
    common::SignatureManager::s_pInstance->Cleanup();
    m_IsStarted = false;
}

// 0x004498B0 | fefates:callgraph
nn::Result nn::pia::session::Mesh::Startup(const StartupSetting& setting)
{
    if (session::IsInSetupMode() || m_IsStarted) {
        return common::RESULT_INVALID_STATE;
    }
    if (transport::Transport::s_pInstance == nullptr || !common::IsValidPointer(transport::IdentificationInfoTable::s_pInstance)) {
        return common::RESULT_INVALID_STATE;
    }
    if (!s_GlobalSetting.m_IsTimeoutFree && (setting.m_TimeoutMSec < TIMEOUT_MIN_MSEC || setting.m_TimeoutMSec > TIMEOUT_MAX_MSEC)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_IsBandwidthCheckEnabled) {
        nn::Result result = m_pBandwidthCheckerProtocol->Setup(setting.m_BandwidthCheckBandwidth, setting.m_BandwidthCheckPacketSize, setting.m_IsBandwidthCheckOneWay,
                                                               setting.m_BandwidthCheckDurationMSec);
        if (result.IsFailure()) {
            return result;
        }
    }
    if (setting.m_pPlayerName != nullptr && !common::IsValidPointer(setting.m_pPlayerName)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    {
        common::SignatureManager::Setting signatureSetting;
        signatureSetting.m_pDefaultSetting = &setting.m_SignatureSetting;
        nn::Result result = common::SignatureManager::s_pInstance->Setup(signatureSetting);
        if (result.IsFailure()) {
            return result;
        }
    }
    if (!setting.m_IsHostMigrationEnabled) {
        common::g_SessionBeginMonitoringContent.m_Unknown0xA5 = 0;
    } else {
        if (m_pProcessHostMigrationJob != nullptr && !m_pProcessHostMigrationJob->IsValidHostMigrationSetting()) {
            return common::RESULT_INVALID_STATE;
        }
        common::g_SessionBeginMonitoringContent.m_Unknown0xA5 = m_HostMigrationMode;
    }
    m_IsHostMigrationEnabled = setting.m_IsHostMigrationEnabled;
    common::g_SessionBeginMonitoringContent.m_Unknown0x144 = setting.m_SignatureSetting.m_KeySize;

    // the identification info of the local station
    transport::Station::IdentificationInfo identificationInfo;
    if (setting.m_pPlayerName != nullptr) {
        memcpy(&identificationInfo.m_PlayerName, setting.m_pPlayerName, sizeof(identificationInfo.m_PlayerName));
    } else {
        memset(&identificationInfo.m_PlayerName, 0, sizeof(identificationInfo.m_PlayerName));
    }
    memcpy(identificationInfo.m_Unknown0x20, setting.m_pIdentificationData, sizeof(identificationInfo.m_Unknown0x20));
    identificationInfo.m_Unknown0x40 = 0;
    identificationInfo.m_Unknown0x42 = setting.m_pIdentificationData[34];
    if (identificationInfo.m_Unknown0x42 > IDENTIFICATION_NAME_LENGTH_MAX) {
        identificationInfo.m_Unknown0x42 = IDENTIFICATION_NAME_LENGTH_MAX;
    }
    identificationInfo.m_Unknown0x44 = IDENTIFICATION_VERSION;
    identificationInfo.m_Unknown0x43 = setting.m_pIdentificationData[35];
    transport::IdentificationInfoTable::s_pInstance->SetLocalIdentificationInfo(&identificationInfo);
    m_pSignatureSettingStorage->SetSetting(&setting.m_SignatureSetting);

    // the timeouts of the jobs
    s32 processTimeout = PROCESS_TIMEOUT_MSEC;
    m_pProcessJoinRequestJob->m_TimeoutMSec = processTimeout;
    m_pProcessUpdateMeshJob->m_ShortTimeLimitMSec = setting.m_TimeoutMSec + UPDATE_EXTRA_TIMEOUT_MSEC;
    m_pProcessUpdateMeshJob->m_TimeLimitMSec = setting.m_TimeoutMSec + processTimeout + UPDATE_EXTRA_TIMEOUT_MSEC;
    m_pProcessUpdateMeshJob->m_DirectConnectionTimeLimitMSec = processTimeout / 2;
    u32 timeout = setting.m_TimeoutMSec;
    if (!s_GlobalSetting.m_IsTimeoutFree) {
        if (timeout < TIMEOUT_MIN_MSEC) {
            timeout = TIMEOUT_MIN_MSEC;
        } else if (timeout > TIMEOUT_MAX_MSEC) {
            timeout = TIMEOUT_MAX_MSEC;
        }
    }
    m_pMeshProtocol->m_KeepAliveTimeoutMSec = timeout;
    m_pStationProtocol->m_ProcessTimeoutMSec = processTimeout;
    transport::Transport::s_pInstance->SetKeepAliveInterval(setting.m_KeepAliveIntervalMSec);
    common::g_SessionBeginMonitoringContent.m_Unknown0x9C = setting.m_TimeoutMSec;
    common::g_SessionBeginMonitoringContent.m_Unknown0xA0 = setting.m_KeepAliveIntervalMSec;
    m_pKickoutManageJob->ClearEntries();
    transport::RelayRouteManager* pRelayRouteManager = transport::Transport::s_pInstance->m_pRelayRouteManager;
    if (common::IsValidPointer(pRelayRouteManager)) {
        pRelayRouteManager->InitRelayRouteManagerData();
    }
    transport::Transport::s_pInstance->Startup(nullptr, setting.m_pCryptoSetting);
    m_MonitoringStartTime = common::Time();
    m_IsMonitoring = false;
    m_pProcessUpdateMeshJob->ClearMonitoringData();
    if (common::IsValidPointer(m_pProcessHostMigrationJob)) {
        m_pProcessHostMigrationJob->ClearMonitoringData();
    }
    m_DisconnectReason = DISCONNECT_REASON_1;
    m_IsStarted = true;
    return nn::Result();
}

// 0x00449C04 | fefates:bytes-fuzzy [tier B]
void nn::pia::session::Mesh::StartUse(nn::pia::StationIndex stationIndex)
{
    if (stationIndex > STATION_INDEX_MAX) {
        return;
    }
    u32 bit = GetStationBit(stationIndex);
    m_StationBitmap |= bit;
    if (m_pRelayRouteManageJob != nullptr) {
        m_pRelayRouteManageJob->m_StationBitmap |= bit;
    }
}

// 0x00449C5C | fefates:callgraph
nn::Result nn::pia::session::Mesh::LeaveMesh(nn::pia::common::CallContext* pCallContext)
{
    if (pCallContext != nullptr && pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    if (!m_IsJoined) {
        if (pCallContext != nullptr) {
            pCallContext->InitiateCall();
            pCallContext->SignalSuccess(nn::Result());
        }
        return nn::Result();
    }
    if (m_LocalStationIndex == STATION_INDEX_UNIDENTIFIED || m_LocalStationIndex == m_HostStationIndex) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_pLeaveMeshJob->Startup(pCallContext)) {
        m_pLeaveMeshJob->Ready(false);
        return nn::Result();
    }
    // a leave runs already: the call context follows it
    if (m_pKickoutManageJob->IsRunning() && m_pKickoutManageJob->AssociateKickoutWith(pCallContext)) {
        return nn::Result();
    }
    if (!m_pLeaveMeshJob->RegisterExtraCallback(pCallContext)) {
        return common::RESULT_INVALID_STATE;
    }
    return nn::Result();
}

// 0x00449D4C
nn::pia::session::Mesh::Mesh()
    : m_pCreateMeshJob(nullptr), m_pJoinMeshJob(nullptr), m_pLeaveMeshJob(nullptr), m_pProcessJoinRequestJob(nullptr), m_pProcessUpdateMeshJob(nullptr),
      m_pDestroyMeshJob(nullptr), m_pProcessDestroyMeshJob(nullptr), m_pProcessHostMigrationJob(nullptr), m_pLeaveWithHostMigrationJob(nullptr),
      m_pSignatureSettingStorage(nullptr), m_pRelayRouteManageJob(nullptr), m_pKickoutManageJob(nullptr), m_pMissingStationHandler(nullptr),
      m_EventCallback(nullptr), m_JoinApprovalCallback(nullptr), m_HostCandidateCallback(nullptr), m_IdentificationCallback(nullptr), m_MeshProtocolId(0, 0),
      m_pMeshProtocol(nullptr), m_pStationProtocol(nullptr), m_BandwidthCheckerProtocolId(0, 0), m_pBandwidthCheckerProtocol(nullptr), m_StationNum(0),
      m_StationNumMax(0), m_IsJoinable(true), m_Unknown0x61(false), m_HostStationIndex(STATION_INDEX_UNIDENTIFIED),
      m_LocalStationIndex(STATION_INDEX_UNIDENTIFIED), m_IsJoined(false), m_DisconnectReason(DISCONNECT_REASON_NONE), m_HostMigrationMode(0),
      m_IsBandwidthCheckEnabled(false), m_IsHostMigrationEnabled(true), m_StationBitmap(0), m_Unknown0x70(0xFFFF), m_SyncClockProtocolId(0, 0),
      m_MonitoringStartTime(), m_IsMonitoring(false), m_IsStarted(false), m_HostMigrationStartFlag(false), m_Unknown0x83(false),
      m_pMonitoringDataSender(nullptr), m_CallContext(), m_AsyncType(ASYNC_TYPE_NONE), m_pEventListener(nullptr), m_IsMonitoringDataSent(false),
      m_Unknown0xA5(false)
{
}

// 0x00449E50
// 0x00449E40 (deleting dtor)
nn::pia::session::Mesh::~Mesh()
{
    if (m_pCreateMeshJob != nullptr) {
        delete m_pCreateMeshJob;
        m_pCreateMeshJob = nullptr;
    }
    if (m_pJoinMeshJob != nullptr) {
        delete m_pJoinMeshJob;
        m_pJoinMeshJob = nullptr;
    }
    if (m_pLeaveMeshJob != nullptr) {
        delete m_pLeaveMeshJob;
        m_pLeaveMeshJob = nullptr;
    }
    if (m_pProcessJoinRequestJob != nullptr) {
        delete m_pProcessJoinRequestJob;
        m_pProcessJoinRequestJob = nullptr;
    }
    if (m_pProcessUpdateMeshJob != nullptr) {
        delete m_pProcessUpdateMeshJob;
        m_pProcessUpdateMeshJob = nullptr;
    }
    if (m_pDestroyMeshJob != nullptr) {
        delete m_pDestroyMeshJob;
        m_pDestroyMeshJob = nullptr;
    }
    if (m_pProcessDestroyMeshJob != nullptr) {
        delete m_pProcessDestroyMeshJob;
        m_pProcessDestroyMeshJob = nullptr;
    }
    if (m_pProcessHostMigrationJob != nullptr) {
        delete m_pProcessHostMigrationJob;
        m_pProcessHostMigrationJob = nullptr;
    }
    if (m_pLeaveWithHostMigrationJob != nullptr) {
        delete m_pLeaveWithHostMigrationJob;
        m_pLeaveWithHostMigrationJob = nullptr;
    }
    if (m_pSignatureSettingStorage != nullptr) {
        delete m_pSignatureSettingStorage;
        m_pSignatureSettingStorage = nullptr;
    }
    if (m_pRelayRouteManageJob != nullptr) {
        delete m_pRelayRouteManageJob;
        m_pRelayRouteManageJob = nullptr;
    }
    transport::RelayRouteManager* pRelayRouteManager = transport::Transport::s_pInstance->m_pRelayRouteManager;
    if (pRelayRouteManager != nullptr) {
        transport::Transport::s_pInstance->m_pRelayRouteManager = nullptr;
        pRelayRouteManager->Finalize();
        delete pRelayRouteManager;
    }
    if (m_pKickoutManageJob != nullptr) {
        delete m_pKickoutManageJob;
        m_pKickoutManageJob = nullptr;
    }
    if (m_pMissingStationHandler != nullptr) {
        delete m_pMissingStationHandler;
        m_pMissingStationHandler = nullptr;
    }
    if (m_SyncClockProtocolId.m_Id != 0) {
        transport::Transport::s_pInstance->m_ProtocolManager.DestroyProtocol(m_SyncClockProtocolId.m_Id);
        m_SyncClockProtocolId = transport::ProtocolId(0, 0);
    }
    if (m_IsBandwidthCheckEnabled && m_BandwidthCheckerProtocolId.m_Id != 0) {
        m_pBandwidthCheckerProtocol->Finalize();
        transport::Transport::s_pInstance->m_ProtocolManager.DestroyProtocol(m_BandwidthCheckerProtocolId.m_Id);
        m_BandwidthCheckerProtocolId = transport::ProtocolId(0, 0);
        m_pBandwidthCheckerProtocol = nullptr;
    }
    if (common::IsValidPointer(m_pMeshProtocol)) {
        m_pMeshProtocol->Finalize();
    }
    if (transport::Transport::s_pInstance != nullptr) {
        transport::Transport::s_pInstance->m_ProtocolManager.DestroyProtocol(m_MeshProtocolId.m_Id);
        transport::Transport::s_pInstance->m_ProtocolManager.CleanupProtocols();
        transport::Transport::s_pInstance->SetState(common::RESULT_INVALID_STATE);
    }
    m_MeshProtocolId = transport::ProtocolId(0, 0);
    m_pMeshProtocol = nullptr;
    m_pStationProtocol = nullptr;
    if (m_BandwidthCheckerProtocolId.m_Id == 0) {
        m_pBandwidthCheckerProtocol = nullptr;
    }
    if (m_pMonitoringDataSender != nullptr) {
        delete m_pMonitoringDataSender;
        m_pMonitoringDataSender = nullptr;
    }
}

// 0x00734218 (name is ours)
nn::pia::session::Mesh::DisconnectReason nn::pia::session::Mesh::GetDisconnectReason() const
{
    if (m_IsJoined == true) {
        return DISCONNECT_REASON_NONE;
    }
    if (m_DisconnectReason == DISCONNECT_REASON_1) {
        return m_DisconnectReason;
    }
    u8 kickoutReason = m_pKickoutManageJob->m_Reason;
    if (kickoutReason == 0) {
        return m_DisconnectReason;
    }
    return kickoutReason == 2 ? DISCONNECT_REASON_KICKOUT_4 : DISCONNECT_REASON_KICKOUT_5;
}

// 0x0073425C (name is ours)
nn::Result nn::pia::session::Mesh::CheckJoined() const
{
    if (m_IsJoined != true) {
        return common::RESULT_NOT_JOINED;
    }
    return nn::Result();
}

// 0x00734274 (name is ours)
bool nn::pia::session::Mesh::IsLeaving() const
{
    bool isLeaving = false;
    if (m_pLeaveMeshJob != nullptr) {
        isLeaving = m_pLeaveMeshJob->IsRunning();
    }
    if (m_pLeaveWithHostMigrationJob != nullptr) {
        isLeaving |= m_pLeaveWithHostMigrationJob->IsRunning();
    }
    if (m_pDestroyMeshJob != nullptr) {
        isLeaving |= m_pDestroyMeshJob->IsRunning();
    }
    return isLeaving;
}

// 0x007342C4 (name is ours)
bool nn::pia::session::Mesh::IsMonitoringDataSenderFlagSet() const
{
    if (!common::IsValidPointer(m_pMonitoringDataSender)) {
        return false;
    }
    return m_pMonitoringDataSender->vf_0x0C();
}

// 0x007342F0 | fefates:callgraph
bool nn::pia::session::Mesh::CheckStationIndexIsValid(nn::pia::StationIndex stationIndex) const
{
    if (stationIndex > STATION_INDEX_MAX) {
        return false;
    }
    return (m_StationBitmap & GetStationBit(stationIndex)) != 0;
}

// 0x00734334 slot 0x00
void nn::pia::session::Mesh::Trace(u64) const
{
    // empty (in the original too)
}

// 0x00734338 | fefates:bytes-fuzzy [tier B]
s64 nn::pia::session::Mesh::GetTime() const
{
    transport::Transport* pTransport = transport::Transport::s_pInstance;
    if (!common::IsValidPointer(pTransport)) {
        return -3;
    }
    SyncClockProtocol* pProtocol = pTransport->m_ProtocolManager.GetProtocol<SyncClockProtocol>(m_SyncClockProtocolId, transport::PROTOCOL_TYPE_SYNC_CLOCK);
    if (!common::IsValidPointer(pProtocol)) {
        return -2;
    }
    return pProtocol->m_SyncClock.GetTime();
}

// 0x00734394 (name is ours)
bool nn::pia::session::Mesh::IsLocalHost()
{
    if (!common::IsValidPointer(s_pInstance) || s_pInstance->m_LocalStationIndex > STATION_INDEX_MAX) {
        return false;
    }
    return s_pInstance->m_LocalStationIndex == s_pInstance->m_HostStationIndex;
}

} // namespace session
} // namespace pia
} // namespace nn
