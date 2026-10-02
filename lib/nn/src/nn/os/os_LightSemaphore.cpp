#include "nn/os/os_LightSemaphore.h"
#include "nn/os/os_Atomic.h"
#include "nn/os/os_WaitableCounter.h"

namespace nn {
namespace os {
namespace {

struct DecrementIfPositive {
    bool operator()(s32& count) const
    {
        if (count <= 0) {
            return false;
        }
        count = count - 1;
        return true;
    }
};

} // namespace

// 0x00143040 | fefates:bytes [tier B]
void nn::os::LightSemaphore::Acquire()
{
    while (!detail::AtomicUpdateConditional(&mCount, DecrementIfPositive())) {
        detail::AtomicAdd(&mNumWaiters, 1);
        detail::ArbitrateWaitIfLessThan(&mCount, 1);
        detail::AtomicAdd(&mNumWaiters, -1);
    }
}

// 0x001430F4 | nintendogs:callgraph [tier A]
s32 nn::os::LightSemaphore::Release(s32 count)
{
    s32 max = mMaxCount;
    s32 old;
    s32 updated;
    do {
        old = detail::LoadExclusive(&mCount);
        updated = (max - count < old) ? max : old + count;
    } while (detail::StoreExclusive(&mCount, updated));
    if (old <= 0 || mNumWaiters > 0) {
        detail::ArbitrateSignal(&mCount, count);
    }
    return old;
}

} // namespace os
} // namespace nn
