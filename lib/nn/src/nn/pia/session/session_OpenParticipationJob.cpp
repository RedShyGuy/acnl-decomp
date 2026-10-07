#include "nn/pia/session/session_OpenParticipationJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace session {
// 0x0043A9A0
nn::pia::common::ExecuteResult nn::pia::session::OpenParticipationJob::OpenParticipation()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_4) {
        m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pCallContext != nullptr && m_pCallContext->m_State == common::CallContext::STATE_CALL_CANCEL) {
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    nn::Result result = m_pSession->OpenParticipationAsync(&m_CallContext, m_SessionId);
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&OpenParticipationJob::WaitOpenParticipation, "OpenParticipationJob::WaitOpenParticipation");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0043AAE4
nn::pia::common::ExecuteResult nn::pia::session::OpenParticipationJob::WaitOpenParticipation()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
    } else {
        if (m_pCallContext != nullptr && m_pCallContext->m_State == common::CallContext::STATE_CALL_CANCEL) {
            m_pCallContext = nullptr;
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
        if (!m_pSession->IsOpenParticipationCompleted()) {
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            if (m_CallContext.m_Result == common::RESULT_INVALID_STATE) {
                Session::s_pInstance->SetDisconnectedByError();
                m_pCallContext->SignalFailure(common::RESULT_SESSION_DISCONNECTED);
            } else {
                m_pCallContext->SignalFailure(m_CallContext.m_Result);
            }
        } else if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            Session::s_pInstance->SetJoinable(m_SessionId, true);
            m_pCallContext->SignalSuccess(nn::Result());
        }
    }
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0043AC0C (name is ours)
void nn::pia::session::OpenParticipationJob::Cleanup()
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
}

// 0x0043AC50 (name is ours)
nn::Result nn::pia::session::OpenParticipationJob::Startup(nn::pia::common::CallContext* pCallContext, u32 sessionId,
                                                           nn::pia::session::CommonMatchmakeSession* pSession)
{
    if (!pSession->vf_0x88()) {
        return common::RESULT_INVALID_STATE;
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        return common::RESULT_NOT_IN_SESSION;
    }
    m_pCallContext = pCallContext;
    pCallContext->InitiateCall();
    m_pSession = pSession;
    m_SessionId = sessionId;
    Reset(true);
    SetStep(&OpenParticipationJob::OpenParticipation, "OpenParticipationJob::OpenParticipation");
    return nn::Result();
}

// 0x0043AD18
nn::pia::session::OpenParticipationJob::OpenParticipationJob() : m_pCallContext(nullptr), m_SessionId(0), m_pSession(nullptr)
{
}

// 0x0043AD6C
// 0x0043AD48 (deleting dtor)
nn::pia::session::OpenParticipationJob::~OpenParticipationJob()
{
    // empty (in the original too)
}

// 0x0073392C
void nn::pia::session::OpenParticipationJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
