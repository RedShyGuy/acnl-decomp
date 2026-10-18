#include "nn/nex/nex_JobWaitForNotification.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_GlobalNotificationEventManager.h"
#include "nn/nex/nex_MyNotificationEventHandler.h"
#include "nn/nex/nex_NotificationEventManager.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_RVClientCore.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
namespace {
// the call could not be sent
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
// the call was cancelled
const int RESULT_CANCELLED = static_cast<int>(0x80010004);
// no event in time
const int RESULT_TIMEOUT = static_cast<int>(0x8001000B);
// the protocol of the notifications
const u16 NOTIFICATION_PROTOCOL_ID = 14;
// the time between the tests of the wait (ms)
const int WAIT_TIME = 16;

// (inline) the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    return Scheduler::GetInstanceInline()->m_Lock;
}
} // namespace

// 0x0039CF1C slot 0x34 (name is ours)
nn::nex::qResult nn::nex::JobWaitForNotification::Initialize(unsigned int callContextId, unsigned int type, unsigned int subType, unsigned int timeout, unsigned int param1,
                                                             nn::nex::NotificationEventFilterInterface** ppFilter, nn::nex::NotificationEvent* pEvent)
{
    ScopedCS lock(GetSchedulerLock());
    m_CallContextId = callContextId;
    m_Timeout = timeout;
    ProtocolCallContext* pContext = static_cast<ProtocolCallContext*>(Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId));
    Credentials* pCredentials = pContext->m_pCredentials;
    m_pManager = new NotificationEventManager(nullptr);
    m_pManager->m_Unknown0x80 = true;
    m_pManager->m_ProtocolId = NOTIFICATION_PROTOCOL_ID;
    m_pManager->SetFlag(Protocol::FLAG_UNKNOWN_8);
    m_pManager->m_PrincipalId = pCredentials->m_PrincipalId;
    m_pManager->m_ConnectionId = pCredentials->m_ConnectionId;
    m_pHandler = new MyNotificationEventHandler(type, subType, &m_Event);
    m_pManager->RegisterEventHandler(m_pHandler);
    RVClientCore::GetInstance()->m_pNotificationManager->RegisterGlobalNotificationEventManager(m_pManager);
    m_IsRegistered = true;
    m_Chrono.Start();
    if (ppFilter != nullptr) {
        *ppFilter = m_pHandler;
    } else {
        m_pHandler->FilterByParam1(param1);
    }
    m_pOutEvent = pEvent;
    SetStep(Step(&JobWaitForNotification::WaitForEvent, ""));
    return qResult(true);
}

// 0x0039D0FC (name is ours)
void nn::nex::JobWaitForNotification::CompleteJob(const nn::nex::qResult& result)
{
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        if (result) {
            if (m_pOutEvent != nullptr) {
                *m_pOutEvent = *m_pHandler->m_pResult;
            }
            pContext->SignalSuccess(result);
        } else {
            pContext->SignalFailure(result);
        }
    }
    SetToComplete();
}

// 0x0039D190 (name is ours)
void nn::nex::JobWaitForNotification::WaitForEvent()
{
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_pHandler->m_IsFound) {
        CompleteJob(qResult(qResult::SUCCESS));
        return;
    }
    if (static_cast<s32>(m_Chrono.Check()) > static_cast<s32>(m_Timeout)) {
        CompleteJob(qResult(RESULT_TIMEOUT));
        return;
    }
    SetToWaiting(WAIT_TIME);
}

// 0x0039D270 (name is ours)
bool nn::nex::JobWaitForNotification::WaitForNotification(nn::nex::CallContext* pContext, unsigned int type, unsigned int subType, unsigned int timeout,
                                                          nn::nex::NotificationEventFilterInterface** ppFilter, nn::nex::NotificationEvent* pEvent)
{
    return WaitForNotification(pContext, type, subType, timeout, 0, ppFilter, pEvent);
}

// 0x0039D29C (name is ours)
bool nn::nex::JobWaitForNotification::WaitForNotification(nn::nex::CallContext* pContext, unsigned int type, unsigned int subType, unsigned int timeout, unsigned int param1,
                                                          nn::nex::NotificationEventFilterInterface** ppFilter, nn::nex::NotificationEvent* pEvent)
{
    if (pContext->InitiateCall()) {
        JobWaitForNotification* pJob = new JobWaitForNotification();
        qResult result = pJob->Initialize(pContext->m_Id, type, subType, timeout, param1, ppFilter, pEvent);
        if (result) {
            Scheduler::GetInstanceInline()->Queue(pJob, false);
            return qResult(qResult::SUCCESS);
        }
        pJob->ReleaseRef();
    }
    return qResult(RESULT_CALL_FAILED);
}

// 0x0039D4FC slot 0x00
// 0x0039D44C (deleting dtor)
nn::nex::JobWaitForNotification::~JobWaitForNotification()
{
    if (m_IsRegistered) {
        RVClientCore::GetInstance()->m_pNotificationManager->UnregisterGlobalNotificationEventManager(m_pManager);
        m_pManager->UnregisterEventHandler(m_pHandler);
        if (m_pHandler != nullptr) {
            delete m_pHandler;
        }
        m_pHandler = nullptr;
        if (m_pManager != nullptr) {
            delete m_pManager;
        }
        m_pManager = nullptr;
    }
}

} // namespace nex
} // namespace nn
