#include "nw/snd/internal/snd_TaskThread.h"

#include "nw/snd/internal/snd_DriverCommandManager.h"
#include "nw/snd/internal/snd_TaskManager.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004C5EE4 | nintendogs:bytes [tier A]
void nw::snd::internal::TaskThread::ThreadFunc(uptr arg)
{
    TaskThread* pThread = reinterpret_cast<TaskThread*>(arg);
    while (!pThread->m_IsFinalize) {
        TaskManager::GetInstance().WaitTask();
        if (pThread->m_IsFinalize) {
            break;
        }
        {
            nn::os::CriticalSection::ScopedLock lock(pThread->m_CriticalSection);
            TaskManager::GetInstance().ExecuteTask();
        }
        DriverCommandManager::GetInstanceForTaskThread().RecvCommandReply();
    }
}

// 0x004C5F70 | mk7dlp:callseq-callee [tier A]
nw::snd::internal::TaskThread& nw::snd::internal::TaskThread::GetInstance()
{
    // 0x00AF4DEC
    static TaskThread s_TaskThread;
    return s_TaskThread;
}

// 0x004C5FEC | fefates:bytes [tier B]
bool nw::snd::internal::TaskThread::Create(int priority, ThreadStack& stack)
{
    if (m_IsCreated) {
        Destroy();
    }
    m_IsFinalize = false;
    m_CriticalSection.Initialize();
    const nn::Result result = TryStart(ThreadFunc, reinterpret_cast<uptr>(this), stack.GetStackBottom(), priority);
    if (result.IsSuccess()) {
        m_IsCreated = true;
    }
    return result.IsSuccess();
}

// 0x004C60B8 | mk7dlp:callseq-callee [tier A]
void nw::snd::internal::TaskThread::Destroy()
{
    if (!m_IsCreated) {
        return;
    }
    m_IsFinalize = true;
    TaskManager::GetInstance().CancelWaitTask();
    Join();
    Finalize();
    m_CriticalSection.Finalize();
    m_IsCreated = false;
}

// 0x004C6128 | fefates:bytes [tier B]
nw::snd::internal::TaskThread::~TaskThread()
{
    if (m_IsCreated) {
        Destroy();
    }
}

} // namespace internal
} // namespace snd
} // namespace nw
