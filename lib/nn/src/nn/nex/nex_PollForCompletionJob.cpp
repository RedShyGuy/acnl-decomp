#include "nn/nex/nex_PollForCompletionJob.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"

namespace nn {
namespace nex {
namespace {
// the deadline passed
const int RESULT_TIMEOUT = static_cast<int>(0x8001000B);
// the time between the polls (ms)
const int POLL_TIME = 100;
} // namespace

// 0x00397A04 | fefates:bytes [tier B]
void nn::nex::PollForCompletionJob::CompleteJob(nn::nex::qResult result)
{
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        pContext->SetStateImpl(result.m_Code >= 0 ? CallContext::STATE_SUCCESS : CallContext::STATE_FAILURE, result, true);
    }
    SetState(STATE_COMPLETED);
}

// 0x00397A68 (name is ours)
void nn::nex::PollForCompletionJob::Poll()
{
    if (m_pIsCompleted(m_pArgument)) {
        if (m_pIsSucceeded(m_pArgument)) {
            CompleteJob(qResult(qResult::SUCCESS));
        } else {
            CompleteJob(qResult(qResult::FAILURE));
        }
        return;
    }
    if (m_Deadline < SystemClock::GetTime()) {
        CompleteJob(qResult(RESULT_TIMEOUT));
        return;
    }
    SetState(STATE_WAITING);
    m_WaitTime = POLL_TIME;
}

} // namespace nex
} // namespace nn
