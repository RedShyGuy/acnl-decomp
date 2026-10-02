#include "nn/util/ADLFireWall/util_NonCopyable.h"
#include "nn/os/os_IWaitTaskInvoker.h"
#include "nn/os/os_ThreadPool.h"
#include "nn/os/os_WaitObject.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace os {

// 0x0034B84C (name is ours)
void nn::os::ThreadPool::Setup(uptr workBuffer, size_t maxWaitTasks, size_t numThreads, const uptr workerStackBottoms[], s32 workerPriority, s32 waitThreadPriority)
{
    mIsFinalizing = false;
    mThreads = reinterpret_cast<Thread*>(workBuffer);
    mNumWaitTasks = 0;
    mMaxWaitTasks = maxWaitTasks;
    mNumThreads = numThreads;

    mWaitEvent.Initialize(RESET_TYPE_ONESHOT);
    mExecuteEvent.Initialize(false);
    mWaitQueueLock.Initialize();
    mExecuteQueueLock.Initialize();

    for (size_t i = 0; i < mNumThreads; i++) {
        Thread* thread = new (&mThreads[i]) Thread;
        thread->Start(WorkerThreadFunc, this, workerStackBottoms[i], workerPriority);
    }

    GetWaitHandles()[0] = mWaitEvent.GetHandle();
    mWaitThread.Start(WaitThreadFunc, this, reinterpret_cast<uptr>(mWaitThreadStack + WAIT_THREAD_STACK_SIZE),
                      waitThreadPriority);
}

// 0x0034B9F0 slot 0x0C
void nn::os::ThreadPool::AddWaitTask(QueueableWaitTask* task)
{
    mWaitQueueLock.Enter();
    mWaitQueue.Enqueue(task);
    mWaitQueueLock.Exit();
    mWaitEvent.Signal();
}

// 0x0034BA7C
// Takes wait tasks from mWaitQueue while there is room, waits for any of their wait objects or
// for mWaitEvent, and hands a signalled task over to the workers.
void nn::os::ThreadPool::WaitThreadFunc(ThreadPool* pool)
{
    while (!pool->mIsFinalizing) {
        pool->mWaitQueueLock.Enter();
        while (pool->mNumWaitTasks < pool->mMaxWaitTasks && !pool->mWaitQueue.IsEmpty()) {
            QueueableWaitTask* task = static_cast<QueueableWaitTask*>(pool->mWaitQueue.Dequeue());
            pool->GetWaitTasks()[pool->mNumWaitTasks] = task;
            pool->GetWaitHandles()[pool->mNumWaitTasks + 1] = task->GetWaitObject()->GetHandle();
            pool->mNumWaitTasks++;
        }
        pool->mWaitQueueLock.Exit();

        // wait for any of the handles
        s32 count = pool->mNumWaitTasks + 1;
        nn::Handle* handles = pool->GetWaitHandles();
        s32 index;
        nn::Result result;
        if (count == 1) {
            index = 0;
            result = nn::svc::WaitSynchronization1(handles[0], -1);
        } else {
            result = nn::svc::WaitSynchronizationN(&index, handles, count, false, -1);
        }
        if (result.IsFailure()) {
            CTR::detail::HandleInternalError(result);
        }

        if (index >= 1) {
            // a task's wait object: remove the task (the last one takes its place) ...
            pool->mWaitQueueLock.Enter();
            QueueableWaitTask** tasks = pool->GetWaitTasks();
            QueueableWaitTask* task = tasks[index - 1];
            tasks[index - 1] = tasks[pool->mNumWaitTasks - 1];
            pool->GetWaitHandles()[index] = pool->GetWaitHandles()[pool->mNumWaitTasks];
            pool->mNumWaitTasks--;
            pool->mWaitQueueLock.Exit();

            // ... and let it run
            pool->mExecuteQueueLock.Enter();
            pool->mExecuteQueue.Enqueue(task);
            pool->mExecuteQueueLock.Exit();
            pool->mExecuteEvent.Signal();
        }
    }
}

// 0x0034BCA0
void nn::os::ThreadPool::WorkerThreadFunc(ThreadPool* pool)
{
    while (!pool->mIsFinalizing) {
        pool->mExecuteEvent.Wait();
        if (pool->mIsFinalizing) {
            break;
        }
        pool->mExecuteQueueLock.Enter();
        QueueableTask* task = pool->mExecuteQueue.Dequeue();
        if (task) {
            if (!pool->mExecuteQueue.IsEmpty()) {
                // more work: wake the next worker
                pool->mExecuteQueueLock.Exit();
                pool->mExecuteEvent.Signal();
            } else {
                pool->mExecuteQueueLock.Exit();
            }
            task->Invoke();
        } else {
            pool->mExecuteQueueLock.Exit();
        }
    }
    // pass the wake up on to the next worker, so that all of them see mIsFinalizing
    pool->mExecuteEvent.Signal();
}

// 0x0034BD68 slot 0x00 (branches to the code at 0x00129638)
void nn::os::ThreadPool::AddTask(QueueableTask* task)
{
    mExecuteQueueLock.Enter();
    mExecuteQueue.Enqueue(task);
    mExecuteQueueLock.Exit();
    mExecuteEvent.Signal();
}

// 0x0034BD6C
// stops all threads and waits for them
void nn::os::ThreadPool::Finalize()
{
    mIsFinalizing = true;
    mWaitEvent.Signal();
    mExecuteEvent.Signal();
    for (size_t i = 0; i < mNumThreads; i++) {
        mThreads[i].Join();
        mThreads[i].Finalize();
    }
    mWaitThread.Join();
    mWaitThread.Finalize();
}

// 0x0034BEA0 slot 0x04
nn::os::ThreadPool::~ThreadPool()
{
    Finalize();
}

} // namespace os
} // namespace nn
