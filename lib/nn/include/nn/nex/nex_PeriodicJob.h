#pragma once

#include "decomp.h"
#include "nn/nex/nex_Job.h"

namespace nn {
namespace nex {
// A Job that runs again after its period (0: once). The member name is ours.
// RTTI N2nn3nex11PeriodicJobE @ 0x008CE068
// vtable 0x008FC3E8 (vptr 0x008FC3F0), offset_to_top 0, 12 entries
class PeriodicJob : public ::nn::nex::Job
{
public:
    // (inline)
    PeriodicJob(nn::nex::Job::JobType type, const nn::nex::DebugString& debugString) : Job(type, debugString), m_Period(0)
    {
        SetToWaiting(m_WaitTime);
    }
    virtual ~PeriodicJob(); // 0x0035B67C slot 0x00
    // 0x0035B650 slot 0x04 (deleting dtor)
    virtual void SetDefaultPostExecutionState(); // 0x003CC720 slot 0x1C | fefates:bytes
    virtual bool SkipWaitDelayAtTermination(); // 0x0035B63C slot 0x20 | fefates:bytes

    s32 m_Period; // 0x3C (in the tail padding of Job)
};
ASSERT_OFFSET(PeriodicJob, m_Period, 0x3C);
ASSERT_SIZE(PeriodicJob, 0x40);
} // namespace nex
} // namespace nn
