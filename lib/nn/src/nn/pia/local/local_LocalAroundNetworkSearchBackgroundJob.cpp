#include "nn/pia/local/local_LocalAroundNetworkSearchBackgroundJob.h"
#include "nn/pia/common/common_Result.h"

namespace nn {
namespace pia {
namespace local {
// 0x00425C30 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::LocalAroundNetworkSearchBackgroundJob::AroundNetworkSearch()
{
    nn::Result result = ScanNetwork();
    if (result.IsSuccess()) {
        m_pCallContext->SignalSuccess(nn::Result());
    } else {
        m_pCallContext->SignalFailure(result);
    }
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00425C88 (name is ours)
void nn::pia::local::LocalAroundNetworkSearchBackgroundJob::Cleanup()
{
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalFailure(common::RESULT_CANCELED);
        }
        m_pCallContext = nullptr;
    }
}

// 0x00425CBC | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalAroundNetworkSearchBackgroundJob::Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalAroundNetworkSearchSetting& setting)
{
    if (IsRunning()) {
        return common::RESULT_INVALID_STATE;
    }
    Reset(false);
    m_pCallContext = pCallContext;
    pCallContext->Reset();
    m_pCallContext->InitiateCall();
    m_Setting = setting;
    SetStep(&LocalAroundNetworkSearchBackgroundJob::AroundNetworkSearch, "LocalAroundNetworkSearchBackgroundJob::AroundNetworkSearch");
    return nn::Result();
}

// 0x00425D74
nn::pia::local::LocalAroundNetworkSearchBackgroundJob::LocalAroundNetworkSearchBackgroundJob() : m_Setting(), m_pCallContext(nullptr)
{
}

// 0x00425DD4
// 0x00425DC0 (deleting dtor)
nn::pia::local::LocalAroundNetworkSearchBackgroundJob::~LocalAroundNetworkSearchBackgroundJob()
{
    // empty (in the original too)
}

// 0x00731808
void nn::pia::local::LocalAroundNetworkSearchBackgroundJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
