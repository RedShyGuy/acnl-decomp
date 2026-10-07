#include "nn/pia/local/local_LocalScanNetworkJob.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalBackgroundProcessJob.h"
#include "nn/pia/local/local_LocalNetwork.h"

namespace nn {
namespace pia {
namespace local {
// 0x0041A0E4 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalScanNetworkJob::WaitForCancel()
{
    if (!m_pBackgroundCallContext->IsFinished()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE_103);
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0041A13C
nn::pia::common::ExecuteResult nn::pia::local::LocalScanNetworkJob::WaitScanNetwork()
{
    if (m_pCallContext->IsCancelRequested()) {
        if (m_pBackgroundCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pBackgroundCallContext->Cancel();
        }
        SetStep(&LocalScanNetworkJob::WaitForCancel, "LocalScanNetworkJob::WaitForCancel");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    switch (m_pBackgroundCallContext->GetState()) {
    case common::CallContext::STATE_CALL_IN_PROGRESS:
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    case common::CallContext::STATE_CALL_SUCCESS:
        LocalNetwork::s_pInstance->m_IsScanned = true;
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

// 0x0041A254 (name is ours)
void nn::pia::local::LocalScanNetworkJob::Cleanup()
{
    m_pBackgroundCallContext->Cancel();
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE_103);
        }
        m_pCallContext = nullptr;
    }
}

// 0x0041A290 | fefates:bytes-fuzzy [tier B]
nn::Result nn::pia::local::LocalScanNetworkJob::Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalScanNetworkSetting* pSetting)
{
    nn::Result result = LocalNetwork::s_pInstance->m_pBackgroundProcessJob->StartupScanNetwork(m_pBackgroundCallContext, pSetting);
    if (result.IsFailure()) {
        return result;
    }
    m_pCallContext = pCallContext;
    pCallContext->Reset();
    m_pCallContext->InitiateCall();
    LocalNetwork::s_pInstance->m_pBackgroundProcessJob->Ready(true);
    LocalNetwork::s_pInstance->m_IsScanned = false;
    SetStep(&LocalScanNetworkJob::WaitScanNetwork, "LocalScanNetworkJob::WaitScanNetwork");
    return nn::Result();
}

// 0x0041A340
nn::pia::local::LocalScanNetworkJob::LocalScanNetworkJob() : m_pCallContext(nullptr)
{
    m_pBackgroundCallContext = common::NewObject<common::CallContext>();
}

// 0x0041A3C8
// 0x0041A384 (deleting dtor)
nn::pia::local::LocalScanNetworkJob::~LocalScanNetworkJob()
{
    if (m_pBackgroundCallContext != nullptr) {
        common::DeleteObject(m_pBackgroundCallContext);
    }
}

// 0x007311CC
void nn::pia::local::LocalScanNetworkJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
