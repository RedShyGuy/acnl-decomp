#include "nn/pia/session/session_UpdateSessionSettingJob.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"

namespace nn {
namespace pia {
namespace session {
// 0x00442B08
nn::pia::common::ExecuteResult nn::pia::session::UpdateSessionSettingJob::CompleteProcess()
{
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    // the monitoring data counts the changes
    u8 count = common::g_SessionStateMonitoringContent.m_Unknown0x3D4;
    common::g_SessionStateMonitoringContent.m_Unknown0x3D4 = count == 0xFF ? 1 : count + 1;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00442B50
void nn::pia::session::UpdateSessionSettingJob::Cleanup()
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

// 0x00442B9C
nn::pia::session::UpdateSessionSettingJob::UpdateSessionSettingJob() : m_pCallContext(nullptr)
{
}

// 0x00442BEC
// 0x00442BC4 (deleting dtor)
nn::pia::session::UpdateSessionSettingJob::~UpdateSessionSettingJob()
{
    // empty (in the original too)
}

// 0x0073416C
void nn::pia::session::UpdateSessionSettingJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
