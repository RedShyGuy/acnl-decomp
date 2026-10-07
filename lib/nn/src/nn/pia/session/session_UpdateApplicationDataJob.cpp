#include "nn/pia/session/session_UpdateApplicationDataJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace session {
// 0x00442C0C
nn::pia::common::ExecuteResult nn::pia::session::UpdateApplicationDataJob::FailureProcess()
{
    if (m_pCallContext->m_Result == common::RESULT_SESSION_DISCONNECTED) {
        Session::s_pInstance->SetDisconnectedByError();
    }
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00442C54
nn::pia::common::ExecuteResult nn::pia::session::UpdateApplicationDataJob::CompleteProcess()
{
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00442C84
void nn::pia::session::UpdateApplicationDataJob::Cleanup()
{
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->m_State == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
    }
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_CallContext.SignalCancel();
    }
    m_CallContext.Reset();
    m_SessionId = 0;
}

// 0x00442CD0
nn::pia::session::UpdateApplicationDataJob::UpdateApplicationDataJob() : m_pCallContext(nullptr)
{
}

// 0x00442D20
// 0x00442CF8 (deleting dtor)
nn::pia::session::UpdateApplicationDataJob::~UpdateApplicationDataJob()
{
    // empty (in the original too)
}

// 0x00734170
void nn::pia::session::UpdateApplicationDataJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
