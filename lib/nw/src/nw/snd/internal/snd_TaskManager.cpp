#include "nw/snd/internal/snd_TaskManager.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0013D9C0 | nintendogs:bytes [tier A]
void nw::snd::internal::TaskManager::Initialize()
{
    m_CriticalSection.Initialize();
    m_Queue.Initialize(m_QueueBuffer, QUEUE_SIZE);
}

// 0x001429BC | nintendogs:bytes [tier A]
void nw::snd::internal::TaskManager::CancelWaitTask()
{
    m_IsCancelWaitTask = true;
    m_Queue.Enqueue(0);
}

// 0x00143B6C | mk7dlp:callseq-callee [tier A]
void nw::snd::internal::TaskManager::Finalize()
{
    m_CriticalSection.Finalize();
    m_Queue.Finalize();
}

// 0x004C688C | nintendogs:bytes [tier A]
void nw::snd::internal::TaskManager::AppendTask(Task* pTask, TaskPriority priority)
{
    nn::os::CriticalSection::ScopedLock lock(m_CriticalSection);
    pTask->m_Event.ClearSignal();
    pTask->m_Status = Task::STATUS_APPEND;
    m_TaskLists[priority].PushBack(pTask);
    m_Queue.TryEnqueue(0);
}

// 0x004C68E4 | nintendogs:callgraph [tier A]
void nw::snd::internal::TaskManager::ExecuteTask()
{
    Task* pTask;
    {
        nn::os::CriticalSection::ScopedLock lock(m_CriticalSection);
        pTask = GetNextTask(PRIORITY_HIGH, true);
        if (pTask == nullptr) {
            pTask = GetNextTask(PRIORITY_MIDDLE, true);
        }
        if (pTask == nullptr) {
            pTask = GetNextTask(PRIORITY_LOW, true);
        }
        if (pTask == nullptr) {
            return;
        }
    }
    m_pCurrentTask = pTask;
    pTask->Execute();
    pTask->m_Status = Task::STATUS_DONE;
    pTask->m_Event.Signal();
    m_pCurrentTask = nullptr;
}

// 0x004C698C | nintendogs:callgraph [tier A]
nw::snd::internal::TaskManager& nw::snd::internal::TaskManager::GetInstance()
{
    // 0x00AF4D00
    static TaskManager s_TaskManager;
    return s_TaskManager;
}

// 0x004C6A3C | nintendogs:bytes [tier A]
nw::snd::internal::Task* nw::snd::internal::TaskManager::GetNextTask(TaskPriority priority, bool isRemove)
{
    nn::os::CriticalSection::ScopedLock lock(m_CriticalSection);
    TaskList& list = m_TaskLists[priority];
    if (list.empty()) {
        return nullptr;
    }
    Task* pTask = TaskList::GetPointerFromNode(list.begin().GetNode());
    if (isRemove) {
        list.erase(list.begin());
    }
    return pTask;
}

// 0x004C6AA0 | nintendogs:bytes [tier A]
void nw::snd::internal::TaskManager::RemoveTaskById(u32 id)
{
    nn::os::CriticalSection::ScopedLock lock(m_CriticalSection);
    for (u32 i = 0; i < PRIORITY_COUNT; ++i) {
        TaskList& list = m_TaskLists[static_cast<u8>(i)];
        for (ut::internal::LinkListImpl::iterator it = list.begin(); it != list.end();) {
            ut::internal::LinkListImpl::iterator current = it;
            Task* pTask = TaskList::GetPointerFromNode(it.GetNode());
            ++it;
            if (pTask->m_Id == id) {
                list.erase(current);
                pTask->m_Status = Task::STATUS_CANCEL;
                pTask->m_Event.Signal();
            }
        }
    }
}

// 0x004C6B2C | nintendogs:bytes [tier A]
void nw::snd::internal::TaskManager::WaitTask()
{
    m_IsCancelWaitTask = false;
    while (GetNextTask(PRIORITY_HIGH, false) == nullptr && GetNextTask(PRIORITY_MIDDLE, false) == nullptr &&
           GetNextTask(PRIORITY_LOW, false) == nullptr && !m_IsCancelWaitTask) {
        if (m_Queue.Dequeue() == 0) {
            return;
        }
    }
}

// 0x004C6BB0 | fefates:bytes [tier B]
nw::snd::internal::TaskManager::~TaskManager()
{
    m_Queue.Finalize();
}

} // namespace internal
} // namespace snd
} // namespace nw
