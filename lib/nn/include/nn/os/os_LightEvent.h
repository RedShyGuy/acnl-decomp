#pragma once

#include "decomp.h"
#include "nn/os/os_SimpleLock.h"

namespace nn {
namespace os {

// An event without a kernel object: one counter word plus the address arbiter.
//
// Counter values (names are ours):
//     -1   auto reset, not signalled         0   auto reset, signalled
//     -2   manual reset, not signalled       1   manual reset, signalled
// An auto reset event lets one waiter through and resets itself; a manual reset event stays
// signalled until ClearSignal. mLock orders the manual reset changes.
class LightEvent
{
public:
    LightEvent() : mCounter(0) {}

    void Signal(); // 0x001296A8 | nintendogs:callgraph [tier A]
    void ClearSignal(); // 0x00129760 | nintendogs:callgraph [tier A]
    void Initialize(bool isManualReset); // 0x001305D8 | nintendogs:bytes-fuzzy [tier A]
    void Wait(); // 0x0013060C | fefates:bytes [tier B]
    bool TryWait(); // 0x001306D4 | fefates:bytes [tier B]
    // wakes the waiters without leaving the event signalled
    void Pulse(); // 0x0034B720 | fefates:bytes [tier B]

private:
    static const s32 AUTO_NOT_SIGNALED = -1;
    static const s32 AUTO_SIGNALED = 0;
    static const s32 MANUAL_NOT_SIGNALED = -2;
    static const s32 MANUAL_SIGNALED = 1;

    volatile s32 mCounter;  // 0x0
    SimpleLock mLock;       // 0x4
};
ASSERT_SIZE(LightEvent, 0x8);

} // namespace os
} // namespace nn
