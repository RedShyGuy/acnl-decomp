#include "nn/pia/local/local_LocalHostMigrationJob.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/local/local_LocalBackgroundProcessJob.h"
#include "nn/pia/local/local_LocalMigrationManager.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"

namespace nn {
namespace pia {
namespace local {
namespace {
// the milliseconds since the time
inline s32 GetElapsedMSec(const common::Time& time)
{
    common::Time now;
    now.SetNow();
    return static_cast<s32>((now - time).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick());
}

inline LocalMigrationManager* GetMigrationManager()
{
    return LocalNetwork::s_pInstance->m_pMigrationManager;
}
} // namespace

inline void nn::pia::local::LocalHostMigrationJob::StartCancel()
{
    if (m_pNetworkCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_pNetworkCallContext->Cancel();
    }
    SetStep(&LocalHostMigrationJob::WaitForCancel, "LocalHostMigrationJob::WaitForCancel");
}

// 0x0041B000 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalHostMigrationJob::ScanNetwork()
{
    if (m_pCallContext->IsCancelRequested()) {
        StartCancel();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (GetElapsedMSec(m_ScanStartTime) > SCAN_NETWORK_TIMEOUT_MSEC) {
        // the new host was not found
        if (m_pNetworkCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pNetworkCallContext->Cancel();
            LocalNetwork::s_pInstance->m_pBackgroundProcessJob->m_IsCancelRequested = true;
        }
        HostMigrationFailureProcess();
        m_pCallContext->SignalFailure(common::RESULT_NOT_FOUND);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    LocalMigrationManager* pManager = GetMigrationManager();
    u8 subId = pManager->GetSubId();
    u32 localCommunicationId = pManager->GetLocalCommunicationId();
    if (LocalNetwork::s_pInstance->ScanNetwork(m_pNetworkCallContext, localCommunicationId, subId).IsFailure()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&LocalHostMigrationJob::WaitScanNetwork, "LocalHostMigrationJob::WaitScanNetwork");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0041B1EC | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalHostMigrationJob::CreateNetwork()
{
    if (m_pCallContext->IsCancelRequested()) {
        StartCancel();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    const LocalCreateNetworkSetting* pSetting = GetMigrationManager()->GetCreateNetworkSetting();
    if (LocalNetwork::s_pInstance->CreateNetwork(m_pNetworkCallContext, pSetting).IsFailure()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&LocalHostMigrationJob::WaitCreateNetwork, "LocalHostMigrationJob::WaitCreateNetwork");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0041B310 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalHostMigrationJob::WaitForCancel()
{
    if (m_pNetworkCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    HostMigrationFailureProcess();
    m_pCallContext->SignalCancel();
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0041B360 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalHostMigrationJob::ConnectNetwork()
{
    if (m_pCallContext->IsCancelRequested()) {
        StartCancel();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    LocalNetworkDescription* pDescription = LocalNetwork::s_pInstance->GetNetworkDescription(m_NetworkIndex);
    const LocalConnectNetworkSetting* pSetting = GetMigrationManager()->GetConnectNetworkSetting(pDescription);
    if (LocalNetwork::s_pInstance->ConnectNetwork(m_pNetworkCallContext, pSetting).IsFailure()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&LocalHostMigrationJob::WaitConnectNetwork, "LocalHostMigrationJob::WaitConnectNetwork");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0041B498
nn::pia::common::ExecuteResult nn::pia::local::LocalHostMigrationJob::WaitScanNetwork()
{
    if (m_pCallContext->IsCancelRequested()) {
        StartCancel();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    switch (m_pNetworkCallContext->GetState()) {
    case common::CallContext::STATE_CALL_IN_PROGRESS:
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    case common::CallContext::STATE_CALL_SUCCESS:
        LocalNetwork::s_pInstance->m_IsScanned = false;
        SetStep(&LocalHostMigrationJob::SearchNewHostNetwork, "LocalHostMigrationJob::SearchNewHostNetwork");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    case common::CallContext::STATE_CALL_FAILURE:
    case common::CallContext::STATE_CALL_CANCEL:
        m_pCallContext->SignalFailure(m_pNetworkCallContext->m_Result);
        m_pCallContext = nullptr;
        HostMigrationFailureProcess();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    default:
        m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        m_pCallContext = nullptr;
        HostMigrationFailureProcess();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
}

// 0x0041B608
nn::pia::common::ExecuteResult nn::pia::local::LocalHostMigrationJob::DisconnectNetwork()
{
    if (m_pCallContext->IsCancelRequested()) {
        StartCancel();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (LocalNetwork::s_pInstance->m_IsLeaveRequested) {
        SetStep(&LocalHostMigrationJob::WaitForCancel, "LocalHostMigrationJob::WaitForCancel");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (LocalNetwork::s_pInstance->IsAsyncRunning() || LocalNetwork::s_pInstance->DisconnectNetwork(m_pNetworkCallContext).IsFailure()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&LocalHostMigrationJob::WaitDisconnectNetwork, "LocalHostMigrationJob::WaitDisconnectNetwork");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0041B760 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalHostMigrationJob::WaitAllClientsAck()
{
    if (m_pCallContext->IsCancelRequested()) {
        StartCancel();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (LocalNetwork::s_pInstance->m_pNetworkManager->IsWaitingUpdateSessionAckMessage()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    GetMigrationManager()->m_State = LocalMigrationManager::MIGRATION_STATE_NONE;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0041B83C | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalHostMigrationJob::WaitCreateNetwork()
{
    if (m_pCallContext->IsCancelRequested()) {
        StartCancel();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    switch (m_pNetworkCallContext->GetState()) {
    case common::CallContext::STATE_CALL_IN_PROGRESS:
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    case common::CallContext::STATE_CALL_SUCCESS:
        SetStep(&LocalHostMigrationJob::WaitUntilAllClientsConnection, "LocalHostMigrationJob::WaitUntilAllClientsConnection");
        m_ConnectionStartTime.SetNow();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    case common::CallContext::STATE_CALL_FAILURE:
    case common::CallContext::STATE_CALL_CANCEL:
        m_pCallContext->SignalFailure(m_pNetworkCallContext->m_Result);
        m_pCallContext = nullptr;
        HostMigrationFailureProcess();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    default:
        m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        m_pCallContext = nullptr;
        HostMigrationFailureProcess();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
}

// 0x0041B9B0 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalHostMigrationJob::WaitConnectNetwork()
{
    if (m_pCallContext->IsCancelRequested()) {
        StartCancel();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    switch (m_pNetworkCallContext->GetState()) {
    case common::CallContext::STATE_CALL_IN_PROGRESS:
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    case common::CallContext::STATE_CALL_SUCCESS:
        m_pCallContext->SignalSuccess(nn::Result());
        m_pCallContext = nullptr;
        GetMigrationManager()->m_State = LocalMigrationManager::MIGRATION_STATE_NONE;
        GetMigrationManager()->m_MigrationResult = LocalMigrationManager::MIGRATION_RESULT_CONNECTED;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    case common::CallContext::STATE_CALL_FAILURE:
    case common::CallContext::STATE_CALL_CANCEL:
        m_pCallContext->SignalFailure(m_pNetworkCallContext->m_Result);
        m_pCallContext = nullptr;
        HostMigrationFailureProcess();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    default:
        m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        m_pCallContext = nullptr;
        HostMigrationFailureProcess();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
}

// 0x0041BAF8 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalHostMigrationJob::SearchNewHostNetwork()
{
    if (m_pCallContext->IsCancelRequested()) {
        StartCancel();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    LocalMigrationManager* pManager = GetMigrationManager();
    bool isFound = false;
    for (u32 i = 0; i < LocalNetwork::s_pInstance->m_DescriptionNum; i++) {
        LocalNetworkDescription* pDescription = LocalNetwork::s_pInstance->GetNetworkDescription(i);
        if (pDescription == nullptr) {
            break;
        }
        if (pManager->IsNextNetwork(pDescription)) {
            m_NetworkIndex = i;
            isFound = true;
        }
    }
    if (isFound) {
        SetStep(&LocalHostMigrationJob::ConnectNetwork, "LocalHostMigrationJob::ConnectNetwork");
    } else {
        SetStep(&LocalHostMigrationJob::ScanNetwork, "LocalHostMigrationJob::ScanNetwork");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0041BCA0 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalHostMigrationJob::WaitDisconnectNetwork()
{
    if (m_pCallContext->IsCancelRequested()) {
        StartCancel();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    switch (m_pNetworkCallContext->GetState()) {
    case common::CallContext::STATE_CALL_IN_PROGRESS:
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    case common::CallContext::STATE_CALL_SUCCESS:
        if (m_IsNextHost) {
            SetStep(&LocalHostMigrationJob::CreateNetwork, "LocalHostMigrationJob::CreateNetwork");
        } else {
            m_ScanStartTime.SetNow();
            SetStep(&LocalHostMigrationJob::ScanNetwork, "LocalHostMigrationJob::ScanNetwork");
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    case common::CallContext::STATE_CALL_FAILURE:
    case common::CallContext::STATE_CALL_CANCEL:
        m_pCallContext->SignalFailure(m_pNetworkCallContext->m_Result);
        m_pCallContext = nullptr;
        HostMigrationFailureProcess();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    default:
        m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
        m_pCallContext = nullptr;
        HostMigrationFailureProcess();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
}

// 0x0041BE44 | fefates:bytes [tier B]
void nn::pia::local::LocalHostMigrationJob::HostMigrationFailureProcess()
{
    GetMigrationManager()->m_State = LocalMigrationManager::MIGRATION_STATE_NONE;
    GetMigrationManager()->m_MigrationResult = LocalMigrationManager::MIGRATION_RESULT_FAILED;
    for (u32 i = 0; i < LocalMigrationManager::NODE_NUM_MAX; i++) {
        if ((LocalNetwork::s_pInstance->m_pNetworkManager->GetConnectedTransportIdBitmap(false) & (1 << i)) == 0) {
            GetMigrationManager()->ClearNodeInfo(static_cast<u8>(i + 1));
        }
    }
}

// 0x0041BEB8
nn::pia::common::ExecuteResult nn::pia::local::LocalHostMigrationJob::WaitUntilAllClientsConnection()
{
    if (m_pCallContext->IsCancelRequested()) {
        StartCancel();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (GetMigrationManager()->IsExistStateMigrating()) {
        if (GetElapsedMSec(m_ConnectionStartTime) <= CONNECTION_TIMEOUT_MSEC) {
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        // the stations that did not come are gone
        for (u32 i = 0; i < LocalMigrationManager::NODE_NUM_MAX; i++) {
            u8 transportId = static_cast<u8>(i + 1);
            if (GetMigrationManager()->GetMigrationState(transportId) == LocalMigrationManager::NodeInfo::STATE_MIGRATING) {
                LocalNetwork::s_pInstance->ProcessUpdateEventDisconnected(transportId);
                LocalNetwork::s_pInstance->m_pNetworkManager->ClearNodeList(GetMigrationManager()->ConvertTransportIdToLocalNodeId(transportId));
                GetMigrationManager()->ClearTransportIdToNodeIdTable(transportId);
                GetMigrationManager()->ClearNodeInfo(transportId);
            }
        }
        GetMigrationManager()->m_MigrationResult = LocalMigrationManager::MIGRATION_RESULT_SOME_CONNECTED;
    } else {
        GetMigrationManager()->m_MigrationResult = LocalMigrationManager::MIGRATION_RESULT_ALL_CONNECTED;
    }
    // the new host takes over the participation state
    nn::Result result = common::RESULT_NOT_SET;
    switch (LocalNetwork::s_pInstance->GetParticipationState()) {
    case LocalNetwork::PARTICIPATION_STATE_ALLOWED:
        result = LocalNetwork::s_pInstance->AllowParticipating();
        break;
    case LocalNetwork::PARTICIPATION_STATE_DISALLOWED_WITH_SPECTATORS:
        result = LocalNetwork::s_pInstance->DisallowParticipating(true);
        break;
    case LocalNetwork::PARTICIPATION_STATE_DISALLOWED:
        result = LocalNetwork::s_pInstance->DisallowParticipating(false);
        break;
    }
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        HostMigrationFailureProcess();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&LocalHostMigrationJob::WaitAllClientsAck, "LocalHostMigrationJob::WaitAllClientsAck");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0041C18C | fefates:bytes [tier B]
void nn::pia::local::LocalHostMigrationJob::Cleanup()
{
    m_pNetworkCallContext->Cancel();
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalFailure(common::RESULT_CANCELED);
        }
        m_pCallContext = nullptr;
    }
    GetMigrationManager()->m_State = LocalMigrationManager::MIGRATION_STATE_NONE;
    m_IsNextHost = false;
}

// 0x0041C1E0 | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalHostMigrationJob::Startup(nn::pia::common::CallContext* pCallContext, bool isNextHost)
{
    m_pCallContext = pCallContext;
    pCallContext->Reset();
    m_pCallContext->InitiateCall();
    m_IsNextHost = isNextHost;
    if (isNextHost) {
        GetMigrationManager()->m_State = LocalMigrationManager::MIGRATION_STATE_NEW_HOST;
    } else {
        GetMigrationManager()->m_State = LocalMigrationManager::MIGRATION_STATE_CLIENT;
    }
    SetStep(&LocalHostMigrationJob::DisconnectNetwork, "LocalHostMigrationJob::DisconnectNetwork");
    return nn::Result();
}

// 0x0041C284 | fefates:bytes [tier B]
nn::pia::local::LocalHostMigrationJob::LocalHostMigrationJob()
    : m_pCallContext(nullptr), m_IsNextHost(false), m_ConnectionStartTime(), m_ScanStartTime()
{
    m_pNetworkCallContext = common::NewObject<common::CallContext>();
}

// 0x0041C324
// 0x0041C2E0 (deleting dtor)
nn::pia::local::LocalHostMigrationJob::~LocalHostMigrationJob()
{
    if (m_pNetworkCallContext != nullptr) {
        common::DeleteObject(m_pNetworkCallContext);
    }
}

// 0x007311E0
void nn::pia::local::LocalHostMigrationJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
