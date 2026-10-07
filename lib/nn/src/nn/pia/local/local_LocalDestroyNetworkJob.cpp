#include "nn/pia/local/local_LocalDestroyNetworkJob.h"
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
} // namespace

inline void nn::pia::local::LocalDestroyNetworkJob::StartCancel()
{
    if (m_pBackgroundCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_pBackgroundCallContext->Cancel();
    }
    if (m_IsHostMigration) {
        LocalNetwork::s_pInstance->m_pMigrationManager->m_State = LocalMigrationManager::MIGRATION_STATE_NONE;
    }
    SetStep(&LocalDestroyNetworkJob::WaitForCancel, "LocalDestroyNetworkJob::WaitForCancel");
}

// 0x0041DA0C
nn::pia::common::ExecuteResult nn::pia::local::LocalDestroyNetworkJob::WaitForCancel()
{
    if (m_pBackgroundCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    m_pCallContext->SignalCancel();
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0041DA54
nn::pia::common::ExecuteResult nn::pia::local::LocalDestroyNetworkJob::WaitDestroyNetwork()
{
    if (m_pCallContext->IsCancelRequested()) {
        StartCancel();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    switch (m_pBackgroundCallContext->GetState()) {
    case common::CallContext::STATE_CALL_IN_PROGRESS:
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    case common::CallContext::STATE_CALL_SUCCESS:
        m_pCallContext->SignalSuccess(m_pBackgroundCallContext->m_Result);
        m_pCallContext = nullptr;
        if (LocalNetwork::s_pInstance->IsEnableHostMigration()) {
            LocalNetwork::s_pInstance->m_pMigrationManager->ClearNodeInfo();
        }
        break;
    case common::CallContext::STATE_CALL_FAILURE:
        m_pCallContext->SignalFailure(m_pBackgroundCallContext->m_Result);
        m_pCallContext = nullptr;
        break;
    case common::CallContext::STATE_CALL_CANCEL:
        m_pCallContext->SignalFailure(m_pBackgroundCallContext->m_Result);
        m_pCallContext = nullptr;
        break;
    default:
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_IsHostMigration) {
        LocalNetwork::s_pInstance->m_pMigrationManager->m_State = LocalMigrationManager::MIGRATION_STATE_NONE;
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0041DBE0
nn::pia::common::ExecuteResult nn::pia::local::LocalDestroyNetworkJob::TryPrepareDestroyNetwork()
{
    if (m_pCallContext->IsCancelRequested()) {
        StartCancel();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (LocalNetwork::s_pInstance->m_pBackgroundProcessJob->PrepareDestroyNetwork().IsFailure()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_IsHostMigration) {
        // the clients get the last node table first
        LocalNetwork::s_pInstance->m_pNetworkManager->SendUpdateSessionMessage();
        m_UpdateSessionTime.SetNow();
        SetStep(&LocalDestroyNetworkJob::WaitUntilAllClientsReceiveUpdateSessionMessage, "LocalDestroyNetworkJob::WaitUntilAllClientsReceiveUpdateSessionMessage");
    } else {
        m_StartTime.SetNow();
        SetStep(&LocalDestroyNetworkJob::SendDestroyNetworkMessage, "LocalDestroyNetworkJob::SendDestroyNetworkMessage");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0041DD58
nn::pia::common::ExecuteResult nn::pia::local::LocalDestroyNetworkJob::SendDestroyNetworkMessage()
{
    if (m_pCallContext->IsCancelRequested()) {
        StartCancel();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (m_IsHostMigration) {
        LocalNetwork::s_pInstance->m_pMigrationManager->m_IsHostLeaving = false;
        LocalNetwork::s_pInstance->m_pNetworkManager->SendStartHostMigrationMessage();
    } else {
        LocalNetwork::s_pInstance->m_pNetworkManager->SendDestroyNetworkMessage();
    }
    m_SendTime.SetNow();
    SetStep(&LocalDestroyNetworkJob::WaitUntilAllClientsDisconnection, "LocalDestroyNetworkJob::WaitUntilAllClientsDisconnection");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0041DEA8
nn::pia::common::ExecuteResult nn::pia::local::LocalDestroyNetworkJob::WaitUntilAllClientsDisconnection()
{
    if (m_pCallContext->IsCancelRequested()) {
        StartCancel();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    // the host is alone or the clients did not leave in time
    if (LocalNetwork::s_pInstance->m_pNetworkManager->GetNodeNum() == 1 || GetElapsedMSec(m_StartTime) > TIMEOUT_MSEC) {
        if (LocalNetwork::s_pInstance->m_pBackgroundProcessJob->StartupDestroyNetwork(m_pBackgroundCallContext).IsFailure()) {
            m_pCallContext->SignalFailure(m_pBackgroundCallContext->m_Result);
            m_pCallContext = nullptr;
            if (m_IsHostMigration) {
                LocalNetwork::s_pInstance->m_pMigrationManager->m_State = LocalMigrationManager::MIGRATION_STATE_NONE;
            }
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
        LocalNetwork::s_pInstance->m_pBackgroundProcessJob->Ready(true);
        SetStep(&LocalDestroyNetworkJob::WaitDestroyNetwork, "LocalDestroyNetworkJob::WaitDestroyNetwork");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (GetElapsedMSec(m_SendTime) > RESEND_INTERVAL_MSEC) {
        SetStep(&LocalDestroyNetworkJob::SendDestroyNetworkMessage, "LocalDestroyNetworkJob::SendDestroyNetworkMessage");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0041E14C
nn::pia::common::ExecuteResult nn::pia::local::LocalDestroyNetworkJob::WaitUntilAllClientsReceiveUpdateSessionMessage()
{
    if (m_pCallContext->IsCancelRequested()) {
        StartCancel();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (LocalNetwork::s_pInstance->m_pNetworkManager->IsWaitingUpdateSessionAckMessage() && GetElapsedMSec(m_UpdateSessionTime) <= TIMEOUT_MSEC) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    m_StartTime.SetNow();
    SetStep(&LocalDestroyNetworkJob::SendDestroyNetworkMessage, "LocalDestroyNetworkJob::SendDestroyNetworkMessage");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0041E2DC (name is ours)
void nn::pia::local::LocalDestroyNetworkJob::Cleanup()
{
    m_pBackgroundCallContext->Cancel();
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalFailure(common::RESULT_CANCELED);
        }
        m_pCallContext = nullptr;
    }
    if (m_IsHostMigration) {
        LocalNetwork::s_pInstance->m_pMigrationManager->m_State = LocalMigrationManager::MIGRATION_STATE_NONE;
        LocalNetwork::s_pInstance->m_pMigrationManager->m_IsHostLeaving = false;
    }
}

// 0x0041E344 (name is ours)
nn::Result nn::pia::local::LocalDestroyNetworkJob::Startup(nn::pia::common::CallContext* pCallContext, bool isHostMigration)
{
    if (!LocalNetwork::s_pInstance->IsHost()) {
        return common::RESULT_INVALID_STATE;
    }
    m_pCallContext = pCallContext;
    pCallContext->Reset();
    m_pCallContext->InitiateCall();
    m_IsHostMigration = isHostMigration;
    if (isHostMigration) {
        LocalNetwork::s_pInstance->m_pMigrationManager->m_State = LocalMigrationManager::MIGRATION_STATE_HOST_LEAVING;
        LocalNetwork::s_pInstance->m_pMigrationManager->m_IsHostLeaving = true;
    }
    SetStep(&LocalDestroyNetworkJob::TryPrepareDestroyNetwork, "LocalDestroyNetworkJob::TryPrepareDestroyNetwork");
    return nn::Result();
}

// 0x0041E408
nn::pia::local::LocalDestroyNetworkJob::LocalDestroyNetworkJob() : m_pCallContext(nullptr), m_SendTime(), m_StartTime(), m_UpdateSessionTime(), m_IsHostMigration(false)
{
    m_pBackgroundCallContext = common::NewObject<common::CallContext>();
}

// 0x0041E4B8
// 0x0041E474 (deleting dtor)
nn::pia::local::LocalDestroyNetworkJob::~LocalDestroyNetworkJob()
{
    if (m_pBackgroundCallContext != nullptr) {
        common::DeleteObject(m_pBackgroundCallContext);
    }
}

// 0x007315C4
void nn::pia::local::LocalDestroyNetworkJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
