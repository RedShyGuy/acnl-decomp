#include "nn/pia/local/local_LocalMatchUpdateApplicationDataJob.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalMatchmakeSession.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace local {
// 0x00425654
nn::Result nn::pia::local::LocalMatchUpdateApplicationDataJob::vf_0x1C(const void* pData, u32 size)
{
    session::Session* pSession = session::Session::s_pInstance;
    nn::Result result = static_cast<LocalMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex])->SetApplicationData(pData, size);
    if (result.IsFailure()) {
        return result;
    }
    SetStep(&LocalMatchUpdateApplicationDataJob::SignalProcess, "LocalMatchUpdateApplicationDataJob::SignalProcess");
    return nn::Result();
}

// 0x004256D8
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchUpdateApplicationDataJob::SignalProcess()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pCallContext != nullptr && m_pCallContext->m_State == common::CallContext::STATE_CALL_CANCEL) {
        Cleanup();
        m_pCallContext->SignalFailure(common::RESULT_CANCELED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&UpdateApplicationDataJob::CompleteProcess, "UpdateApplicationDataJob::CompleteProcess");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x004257C8
nn::pia::local::LocalMatchUpdateApplicationDataJob::LocalMatchUpdateApplicationDataJob()
{
    // nothing more than the base (in the original too)
}

// 0x004257F0
// 0x004257E0 (deleting dtor)
nn::pia::local::LocalMatchUpdateApplicationDataJob::~LocalMatchUpdateApplicationDataJob()
{
    // empty (in the original too)
}

// 0x00442C80
void nn::pia::local::LocalMatchUpdateApplicationDataJob::Cleanup()
{
    UpdateApplicationDataJob::Cleanup();
}

// 0x007317FC
void nn::pia::local::LocalMatchUpdateApplicationDataJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
