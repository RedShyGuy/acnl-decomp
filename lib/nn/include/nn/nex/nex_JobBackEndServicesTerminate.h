#pragma once

#include "decomp.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
class BackEndServices;

// The termination of BackEndServices: the login is cancelled, then the logout. The names of the
// members are ours.
// RTTI N2nn3nex27JobBackEndServicesTerminateE @ 0x008CF03C
// vtable 0x008FEF6C (vptr 0x008FEF74), offset_to_top 0, 13 entries
class JobBackEndServicesTerminate : public ::nn::nex::StepSequenceJob
{
public:
    // (inline in BackEndServices::TerminateImpl)
    JobBackEndServicesTerminate(BackEndServices* pBackEndServices, unsigned int callContextId)
        : StepSequenceJob(static_cast<JobType>(2), DebugString()), m_pBackEndServices(pBackEndServices), m_CallContextId(callContextId)
    {
        m_TraceFlags = 4096;
        SetStep(Step(&JobBackEndServicesTerminate::StepWaitOnPendingJobs, ""));
    }
    virtual ~JobBackEndServicesTerminate(); // 0x003BD298 slot 0x00
    // 0x003BD274 (deleting dtor)
    void StepLogoutComplete(); // 0x003BCFEC | fefates:bytes [tier B]
    void StepWaitOnLoginCancel(); // 0x003BD060 | fefates:bytes [tier B]
    void StepWaitOnPendingJobs(); // 0x003BD0CC | fefates:bytes [tier B]
    void StepInitLogoutIfRequired(); // 0x003BD190 | fefates:bytes [tier B]

    BackEndServices* m_pBackEndServices; // 0x68
    u32 m_CallContextId;                 // 0x6C
    CallContext m_CallContext;           // 0x70 (of the logout)
};
ASSERT_SIZE(JobBackEndServicesTerminate, 0xD0);
} // namespace nex
} // namespace nn
