#pragma once

#include "decomp.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_Thread.h"
#include "nw/snd/internal/snd_ThreadStack.h"

namespace nw {
namespace snd {
namespace internal {
// The thread that runs the tasks of the TaskManager (the class and function names are from the reference
// symbols, the members are ours).
class TaskThread : public nn::os::Thread
{
public:
    // (inline)
    TaskThread() : m_IsFinalize(false), m_IsCreated(false) {}
    ~TaskThread(); // 0x004C6128 | fefates:bytes [tier B]

    static TaskThread& GetInstance(); // 0x004C5F70 | mk7dlp:callseq-callee [tier A]

    // starts the thread on the stack; false on failure
    bool Create(int priority, ThreadStack& stack); // 0x004C5FEC | fefates:bytes [tier B]
    void Destroy(); // 0x004C60B8 | mk7dlp:callseq-callee [tier A]

    static void ThreadFunc(uptr arg); // 0x004C5EE4 | nintendogs:bytes [tier A]

    nn::os::CriticalSection m_CriticalSection; // 0x08 held while a task runs
    bool m_IsFinalize;                         // 0x14
    bool m_IsCreated;                          // 0x15
};
ASSERT_SIZE(TaskThread, 0x18);
} // namespace internal
} // namespace snd
} // namespace nw
