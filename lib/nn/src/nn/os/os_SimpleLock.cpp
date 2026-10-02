#include "nn/os/os_SimpleLock.h"
#include "nn/os/os_Atomic.h"
#include "nn/os/os_WaitableCounter.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace os {
namespace {

// free -> locked (keeps the number of woken waiters as waiters)
struct TakeIfFree {
    bool operator()(s32& counter) const
    {
        if (counter <= 0) {
            return false;
        }
        counter = -counter;
        return true;
    }
};

// locked -> locked with one more waiter
struct AddWaiterIfLocked {
    bool operator()(s32& counter) const
    {
        if (counter >= 0) {
            return false;
        }
        counter = counter - 1;
        return true;
    }
};

// woken waiter: free -> locked, one waiter less
struct TakeAsWaiter {
    bool operator()(s32& counter) const
    {
        if (counter <= 0) {
            return false;
        }
        counter = 1 - counter;
        return true;
    }
};

} // namespace

// 0x001297C4 | nintendogs:callgraph [tier A]
void nn::os::SimpleLock::Unlock()
{
    // locked -> free
    s32 counter;
    do {
        counter = -detail::LoadExclusive(&mCounter);
    } while (detail::StoreExclusive(&mCounter, counter));
    if (counter > 1) {
        // somebody waits: wake one of them
        nn::svc::ArbitrateAddress(WaitableCounter::s_ArbitrationObject, reinterpret_cast<uptr>(&mCounter),
                                  ARBITRATION_TYPE_SIGNAL, 1, 0);
    }
}

// 0x00130724 | nintendogs:callgraph [tier A]
void nn::os::SimpleLock::Initialize()
{
    do {
        detail::LoadExclusive(&mCounter);
    } while (detail::StoreExclusive(&mCounter, 1));
}

// 0x0013073C | nintendogs:callgraph [tier A]
void nn::os::SimpleLock::Lock()
{
    if (!detail::AtomicUpdateConditional(&mCounter, TakeIfFree())) {
        LockImpl();
    }
}

// 0x00130764 | fefates:bytes [tier B]
void nn::os::SimpleLock::LockImpl()
{
    for (;;) {
        if (detail::AtomicUpdateConditional(&mCounter, AddWaiterIfLocked())) {
            break;
        }
        // released in the meantime
        if (detail::AtomicUpdateConditional(&mCounter, TakeIfFree())) {
            return;
        }
    }
    do {
        nn::svc::ArbitrateAddress(WaitableCounter::s_ArbitrationObject, reinterpret_cast<uptr>(&mCounter),
                                  ARBITRATION_TYPE_WAIT_IF_LESS_THAN, 0, 0);
    } while (!detail::AtomicUpdateConditional(&mCounter, TakeAsWaiter()));
}

// 0x0034B81C | nintendogs:callgraph [tier A]
bool nn::os::SimpleLock::TryLock()
{
    return detail::AtomicUpdateConditional(&mCounter, TakeIfFree());
}

} // namespace os
} // namespace nn
