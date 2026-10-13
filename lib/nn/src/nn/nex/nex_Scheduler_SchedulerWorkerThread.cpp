#include "nn/nex/nex_Scheduler_SchedulerWorkerThread.h"
#include "nn/nex/nex_PseudoSingleton.h"
#include "nn/os/os_Api.h"

namespace nn {
namespace nex {
namespace {
// the time between two dispatches of the worker thread (ms)
const u32 DISPATCH_TIMEOUT = 100;
const s64 NS_PER_MS = 1000000;
} // namespace

// 0x0096C98C (name is ours)
u32 Scheduler::SchedulerWorkerThread::s_SleepTime = 8;

// 0x0036FA3C
// 0x003D879C (deleting dtor)
nn::nex::Scheduler::SchedulerWorkerThread::~SchedulerWorkerThread()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003D8734
void nn::nex::Scheduler::SchedulerWorkerThread::OnThreadStart()
{
    u32 context = m_pScheduler->m_Context;
    if (PseudoSingleton::s_IsContextUsed) {
        PseudoSingleton::SetContext(context);
    }
}

// 0x003D8758
void nn::nex::Scheduler::SchedulerWorkerThread::DoWork()
{
    m_pScheduler->Dispatch(DISPATCH_TIMEOUT, 0);
    if (s_SleepTime != 0) {
        OsSleepThread(static_cast<u64>(s_SleepTime) * NS_PER_MS);
    } else {
        OsYieldThread();
    }
}

} // namespace nex
} // namespace nn
