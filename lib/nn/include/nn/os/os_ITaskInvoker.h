#pragma once

#include "decomp.h"

namespace nn {
namespace os {
namespace detail {
// link of a task in a TaskQueue (name is ours)
struct TaskQueueItem {
    TaskQueueItem* mNext;
};
} // namespace detail

// A job for an ITaskInvoker. The invoker keeps it in a queue through the TaskQueueItem base,
// which lies behind the vtable pointer (+0x4). Names are ours.
class QueueableTask : public detail::TaskQueueItem
{
public:
    virtual void Invoke() = 0;      // slot 0x00, runs on a worker thread
    virtual ~QueueableTask() {}     // slots 0x04 / 0x08
};

// RTTI N2nn2os12ITaskInvokerE @ 0x008CDDF4
// Something that runs QueueableTasks (slot order from the vtable of ThreadPool).
class ITaskInvoker
{
public:
    ITaskInvoker() {}
    virtual void AddTask(QueueableTask* task) = 0; // slot 0x00 (name is ours)
    virtual ~ITaskInvoker() {}                     // slots 0x04 / 0x08
};
} // namespace os
} // namespace nn
