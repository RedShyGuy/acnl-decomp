// The thread pool that runs the alarms. The name of the original file is unknown; its static
// initializer (0x00786384, no symbol) constructs s_AlarmThreadPoolBlock and registers its
// destructor (0x0034C010) with __aeabi_atexit. All names in this file are ours.

#include "nn/os/detail/detail_Api.h"
#include "nn/os/os_MemoryBlock.h"
#include "nn/os/os_ThreadPool.h"

#include <new>

namespace nn {
namespace os {
namespace detail {
namespace {

const size_t NUM_WORKER_THREADS = 2;
const size_t WORKER_STACK_SIZE = 0x1000;
const size_t MAX_WAIT_TASKS = 100;

// 0x00AE9600
u8 s_AlarmWorkerStacks[NUM_WORKER_THREADS][WORKER_STACK_SIZE] DECOMP_ALIGN(8);
// holds the ThreadPool and its work buffer
// 0x00AEB600
MemoryBlock s_AlarmThreadPoolBlock;

} // namespace

// 0x0097F014
ThreadPool* s_pAlarmThreadPool;

// 0x0034C4B4 (name is ours)
DECOMP_NOINLINE void StartAlarmThreadPool(s32 workerPriority, s32 waitThreadPriority)
{
    if (s_pAlarmThreadPool != 0) {
        return;
    }
    const uptr stackBottoms[NUM_WORKER_THREADS] = {
        reinterpret_cast<uptr>(s_AlarmWorkerStacks[0] + WORKER_STACK_SIZE),
        reinterpret_cast<uptr>(s_AlarmWorkerStacks[1] + WORKER_STACK_SIZE),
    };
    // the work buffer follows the pool in the same block (the page has room for both)
    s_AlarmThreadPoolBlock.AllocateBlock(sizeof(ThreadPool));
    uptr address = s_AlarmThreadPoolBlock.GetAddress();
    s_pAlarmThreadPool = new (reinterpret_cast<void*>(address))
        ThreadPool(address + sizeof(ThreadPool), MAX_WAIT_TASKS, NUM_WORKER_THREADS, stackBottoms, workerPriority, waitThreadPriority);
}

// 0x0034C4AC (name is ours); armlink replaced the tail call by a nop, so this falls through
void StartAlarmThreadPool(s32 workerPriority)
{
    StartAlarmThreadPool(workerPriority, 0);
}

} // namespace detail
} // namespace os
} // namespace nn
