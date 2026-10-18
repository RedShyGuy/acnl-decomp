#include "nn/nex/nex_JobTerminate.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_JobLogin.h"
#include "nn/nex/nex_NgsFacade.h"

namespace nn {
namespace nex {
// 0x0035D428 | fefates:bytes [tier B]
void nn::nex::JobTerminate::StepWaitOnPendingJobs()
{
    const int RESULT_SUCCESS = qResult::SUCCESS;
    Job* pLoginJob = m_pFacade->m_pLoginJob;
    if (pLoginJob != nullptr && pLoginJob->m_State != STATE_COMPLETED) {
        SetToWaiting(100);
        return;
    }
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        pContext->SetStateImpl(CallContext::STATE_SUCCESS, qResult(RESULT_SUCCESS), true);
    }
    SetToComplete();
}

// 0x0035D4F8 slot 0x00
// 0x0035D4D4 (deleting dtor)
nn::nex::JobTerminate::~JobTerminate()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
