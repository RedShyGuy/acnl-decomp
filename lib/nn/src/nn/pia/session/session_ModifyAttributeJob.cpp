#include "nn/pia/session/session_ModifyAttributeJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace session {
// 0x00439AB0
nn::pia::common::ExecuteResult nn::pia::session::ModifyAttributeJob::FailureProcess()
{
    if (m_pCallContext->m_Result == common::RESULT_SESSION_DISCONNECTED) {
        Session::s_pInstance->SetDisconnectedByError();
    }
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00439AF8
nn::pia::common::ExecuteResult nn::pia::session::ModifyAttributeJob::CompleteProcess()
{
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    // the monitoring data counts the changes
    u8 count = common::g_SessionStateMonitoringContent.m_Unknown0x3CD;
    common::g_SessionStateMonitoringContent.m_Unknown0x3CD = count == 0xFF ? 1 : count + 1;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00439B40
void nn::pia::session::ModifyAttributeJob::Cleanup()
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
    m_Index = 0;
    m_Value = 0;
}

// 0x00439B94 (name is ours)
nn::Result nn::pia::session::ModifyAttributeJob::Startup(nn::pia::common::CallContext* pCallContext, u32 sessionId, u32 index, u32 value,
                                                         nn::pia::session::CommonMatchmakeSession* pSession)
{
    if (!common::IsValidPointer(pCallContext) || !common::IsValidPointer(pSession)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (!pSession->vf_0x88()) {
        return common::RESULT_INVALID_STATE;
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        return common::RESULT_NOT_IN_SESSION;
    }
    nn::Result result = vf_0x1C();
    if (result.IsFailure()) {
        return result;
    }
    m_pCallContext = pCallContext;
    pCallContext->InitiateCall();
    m_SessionId = sessionId;
    m_Index = index;
    m_Value = value;
    Reset(true);
    return nn::Result();
}

// 0x00439C60
nn::pia::session::ModifyAttributeJob::ModifyAttributeJob() : m_pCallContext(nullptr), m_SessionId(0), m_Index(0), m_Value(0)
{
}

// 0x00439CBC
// 0x00439C94 (deleting dtor)
nn::pia::session::ModifyAttributeJob::~ModifyAttributeJob()
{
    // empty (in the original too)
}

// 0x0073391C
void nn::pia::session::ModifyAttributeJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
