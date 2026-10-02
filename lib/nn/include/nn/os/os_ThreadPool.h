#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_EventBase.h"
#include "nn/os/os_IWaitTaskInvoker.h"
#include "nn/os/os_LightEvent.h"
#include "nn/os/os_Thread.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace nn {
namespace os {
namespace detail {

// Singly linked FIFO of tasks (name is ours). The tail's link of a queue that was empty points
// to the tail itself; Dequeue clears the link of the task it returns.
class TaskQueue
{
public:
    TaskQueue() : mHead(0), mTail(0) {}

    bool IsEmpty() const { return mHead == 0; }

    void Enqueue(QueueableTask* task)
    {
        TaskQueueItem* item = task;
        if (mHead == 0) {
            mHead = item;
            mTail = item;
        }
        mTail->mNext = item;
        mTail = item;
    }

    QueueableTask* Dequeue()
    {
        TaskQueueItem* item = mHead;
        if (item == 0) {
            return 0;
        }
        mHead = (item == mTail) ? 0 : item->mNext;
        item->mNext = 0;
        return static_cast<QueueableTask*>(item);
    }

private:
    TaskQueueItem* mHead;
    TaskQueueItem* mTail;
};

} // namespace detail

// RTTI N2nn2os10ThreadPoolE @ 0x008CDDB0
// vtable 0x008FBFD8 (vptr 0x008FBFE0), offset_to_top 0, 4 entries
//
// Runs tasks on a set of worker threads. Wait tasks are first given to an internal wait thread,
// which waits for up to maxWaitTasks of them at once (WaitSynchronizationN) and moves each one
// whose wait object is signalled to the execute queue. Member names are ours.
class ThreadPool : public ::nn::os::IWaitTaskInvoker, public ::nn::util::ADLFireWall::NonCopyable<nn::os::ThreadPool>
{
public:
    // inline (detail::StartAlarmThreadPool); the arguments are those of Setup
    ThreadPool(uptr workBuffer, size_t maxWaitTasks, size_t numThreads, const uptr workerStackBottoms[], s32 workerPriority, s32 waitThreadPriority)
        : mThreads(0)
    {
        Setup(workBuffer, maxWaitTasks, numThreads, workerStackBottoms, workerPriority, waitThreadPriority);
    }
    virtual void AddTask(QueueableTask* task); // 0x0034BD68 slot 0x00 (the code is at 0x00129638)
    virtual ~ThreadPool(); // 0x0034BEA0 slot 0x04
    // 0x0034BE3C slot 0x08 (deleting dtor)
    virtual void AddWaitTask(QueueableWaitTask* task); // 0x0034B9F0 slot 0x0C

    // workBuffer: GetWorkBufferSize bytes; workerStackBottoms: one stack per worker thread
    void Setup(uptr workBuffer, size_t maxWaitTasks, size_t numThreads, const uptr workerStackBottoms[], s32 workerPriority, s32 waitThreadPriority); // 0x0034B84C (name is ours)
    void Finalize(); // 0x0034BD6C

    // the work buffer: the worker threads, then 1 + maxWaitTasks handles, then as many task pointers
    static size_t GetWorkBufferSize(size_t maxWaitTasks, size_t numThreads)
    {
        return numThreads * sizeof(Thread) + (maxWaitTasks + 1) * (sizeof(nn::Handle) + sizeof(QueueableWaitTask*));
    }

private:
    static void WaitThreadFunc(ThreadPool* pool); // 0x0034BA7C
    static void WorkerThreadFunc(ThreadPool* pool); // 0x0034BCA0

    // [0]: mWaitEvent, [1 + i]: handle of the wait object of GetWaitTasks()[i]
    nn::Handle* GetWaitHandles() { return reinterpret_cast<nn::Handle*>(mThreads + mNumThreads); }
    QueueableWaitTask** GetWaitTasks()
    {
        return reinterpret_cast<QueueableWaitTask**>(GetWaitHandles() + mMaxWaitTasks + 1);
    }

    static const size_t WAIT_THREAD_STACK_SIZE = 392;

    size_t mMaxWaitTasks;                           // 0x004
    size_t mNumThreads;                             // 0x008
    Thread* mThreads;                               // 0x00C, the start of the work buffer
    size_t mNumWaitTasks;                           // 0x010, tasks the wait thread waits for
    bool mIsFinalizing;                             // 0x014
    // 0x018; a stack is 8 byte aligned, so is the class: sizeof is 0x1E0 (StartAlarmThreadPool)
    u8 mWaitThreadStack[WAIT_THREAD_STACK_SIZE] DECOMP_ALIGN(8);
    Thread mWaitThread;                             // 0x1A0
    detail::TaskQueue mWaitQueue;                   // 0x1A8, added, not yet waited for
    CriticalSection mWaitQueueLock;                 // 0x1B0, also guards the wait arrays
    EventBase mWaitEvent;                           // 0x1BC, wakes the wait thread
    detail::TaskQueue mExecuteQueue;                // 0x1C0, ready to run
    CriticalSection mExecuteQueueLock;              // 0x1C8
    LightEvent mExecuteEvent;                       // 0x1D4, wakes a worker

    // layout checks (inside the class because the members are private; generates no code)
    static void CheckLayout()
    {
        ASSERT_OFFSET(ThreadPool, mMaxWaitTasks, 0x4);
        ASSERT_OFFSET(ThreadPool, mIsFinalizing, 0x14);
        ASSERT_OFFSET(ThreadPool, mWaitThread, 0x1A0);
        ASSERT_OFFSET(ThreadPool, mWaitQueue, 0x1A8);
        ASSERT_OFFSET(ThreadPool, mWaitQueueLock, 0x1B0);
        ASSERT_OFFSET(ThreadPool, mWaitEvent, 0x1BC);
        ASSERT_OFFSET(ThreadPool, mExecuteQueue, 0x1C0);
        ASSERT_OFFSET(ThreadPool, mExecuteQueueLock, 0x1C8);
        ASSERT_OFFSET(ThreadPool, mExecuteEvent, 0x1D4);
        ASSERT_SIZE(ThreadPool, 0x1E0);
    }
};

} // namespace os
} // namespace nn
