#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_ObjectThread.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
// Threads that call DoWork until they are stopped. The names of the members, of the states and of
// the functions are ours.
// RTTI N2nn3nex13WorkerThreadsE @ 0x008CE2C0
// vtable 0x008FCA20 (vptr 0x008FCA28), offset_to_top 0, 5 entries
class WorkerThreads : public ::nn::nex::RootObject
{
public:
    enum State : u8
    {
        STATE_RUNNING = 0,
        STATE_STOPPING = 1,
        STATE_STOPPED = 2
    };

    // (inline)
    WorkerThreads() : m_Lock(0x20000000), m_State(STATE_STOPPED) {}
    virtual ~WorkerThreads(); // 0x0036FA40 slot 0x00
    // 0x0036FA0C slot 0x04 (deleting dtor)
    // called on each thread before and after its work
    virtual void OnThreadStart(); // 0x0036F478 slot 0x08
    virtual void OnThreadEnd(); // 0x0036FA08 slot 0x0C
    // a piece of the work (called again and again)
    virtual void DoWork() = 0; // slot 0x10

    // the function of the threads
    void ThreadProc(int unused); // 0x0036F47C
    // stops and deletes the threads (false if they do not run)
    bool Stop(); // 0x0036F4D0
    // starts count threads (false if they run)
    bool Launch(unsigned int count); // 0x0036F618

    CriticalSection m_Lock;                                  // 0x04
    State m_State;                                           // 0x2C
    qVector<ObjectThread<WorkerThreads, int>*> m_Threads;    // 0x30
};
ASSERT_OFFSET(WorkerThreads, m_Threads, 0x30);
ASSERT_SIZE(WorkerThreads, 0x3C);
} // namespace nex
} // namespace nn
