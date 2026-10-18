#pragma once

#include "decomp.h"
#include "nn/nex/nex_ForcedCriticalSection.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_ObjectThread.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
// Resolves the names of the root transport (a member function of it) on a thread of its own
// ("DNS Check Thread"; one at a time). The names of the members and of the steps are ours.
// RTTI N2nn3nex14JobNameResolveE @ 0x008CE31C
// vtable 0x008FCB74 (vptr 0x008FCB7C), offset_to_top 0, 13 entries
class JobNameResolve : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    // the resolution: a member function of the root transport (true on success)
    typedef bool (RootTransport::*ResolveMethod)();

    JobNameResolve(unsigned int threadPriority, int coreNo, ResolveMethod method); // 0x003715FC
    virtual ~JobNameResolve(); // 0x00371710 slot 0x00
    // 0x003716E0 slot 0x04 (deleting dtor)

    // the job starts for the call
    void Initialize(unsigned int callContextId); // 0x0037105C
    // the function of the thread
    void Resolve(int unused); // 0x003710E4
    void CompleteJob(const nn::nex::qResult& result); // 0x0037117C | fefates:bytes [tier B]
    void Start(); // 0x003711E0
    // the call was cancelled: waits for the end of the thread
    void WaitForThread(); // 0x0037143C
    void WaitForResolution(); // 0x003714CC

    u32 m_CallContextId;                           // 0x68
    qResult m_Result;                              // 0x6C (FAILURE while the thread runs)
    ObjectThread<JobNameResolve, int>* m_pThread;  // 0x78
    ForcedCriticalSection m_Lock;                  // 0x7C (of m_Result)
    u32 m_ThreadPriority;                          // 0xA4
    s32 m_CoreNo;                                  // 0xA8
    ResolveMethod m_Method;                        // 0xAC
};
ASSERT_OFFSET(JobNameResolve, m_Lock, 0x7C);
ASSERT_OFFSET(JobNameResolve, m_Method, 0xAC);
ASSERT_SIZE(JobNameResolve, 0xB8);

// the priority and the core of the thread of the name resolution of the NAT jobs
extern u32 g_NameResolutionThreadPriority;
extern s32 g_NameResolutionCoreNo;
} // namespace nex
} // namespace nn
