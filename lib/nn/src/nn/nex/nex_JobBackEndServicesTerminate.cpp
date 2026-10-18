#include "nn/nex/nex_JobBackEndServicesTerminate.h"
#include "nn/nex/nex_BackEndServices.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"

namespace nn {
namespace nex {
// 0x003BCFEC | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesTerminate::StepLogoutComplete()
{
    qResult result = m_CallContext.m_Result;
    if (!result) {
        // (the failure was logged)
    }
    CallContext::State state = static_cast<CallContext::State>(m_CallContext.m_State);
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        pContext->SetStateImpl(state, result, true);
    }
    SetToComplete();
}

// 0x003BD060 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesTerminate::StepWaitOnLoginCancel()
{
    Job* pLoginJob = m_pBackEndServices->m_pLoginJob;
    if (pLoginJob != nullptr && pLoginJob->m_State != STATE_COMPLETED) {
        SetToWaiting(100);
        return;
    }
    SetStep(Step(&JobBackEndServicesTerminate::StepInitLogoutIfRequired, ""));
}

// 0x003BD0CC | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesTerminate::StepWaitOnPendingJobs()
{
    Job* pLoginJob = m_pBackEndServices->m_pLoginJob;
    if (pLoginJob != nullptr && pLoginJob->m_State != STATE_COMPLETED) {
        pLoginJob->CancelJob();
        SetStep(Step(&JobBackEndServicesTerminate::StepWaitOnLoginCancel, ""));
        return;
    }
    Job* pLogoutJob = m_pBackEndServices->m_pLogoutJob;
    if (pLogoutJob != nullptr && pLogoutJob->m_State != STATE_COMPLETED) {
        SetToWaiting(100);
        return;
    }
    SetStep(Step(&JobBackEndServicesTerminate::StepInitLogoutIfRequired, ""));
}

// 0x003BD190 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesTerminate::StepInitLogoutIfRequired()
{
    const int RESULT_SUCCESS = qResult::SUCCESS;
    if (m_pBackEndServices->m_pCredentials != nullptr) {
        m_pBackEndServices->LogoutImpl(&m_CallContext, m_pBackEndServices->m_pCredentials);
        SetToSuspended();
        ResumeOnCallCompletion(&m_CallContext, new Step(&JobBackEndServicesTerminate::StepLogoutComplete, ""));
        return;
    }
    qResult result(RESULT_SUCCESS);
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        pContext->SetStateImpl(CallContext::STATE_SUCCESS, result, true);
    }
    SetToComplete();
}

// 0x003BD298 slot 0x00
// 0x003BD274 (deleting dtor)
nn::nex::JobBackEndServicesTerminate::~JobBackEndServicesTerminate()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
