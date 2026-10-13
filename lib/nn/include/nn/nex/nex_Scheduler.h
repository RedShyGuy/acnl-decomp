#pragma once

#include "decomp.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_EventHandler.h"
#include "nn/nex/nex_Job.h"
#include "nn/nex/nex_ProfilingUnit.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_SingleThreadCallPolicy.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_qChain.h"
#include "nn/nex/nex_qMap.h"
#include "nn/nex/nex_qResult.h"

namespace __rw {
// (the original calls erase(first, last) of the waiting jobs out of line; defined in nex_Scheduler.cpp)
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::Time, std::pair<const nn::nex::Time, nn::nex::Job*>, __select1st<std::pair<const nn::nex::Time, nn::nex::Job*>, nn::nex::Time>,
                          std::less<nn::nex::Time>, nn::nex::MemAllocator<std::pair<const nn::nex::Time, nn::nex::Job*> > >::iterator
__rb_tree<nn::nex::Time, std::pair<const nn::nex::Time, nn::nex::Job*>, __select1st<std::pair<const nn::nex::Time, nn::nex::Job*>, nn::nex::Time>,
          std::less<nn::nex::Time>, nn::nex::MemAllocator<std::pair<const nn::nex::Time, nn::nex::Job*> > >::erase(iterator first, iterator last);
} // namespace __rw

namespace nn {
namespace nex {
// Runs the jobs of a core: the ready jobs one after the other, the waiting jobs when their time
// came, the suspended ones when they are ready again, and the regular jobs on every dispatch.
// Dispatch runs on the worker thread or (without one) on the thread of the application. The names
// of the members and of the functions without a reference name are ours (GetTotalNumberOfJobs is
// from an assert text).
// RTTI N2nn3nex9SchedulerE @ 0x008CF798
// vtable 0x008FFDA8 (vptr 0x008FFDB0), offset_to_top 0, 2 entries
class Scheduler : public ::nn::nex::RootObject
{
public:
    class SchedulerWorkerThread;
    typedef qChain<Job*, DefaultChainPolicy<Job*> > JobChain;

    // flags of Dispatch
    // wait (until the timeout) for jobs to become ready
    static const u32 DISPATCH_WAIT = 1 << 0;
    // only the regular jobs
    static const u32 DISPATCH_REGULAR_ONLY = 1 << 1;

    Scheduler(unsigned int lockLevel, nn::nex::Scheduler::SchedulerWorkerThread* pWorkerThread); // 0x003D95FC | fefates:callseq [tier C]
    virtual ~Scheduler(); // 0x003D987C slot 0x00
    // 0x003D984C slot 0x04 (deleting dtor)

    // runs the job if it is ready and queues it again unless it completed
    void ExecuteJob(nn::nex::Job* pJob, const nn::nex::Time& deadline); // 0x003D7938
    // runs the ready jobs until the deadline (0: none) or until no job is ready
    void ExecuteReadyJobs(const nn::nex::Time& deadline); // 0x003D7AC4
    // runs each regular job once
    void ExecuteRegularJobs(const nn::nex::Time& deadline); // 0x003D7D10
    // one dispatch of the jobs; the number of ready jobs afterwards
    u32 Dispatch(unsigned int timeout, unsigned int flags); // 0x003D7EB0
    // false if the call cannot block; otherwise dispatches the jobs of all contexts until the
    // callback no longer returns errorValue or the timeout passed (true; the result of the callback)
    static bool PreventBlockCall(bool isBlocking, unsigned int timeout, bool errorValue, bool (*pCallback)(nn::nex::RootObject*, unsigned int), nn::nex::RootObject* pObject, bool* pResult); // 0x003D80B0 | fefates:bytes-fuzzy [tier B]
    // moves the waiting jobs whose time came and the suspended jobs that are ready to the ready
    // jobs; true if it moved one
    bool QueueNewlyReadyJobs(); // 0x003D8280 | fefates:callseq [tier C]
    // dispatches the scheduler of the current context on this thread (if it has no worker thread)
    static void DispatchCurrentContext(unsigned int timeout); // 0x003D8530
    // the same as PreventBlockCall with a qResult
    static bool PreventRegularBlockCall(bool isBlocking, unsigned int timeout, bool (*pCallback)(nn::nex::RootObject*, unsigned int), nn::nex::RootObject* pObject, nn::nex::qResult* pResult); // 0x003D87C0 | fefates:bytes-fuzzy [tier B]
    // waits until no jobs are left (isCancelling: the waiting jobs are made ready first)
    void WaitForAllJobsToComplete(unsigned int timeout, bool isCancelling); // 0x003D89FC | fefates:callseq [tier C]
    // dispatches the schedulers of all contexts (or of the current one) on this thread
    static void GlobalSingleThreadDispatch(unsigned int timeout); // 0x003D8CC4 | fefates:callseq [tier C]
    // adds a job that runs on every dispatch
    void RegisterRegularJob(nn::nex::Job* pJob, bool isFirst); // 0x003D8DEC
    // removes a regular job and releases it
    void UnregisterRegularJob(nn::nex::Job* pJob); // 0x003D8EB0
    // runs the regular jobs of all contexts (or of the current one) on this thread
    static void GlobalSingleThreadSpecialDispatch(); // 0x003D8F84 | fefates:callseq [tier C]
    // queues the job by its state (isFirst: in front of the others)
    void Queue(nn::nex::Job* pJob, bool isFirst); // 0x003D9184
    // removes the job; false if it was not queued
    bool Cancel(nn::nex::Job* pJob); // 0x003D91EC | fefates:callgraph [tier C]
    // Dispatch if the scheduler has no worker thread (otherwise 0)
    u32 DispatchIfSingleThreaded(unsigned int timeout, unsigned int flags); // 0x003D93E4
    void QueueImpl(nn::nex::Job* pJob, bool isFirst); // 0x003D9408 | fefates:callseq [tier C]

    // the time until the deadline (-1: none, 0: passed; inline in the original, with a copy out of
    // line)
    DECOMP_NOINLINE u32 GetTimeout(const nn::nex::Time& deadline) const; // 0x0072EA8C
    // true if there are jobs (the ready, waiting and suspended ones and the running one; inline in
    // the original, with a copy out of line)
    bool GetTotalNumberOfJobs(); // 0x0072EAD0
    // (inline) the queued jobs
    u32 GetNumberOfQueuedJobs()
    {
        ScopedCS lock(m_QueueLock);
        return m_ReadyJobs.size() + m_WaitingJobs.size() + m_SuspendedJobs.size();
    }

    // the scheduler of the core of the current context (inline in the original, with a copy out of
    // line)
    static Scheduler* GetInstance() // 0x003D7A2C | fefates:callseq [tier C]
    {
        Core* pCore = Core::GetInstance();
        return pCore != nullptr ? pCore->m_pScheduler : nullptr;
    }

    Job* m_pCurrentJob;                     // 0x004
    bool m_IsJobRunning;                    // 0x008 (cleared when the running job is cancelled)
    SchedulerWorkerThread* m_pWorkerThread; // 0x00C
    EventHandler m_EventHandler;            // 0x010
    Event* m_pWakeupEvent;                  // 0x04C (signalled when a job is queued or cancelled)
    CriticalSection m_Lock;                 // 0x050 (while jobs run)
    CriticalSection m_QueueLock;            // 0x078
    CriticalSection m_RegularLock;          // 0x0A0
    JobChain m_ReadyJobs;                   // 0x0C8
    JobChain m_SuspendedJobs;               // 0x0DC
    std::multimap<Time, Job*> m_WaitingJobs; // 0x0F0
    JobChain m_RegularJobs;                 // 0x10C
    bool m_IsTerminating;                   // 0x120 (the waiting jobs that skip the delay then are ready)
    ProfilingUnit m_ProfilingUnit;          // 0x128
    u64 m_Unknown0x198;                     // 0x198
    u64 m_Unknown0x1A0;                     // 0x1A0
    u32 m_Context;                          // 0x1A8 (of the PseudoSingletons)
    SingleThreadCallPolicy m_CallPolicy;    // 0x1AC (the thread inside Dispatch)
    Time m_LastDispatchTime;                // 0x1C0
    bool m_IsTracing;                       // 0x1C8 (the jobs note an activity when they run)

    // dispatch the schedulers of all contexts
    static bool s_DispatchesAllContexts;
};
ASSERT_OFFSET(Scheduler, m_Lock, 0x50);
ASSERT_OFFSET(Scheduler, m_WaitingJobs, 0xF0);
ASSERT_OFFSET(Scheduler, m_IsTerminating, 0x120);
ASSERT_OFFSET(Scheduler, m_ProfilingUnit, 0x128);
ASSERT_OFFSET(Scheduler, m_CallPolicy, 0x1AC);
ASSERT_SIZE(Scheduler, 0x1D0);
} // namespace nex
} // namespace nn
