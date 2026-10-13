#pragma once

#include "decomp.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_WorkerThreads.h"

// The worker thread of a Scheduler: it dispatches the jobs and sleeps in between. The names of the
// members and of the functions are ours.
// RTTI N2nn3nex9Scheduler21SchedulerWorkerThreadE @ 0x008CF78C
// vtable 0x008FFD8C (vptr 0x008FFD94), offset_to_top 0, 5 entries
class nn::nex::Scheduler::SchedulerWorkerThread : public ::nn::nex::WorkerThreads
{
public:
    // (inline)
    SchedulerWorkerThread() : m_pScheduler(nullptr) {}
    virtual ~SchedulerWorkerThread(); // 0x0036FA3C slot 0x00
    // 0x003D879C slot 0x04 (deleting dtor)
    // the thread uses the context of the scheduler
    virtual void OnThreadStart(); // 0x003D8734 slot 0x08
    virtual void DoWork(); // 0x003D8758 slot 0x10

    nn::nex::Scheduler* m_pScheduler; // 0x3C

    // the sleep between two dispatches (ms; 0: yield)
    static u32 s_SleepTime;
};
ASSERT_SIZE(nn::nex::Scheduler::SchedulerWorkerThread, 0x40);
