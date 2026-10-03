#include "nn/os/detail/os_BlockingQueueBase.h"
#include "nn/os/os_Atomic.h"

// The functions are the instantiation for CriticalSection (explicit, at the end of the file);
// check.py puts the argument of that instantiation in for LockT.

namespace nn {
namespace os {
namespace detail {

// 0x00143BB8 | nintendogs:callgraph [tier A]
template <typename LockT>
BlockingQueueBase<LockT>::~BlockingQueueBase()
{
    Finalize();
}

// 0x0013D9E4 | nintendogs:callgraph [tier A]
template <typename LockT>
void BlockingQueueBase<LockT>::Initialize(uptr* buffer, size_t size)
{
    mBuffer = buffer;
    mSize = size;
    mFirstIndex = 0;
    mUsedCount = 0;
    AtomicStore(&mNumDequeueWaiters, 0);
    AtomicStore(&mNumEnqueueWaiters, 0);
    mNotEmpty.Initialize(0, 0x7FFF);
    mNotFull.Initialize(0, 0x7FFF);
    mLock.Initialize();
}

// 0x00143B7C | nintendogs:callgraph [tier A]
template <typename LockT>
void BlockingQueueBase<LockT>::Finalize()
{
    mLock.Finalize();
}

// 0x00142844 | nintendogs:bytes [tier A]
template <typename LockT>
bool BlockingQueueBase<LockT>::TryEnqueue(uptr value)
{
    typename LockT::ScopedLock lock(mLock);
    if (mUsedCount >= mSize) {
        return false;
    }
    mBuffer[(mFirstIndex + mUsedCount) % mSize] = value;
    mUsedCount++;
    if (mNumDequeueWaiters > 0) {
        mNotEmpty.Release(1);
    }
    return true;
}

// 0x001429D0 | nintendogs:callgraph [tier A]
template <typename LockT>
void BlockingQueueBase<LockT>::Enqueue(uptr value)
{
    AtomicAdd(&mNumEnqueueWaiters, 1);
    for (;;) {
        if (TryEnqueue(value)) {
            break;
        }
        mNotFull.Acquire();
    }
    AtomicAdd(&mNumEnqueueWaiters, -1);
}

// 0x007D3798 | nintendogs:bytes [tier A]
template <typename LockT>
bool BlockingQueueBase<LockT>::TryJam(uptr value)
{
    typename LockT::ScopedLock lock(mLock);
    if (mUsedCount >= mSize) {
        return false;
    }
    mFirstIndex = (mFirstIndex + mSize - 1) % mSize;
    mBuffer[mFirstIndex] = value;
    mUsedCount++;
    if (mNumDequeueWaiters > 0) {
        mNotEmpty.Release(1);
    }
    return true;
}

// 0x007D36C8 | fefates:bytes [tier B]
template <typename LockT>
void BlockingQueueBase<LockT>::Jam(uptr value)
{
    AtomicAdd(&mNumEnqueueWaiters, 1);
    for (;;) {
        if (TryJam(value)) {
            break;
        }
        mNotFull.Acquire();
    }
    AtomicAdd(&mNumEnqueueWaiters, -1);
}

// 0x001427B8 | nintendogs:bytes [tier A]
template <typename LockT>
bool BlockingQueueBase<LockT>::TryDequeue(uptr* value)
{
    typename LockT::ScopedLock lock(mLock);
    if (mUsedCount <= 0) {
        return false;
    }
    *value = mBuffer[mFirstIndex];
    mFirstIndex = (mFirstIndex + 1) % mSize;
    mUsedCount--;
    if (mNumEnqueueWaiters > 0) {
        mNotFull.Release(1);
    }
    return true;
}

// 0x001428F4 | nintendogs:callgraph [tier A]
template <typename LockT>
uptr BlockingQueueBase<LockT>::Dequeue()
{
    AtomicAdd(&mNumDequeueWaiters, 1);
    uptr value;
    for (;;) {
        if (TryDequeue(&value)) {
            break;
        }
        mNotEmpty.Acquire();
    }
    AtomicAdd(&mNumDequeueWaiters, -1);
    return value;
}

template class BlockingQueueBase<nn::os::CriticalSection>;
ASSERT_SIZE(BlockingQueueBase<CriticalSection>, 0x34);

} // namespace detail
} // namespace os
} // namespace nn
