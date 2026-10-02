#pragma once

#include "decomp.h"
#include "nn/os/os_ITaskInvoker.h"

namespace nn {
namespace os {
class WaitObject;

// A task that runs once its wait object is signalled (names are ours).
class QueueableWaitTask : public QueueableTask
{
public:
    virtual WaitObject* GetWaitObject() = 0; // slot 0x0C
};

// RTTI N2nn2os16IWaitTaskInvokerE @ 0x008CDE10
class IWaitTaskInvoker : public ::nn::os::ITaskInvoker
{
public:
    IWaitTaskInvoker() {}
    virtual void AddWaitTask(QueueableWaitTask* task) = 0; // slot 0x0C (name is ours)
};
} // namespace os
} // namespace nn
