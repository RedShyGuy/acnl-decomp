#include "nn/nex/nex_JobWaitForResult.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"

namespace nn {
namespace nex {
namespace {
// the results of the job (the names are ours)
const int RESULT_FAILURE = static_cast<int>(0x80010001);
const int RESULT_CANCELLED = static_cast<int>(0x80010004);
// the time between the tests (ms)
const int WAIT_TIME = 16;
} // namespace

inline void JobWaitForResult::CompleteJob(nn::nex::qResult result)
{
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        pContext->SetStateImpl(result.m_Code >= 0 ? CallContext::STATE_SUCCESS : CallContext::STATE_FAILURE, result, true);
    }
    SetState(STATE_COMPLETED);
}

// 0x0036D7B0 (name is ours)
void nn::nex::JobWaitForResult::WaitForResult()
{
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    qResult result(RESULT_FAILURE);
    if (!IsCompleted(&result)) {
        SetState(STATE_WAITING);
        m_WaitTime = WAIT_TIME;
        return;
    }
    CompleteJob(result);
}

} // namespace nex
} // namespace nn
