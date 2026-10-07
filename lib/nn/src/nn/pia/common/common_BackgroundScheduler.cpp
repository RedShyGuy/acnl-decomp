#include "nn/pia/common/common_BackgroundScheduler.h"
#include "nn/pia/common/common_CriticalSection.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Time.h"

// 0x008100C8
template void pead::Delegate2<nn::pia::common::BackgroundScheduler, pead::Thread*, int>::invoke(pead::Thread*, int);

namespace nn {
namespace pia {
namespace common {
namespace {
// the arguments of the thread (block type, quit message, stack size, message queue size)
const int THREAD_BLOCK_TYPE = 1;
const int THREAD_QUIT_MESSAGE = 0x7FFFFFFF;
const int THREAD_STACK_SIZE = 0x1000;
const int THREAD_MESSAGE_QUEUE_SIZE = 32;
} // namespace

// 0x00428128 | fefates:bytes [tier B]
void nn::pia::common::BackgroundScheduler::Dispatch(pead::Thread* thread, int)
{
    m_pCriticalSection->Lock();
    for (;;) {
        if (m_IsFinalizing) {
            thread->quit(false);
            m_pCriticalSection->Unlock();
            return;
        }
        if (m_JobList.GetCount() == 0) {
            m_pCriticalSection->Unlock();
            m_Event.Wait();
            m_pCriticalSection->Lock();
            continue;
        }
        Job* job = m_JobList.Back();
        Time now;
        now.SetNow();
        TimeSpan wait = job->m_ExecuteTime - now;
        if (wait.m_Tick > 0) {
            m_pCriticalSection->Unlock();
            m_Event.Wait(wait.m_Tick);
            m_pCriticalSection->Lock();
            continue;
        }
        m_JobList.PopBackNode();
        job->Execute(true);
        m_Event.Clear();
    }
}

// 0x00429618 (name is ours)
void nn::pia::common::BackgroundScheduler::EntryJob(nn::pia::common::Job* job)
{
    for (Job* pos = m_JobList.Begin(); pos != m_JobList.End(); pos = m_JobList.Advance(pos)) {
        if (!(job->m_ExecuteTime < pos->m_ExecuteTime)) {
            m_JobList.InsertBefore(pos, job);
            return;
        }
    }
    // the earliest job: wake the thread
    m_Event.Signal();
    m_JobList.PushBack(job);
}

// 0x00428250 | fefates:bytes [tier B]
void nn::pia::common::BackgroundScheduler::ResetJob(nn::pia::common::Job* job)
{
    if (m_JobList.IsInclude(job)) {
        m_JobList.Erase(job);
    }
}

// 0x0042828C | fefates:bytes [tier B]
nn::pia::common::BackgroundScheduler::BackgroundScheduler(int priority, nn::pia::common::CriticalSection* pCriticalSection)
    : m_Delegate(this, &BackgroundScheduler::Dispatch),
      m_Thread(pead::SafeStringBase<char>("Pia BackgroundScheduler"), &m_Delegate, HeapManager::GetHeap(), priority,
               THREAD_BLOCK_TYPE, THREAD_QUIT_MESSAGE, THREAD_STACK_SIZE, THREAD_MESSAGE_QUEUE_SIZE),
      m_pCriticalSection(pCriticalSection),
      m_Event(false),
      m_IsFinalizing(false)
{
    m_Thread.start();
    m_JobList.SetOffset(offsetof(Job, m_ListNode));
}

// 0x00428378 | fefates:bytes [tier B]
nn::pia::common::BackgroundScheduler::~BackgroundScheduler()
{
    m_IsFinalizing = true;
    m_Event.Signal();
    m_Thread.waitDone();
}

} // namespace common
} // namespace pia
} // namespace nn
