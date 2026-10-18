#pragma once

#include "decomp.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
class NgsFacade;

// The pre-terminate job of NgsFacade: it waits for the login job, then the termination goes on.
// The names of the members are ours.
// RTTI N2nn3nex12JobTerminateE @ 0x008CE0E0
// vtable 0x008FC530 (vptr 0x008FC538), offset_to_top 0, 13 entries
class JobTerminate : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    // (inline in NgsFacade::StartTerminate)
    JobTerminate(NgsFacade* pFacade, unsigned int callContextId) : StepSequenceJob(static_cast<JobType>(2), DebugString()), m_pFacade(pFacade), m_CallContextId(callContextId)
    {
        m_TraceFlags = 4096;
        SetStep(Step(&JobTerminate::StepWaitOnPendingJobs, ""));
    }
    virtual ~JobTerminate(); // 0x0035D4F8 slot 0x00
    // 0x0035D4D4 (deleting dtor)
    void StepWaitOnPendingJobs(); // 0x0035D428 | fefates:bytes [tier B]

    NgsFacade* m_pFacade;      // 0x68
    u32 m_CallContextId;       // 0x6C
    CallContext m_CallContext; // 0x70
};
ASSERT_SIZE(JobTerminate, 0xD0);
} // namespace nex
} // namespace nn
