#pragma once

#include "decomp.h"
#include "nn/os/detail/os_BlockingQueueBase.h"
#include "nn/os/os_CriticalSection.h"
#include "nw/snd/internal/snd_Task.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace snd {
namespace internal {
// The tasks of the task thread in three priorities (the class and function names are from the reference
// symbols, the members are ours). A message in the queue wakes the waiting thread.
class TaskManager
{
public:
    // (the values from the code; names are ours)
    enum TaskPriority
    {
        PRIORITY_LOW = 0,
        PRIORITY_MIDDLE = 1,
        PRIORITY_HIGH = 2,
        PRIORITY_COUNT = 3,
    };
    static const int QUEUE_SIZE = 32;

    typedef ut::LinkList<Task, 4> TaskList;

    // (inline)
    TaskManager() : m_pCurrentTask(nullptr), m_IsCancelWaitTask(false) {}
    ~TaskManager(); // 0x004C6BB0 | fefates:bytes [tier B]

    static TaskManager& GetInstance(); // 0x004C698C | nintendogs:callgraph [tier A]

    void Initialize(); // 0x0013D9C0 | nintendogs:bytes [tier A]
    void Finalize(); // 0x00143B6C | mk7dlp:callseq-callee [tier A]
    void AppendTask(Task* pTask, TaskPriority priority); // 0x004C688C | nintendogs:bytes [tier A]
    // the first task of a priority (NULL: none), taken out if isRemove
    DECOMP_NOINLINE Task* GetNextTask(TaskPriority priority, bool isRemove); // 0x004C6A3C | nintendogs:bytes [tier A]
    // the tasks with the id are removed (canceled)
    void RemoveTaskById(u32 id); // 0x004C6AA0 | nintendogs:bytes [tier A]
    // runs the next task, the highest priority first
    void ExecuteTask(); // 0x004C68E4 | nintendogs:callgraph [tier A]
    // until there is a task or CancelWaitTask
    void WaitTask(); // 0x004C6B2C | nintendogs:bytes [tier A]
    void CancelWaitTask(); // 0x001429BC | nintendogs:bytes [tier A]

    TaskList m_TaskLists[PRIORITY_COUNT];                     // 0x00
    Task* m_pCurrentTask;                                    // 0x24
    bool m_IsCancelWaitTask;                                 // 0x28
    nn::os::CriticalSection m_CriticalSection;               // 0x2C
    nn::os::detail::BlockingQueueBase<nn::os::CriticalSection> m_Queue; // 0x38
    uptr m_QueueBuffer[QUEUE_SIZE];                          // 0x6C
};
ASSERT_SIZE(TaskManager, 0xEC);
} // namespace internal
} // namespace snd
} // namespace nw
