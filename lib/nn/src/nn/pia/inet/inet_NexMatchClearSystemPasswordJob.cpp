#include "nn/pia/inet/inet_NexMatchClearSystemPasswordJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/inet/inet_NexMatchmakeSession.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace inet {
// 0x00411158
nn::Result nn::pia::inet::NexMatchClearSystemPasswordJob::vf_0x1C()
{
    session::Session* pSession = session::Session::s_pInstance;
    m_pSession = static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]);
    SetStep(&NexMatchClearSystemPasswordJob::ClearMatchmakeSystemPassword, "NexMatchClearSystemPasswordJob::ClearMatchmakeSystemPassword");
    return nn::Result();
}

// 0x004111DC
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchClearSystemPasswordJob::ClearMatchmakeSystemPassword()
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
    nn::Result result = m_pSession->ClearSystemPasswordAsync(&m_CallContext, m_SessionId);
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&NexMatchClearSystemPasswordJob::WaitClearMatchmakeSystemPassword, "NexMatchClearSystemPasswordJob::WaitClearMatchmakeSystemPassword");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004112E0
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchClearSystemPasswordJob::WaitClearMatchmakeSystemPassword()
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
    if (m_pSession->IsClearSystemPasswordCompleted()) {
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            if (m_CallContext.m_Result == common::RESULT_INVALID_STATE) {
                m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
            } else {
                m_pCallContext->SignalFailure(m_CallContext.m_Result);
            }
            SetStep(&ClearMatchmakeSystemPasswordJob::FailureProcess, "ClearMatchmakeSystemPasswordJob::FailureProcess");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            SetStep(&ClearMatchmakeSystemPasswordJob::CompleteProcess, "ClearMatchmakeSystemPasswordJob::CompleteProcess");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004114AC
void nn::pia::inet::NexMatchClearSystemPasswordJob::Cleanup()
{
    ClearMatchmakeSystemPasswordJob::Cleanup();
    m_pSession = nullptr;
}

// 0x004114C4
nn::pia::inet::NexMatchClearSystemPasswordJob::NexMatchClearSystemPasswordJob() : m_pSession(nullptr)
{
}

// 0x00447A2C
// 0x004114E4 (deleting dtor)
nn::pia::inet::NexMatchClearSystemPasswordJob::~NexMatchClearSystemPasswordJob()
{
    // empty (in the original too)
}

// 0x0072FA64
void nn::pia::inet::NexMatchClearSystemPasswordJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
