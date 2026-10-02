#pragma once

#include "decomp.h"

namespace nn {
namespace os {

// A lock that needs no kernel object: one counter word, waiting threads sleep on its address
// with the process wide address arbiter (WaitableCounter::s_ArbitrationObject).
//
// Counter values:
//      1       free
//      1 + n   free, n waiters were woken and race for it
//     -1       locked, nobody waits
//     -1 - n   locked, n threads wait
// Lock negates a positive counter, Unlock negates the counter back and wakes one waiter if
// any wait. A woken waiter takes the lock with 1 - counter (one waiter less).
class SimpleLock
{
public:
    SimpleLock() : mCounter(0) {}

    void Unlock(); // 0x001297C4 | nintendogs:callgraph [tier A]
    void Initialize(); // 0x00130724 | nintendogs:callgraph [tier A]
    void Lock(); // 0x0013073C | nintendogs:callgraph [tier A]
    bool TryLock(); // 0x0034B81C | nintendogs:callgraph [tier A]

private:
    void LockImpl(); // 0x00130764 | fefates:bytes [tier B]

    volatile s32 mCounter;
};

} // namespace os
} // namespace nn
