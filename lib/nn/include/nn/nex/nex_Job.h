#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_qChain.h"

namespace nn {
namespace nex {
class RootObject;

// The name of a job for the debug output (empty in this build; the class name is from the binary).
class DebugString
{
public:
    DebugString() {}
    explicit DebugString(const String& string) {}
};

// A unit of work of the Scheduler. The names of the states, of the members and of the functions
// without a reference name are ours.
// RTTI N2nn3nex3JobE @ 0x008CF458
// vtable 0x008FF85C (vptr 0x008FF864), offset_to_top 0, 12 entries
class Job : public ::nn::nex::RefCountedObject
{
public:
    enum JobType : u8
    {
        JOB_TYPE_DEFAULT = 0
    };
    enum State : u8
    {
        STATE_INITIAL = 0,
        // in the waiting jobs of the Scheduler until m_WaitTime passed
        STATE_WAITING = 1,
        // in the suspended jobs until TestSuspendedJobState changes the state
        STATE_SUSPENDED = 2,
        // in the ready jobs
        STATE_READY = 3,
        STATE_RUNNING = 4,
        STATE_COMPLETED = 5
    };

    Job(); // TODO: default ctor added so derived stubs compile - does not exist
    Job(nn::nex::Job::JobType type, const nn::nex::DebugString& debugString); // 0x003CC778 | fefates:callgraph [tier C]
    virtual ~Job(); // 0x003CC80C slot 0x00 | fefates:callgraph
    // 0x003CC7E0 slot 0x04 (deleting dtor)
    virtual void DecoratedExecute(); // 0x003CC644 slot 0x08 (slot name from the reference symbols)
    virtual void Execute() = 0; // slot 0x0C
    virtual void TestSuspendedJobState(); // 0x003CC704 slot 0x10 (slot name from the reference symbols)
    virtual void AddActivity(const wchar_t* pActivity); // 0x003CC588 slot 0x14 (slot name from the reference symbols)
    // the description of the job for the trace (empty here; inline in the original, with a copy out of
    // line)
    virtual nn::nex::String GetTraceString() const; // 0x0072DF28 slot 0x18
    virtual void SetDefaultPostExecutionState(); // 0x003CC710 slot 0x1C (slot name from the reference symbols)
    virtual bool SkipWaitDelayAtTermination(); // 0x003CC708 slot 0x20 (slot name from the reference symbols)
    virtual void CancelJob(); // 0x003CC774 slot 0x24 (slot name from the reference symbols)
    virtual void vf_0x28(); // 0x003CC700 slot 0x28
    virtual void vf_0x2C(); // 0x003CC638 slot 0x2C

    void SetToWaiting(int waitTime); // 0x003CC594 | fefates:bytes [tier B]
    void SetToComplete(); // 0x003CC630 | mk7dlp:callseq-callee [tier A]
    void SetToSuspended(); // 0x003CC63C | fefates:callgraph [tier C]
    DECOMP_NOINLINE void SetState(nn::nex::Job::State state); // 0x003CC750 | fefates:bytes [tier B]
    // waits (with the scheduler) until the job is complete
    bool WaitForCompletion(unsigned int timeout); // 0x003CC650 | fefates:callgraph [tier C]
    // WaitForCompletion for the scheduler (armlink placed it far from the others)
    static bool WaitForCompletionCallback(nn::nex::RootObject* pObject, unsigned int timeout); // 0x0017F290 (name is ours)

    Job* m_pChainNext;                                    // 0x0C
    Job* m_pChainPrev;                                    // 0x10
    u8 m_Unknown0x14;                                     // 0x14
    void (*m_pCompletionCallback)(Job* pJob, u32* pContext); // 0x18
    u32 m_CompletionContext;                              // 0x1C
    JobType m_Type;                                       // 0x20
    DebugString m_DebugString;                            // 0x21
    State m_State;                                        // 0x22
    s32 m_WaitTime;                                       // 0x24 (ms)
    u32 m_ExecutionCount;                                 // 0x28
    Time m_DispatchDeadline;                              // 0x30 (of the dispatch that ran it last; 0: none)
    bool m_UsesGlobalLock;                                // 0x38 (it runs with the lock of the InstanceTable)
    u8 m_Unknown0x39;                                     // 0x39
};
ASSERT_OFFSET(Job, m_DispatchDeadline, 0x30);

// (the original calls erase of the job chains out of line; defined in nex_Scheduler.cpp)
template <>
DECOMP_NOINLINE qChain<Job*, DefaultChainPolicy<Job*> >::iterator qChain<Job*, DefaultChainPolicy<Job*> >::erase(iterator it);
} // namespace nex
} // namespace nn
