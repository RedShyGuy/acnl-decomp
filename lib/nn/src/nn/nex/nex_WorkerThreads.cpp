#include "nn/nex/nex_WorkerThreads.h"

namespace nn {
namespace nex {
// 0x0036F478
void nn::nex::WorkerThreads::OnThreadStart()
{
    // nothing to do in this build
}

// 0x0036F47C
void nn::nex::WorkerThreads::ThreadProc(int unused)
{
    OnThreadStart();
    while (m_State == STATE_RUNNING) {
        DoWork();
    }
    OnThreadEnd();
}

// 0x0036F4D0
bool nn::nex::WorkerThreads::Stop()
{
    {
        ScopedCS lock(m_Lock);
        if (m_State != STATE_RUNNING) {
            return false;
        }
        m_State = STATE_STOPPING;
    }
    for (u32 i = 0; i < m_Threads.size(); i++) {
        m_Threads[i]->Wait(static_cast<unsigned int>(-1));
        delete m_Threads[i];
    }
    m_Threads.clear();
    {
        ScopedCS lock(m_Lock);
        m_State = STATE_STOPPED;
    }
    return true;
}

// 0x0036F618
bool nn::nex::WorkerThreads::Launch(unsigned int count)
{
    ScopedCS lock(m_Lock);
    if (m_State != STATE_STOPPED) {
        return false;
    }
    m_State = STATE_RUNNING;
    for (u32 i = 0; i < count; i++) {
        String name(L"");
        name.Format(L"WorkerThread ID %d", i + 1);
        ObjectThread<WorkerThreads, int>* pThread = new ObjectThread<WorkerThreads, int>(name);
        pThread->SetMethod(this, &WorkerThreads::ThreadProc, 0);
        pThread->Launch(0);
        m_Threads.push_back(pThread);
    }
    return true;
}

// 0x0036FA08
void nn::nex::WorkerThreads::OnThreadEnd()
{
    // nothing to do in this build
}

// 0x0036FA40
// 0x0036FA0C (deleting dtor)
nn::nex::WorkerThreads::~WorkerThreads()
{
    Stop();
}

// 0x00382DC0
// 0x007D4490 (deleting dtor)
template nn::nex::ObjectThread<nn::nex::WorkerThreads, int>::~ObjectThread();

// 0x007D445C
template void nn::nex::ObjectThread<nn::nex::WorkerThreads, int>::Run();

} // namespace nex
} // namespace nn
