#include "nn/pia/local/local_LocalCreateNetworkJob.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalBackgroundProcessJob.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"

namespace nn {
namespace pia {
namespace local {
// 0x0041ACC0
nn::pia::common::ExecuteResult nn::pia::local::LocalCreateNetworkJob::WaitForCancel()
{
    if (!m_pBackgroundCallContext->IsFinished()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    m_pCallContext->SignalCancel();
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0041AD10 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalCreateNetworkJob::WaitCreateNetwork()
{
    if (m_pCallContext->IsCancelRequested()) {
        if (m_pBackgroundCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pBackgroundCallContext->Cancel();
        }
        SetStep(&LocalCreateNetworkJob::WaitForCancel, "LocalCreateNetworkJob::WaitForCancel");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    switch (m_pBackgroundCallContext->GetState()) {
    case common::CallContext::STATE_CALL_IN_PROGRESS:
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    case common::CallContext::STATE_CALL_SUCCESS:
        LocalNetwork::s_pInstance->m_pNetworkManager->vf_0x10();
        m_pCallContext->SignalSuccess(m_pBackgroundCallContext->m_Result);
        m_pCallContext = nullptr;
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

// 0x0041AE38 (name is ours)
void nn::pia::local::LocalCreateNetworkJob::Cleanup()
{
    m_pBackgroundCallContext->Cancel();
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalFailure(common::RESULT_CANCELED);
        }
        m_pCallContext = nullptr;
    }
}

// 0x0041AE74 (name is ours)
nn::Result nn::pia::local::LocalCreateNetworkJob::Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalCreateNetworkSetting* pSetting)
{
    nn::Result result = LocalNetwork::s_pInstance->m_pBackgroundProcessJob->StartupCreateNetwork(m_pBackgroundCallContext, pSetting);
    if (result.IsFailure()) {
        return result;
    }
    m_pCallContext = pCallContext;
    pCallContext->Reset();
    m_pCallContext->InitiateCall();
    LocalNetwork::s_pInstance->m_pBackgroundProcessJob->Ready(true);
    LocalNetwork::s_pInstance->m_IsScanned = false;
    LocalNetwork::s_pInstance->m_pNetworkManager->m_IsSessionStarted = true;
    SetStep(&LocalCreateNetworkJob::WaitCreateNetwork, "LocalCreateNetworkJob::WaitCreateNetwork");
    return nn::Result();
}

// 0x0041AF3C
nn::pia::local::LocalCreateNetworkJob::LocalCreateNetworkJob() : m_pCallContext(nullptr)
{
    m_pBackgroundCallContext = common::NewObject<common::CallContext>();
}

// 0x0041AFC4
// 0x0041AF80 (deleting dtor)
nn::pia::local::LocalCreateNetworkJob::~LocalCreateNetworkJob()
{
    if (m_pBackgroundCallContext != nullptr) {
        common::DeleteObject(m_pBackgroundCallContext);
    }
}

// 0x007311DC
void nn::pia::local::LocalCreateNetworkJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
