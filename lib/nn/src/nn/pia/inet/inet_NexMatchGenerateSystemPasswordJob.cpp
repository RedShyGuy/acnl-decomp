#include "nn/pia/inet/inet_NexMatchGenerateSystemPasswordJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/inet/inet_NexMatchmakeSession.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace inet {
// 0x00411FB4
nn::Result nn::pia::inet::NexMatchGenerateSystemPasswordJob::vf_0x1C()
{
    session::Session* pSession = session::Session::s_pInstance;
    m_pSession = static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]);
    SetStep(&NexMatchGenerateSystemPasswordJob::GenerateMatchmakeSystemPassword, "NexMatchGenerateSystemPasswordJob::GenerateMatchmakeSystemPassword");
    return nn::Result();
}

// 0x00411FF8
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchGenerateSystemPasswordJob::GenerateMatchmakeSystemPassword()
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
    nn::Result result = m_pSession->GenerateSystemPasswordAsync(&m_CallContext, m_SessionId);
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&NexMatchGenerateSystemPasswordJob::WaitGenerateMatchmakeSystemPassword, "NexMatchGenerateSystemPasswordJob::WaitGenerateMatchmakeSystemPassword");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004120FC
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchGenerateSystemPasswordJob::WaitGenerateMatchmakeSystemPassword()
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
    if (m_pSession->IsGenerateSystemPasswordCompleted(m_pUnknown0x5C)) {
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            if (m_CallContext.m_Result == common::RESULT_INVALID_STATE) {
                m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
            } else {
                m_pCallContext->SignalFailure(m_CallContext.m_Result);
            }
            SetStep(&GenerateMatchmakeSystemPasswordJob::FailureProcess, "GenerateMatchmakeSystemPasswordJob::FailureProcess");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            SetStep(&GenerateMatchmakeSystemPasswordJob::CompleteProcess, "GenerateMatchmakeSystemPasswordJob::CompleteProcess");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004122D0
void nn::pia::inet::NexMatchGenerateSystemPasswordJob::Cleanup()
{
    GenerateMatchmakeSystemPasswordJob::Cleanup();
    m_pSession = nullptr;
}

// 0x004122E8
nn::pia::inet::NexMatchGenerateSystemPasswordJob::NexMatchGenerateSystemPasswordJob() : m_pSession(nullptr)
{
}

// 0x00447B90
// 0x00412308 (deleting dtor)
nn::pia::inet::NexMatchGenerateSystemPasswordJob::~NexMatchGenerateSystemPasswordJob()
{
    // empty (in the original too)
}

// 0x0072FA70
void nn::pia::inet::NexMatchGenerateSystemPasswordJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
