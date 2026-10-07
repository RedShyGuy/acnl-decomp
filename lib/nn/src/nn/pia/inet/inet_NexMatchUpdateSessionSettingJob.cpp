#include "nn/pia/inet/inet_NexMatchUpdateSessionSettingJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/inet/inet_NexUpdateSessionSetting.h"
#include "nn/pia/inet/inet_NexMatchmakeSession.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace inet {
// 0x004115A8
nn::Result nn::pia::inet::NexMatchUpdateSessionSettingJob::vf_0x1C(const nn::pia::session::CreateSessionSetting* pSetting)
{
    session::Session* pSession = session::Session::s_pInstance;
    m_pSession = static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]);
    m_pSession->SetSessionSetting(reinterpret_cast<const NexUpdateSessionSetting*>(pSetting));
    SetStep(&NexMatchUpdateSessionSettingJob::UpdateSessionSetting, "NexMatchUpdateSessionSettingJob::UpdateSessionSetting");
    return nn::Result();
}

// 0x0041162C
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchUpdateSessionSettingJob::UpdateSessionSetting()
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
    nn::Result result = m_pSession->UpdateSessionSettingAsync(&m_CallContext, m_SessionId);
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&NexMatchUpdateSessionSettingJob::WaitUpdateSessionSetting, "NexMatchUpdateSessionSettingJob::WaitUpdateSessionSetting");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0041176C
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchUpdateSessionSettingJob::WaitUpdateSessionSetting()
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
    if (m_pSession->IsUpdateSessionSettingCompleted()) {
        // unlike the other jobs there is no FailureProcess step
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            m_pCallContext->SignalFailure(m_CallContext.m_Result);
            m_pCallContext = nullptr;
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            SetStep(&UpdateSessionSettingJob::CompleteProcess, "UpdateSessionSettingJob::CompleteProcess");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004118B8
void nn::pia::inet::NexMatchUpdateSessionSettingJob::Cleanup()
{
    UpdateSessionSettingJob::Cleanup();
    m_pSession = nullptr;
}

// 0x004118D0
nn::pia::inet::NexMatchUpdateSessionSettingJob::NexMatchUpdateSessionSettingJob() : m_pSession(nullptr)
{
}

// 0x00442BE8
// 0x004118F0 (deleting dtor)
nn::pia::inet::NexMatchUpdateSessionSettingJob::~NexMatchUpdateSessionSettingJob()
{
    // empty (in the original too)
}

// 0x0072FA68
void nn::pia::inet::NexMatchUpdateSessionSettingJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
