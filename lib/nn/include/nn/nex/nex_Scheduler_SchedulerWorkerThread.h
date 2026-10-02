#pragma once

#include "decomp.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_WorkerThreads.h"

// RTTI N2nn3nex9Scheduler21SchedulerWorkerThreadE @ 0x008CF78C
// vtable 0x008FFD8C (vptr 0x008FFD94), offset_to_top 0, 5 entries
class nn::nex::Scheduler::SchedulerWorkerThread : public ::nn::nex::WorkerThreads
{
public:
    SchedulerWorkerThread(); // ctor candidate(s) 0x003D95FC (unverified)
    virtual ~SchedulerWorkerThread(); // 0x0036FA3C slot 0x00 | slot vf_0x00 of nn::nex::WorkerThreads
    // 0x003D879C slot 0x04 | slot vf_0x04 of nn::nex::WorkerThreads (deleting dtor)
    virtual void vf_0x08(); // 0x003D8734 slot 0x08 | virtual slot, introduced by nn::nex::WorkerThreads
    virtual void vf_0x10(); // 0x003D8758 slot 0x10 | virtual slot, introduced by nn::nex::WorkerThreads
};
