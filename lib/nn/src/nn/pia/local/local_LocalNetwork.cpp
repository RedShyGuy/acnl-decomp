#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/os/CTR/CTR_Api.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/local/local_Api.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager.h"
#include "nn/pia/local/local_LocalBackgroundProcessJob.h"
#include "nn/pia/local/local_LocalConnectNetworkJob.h"
#include "nn/pia/local/local_LocalCreateNetworkJob.h"
#include "nn/pia/local/local_LocalDestroyNetworkJob.h"
#include "nn/pia/local/local_LocalDisconnectNetworkJob.h"
#include "nn/pia/local/local_LocalForceDisconnectNetworkJob.h"
#include "nn/pia/local/local_LocalMigrationManager.h"
#include "nn/pia/local/local_LocalNetworkManager.h"
#include "nn/pia/local/local_LocalScanNetworkJob.h"
#include "nn/pia/local/local_UdsAroundNetworkSearchManager.h"
#include "nn/pia/local/local_UdsBackgroundProcessJob.h"
#include "nn/pia/local/local_UdsMigrationManagerNew.h"
#include "nn/pia/local/local_UdsNetworkDescription.h"
#include "nn/pia/local/local_UdsNetworkManager.h"
#include "nn/pia/local/local_UdsNetworkSetting.h"
#include "nn/svc/svc_Api.h"
#include "pead/peadHeapMgr.h"
#include "pead/peadTickSpan.h"

namespace nn {
namespace pia {
namespace local {
// 0x00975A74 (name is ours)
bool LocalNetwork::s_IsInitialized;
// 0x00975A78 (name is ours)
LocalNetwork* LocalNetwork::s_pInstance;

namespace {
// the system ticks per second and the time between the sends of an extended application
const s64 TICKS_PER_SECOND = 268111856;
const s64 SEND_INTERVAL_MSEC = 2;

// a job that ran is reset before it starts again
inline void ResetJob(common::Job* pJob)
{
    if (pJob->GetState() != common::Job::EXECUTE_STATE_IDLE) {
        pJob->Reset(true);
    }
}

// a job object on the pia heap and its release (the virtual destructor)
template <typename T>
inline void DeleteJob(T*& pJob)
{
    if (pJob != nullptr) {
        common::DeleteObject(pJob);
        pJob = nullptr;
    }
}
} // namespace

// 0x00414AB4 | fefates:bytes-fuzzy [tier B]
void nn::pia::local::LocalNetwork::CreateJobs()
{
    m_pCreateNetworkJob = common::NewObject<LocalCreateNetworkJob>();
    m_pDestroyNetworkJob = common::NewObject<LocalDestroyNetworkJob>();
    m_pScanNetworkJob = common::NewObject<LocalScanNetworkJob>();
    m_pConnectNetworkJob = common::NewObject<LocalConnectNetworkJob>();
    m_pDisconnectNetworkJob = common::NewObject<LocalDisconnectNetworkJob>();
    m_pForceDisconnectNetworkJob = common::NewObject<LocalForceDisconnectNetworkJob>();
    if (!m_Unknown0x4) {
        m_pBackgroundProcessJob = common::NewObject<UdsBackgroundProcessJob>();
    }
}

// 0x00414B90 | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalNetwork::Initialize(const nn::pia::local::LocalNetworkSetting& setting)
{
    if (s_IsInitialized) {
        return common::RESULT_ALREADY_INITIALIZED;
    }
    m_Unknown0x4 = setting.vf_0x00();
    if (m_Unknown0x4) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    nn::Result result = InitializeCore(static_cast<const UdsNetworkSetting&>(setting));
    if (result.IsFailure()) {
        return result;
    }
    CreateJobs();
    s_IsInitialized = true;
    common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
    content.m_Unknown0x16C = setting.m_ApplicationVersion;
    content.m_Unknown0x16D = m_Unknown0x4;
    if (setting.m_IsAroundNetworkSearchEnabled) {
        content.m_Unknown0x16E &= ~1;
    }
    if (setting.m_IsHostMigrationEnabled) {
        content.m_Unknown0x16E &= ~2;
    }
    return nn::Result();
}

// 0x00414C50 | fefates:bytes-fuzzy [tier B]
void nn::pia::local::LocalNetwork::CleanupJobs()
{
    if (m_pCreateNetworkJob != nullptr) {
        m_pCreateNetworkJob->Cleanup();
        m_pCreateNetworkJob->Reset(false);
    }
    if (m_pDestroyNetworkJob != nullptr) {
        m_pDestroyNetworkJob->Cleanup();
        m_pDestroyNetworkJob->Reset(false);
    }
    if (m_pScanNetworkJob != nullptr) {
        m_pScanNetworkJob->Cleanup();
        m_pScanNetworkJob->Reset(false);
    }
    if (m_pConnectNetworkJob != nullptr) {
        m_pConnectNetworkJob->Cleanup();
        m_pConnectNetworkJob->Reset(false);
    }
    if (m_pDisconnectNetworkJob != nullptr) {
        m_pDisconnectNetworkJob->Cleanup();
        m_pDisconnectNetworkJob->Reset(false);
    }
    if (m_pForceDisconnectNetworkJob != nullptr) {
        m_pForceDisconnectNetworkJob->Cleanup();
        m_pForceDisconnectNetworkJob->Reset(false);
    }
    if (m_pBackgroundProcessJob != nullptr) {
        m_pBackgroundProcessJob->m_IsCancelRequested = true;
        m_pBackgroundProcessJob->WaitForCompletion(2);
        m_pBackgroundProcessJob->Cleanup();
    }
    m_CallContext.Reset();
    m_AsyncType = ASYNC_TYPE_NONE;
}

// 0x00414D70 | fefates:bytes [tier B]
void nn::pia::local::LocalNetwork::DestroyJobs()
{
    CleanupJobs();
    DeleteJob(m_pBackgroundProcessJob);
    DeleteJob(m_pDisconnectNetworkJob);
    DeleteJob(m_pForceDisconnectNetworkJob);
    DeleteJob(m_pConnectNetworkJob);
    DeleteJob(m_pScanNetworkJob);
    DeleteJob(m_pDestroyNetworkJob);
    DeleteJob(m_pCreateNetworkJob);
}

// 0x00414E9C (name is ours)
nn::Result nn::pia::local::LocalNetwork::EjectClient(const nn::pia::common::StationAddress& address)
{
    if (!IsHost()) {
        return common::RESULT_INVALID_STATE;
    }
    return m_pNetworkManager->EjectClient(address);
}

// 0x00414EF0 | fefates:bytes-fuzzy [tier B]
nn::Result nn::pia::local::LocalNetwork::ScanNetwork(nn::pia::common::CallContext* pCallContext, u32 localCommunicationId, u8 subId)
{
    if (!s_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!common::IsValidPointer(pCallContext)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (IsAsyncRunning() || m_pScanNetworkJob->IsRunning() || IsHost() || IsClient()) {
        return common::RESULT_INVALID_STATE;
    }
    ResetJob(m_pScanNetworkJob);
    m_DescriptionNum = 0;
    nn::Result result;
    if (m_Unknown0x4) {
        result = common::RESULT_INVALID_STATE;
    } else {
        LocalScanNetworkSetting setting;
        setting.m_SubId = subId;
        setting.m_LocalCommunicationId = localCommunicationId;
        setting.m_pBuffer = m_pScanBuffer;
        setting.m_BufferSize = m_ScanBufferSize;
        setting.m_pDescriptions = m_pDescriptions;
        setting.m_pDescriptionNum = &m_DescriptionNum;
        result = m_pScanNetworkJob->Startup(pCallContext, &setting);
    }
    if (result.IsSuccess()) {
        m_pScanNetworkJob->Ready(false);
    }
    return result;
}

// 0x0041505C (name is ours)
nn::Result nn::pia::local::LocalNetwork::CreateNetwork(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalCreateNetworkSetting* pSetting)
{
    if (!s_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!common::IsValidPointer(pCallContext) || !common::IsValidPointer(pSetting)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (IsAsyncRunning() || m_pCreateNetworkJob->IsRunning() || IsHost() || IsClient()) {
        return common::RESULT_INVALID_STATE;
    }
    ResetJob(m_pCreateNetworkJob);
    m_pNetworkManager->RenewUnknown0x123C();
    if (!IsDuringHostMigration() && m_pNetworkManager->CreateSessionId().IsFailure()) {
        return common::RESULT_LOCAL_NETWORK_UNAVAILABLE;
    }
    if (!IsDuringHostMigration()) {
        m_ParticipationState = PARTICIPATION_STATE_ALLOWED;
    }
    nn::Result result = m_pCreateNetworkJob->Startup(pCallContext, pSetting);
    if (result.IsSuccess()) {
        m_pNetworkManager->ClearNodeList();
        m_pCreateNetworkJob->Ready(false);
    }
    return result;
}

// 0x004151C8 (name is ours)
nn::Result nn::pia::local::LocalNetwork::ConnectNetwork(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalConnectNetworkSetting* pSetting)
{
    if (!s_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!common::IsValidPointer(pCallContext) || !common::IsValidPointer(pSetting) || !common::IsValidPointer(pSetting->m_pDescription)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (IsAsyncRunning() || m_pConnectNetworkJob->IsRunning() || IsHost() || IsClient()) {
        return common::RESULT_INVALID_STATE;
    }
    ResetJob(m_pConnectNetworkJob);
    nn::Result result = m_pConnectNetworkJob->Startup(pCallContext, pSetting);
    if (result.IsSuccess()) {
        m_pNetworkManager->ClearNodeList();
        m_pConnectNetworkJob->Ready(false);
    }
    return result;
}

// 0x004152EC (name is ours, as LocalFacade's)
nn::Result nn::pia::local::LocalNetwork::CreateInstance()
{
    if (!local::IsInitialized()) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!IsDuringSetup()) {
        return common::RESULT_INVALID_STATE;
    }
    if (s_pInstance != nullptr) {
        return common::RESULT_ALREADY_EXISTS;
    }
    s_pInstance = new LocalNetwork();
    return nn::Result();
}

// 0x00415350 | fefates:bytes-fuzzy [tier B]
nn::Result nn::pia::local::LocalNetwork::DestroyNetwork(nn::pia::common::CallContext* pCallContext)
{
    if (!s_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!common::IsValidPointer(pCallContext)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (IsAsyncRunning() || m_pDestroyNetworkJob->IsRunning()) {
        return common::RESULT_INVALID_STATE;
    }
    ResetJob(m_pDestroyNetworkJob);
    nn::Result result = m_pDestroyNetworkJob->Startup(pCallContext, false);
    if (result.IsSuccess()) {
        m_pDestroyNetworkJob->Ready(false);
    }
    return result;
}

// 0x00415418 | fefates:bytes-fuzzy [tier B]
nn::Result nn::pia::local::LocalNetwork::InitializeCore(const nn::pia::local::UdsNetworkSetting& setting)
{
    if (setting.m_ReceiveBufferSize < RECEIVE_BUFFER_SIZE_MIN || (setting.m_ReceiveBufferSize & (RECEIVE_BUFFER_ALIGNMENT - 1)) != 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (setting.m_pReceiveBuffer != nullptr && (reinterpret_cast<uptr>(setting.m_pReceiveBuffer) & (RECEIVE_BUFFER_ALIGNMENT - 1)) != 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (setting.m_ScanBufferSize < SCAN_BUFFER_SIZE_MIN || (setting.m_ReceiveOption & 1) == 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pSetting == nullptr) {
        m_pSetting = ::new (pead::AllocMemory(sizeof(UdsNetworkSetting), common::HeapManager::GetHeap())) UdsNetworkSetting(setting);
    }
    if (m_pDescriptions == nullptr) {
        m_pDescriptions = common::NewArray<UdsNetworkDescription>(NETWORK_DESCRIPTION_NUM);
    }
    m_ReceiveBufferSize = setting.m_ReceiveBufferSize;
    if (m_pReceiveBuffer == nullptr) {
        if (setting.m_pReceiveBuffer != nullptr) {
            m_pReceiveBuffer = setting.m_pReceiveBuffer;
            m_IsReceiveBufferExternal = true;
        } else {
            m_pReceiveBuffer = common::NewArray<u8>(m_ReceiveBufferSize, RECEIVE_BUFFER_ALIGNMENT);
            m_IsReceiveBufferExternal = false;
        }
    }
    m_ScanBufferSize = setting.m_ScanBufferSize;
    if (m_pScanBuffer == nullptr) {
        m_pScanBuffer = common::NewArray<u8>(m_ScanBufferSize);
    }
    if (m_pNetworkManager == nullptr) {
        m_pNetworkManager = common::NewObject<UdsNetworkManager>();
    }
    nn::Result result = m_pNetworkManager->Initialize(m_pSetting);
    if (result.IsFailure()) {
        return result;
    }
    if (setting.m_IsHostMigrationEnabled) {
        if (m_pMigrationManager == nullptr) {
            m_pMigrationManager = common::NewObject<UdsMigrationManagerNew>();
        }
        result = m_pMigrationManager->Initialize();
        if (result.IsFailure()) {
            return result;
        }
    }
    if (setting.m_IsAroundNetworkSearchEnabled) {
        if (m_pAroundNetworkSearchManager == nullptr) {
            m_pAroundNetworkSearchManager = common::NewObject<UdsAroundNetworkSearchManager>();
        }
        result = m_pAroundNetworkSearchManager->Initialize();
        if (result.IsFailure()) {
            return result;
        }
    }
    common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
    content.m_ReceiveBufferSize = setting.m_ReceiveBufferSize;
    content.m_ScanBufferSize = setting.m_ScanBufferSize;
    content.m_SendOption = setting.m_SendOption;
    content.m_ReceiveOption = setting.m_ReceiveOption;
    return nn::Result();
}

// 0x004157F8 | fefates:bytes [tier B]
void nn::pia::local::LocalNetwork::DestroyInstance()
{
    if (s_pInstance == nullptr) {
        return;
    }
    s_pInstance->Finalize();
    delete s_pInstance;
    s_pInstance = nullptr;
}

// 0x00415838 | fefates:bytes [tier B]
void nn::pia::local::LocalNetwork::SleepBeforeSend()
{
    if (!m_IsExtApplication || m_SendIntervalTick <= 0) {
        return;
    }
    s64 elapsed = nn::svc::GetSystemTick() - m_LastSendTick;
    if (elapsed < m_SendIntervalTick) {
        pead::SleepThread(pead::TickSpan(m_SendIntervalTick - elapsed));
    }
    // (a debug log call was removed by the linker here)
    m_LastSendTick = nn::svc::GetSystemTick();
}

// 0x004158AC | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalNetwork::ScanNetworkAsync(u32 localCommunicationId, u8 subId)
{
    if (IsAsyncRunning()) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_AsyncType != ASYNC_TYPE_NONE) {
        m_CallContext.Reset();
        m_AsyncType = ASYNC_TYPE_NONE;
    }
    nn::Result result = ScanNetwork(&m_CallContext, localCommunicationId, subId);
    if (result.IsSuccess()) {
        m_AsyncType = ASYNC_TYPE_SCAN_NETWORK;
    }
    return result;
}

// 0x00415914 (name is ours)
nn::Result nn::pia::local::LocalNetwork::DisconnectNetwork(nn::pia::common::CallContext* pCallContext)
{
    if (!s_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!common::IsValidPointer(pCallContext)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (IsAsyncRunning()) {
        return common::RESULT_INVALID_STATE;
    }
    nn::Result result;
    if (m_pNetworkManager->IsHost()) {
        // the host leaves with the host migration, else the network ends
        if (!IsEnableHostMigration()) {
            return common::RESULT_INVALID_STATE;
        }
        if (IsDuringHostMigration()) {
            if (!m_IsLeaveRequested) {
                return common::RESULT_INVALID_STATE;
            }
            m_pMigrationManager->CancelHostMigration();
            if (m_pForceDisconnectNetworkJob->IsRunning()) {
                return common::RESULT_INVALID_STATE;
            }
            ResetJob(m_pForceDisconnectNetworkJob);
            result = m_pForceDisconnectNetworkJob->Startup(pCallContext, LocalForceDisconnectNetworkJob::PROC_TYPE_WAIT_HOST_MIGRATION_END);
            if (result.IsSuccess()) {
                m_pForceDisconnectNetworkJob->Ready(false);
            }
            return result;
        }
        if (m_pDestroyNetworkJob->IsRunning()) {
            return common::RESULT_INVALID_STATE;
        }
        ResetJob(m_pDestroyNetworkJob);
        result = m_pDestroyNetworkJob->Startup(pCallContext, true);
        if (result.IsSuccess()) {
            m_pDestroyNetworkJob->Ready(false);
        }
        return result;
    }
    if (IsDuringHostMigration() && m_IsLeaveRequested) {
        m_pMigrationManager->CancelHostMigration();
        if (m_pForceDisconnectNetworkJob->IsRunning()) {
            return common::RESULT_INVALID_STATE;
        }
        ResetJob(m_pForceDisconnectNetworkJob);
        result = m_pForceDisconnectNetworkJob->Startup(pCallContext, LocalForceDisconnectNetworkJob::PROC_TYPE_WAIT_HOST_MIGRATION_END);
        if (result.IsSuccess()) {
            m_pForceDisconnectNetworkJob->Ready(false);
        }
        return result;
    }
    if (m_pDisconnectNetworkJob->IsRunning()) {
        // a second request ends the running disconnection at once
        if (!m_IsLeaveRequested || m_pForceDisconnectNetworkJob->IsRunning()) {
            return common::RESULT_INVALID_STATE;
        }
        ResetJob(m_pForceDisconnectNetworkJob);
        result = m_pForceDisconnectNetworkJob->Startup(pCallContext, LocalForceDisconnectNetworkJob::PROC_TYPE_WAIT_DISCONNECTED);
        if (result.IsSuccess()) {
            m_pForceDisconnectNetworkJob->Ready(false);
        }
        return result;
    }
    ResetJob(m_pDisconnectNetworkJob);
    result = m_pDisconnectNetworkJob->Startup(pCallContext);
    if (result.IsSuccess()) {
        m_pDisconnectNetworkJob->Ready(false);
    }
    return result;
}

// 0x00415BF4 | fefates:bytes-fuzzy [tier B]
nn::Result nn::pia::local::LocalNetwork::AllowParticipating()
{
    if (!IsHost()) {
        return common::RESULT_INVALID_STATE;
    }
    nn::Result result = m_pNetworkManager->AllowParticipating();
    if (result.IsSuccess()) {
        m_ParticipationState = PARTICIPATION_STATE_ALLOWED;
        m_pNetworkManager->SendUpdateSessionMessage();
    }
    return result;
}

// 0x00415C60
nn::Result nn::pia::local::LocalNetwork::CreateNetworkAsync(const nn::pia::local::LocalCreateNetworkSetting* pSetting)
{
    if (IsAsyncRunning()) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_AsyncType != ASYNC_TYPE_NONE) {
        m_CallContext.Reset();
        m_AsyncType = ASYNC_TYPE_NONE;
    }
    nn::Result result = CreateNetwork(&m_CallContext, pSetting);
    if (result.IsSuccess()) {
        m_AsyncType = ASYNC_TYPE_CREATE_NETWORK;
    }
    return result;
}

// 0x00415CC0
nn::Result nn::pia::local::LocalNetwork::ConnectNetworkAsync(const nn::pia::local::LocalConnectNetworkSetting* pSetting)
{
    if (IsAsyncRunning()) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_AsyncType != ASYNC_TYPE_NONE) {
        m_CallContext.Reset();
        m_AsyncType = ASYNC_TYPE_NONE;
    }
    nn::Result result = ConnectNetwork(&m_CallContext, pSetting);
    if (result.IsSuccess()) {
        m_AsyncType = ASYNC_TYPE_CONNECT_NETWORK;
    }
    return result;
}

// 0x00415D20 | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalNetwork::DestroyNetworkAsync()
{
    if (IsAsyncRunning()) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_AsyncType != ASYNC_TYPE_NONE) {
        m_CallContext.Reset();
        m_AsyncType = ASYNC_TYPE_NONE;
    }
    m_IsLeaveRequested = true;
    nn::Result result = DestroyNetwork(&m_CallContext);
    if (result.IsSuccess()) {
        m_AsyncType = ASYNC_TYPE_DESTROY_NETWORK;
    }
    return result;
}

// 0x00415D80 | fefates:bytes-fuzzy [tier B]
nn::Result nn::pia::local::LocalNetwork::DisallowParticipating(bool isSpectatorDisallowed)
{
    if (!IsHost()) {
        return common::RESULT_INVALID_STATE;
    }
    nn::Result result = m_pNetworkManager->DisallowParticipating(isSpectatorDisallowed);
    if (result.IsSuccess()) {
        m_ParticipationState = isSpectatorDisallowed ? PARTICIPATION_STATE_DISALLOWED_WITH_SPECTATORS : PARTICIPATION_STATE_DISALLOWED;
        m_pNetworkManager->SendUpdateSessionMessage();
    }
    return result;
}

// 0x00415DFC
nn::Result nn::pia::local::LocalNetwork::GetScanNetworkAsyncResult() const
{
    return GetAsyncResult(ASYNC_TYPE_SCAN_NETWORK);
}

// 0x00415E04
nn::Result nn::pia::local::LocalNetwork::GetAsyncResult(u8 asyncType) const
{
    if (m_AsyncType != asyncType || !m_CallContext.IsFinished()) {
        return common::RESULT_INVALID_STATE;
    }
    return m_CallContext.m_Result;
}

// 0x00415E38 (name is ours)
nn::Result nn::pia::local::LocalNetwork::GetNetworkDescription(nn::pia::local::LocalNetworkDescription* pDescription, u32 index) const
{
    if (!s_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!m_IsScanned || IsDuringHostMigration() || IsHost() || IsClient()) {
        return common::RESULT_INVALID_STATE;
    }
    if (!common::IsValidPointer(pDescription) || index >= m_DescriptionNum) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_Unknown0x4) {
        return common::RESULT_INVALID_STATE;
    }
    static_cast<UdsNetworkDescription*>(pDescription)->m_Description = m_pDescriptions[index].m_Description;
    return nn::Result();
}

// 0x00415F20 | fefates:bytes [tier B]
nn::pia::local::LocalNetworkDescription* nn::pia::local::LocalNetwork::GetNetworkDescription(u32 index)
{
    if (!s_IsInitialized || index >= m_DescriptionNum || m_Unknown0x4) {
        return nullptr;
    }
    return &m_pDescriptions[index];
}

// 0x00415F64
nn::Result nn::pia::local::LocalNetwork::CancelScanNetworkAsync()
{
    if (m_AsyncType != ASYNC_TYPE_SCAN_NETWORK || m_CallContext.GetState() != common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_CallContext.Cancel();
    return nn::Result();
}

// 0x00415F98
nn::Result nn::pia::local::LocalNetwork::DisconnectNetworkAsync()
{
    if (IsAsyncRunning()) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_AsyncType != ASYNC_TYPE_NONE) {
        m_CallContext.Reset();
        m_AsyncType = ASYNC_TYPE_NONE;
    }
    m_IsLeaveRequested = true;
    nn::Result result = DisconnectNetwork(&m_CallContext);
    if (result.IsSuccess()) {
        m_AsyncType = ASYNC_TYPE_DISCONNECT_NETWORK;
    }
    return result;
}

// 0x00415FF8
nn::Result nn::pia::local::LocalNetwork::GetCreateNetworkAsyncResult() const
{
    return GetAsyncResult(ASYNC_TYPE_CREATE_NETWORK);
}

// 0x00416000
bool nn::pia::local::LocalNetwork::IsScanNetworkAsyncCompleted() const
{
    return m_AsyncType == ASYNC_TYPE_SCAN_NETWORK && m_CallContext.IsFinished();
}

// 0x00416030
nn::Result nn::pia::local::LocalNetwork::GetConnectNetworkAsyncResult() const
{
    return GetAsyncResult(ASYNC_TYPE_CONNECT_NETWORK);
}

// 0x00416038
nn::Result nn::pia::local::LocalNetwork::GetDestroyNetworkAsyncResult() const
{
    return GetAsyncResult(ASYNC_TYPE_DESTROY_NETWORK);
}

// 0x00416040
bool nn::pia::local::LocalNetwork::IsCreateNetworkAsyncCompleted() const
{
    return m_AsyncType == ASYNC_TYPE_CREATE_NETWORK && m_CallContext.IsFinished();
}

// 0x00416070
nn::Result nn::pia::local::LocalNetwork::CancelConnectNetworkAsync()
{
    if (m_AsyncType != ASYNC_TYPE_CONNECT_NETWORK || m_CallContext.GetState() != common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::RESULT_INVALID_STATE;
    }
    m_CallContext.Cancel();
    return nn::Result();
}

// 0x004160A4
bool nn::pia::local::LocalNetwork::IsConnectNetworkAsyncCompleted() const
{
    return m_AsyncType == ASYNC_TYPE_CONNECT_NETWORK && m_CallContext.IsFinished();
}

// 0x004160D4
bool nn::pia::local::LocalNetwork::IsDestroyNetworkAsyncCompleted() const
{
    return m_AsyncType == ASYNC_TYPE_DESTROY_NETWORK && m_CallContext.IsFinished();
}

// 0x00416104
nn::Result nn::pia::local::LocalNetwork::GetDisconnectNetworkAsyncResult() const
{
    return GetAsyncResult(ASYNC_TYPE_DISCONNECT_NETWORK);
}

// 0x0041610C (name is ours)
nn::Result nn::pia::local::LocalNetwork::SetApplicationData(const void* pData, u32 size)
{
    if (!IsHost()) {
        return common::RESULT_INVALID_STATE;
    }
    if (!common::IsValidPointer(pData)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    return m_pNetworkManager->SetApplicationData(pData, size);
}

// 0x00416184 (name is ours)
void nn::pia::local::LocalNetwork::RegisterUpdateEventCallback(nn::pia::local::LocalUpdateEventCallback callback, void* pArg)
{
    m_UpdateEventCallback = callback;
    m_pUpdateEventCallbackArg = pArg;
}

// 0x00416190
bool nn::pia::local::LocalNetwork::IsDisconnectNetworkAsyncCompleted() const
{
    return m_AsyncType == ASYNC_TYPE_DISCONNECT_NETWORK && m_CallContext.IsFinished();
}

// 0x004161C0 | fefates:bytes [tier B]
void nn::pia::local::LocalNetwork::UnregisterUpdateEventCallback()
{
    m_UpdateEventCallback = nullptr;
    m_pUpdateEventCallbackArg = nullptr;
}

// 0x004161D0 | fefates:bytes [tier B]
void nn::pia::local::LocalNetwork::ProcessUpdateEventDisconnected(u8 transportId)
{
    if (m_UpdateEventCallback == nullptr) {
        return;
    }
    m_UpdateEventCallback(LOCAL_UPDATE_EVENT_DISCONNECTED, transportId, m_pUpdateEventCallbackArg);
}

// 0x004161EC | fefates:bytes [tier B]
void nn::pia::local::LocalNetwork::ProcessUpdateEventMigrationStarted()
{
    if (m_UpdateEventCallback == nullptr) {
        return;
    }
    m_UpdateEventCallback(LOCAL_UPDATE_EVENT_MIGRATION_STARTED, m_pNetworkManager->m_LocalTransportId, m_pUpdateEventCallbackArg);
}

// 0x00416210 | fefates:bytes-fuzzy [tier B]
void nn::pia::local::LocalNetwork::Cleanup()
{
    CleanupJobs();
    if (common::IsValidPointer(m_pSetting)) {
        if (m_pSetting->m_IsAroundNetworkSearchEnabled && common::IsValidPointer(m_pAroundNetworkSearchManager)) {
            m_pAroundNetworkSearchManager->Cleanup();
        }
        if (m_pSetting->m_IsHostMigrationEnabled && common::IsValidPointer(m_pMigrationManager)) {
            m_pMigrationManager->Cleanup();
        }
    }
    if (common::IsValidPointer(m_pNetworkManager)) {
        m_pNetworkManager->Cleanup();
    }
    m_DisconnectReason = 0;
    m_ParticipationState = PARTICIPATION_STATE_NONE;
    m_IsScanned = false;
    m_DescriptionNum = 0;
}

// 0x00416298 | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalNetwork::Startup()
{
    m_DisconnectReason = 0;
    m_ParticipationState = PARTICIPATION_STATE_NONE;
    m_IsScanned = false;
    m_IsLeaveRequested = false;
    m_Unknown0x99 = false;
    nn::Result result = m_pNetworkManager->Startup();
    if (result.IsFailure()) {
        return result;
    }
    if (m_pSetting->m_IsHostMigrationEnabled) {
        result = m_pMigrationManager->Startup();
        if (result.IsFailure()) {
            return result;
        }
    }
    if (m_pSetting->m_IsAroundNetworkSearchEnabled) {
        result = m_pAroundNetworkSearchManager->Startup();
        if (result.IsFailure()) {
            return result;
        }
    }
    return nn::Result();
}

// 0x00416314 | fefates:bytes [tier B]
void nn::pia::local::LocalNetwork::Finalize()
{
    Cleanup();
    DestroyJobs();
    if (m_pNetworkManager != nullptr) {
        m_pNetworkManager->Finalize();
        DeleteJob(m_pNetworkManager);
    }
    if (m_pMigrationManager != nullptr) {
        m_pMigrationManager->Finalize();
        DeleteJob(m_pMigrationManager);
    }
    if (m_pAroundNetworkSearchManager != nullptr) {
        m_pAroundNetworkSearchManager->Finalize();
        DeleteJob(m_pAroundNetworkSearchManager);
    }
    if (m_pDescriptions != nullptr) {
        common::DeleteArray(m_pDescriptions);
        m_pDescriptions = nullptr;
    }
    if (m_pSetting != nullptr) {
        pead::FreeMemory(m_pSetting);
        m_pSetting = nullptr;
    }
    if (m_pScanBuffer != nullptr) {
        pead::FreeMemory(m_pScanBuffer);
        m_pScanBuffer = nullptr;
    }
    if (m_pReceiveBuffer != nullptr) {
        if (!m_IsReceiveBufferExternal) {
            pead::FreeMemory(m_pReceiveBuffer);
        }
        m_pReceiveBuffer = nullptr;
    }
    s_IsInitialized = false;
}

// 0x004164D0 | fefates:bytes [tier B]
nn::pia::local::LocalNetwork::LocalNetwork()
    : m_Unknown0x4(false), m_pSetting(nullptr), m_pDescriptions(nullptr), m_DescriptionNum(0), m_pNetworkManager(nullptr), m_pMigrationManager(nullptr),
      m_pAroundNetworkSearchManager(nullptr), m_pCreateNetworkJob(nullptr), m_pDestroyNetworkJob(nullptr), m_pScanNetworkJob(nullptr),
      m_pConnectNetworkJob(nullptr), m_pDisconnectNetworkJob(nullptr), m_pForceDisconnectNetworkJob(nullptr), m_pBackgroundProcessJob(nullptr),
      m_pReceiveBuffer(nullptr), m_ReceiveBufferSize(0), m_IsReceiveBufferExternal(false), m_pScanBuffer(nullptr), m_ScanBufferSize(0),
      m_CriticalSection(-1), m_DisconnectReason(0), m_CallContext(), m_AsyncType(ASYNC_TYPE_NONE), m_UpdateEventCallback(nullptr),
      m_pUpdateEventCallbackArg(nullptr), m_ParticipationState(PARTICIPATION_STATE_NONE), m_IsScanned(false)
{
    m_IsExtApplication = nn::os::CTR::IsRunningAsExtApplication();
    m_LastSendTick = nn::svc::GetSystemTick();
    m_SendIntervalTick = TICKS_PER_SECOND / 1000 * SEND_INTERVAL_MSEC;
    m_IsLeaveRequested = false;
    m_Unknown0x99 = false;
}

// 0x00416600
// 0x004165D8 (deleting dtor)
nn::pia::local::LocalNetwork::~LocalNetwork()
{
    // only the members (in the original too)
}

// 0x0072FBDC (name is ours)
u32 nn::pia::local::LocalNetwork::GetSessionId() const
{
    return m_pNetworkManager->m_SessionId;
}

// 0x0072FBEC
nn::Result nn::pia::local::LocalNetwork::GetStationInfo(nn::pia::local::LocalStationInfo* pInfo, const nn::pia::common::StationAddress& address) const
{
    if (!common::IsValidPointer(pInfo)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    return m_pNetworkManager->GetStationInfo(pInfo, address);
}

// 0x0072FC1C (name is ours)
nn::Result nn::pia::local::LocalNetwork::GetLinkLevel(u8* pLevel, u32 index)
{
    if (!m_IsScanned || IsDuringHostMigration() || IsHost() || IsClient()) {
        return common::RESULT_INVALID_STATE;
    }
    if (!common::IsValidPointer(pLevel)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    common::CriticalSection& criticalSection = s_pInstance->m_CriticalSection;
    criticalSection.Lock();
    nn::Result result = m_pNetworkManager->GetLinkLevel(pLevel, index, m_pScanBuffer);
    criticalSection.Unlock();
    return result;
}

// 0x0072FCF0 (name is ours)
u8 nn::pia::local::LocalNetwork::GetChannel() const
{
    return m_pNetworkManager->GetChannel();
}

// 0x0072FD00 | fefates:bytes-fuzzy [tier B]
nn::Result nn::pia::local::LocalNetwork::GetApplicationData(void* pBuffer, u32* pSize, u32 bufferSize, const nn::pia::local::LocalNetworkDescription* pDescription) const
{
    if (!s_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!common::IsValidPointer(pBuffer) || !common::IsValidPointer(pSize) || !common::IsValidPointer(pDescription)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    return m_pNetworkManager->GetApplicationData(pBuffer, pSize, bufferSize, pDescription);
}

// 0x0072FD70 (name is ours)
nn::Result nn::pia::local::LocalNetwork::GetStationInfoList(nn::pia::local::LocalStationInfo* pInfos, u8 infoNum, u32 index)
{
    if (!m_IsScanned || IsDuringHostMigration() || IsHost() || IsClient()) {
        return common::RESULT_INVALID_STATE;
    }
    if (!common::IsValidPointer(pInfos)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    common::CriticalSection& criticalSection = s_pInstance->m_CriticalSection;
    criticalSection.Lock();
    nn::Result result = m_pNetworkManager->GetStationInfoList(pInfos, infoNum, index, m_pScanBuffer);
    criticalSection.Unlock();
    return result;
}

// 0x0072FE50 | fefates:bytes [tier B]
bool nn::pia::local::LocalNetwork::IsDuringHostMigration() const
{
    if (!s_IsInitialized || !m_pSetting->m_IsHostMigrationEnabled) {
        return false;
    }
    return m_pMigrationManager->m_State != LocalMigrationManager::MIGRATION_STATE_NONE;
}

// 0x0072FE90 | fefates:bytes [tier B]
bool nn::pia::local::LocalNetwork::IsEnableHostMigration() const
{
    if (!s_IsInitialized) {
        return false;
    }
    return m_pSetting->m_IsHostMigrationEnabled;
}

// 0x0072FEB0 (name is ours)
nn::Result nn::pia::local::LocalNetwork::GetApplicationDataSize(u32* pSize, const nn::pia::local::LocalNetworkDescription* pDescription) const
{
    if (!s_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!common::IsValidPointer(pSize) || !common::IsValidPointer(pDescription)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    return m_pNetworkManager->GetApplicationDataSize(pSize, pDescription);
}

// 0x0072FF14
bool nn::pia::local::LocalNetwork::IsAsyncRunning() const
{
    return m_CallContext.GetState() == common::CallContext::STATE_CALL_IN_PROGRESS;
}

// 0x0072FF24 (name is ours)
u32 nn::pia::local::LocalNetwork::CreateLocalCommunicationId(u32 uniqueId, bool isDemo) const
{
    return m_pNetworkManager->CreateLocalCommunicationId(uniqueId, isDemo);
}

// 0x0072FF34 (name is ours)
u8 nn::pia::local::LocalNetwork::GetParticipationState() const
{
    if (IsDuringHostMigration() || IsHost() || IsClient()) {
        return m_ParticipationState;
    }
    return PARTICIPATION_STATE_NONE;
}

// 0x0072FF98 | fefates:bytes [tier B]
bool nn::pia::local::LocalNetwork::IsEnableAroundNetworkSearch() const
{
    if (!s_IsInitialized) {
        return false;
    }
    return m_pSetting->m_IsAroundNetworkSearchEnabled;
}

// 0x0072FFB8 (name is ours)
u32 nn::pia::local::LocalNetwork::GetBeaconApplicationDataSizeMax() const
{
    return m_pNetworkManager->GetBeaconApplicationDataSizeMax();
}

// 0x0072FFC8 (name is ours)
u32 nn::pia::local::LocalNetwork::GetSessionId(const nn::pia::local::LocalNetworkDescription* pDescription)
{
    common::CriticalSection& criticalSection = s_pInstance->m_pNetworkManager->m_SystemDataCriticalSection;
    criticalSection.Lock();
    const LocalBeaconSystemData* pSystemData = s_pInstance->m_pNetworkManager->GetSystemData(pDescription);
    if (pSystemData == nullptr) {
        criticalSection.Unlock();
        return 0;
    }
    u32 sessionId = pSystemData->m_SessionId;
    criticalSection.Unlock();
    return sessionId;
}

// 0x00730034
void nn::pia::local::LocalNetwork::vf_0x08()
{
    // empty (in the original too)
}

// 0x00730038 | fefates:bytes [tier B]
bool nn::pia::local::LocalNetwork::IsHost() const
{
    if (!s_IsInitialized) {
        return false;
    }
    return m_pNetworkManager->IsHost();
}

// 0x0073005C | fefates:bytes [tier B]
bool nn::pia::local::LocalNetwork::IsClient() const
{
    if (!s_IsInitialized) {
        return false;
    }
    return m_pNetworkManager->IsClient();
}

// 0x00730F68 (name is ours)
u8 nn::pia::local::LocalNetwork::GetConnectedNodeNum() const
{
    return m_pNetworkManager->GetConnectedNodeNum();
}

} // namespace local
} // namespace pia
} // namespace nn
