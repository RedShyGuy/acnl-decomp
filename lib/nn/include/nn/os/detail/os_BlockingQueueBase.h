#pragma once

// nn::os::detail::BlockingQueueBase - a fixed size ring buffer of words that blocks when it is
// empty (Dequeue) or full (Enqueue, Jam). Used by sead::MessageQueue and
// nw::snd::internal::DriverCommandManager. The class name is from symbols.json, the member names
// are ours. The only instantiation in ACNL is with CriticalSection (os_BlockingQueueBase.cpp).

#include "decomp.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_LightSemaphore.h"

namespace nn {
namespace os {
namespace detail {

template <typename LockT>
class BlockingQueueBase
{
public:
    ~BlockingQueueBase();

    void Initialize(uptr* buffer, size_t size);
    void Finalize();

    // add at the back / the front; the Try variants return false instead of waiting while full
    bool TryEnqueue(uptr value);
    void Enqueue(uptr value);
    bool TryJam(uptr value);
    void Jam(uptr value);

    // take from the front; TryDequeue returns false instead of waiting while empty
    bool TryDequeue(uptr* value);
    uptr Dequeue();

private:
    uptr* mBuffer;                      // 0x00
    LightSemaphore mNotEmpty;           // 0x04, Dequeue waits on it
    LightSemaphore mNotFull;            // 0x0C, Enqueue and Jam wait on it
    LockT mLock;                        // 0x14
    size_t mSize;                       // 0x20, number of words in mBuffer
    size_t mFirstIndex;                 // 0x24
    s32 mUsedCount;                     // 0x28
    volatile s32 mNumDequeueWaiters;    // 0x2C, threads in Dequeue
    volatile s32 mNumEnqueueWaiters;    // 0x30, threads in Enqueue or Jam

    static void CheckLayout()
    {
        ASSERT_OFFSET(BlockingQueueBase, mLock, 0x14);
        ASSERT_OFFSET(BlockingQueueBase, mSize, 0x20);
        ASSERT_OFFSET(BlockingQueueBase, mNumEnqueueWaiters, 0x30);
    }
};

} // namespace detail
} // namespace os
} // namespace nn
