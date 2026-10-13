#include "nn/nex/nex_PeriodicJob.h"

namespace nn {
namespace nex {
// 0x0035B63C | fefates:bytes
bool nn::nex::PeriodicJob::SkipWaitDelayAtTermination()
{
    return m_Period == 0;
}

// 0x0035B67C
// 0x0035B650 (deleting dtor)
nn::nex::PeriodicJob::~PeriodicJob()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003CC720 | fefates:bytes
void nn::nex::PeriodicJob::SetDefaultPostExecutionState()
{
    if (m_Period > 0) {
        SetToWaiting(m_Period);
        return;
    }
    SetState(STATE_COMPLETED);
}

} // namespace nex
} // namespace nn
