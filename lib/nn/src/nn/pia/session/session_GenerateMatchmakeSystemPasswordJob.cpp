#include "nn/pia/session/session_GenerateMatchmakeSystemPasswordJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace session {
// 0x00447A50
nn::pia::common::ExecuteResult nn::pia::session::GenerateMatchmakeSystemPasswordJob::FailureProcess()
{
    if (m_pCallContext->m_Result == common::RESULT_SESSION_DISCONNECTED) {
        Session::s_pInstance->SetDisconnectedByError();
    }
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00447A98
nn::pia::common::ExecuteResult nn::pia::session::GenerateMatchmakeSystemPasswordJob::CompleteProcess()
{
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    // the monitoring data counts the passwords
    u8 count = common::g_SessionStateMonitoringContent.m_Unknown0x3CB;
    common::g_SessionStateMonitoringContent.m_Unknown0x3CB = count == 0xFF ? 1 : count + 1;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00447AE0
void nn::pia::session::GenerateMatchmakeSystemPasswordJob::Cleanup()
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
    if (m_pUnknown0x5C != nullptr) {
        m_pUnknown0x5C = nullptr;
    }
    m_SessionId = 0;
}

// 0x00447B38
nn::pia::session::GenerateMatchmakeSystemPasswordJob::GenerateMatchmakeSystemPasswordJob()
    : m_SessionId(0), m_pCallContext(nullptr), m_pUnknown0x5C(nullptr)
{
}

// 0x00447B94
// 0x00447B6C (deleting dtor)
nn::pia::session::GenerateMatchmakeSystemPasswordJob::~GenerateMatchmakeSystemPasswordJob()
{
    // empty (in the original too)
}

// 0x00734214
void nn::pia::session::GenerateMatchmakeSystemPasswordJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
