#include "nn/pia/local/local_LocalConnectNetworkJob.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalBackgroundProcessJob.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"

namespace nn {
namespace pia {
namespace local {
// 0x0041D5D4
nn::pia::common::ExecuteResult nn::pia::local::LocalConnectNetworkJob::WaitForCancel()
{
    if (!m_pBackgroundCallContext->IsFinished()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    m_pCallContext->SignalCancel();
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0041D624 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalConnectNetworkJob::ProcessSucceeded()
{
    if (m_pCallContext->IsCancelRequested()) {
        if (m_pBackgroundCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pBackgroundCallContext->Cancel();
        }
        SetStep(&LocalConnectNetworkJob::WaitForCancel, "LocalConnectNetworkJob::WaitForCancel");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0041D6C4
nn::pia::common::ExecuteResult nn::pia::local::LocalConnectNetworkJob::WaitConnectNetwork()
{
    if (m_pCallContext->IsCancelRequested()) {
        if (m_pBackgroundCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pBackgroundCallContext->Cancel();
        }
        SetStep(&LocalConnectNetworkJob::WaitForCancel, "LocalConnectNetworkJob::WaitForCancel");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    switch (m_pBackgroundCallContext->GetState()) {
    case common::CallContext::STATE_CALL_IN_PROGRESS:
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    case common::CallContext::STATE_CALL_SUCCESS:
        // the background job still ends the connection
        if (LocalNetwork::s_pInstance->m_pBackgroundProcessJob->IsRunning()) {
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        SetStep(&LocalConnectNetworkJob::ProcessSucceeded, "LocalConnectNetworkJob::ProcessSucceeded");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    case common::CallContext::STATE_CALL_FAILURE:
        m_pCallContext->SignalFailure(m_pBackgroundCallContext->m_Result);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    case common::CallContext::STATE_CALL_CANCEL:
        m_pCallContext->SignalFailure(m_pBackgroundCallContext->m_Result);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    default:
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
}

// 0x0041D844 (name is ours)
void nn::pia::local::LocalConnectNetworkJob::Cleanup()
{
    m_pBackgroundCallContext->Cancel();
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalFailure(common::RESULT_CANCELED);
        }
        m_pCallContext = nullptr;
    }
}

// 0x0041D880 (name is ours)
nn::Result nn::pia::local::LocalConnectNetworkJob::Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalConnectNetworkSetting* pSetting)
{
    nn::Result result = LocalNetwork::s_pInstance->m_pBackgroundProcessJob->StartupConnectNetwork(m_pBackgroundCallContext, pSetting);
    if (result.IsFailure()) {
        return result;
    }
    m_pCallContext = pCallContext;
    pCallContext->Reset();
    m_pCallContext->InitiateCall();
    LocalNetwork::s_pInstance->m_pBackgroundProcessJob->Ready(true);
    LocalNetwork::s_pInstance->m_IsScanned = false;
    LocalNetwork::s_pInstance->m_pNetworkManager->m_IsSessionStarted = true;
    SetStep(&LocalConnectNetworkJob::WaitConnectNetwork, "LocalConnectNetworkJob::WaitConnectNetwork");
    return nn::Result();
}

// 0x0041D948
nn::pia::local::LocalConnectNetworkJob::LocalConnectNetworkJob() : m_pCallContext(nullptr)
{
    m_pBackgroundCallContext = common::NewObject<common::CallContext>();
}

// 0x0041D9D0
// 0x0041D98C (deleting dtor)
nn::pia::local::LocalConnectNetworkJob::~LocalConnectNetworkJob()
{
    if (m_pBackgroundCallContext != nullptr) {
        common::DeleteObject(m_pBackgroundCallContext);
    }
}

// 0x007315C0
void nn::pia::local::LocalConnectNetworkJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
