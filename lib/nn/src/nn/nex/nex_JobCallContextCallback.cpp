#include "nn/nex/nex_JobCallContextCallback.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"

namespace nn {
namespace nex {
namespace {
// the call was cancelled
const int RESULT_CANCELLED = static_cast<int>(0x80010004);
} // namespace

// 0x0039C274 (name is ours)
void nn::nex::JobCallContextCallback::ProcessCallResult()
{
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    CompleteJob(m_pContext->m_Result);
}

// 0x0039C2F8 | fefates:bytes [tier B]
void nn::nex::JobCallContextCallback::CompleteJob(const nn::nex::qResult& result)
{
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        if (result.m_Code >= 0) {
            pContext->SetStateImpl(CallContext::STATE_SUCCESS, result, true);
            OnSuccess(pContext);
        } else {
            pContext->SetStateImpl(CallContext::STATE_FAILURE, result, true);
            OnFailure(pContext);
        }
    } else {
        m_pContext->Cancel(CallContext::STATE_CANCELLED);
        OnCancel();
    }
    SetState(STATE_COMPLETED);
}

// 0x0039C3B8 (name is ours)
void nn::nex::JobCallContextCallback::WaitForCall()
{
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_pContext->m_State != CallContext::STATE_CALL_IN_PROGRESS) {
        SetStepInline(&JobCallContextCallback::StartCall, "");
        return;
    }
    SetState(STATE_SUSPENDED);
    ResumeOnCallCompletion(m_pContext, new Step(&JobCallContextCallback::StartCall, ""));
}

// 0x0039C4C8 (name is ours)
void nn::nex::JobCallContextCallback::StartCall()
{
    Call();
}

} // namespace nex
} // namespace nn
