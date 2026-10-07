#include "nn/pia/local/local_LocalMatchBrowseMatchmakeJob.h"
#include "nn/pia/local/local_LocalMatchmakeSession.h"
#include "nn/pia/local/local_LocalSessionSearchCriteria.h"

namespace nn {
namespace pia {
namespace local {
// 0x0042304C
nn::Result nn::pia::local::LocalMatchBrowseMatchmakeJob::vf_0x1C(nn::pia::session::CommonMatchmakeSession* pSession, const nn::pia::session::SessionSearchCriteria* pCriteria)
{
    m_pSession = static_cast<LocalMatchmakeSession*>(pSession);
    m_pSession->SetSearchCriteria(static_cast<const LocalSessionSearchCriteria*>(pCriteria));
    SetStep(&LocalMatchBrowseMatchmakeJob::BrowseMatchmake, "LocalMatchBrowseMatchmakeJob::BrowseMatchmake");
    return nn::Result();
}

// 0x004230B8
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchBrowseMatchmakeJob::BrowseMatchmake()
{
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
    SetStep(&LocalMatchBrowseMatchmakeJob::WaitBrowseMatchmake, "LocalMatchBrowseMatchmakeJob::WaitBrowseMatchmake");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00423194
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchBrowseMatchmakeJob::WaitBrowseMatchmake()
{
    if (m_pCallContext->m_State == common::CallContext::STATE_CALL_CANCEL) {
        Cleanup();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pSession->IsBrowseCompleted()) {
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

// 0x0042328C
void nn::pia::local::LocalMatchBrowseMatchmakeJob::Cleanup()
{
    BrowseMatchmakeJob::Cleanup();
}

// 0x00423290
nn::pia::local::LocalMatchBrowseMatchmakeJob::LocalMatchBrowseMatchmakeJob() : m_CallContext()
{
    // (m_pSession is set by vf_0x1C)
}

// 0x004232D4
// 0x004232B0 (deleting dtor)
nn::pia::local::LocalMatchBrowseMatchmakeJob::~LocalMatchBrowseMatchmakeJob()
{
    // empty (in the original too)
}

// 0x007316CC
void nn::pia::local::LocalMatchBrowseMatchmakeJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
