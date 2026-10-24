#pragma once

#include "decomp.h"
#include "nn/os/os_LightEvent.h"
#include "nw/ut/internal/ut_LinkListImpl.h"

namespace nw {
namespace snd {
namespace internal {
// A job of the task thread (loading sound data; the class name and Execute are from the reference
// symbols, the members are ours). The event is signaled when the task is done or removed.
class Task
{
public:
    // (names are ours)
    enum Status : u8
    {
        STATUS_FREE = 0,
        STATUS_APPEND = 1,
        STATUS_EXECUTE = 2,
        STATUS_DONE = 3,
        STATUS_CANCEL = 4,
    };

    Task(); // 0x004C9B60 | fefates:bytes [tier B]
    virtual ~Task(); // 0x004C9BBC slot 0x00 | fefates:callgraph

    virtual void Execute() = 0; // slot 0x08

    ut::LinkListNode m_Link;  // 0x04 (TaskManager)
    nn::os::LightEvent m_Event; // 0x0C
    Status m_Status;          // 0x14
    u32 m_Id;                 // 0x18 (TaskManager::RemoveTaskById)
};
ASSERT_SIZE(Task, 0x1C);
} // namespace internal
} // namespace snd
} // namespace nw
