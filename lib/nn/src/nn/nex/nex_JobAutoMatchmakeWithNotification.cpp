#include "nn/nex/nex_JobAutoMatchmakeWithNotification.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_JobWaitForNotification.h"
#include "nn/nex/nex_MatchmakeExtensionClient.h"
#include "nn/nex/nex_NotificationEventFilterInterface.h"

#include <cstdlib>

namespace nn {
namespace nex {
namespace {
// the results of the job (the names are ours)
const int RESULT_SUCCESS = 0x10001;
const int RESULT_FAILURE = static_cast<int>(0x80010001);
const int RESULT_CANCELLED = static_cast<int>(0x80010004);
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
const int RESULT_TIMEOUT = static_cast<int>(0x8001000A);
const int RESULT_GATHERING_UNAVAILABLE = static_cast<int>(0x800300DE);
const int RESULT_GATHERING_FULL = static_cast<int>(0x80030070);
// the notification of the gathering and the outcomes that it gives (m_uiParam3)
const u32 NOTIFICATION_TYPE = 125;
const u32 NOTIFICATION_GATHERING = 1;
const u32 NOTIFICATION_UNAVAILABLE = 3;
const u32 NOTIFICATION_TIMEOUT = 4;
const u32 NOTIFICATION_FULL = 5;
const u32 NOTIFICATION_TIMEOUT_MS = 0x7FFFFFFF;

// (inline) the call of the caller (null if it was cancelled)
inline CallContext* GetCallContext(u32 id)
{
    return Core::GetInstance()->m_pCallContextRegister->GetCallContext(id);
}
} // namespace

// 0x00380920 (name is ours)
void nn::nex::JobAutoMatchmakeWithNotification::CompleteJob(const nn::nex::qResult& result)
{
    CallContext* pContext = GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        if (result) {
            *m_pSession = m_Session;
            pContext->SignalSuccess(result);
        } else {
            pContext->SignalFailure(result);
        }
    } else {
        m_ProtocolCallContext.Cancel(CallContext::STATE_CANCELLED);
        m_NotificationContext.Cancel(CallContext::STATE_CANCELLED);
    }
    SetToComplete();
}

// 0x003809C4 (name is ours)
void nn::nex::JobAutoMatchmakeWithNotification::ProcessNotification()
{
    if (GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_NotificationContext.m_State != CallContext::STATE_SUCCESS) {
        CompleteJob(qResult(m_NotificationContext.m_Result));
        return;
    }
    switch (m_Event.m_uiParam3) {
    case NOTIFICATION_GATHERING: {
        // the gathering id is the text of the notification
        char* pEnd = nullptr;
        char* pText;
        m_Event.m_strParam.CreateCopy(&pText);
        m_NotifiedGatheringId = std::strtoul(pText, &pEnd, 10);
        String::ReleaseCopy(pText);
        if (m_NotifiedGatheringId != 0) {
            SetStep(Step(&JobAutoMatchmakeWithNotification::CallGetMatchmakeSession, ""));
            return;
        }
        CompleteJob(qResult(RESULT_FAILURE));
        return;
    }
    case NOTIFICATION_UNAVAILABLE:
        CompleteJob(qResult(RESULT_GATHERING_UNAVAILABLE));
        return;
    case NOTIFICATION_TIMEOUT:
        CompleteJob(qResult(RESULT_TIMEOUT));
        return;
    case NOTIFICATION_FULL:
        CompleteJob(qResult(RESULT_GATHERING_FULL));
        return;
    default:
        CompleteJob(qResult(RESULT_FAILURE));
        return;
    }
}

// 0x00380BB8 (name is ours)
void nn::nex::JobAutoMatchmakeWithNotification::CallAutoMatchmake()
{
    if (GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    m_ProtocolCallContext.Reset();
    if (!m_pClient->m_Extension.CallMethod45(&m_ProtocolCallContext, m_Param, &m_GatheringId)) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_ProtocolCallContext.m_Id);
    m_NotificationContext.Reset();
    if (!JobWaitForNotification::WaitForNotification(&m_NotificationContext, NOTIFICATION_TYPE, 0, NOTIFICATION_TIMEOUT_MS, &m_pFilter, &m_Event)) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_NotificationContext.m_Id);
    SetToSuspended();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobAutoMatchmakeWithNotification::WaitForAutoMatchmake, ""));
}

// 0x00380D0C (name is ours)
void nn::nex::JobAutoMatchmakeWithNotification::WaitForAutoMatchmake()
{
    if (GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
        CompleteJob(qResult(m_ProtocolCallContext.m_Result));
        return;
    }
    // the notification of the gathering
    m_pFilter->FilterByParam64(m_GatheringId);
    SetToSuspended();
    ContinueAfterCall(&m_NotificationContext, new Step(&JobAutoMatchmakeWithNotification::ProcessNotification, ""));
}

// 0x00380E08 (name is ours)
void nn::nex::JobAutoMatchmakeWithNotification::CallGetMatchmakeSession()
{
    if (GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    m_ProtocolCallContext.Reset();
    u32 gatheringId = m_NotifiedGatheringId;
    if (!m_pClient->m_Extension.CallGetMatchmakeSession(&m_ProtocolCallContext, gatheringId, &m_Session)) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_ProtocolCallContext.m_Id);
    SetToSuspended();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobAutoMatchmakeWithNotification::ProcessMatchmakeSession, ""));
}

// 0x00380F28 (name is ours)
void nn::nex::JobAutoMatchmakeWithNotification::ProcessMatchmakeSession()
{
    if (GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
        CompleteJob(qResult(m_ProtocolCallContext.m_Result));
        return;
    }
    CompleteJob(qResult(RESULT_SUCCESS));
}

} // namespace nex
} // namespace nn
