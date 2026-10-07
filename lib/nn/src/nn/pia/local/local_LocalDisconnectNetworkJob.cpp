#include "nn/pia/local/local_LocalDisconnectNetworkJob.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalBackgroundProcessJob.h"
#include "nn/pia/local/local_LocalMigrationManager.h"
#include "nn/pia/local/local_LocalNetwork.h"

namespace nn {
namespace pia {
namespace local {
inline void nn::pia::local::LocalDisconnectNetworkJob::StartCancel()
{
    if (m_pBackgroundCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_pBackgroundCallContext->Cancel();
    }
    SetStep(&LocalDisconnectNetworkJob::WaitForCancel, "LocalDisconnectNetworkJob::WaitForCancel");
}

// 0x00420304
nn::pia::common::ExecuteResult nn::pia::local::LocalDisconnectNetworkJob::WaitForCancel()
{
    if (m_pBackgroundCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    m_pCallContext->SignalCancel();
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0042034C
nn::pia::common::ExecuteResult nn::pia::local::LocalDisconnectNetworkJob::WaitDisconnectNetwork()
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
        // the nodes stay known while the host migration goes on
        if (LocalNetwork::s_pInstance->IsEnableHostMigration() &&
            !(LocalNetwork::s_pInstance->IsDuringHostMigration() &&
              LocalNetwork::s_pInstance->m_pMigrationManager->m_State == LocalMigrationManager::MIGRATION_STATE_NEW_HOST)) {
            LocalNetwork::s_pInstance->m_pMigrationManager->ClearNodeInfo();
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    case common::CallContext::STATE_CALL_FAILURE:
    case common::CallContext::STATE_CALL_CANCEL:
        m_pCallContext->SignalFailure(m_pBackgroundCallContext->m_Result);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    default:
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
}

// 0x004204A0
nn::pia::common::ExecuteResult nn::pia::local::LocalDisconnectNetworkJob::TryPrepareDisconnectNetwork()
{
    if (m_pCallContext->IsCancelRequested()) {
        StartCancel();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (LocalNetwork::s_pInstance->m_pBackgroundProcessJob->PrepareDisconnectNetwork().IsFailure()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (LocalNetwork::s_pInstance->m_pBackgroundProcessJob->StartupDisconnectNetwork(m_pBackgroundCallContext).IsFailure()) {
        m_pCallContext->SignalFailure(m_pBackgroundCallContext->m_Result);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    LocalNetwork::s_pInstance->m_pBackgroundProcessJob->Ready(true);
    SetStep(&LocalDisconnectNetworkJob::WaitDisconnectNetwork, "LocalDisconnectNetworkJob::WaitDisconnectNetwork");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0042060C (name is ours)
void nn::pia::local::LocalDisconnectNetworkJob::Cleanup()
{
    m_pBackgroundCallContext->Cancel();
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalFailure(common::RESULT_CANCELED);
        }
        m_pCallContext = nullptr;
    }
}

// 0x00420648
nn::Result nn::pia::local::LocalDisconnectNetworkJob::Startup(nn::pia::common::CallContext* pCallContext)
{
    m_pCallContext = pCallContext;
    pCallContext->Reset();
    m_pCallContext->InitiateCall();
    SetStep(&LocalDisconnectNetworkJob::TryPrepareDisconnectNetwork, "LocalDisconnectNetworkJob::TryPrepareDisconnectNetwork");
    return nn::Result();
}

// 0x004206C0
nn::pia::local::LocalDisconnectNetworkJob::LocalDisconnectNetworkJob() : m_pCallContext(nullptr)
{
    m_pBackgroundCallContext = common::NewObject<common::CallContext>();
}

// 0x00420748
// 0x00420704 (deleting dtor)
nn::pia::local::LocalDisconnectNetworkJob::~LocalDisconnectNetworkJob()
{
    if (m_pBackgroundCallContext != nullptr) {
        common::DeleteObject(m_pBackgroundCallContext);
    }
}

// 0x007316A4
void nn::pia::local::LocalDisconnectNetworkJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
