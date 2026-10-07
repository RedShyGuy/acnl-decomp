#include "nn/pia/inet/inet_NexMatchModifyAttributeJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace inet {
// 0x0040CC50
nn::Result nn::pia::inet::NexMatchModifyAttributeJob::vf_0x1C()
{
    session::Session* pSession = session::Session::s_pInstance;
    m_pSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex];
    SetStep(&NexMatchModifyAttributeJob::ModifyAttribute, "NexMatchModifyAttributeJob::ModifyAttribute");
    return nn::Result();
}

// 0x0040CCC0
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchModifyAttributeJob::ModifyAttribute()
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
    nn::Result result = m_pSession->ModifyAttributeAsync(&m_CallContext, m_SessionId, m_Index, m_Value);
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&NexMatchModifyAttributeJob::WaitModifyAttribute, "NexMatchModifyAttributeJob::WaitModifyAttribute");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040CDF8
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchModifyAttributeJob::WaitModifyAttribute()
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
    if (m_pSession->IsModifyAttributeCompleted()) {
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            if (m_CallContext.m_Result == common::RESULT_INVALID_STATE) {
                m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
            } else {
                m_pCallContext->SignalFailure(m_CallContext.m_Result);
            }
            SetStep(&ModifyAttributeJob::FailureProcess, "ModifyAttributeJob::FailureProcess");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            SetStep(&ModifyAttributeJob::CompleteProcess, "ModifyAttributeJob::CompleteProcess");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040CFA8
void nn::pia::inet::NexMatchModifyAttributeJob::Cleanup()
{
    ModifyAttributeJob::Cleanup();
    m_pSession = nullptr;
}

// 0x0040CFC0
nn::pia::inet::NexMatchModifyAttributeJob::NexMatchModifyAttributeJob() : m_pSession(nullptr)
{
}

// 0x00439CB8
// 0x0040CFE0 (deleting dtor)
nn::pia::inet::NexMatchModifyAttributeJob::~NexMatchModifyAttributeJob()
{
    // empty (in the original too)
}

// 0x0072F858
void nn::pia::inet::NexMatchModifyAttributeJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
