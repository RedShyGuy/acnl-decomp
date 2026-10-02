#pragma once

#include "decomp.h"

namespace nn {
namespace os {

// A counting semaphore without a kernel object (counter + address arbiter). Member names are ours.
class LightSemaphore
{
public:
    void Acquire(); // 0x00143040 | fefates:bytes [tier B]
    // adds count (up to the maximum), returns the count before
    s32 Release(s32 count); // 0x001430F4 | nintendogs:callgraph [tier A]

private:
    volatile s32 mCount;        // 0x0
    volatile s16 mNumWaiters;   // 0x4, threads sleeping in Acquire
    s16 mMaxCount;              // 0x6
};
ASSERT_SIZE(LightSemaphore, 0x8);

} // namespace os
} // namespace nn
