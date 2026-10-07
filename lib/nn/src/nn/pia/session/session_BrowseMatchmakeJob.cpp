#include "nn/pia/session/session_BrowseMatchmakeJob.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace session {
// 0x004398E8
nn::pia::common::ExecuteResult nn::pia::session::BrowseMatchmakeJob::CompleteProcess()
{
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00439964
void nn::pia::session::BrowseMatchmakeJob::Cleanup()
{
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->m_State == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
    }
}

// 0x00439990 (name is ours)
nn::Result nn::pia::session::BrowseMatchmakeJob::Startup(nn::pia::common::CallContext* pCallContext, nn::pia::session::CommonMatchmakeSession* pSession,
                                                         const nn::pia::session::SessionSearchCriteria* pCriteria)
{
    if (!common::IsValidPointer(pCallContext) || !common::IsValidPointer(pCriteria)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (!common::IsValidPointer(pSession)) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_pCallContext != nullptr) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        return common::RESULT_NOT_IN_SESSION;
    }
    nn::Result result = vf_0x1C(pSession, pCriteria);
    if (result.IsFailure()) {
        return result;
    }
    m_pCallContext = pCallContext;
    pCallContext->InitiateCall();
    Reset(true);
    return nn::Result();
}

// 0x00439A58
nn::pia::session::BrowseMatchmakeJob::BrowseMatchmakeJob() : m_pCallContext(nullptr)
{
}

// 0x00439A88
// 0x00439A78 (deleting dtor)
nn::pia::session::BrowseMatchmakeJob::~BrowseMatchmakeJob()
{
    // empty (in the original too)
}

// 0x0073390C
void nn::pia::session::BrowseMatchmakeJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
