#include "nn/os/os_LightEvent.h"
#include "nn/os/os_Atomic.h"
#include "nn/os/os_WaitableCounter.h"

namespace nn {
namespace os {

// 0x001296A8 | nintendogs:callgraph [tier A]
void nn::os::LightEvent::Signal()
{
    if (mCounter == AUTO_NOT_SIGNALED) {
        detail::AtomicStore(&mCounter, AUTO_SIGNALED);
        detail::ArbitrateSignal(&mCounter, 1);
    } else if (mCounter == MANUAL_NOT_SIGNALED) {
        mLock.Lock();
        detail::AtomicStore(&mCounter, MANUAL_SIGNALED);
        detail::ArbitrateSignal(&mCounter, -1);
        mLock.Unlock();
    }
}

// 0x00129760 | nintendogs:callgraph [tier A]
void nn::os::LightEvent::ClearSignal()
{
    if (mCounter == MANUAL_SIGNALED) {
        mLock.Lock();
        detail::AtomicStore(&mCounter, MANUAL_NOT_SIGNALED);
        mLock.Unlock();
    } else if (mCounter == AUTO_SIGNALED) {
        detail::AtomicStore(&mCounter, AUTO_NOT_SIGNALED);
    }
}

// 0x001305D8 | nintendogs:bytes-fuzzy [tier A]
void nn::os::LightEvent::Initialize(bool isManualReset)
{
    mLock.Initialize();
    detail::AtomicStore(&mCounter, isManualReset ? MANUAL_NOT_SIGNALED : AUTO_NOT_SIGNALED);
}

// 0x0013060C | fefates:bytes [tier B]
void nn::os::LightEvent::Wait()
{
    for (;;) {
        s32 counter = mCounter;
        if (counter == MANUAL_NOT_SIGNALED) {
            detail::ArbitrateWaitIfLessThan(&mCounter, 0);
            return;
        }
        if (counter == AUTO_NOT_SIGNALED) {
            // wait below
        } else if (counter == AUTO_SIGNALED) {
            // take the signal, unless another waiter was faster
            if (detail::AtomicCompareAndSwap(&mCounter, AUTO_SIGNALED, AUTO_NOT_SIGNALED) == AUTO_SIGNALED) {
                return;
            }
        } else if (counter == MANUAL_SIGNALED) {
            return;
        }
        detail::ArbitrateWaitIfLessThan(&mCounter, 0);
    }
}

// 0x001306D4 | fefates:bytes [tier B]
bool nn::os::LightEvent::TryWait()
{
    if (mCounter == MANUAL_SIGNALED) {
        return true;
    }
    return detail::AtomicCompareAndSwap(&mCounter, AUTO_SIGNALED, AUTO_NOT_SIGNALED) == AUTO_SIGNALED;
}

// 0x0034B720 | fefates:bytes [tier B]
void nn::os::LightEvent::Pulse()
{
    s32 counter = mCounter;
    if (counter == MANUAL_NOT_SIGNALED) {
        detail::ArbitrateSignal(&mCounter, -1);
    } else if (counter == AUTO_NOT_SIGNALED) {
        detail::ArbitrateSignal(&mCounter, 1);
    } else if (counter == AUTO_SIGNALED) {
        detail::AtomicStore(&mCounter, AUTO_NOT_SIGNALED);
    } else if (counter == MANUAL_SIGNALED) {
        mLock.Lock();
        detail::AtomicStore(&mCounter, MANUAL_NOT_SIGNALED);
        detail::ArbitrateSignal(&mCounter, -1);
        mLock.Unlock();
    }
}

} // namespace os
} // namespace nn
