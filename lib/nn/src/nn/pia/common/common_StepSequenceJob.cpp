#include "nn/pia/common/common_StepSequenceJob.h"
#include "pead/peadTickSpan.h"

namespace nn {
namespace pia {
namespace common {
// 0x004273C0 | fefates:bytes [tier B]
ExecuteResult nn::pia::common::StepSequenceJob::ExecuteCore()
{
    for (;;) {
        if (m_IsCancelRequested) {
            CancelCleanup();
            ExecuteResult result(ExecuteResult::STATE_FAILURE);
            m_IsCancelRequested = false;
            return result;
        }
        // a step that continues at once goes on with the next one
        ExecuteResult result = (this->*m_Step.m_Function)();
        if (result.m_State != ExecuteResult::STATE_CONTINUE) {
            return result;
        }
    }
}

// 0x00427434 | slot vf_0x10 of nn::pia::common::StepSequenceJob
void nn::pia::common::StepSequenceJob::CancelCleanup()
{
    // empty (in the original too)
}

// 0x0042744C | fefates:bytes [tier B]
void nn::pia::common::StepSequenceJob::WaitForCompletion(unsigned int intervalMSec)
{
    for (;;) {
        if (IsForeground()) {
            break;
        }
        if (!IsBackground()) {
            if (GetState() != EXECUTE_STATE_SUSPENDED) {
                return;
            }
            break;
        }
        pead::SleepThread(pead::TickSpan::fromMilliSeconds(intervalMSec));
    }
    CancelCleanup();
    m_IsCancelRequested = false;
    Reset(false);
}

// 0x00427514 | fefates:bytes [tier B]
void nn::pia::common::StepSequenceJob::Reset(bool clearAll)
{
    Job::Reset(clearAll);
    m_IsCancelRequested = false;
}

// 0x0042752C | fefates:bytes [tier B]
nn::pia::common::StepSequenceJob::StepSequenceJob() : m_IsCancelRequested(false), m_Unknown0x38(0)
{
}

// 0x004275A0 | fefates:callgraph [tier C]
// 0x00427580 (deleting dtor)
nn::pia::common::StepSequenceJob::~StepSequenceJob()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00731AF4 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::common::StepSequenceJob::Trace(unsigned long long) const
{
    // empty (in the original too)
}

} // namespace common
} // namespace pia
} // namespace nn
