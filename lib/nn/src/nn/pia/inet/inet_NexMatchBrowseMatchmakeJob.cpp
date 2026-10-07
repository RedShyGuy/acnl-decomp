#include "nn/pia/inet/inet_NexMatchBrowseMatchmakeJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/inet/inet_NexMatchmakeSession.h"
#include "nn/pia/inet/inet_NexSessionSearchCriteria.h"
#include "nn/pia/inet/inet_NexSessionSearchCriteriaOwner.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace inet {
// 0x0040C64C
nn::Result nn::pia::inet::NexMatchBrowseMatchmakeJob::vf_0x1C(nn::pia::session::CommonMatchmakeSession* pSession,
                                                              const nn::pia::session::SessionSearchCriteria* pCriteria)
{
    m_pSession = static_cast<NexMatchmakeSession*>(pSession);
    if (pCriteria->m_Unknown0x4 != 1) {
        if (!m_pSession->SetSearchCriteria(static_cast<const NexSessionSearchCriteria*>(pCriteria), 1)) {
            return common::RESULT_INVALID_ARGUMENT;
        }
        SetStep(&NexMatchBrowseMatchmakeJob::BrowseMatchmake, "NexMatchBrowseMatchmakeJob::BrowseMatchmake");
    } else {
        // the sessions of an owner
        m_OwnerPrincipalId = static_cast<const NexSessionSearchCriteriaOwner*>(pCriteria)->m_OwnerPrincipalId;
        m_Offset = pCriteria->m_ResultOffset;
        m_ResultNumMax = pCriteria->m_ResultNumMax;
        SetStep(&NexMatchBrowseMatchmakeJob::FindSessionByOwner, "NexMatchBrowseMatchmakeJob::FindSessionByOwner");
    }
    return nn::Result();
}

// 0x0040C738
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchBrowseMatchmakeJob::BrowseMatchmake()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pCallContext->m_State == common::CallContext::STATE_CALL_CANCEL) {
        Cleanup();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    nn::Result result = m_pSession->BrowseAsync(&m_CallContext);
    if (result.IsFailure()) {
        if (m_pCallContext != nullptr) {
            m_pCallContext->SignalFailure(result);
            m_pCallContext = nullptr;
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&NexMatchBrowseMatchmakeJob::WaitBrowseMatchmake, "NexMatchBrowseMatchmakeJob::WaitBrowseMatchmake");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040C84C
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchBrowseMatchmakeJob::FindSessionByOwner()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pCallContext->m_State == common::CallContext::STATE_CALL_CANCEL) {
        Cleanup();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    nn::Result result = m_pSession->FindSessionByOwnerAsync(&m_CallContext, m_OwnerPrincipalId, m_Offset, m_ResultNumMax);
    if (result.IsFailure()) {
        if (m_pCallContext != nullptr) {
            m_pCallContext->SignalFailure(result);
            m_pCallContext = nullptr;
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&NexMatchBrowseMatchmakeJob::WaitFindSessionByOwner, "NexMatchBrowseMatchmakeJob::WaitFindSessionByOwner");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0040C96C
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchBrowseMatchmakeJob::WaitBrowseMatchmake()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pCallContext->m_State == common::CallContext::STATE_CALL_CANCEL) {
        Cleanup();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pSession->IsBrowseCompleted()) {
        m_pSession->FinishBrowse();
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            m_pCallContext->SignalFailure(m_CallContext.m_Result);
            m_pCallContext = nullptr;
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            SetStep(&BrowseMatchmakeJob::CompleteProcess, "session::BrowseMatchmakeJob::CompleteProcess");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040CAA8
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchBrowseMatchmakeJob::WaitFindSessionByOwner()
{
    if (session::Session::s_pInstance->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pCallContext->m_State == common::CallContext::STATE_CALL_CANCEL) {
        Cleanup();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pSession->IsFindSessionByOwnerCompleted()) {
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            m_pCallContext->SignalFailure(m_CallContext.m_Result);
            m_pCallContext = nullptr;
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            SetStep(&BrowseMatchmakeJob::CompleteProcess, "session::BrowseMatchmakeJob::CompleteProcess");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040CBD8
nn::pia::inet::NexMatchBrowseMatchmakeJob::NexMatchBrowseMatchmakeJob() : m_pSession(nullptr), m_OwnerPrincipalId(0), m_Offset(0), m_ResultNumMax(0)
{
}

// 0x0040CC30
// 0x0040CC0C (deleting dtor)
nn::pia::inet::NexMatchBrowseMatchmakeJob::~NexMatchBrowseMatchmakeJob()
{
    // empty (in the original too)
}

// 0x00439914
void nn::pia::inet::NexMatchBrowseMatchmakeJob::Cleanup()
{
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_CallContext.SignalCancel();
    }
    m_CallContext.Reset();
    if (m_pSession != nullptr) {
        m_pSession->FinishBrowse();
        m_pSession = nullptr;
    }
    m_OwnerPrincipalId = 0;
    m_Offset = 0;
    m_ResultNumMax = 0;
    BrowseMatchmakeJob::Cleanup();
}

// 0x0072F854
void nn::pia::inet::NexMatchBrowseMatchmakeJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
