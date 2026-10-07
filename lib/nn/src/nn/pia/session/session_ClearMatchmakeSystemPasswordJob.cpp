#include "nn/pia/session/session_ClearMatchmakeSystemPasswordJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace session {
// 0x00447918
nn::pia::common::ExecuteResult nn::pia::session::ClearMatchmakeSystemPasswordJob::FailureProcess()
{
    if (m_pCallContext->m_Result == common::RESULT_SESSION_DISCONNECTED) {
        Session::s_pInstance->SetDisconnectedByError();
    }
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00447960
nn::pia::common::ExecuteResult nn::pia::session::ClearMatchmakeSystemPasswordJob::CompleteProcess()
{
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0044798C
void nn::pia::session::ClearMatchmakeSystemPasswordJob::Cleanup()
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

// 0x004479D8
nn::pia::session::ClearMatchmakeSystemPasswordJob::ClearMatchmakeSystemPasswordJob() : m_SessionId(0), m_pCallContext(nullptr)
{
}

// 0x00447A30
// 0x00447A08 (deleting dtor)
nn::pia::session::ClearMatchmakeSystemPasswordJob::~ClearMatchmakeSystemPasswordJob()
{
    // empty (in the original too)
}

// 0x00734210
void nn::pia::session::ClearMatchmakeSystemPasswordJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
