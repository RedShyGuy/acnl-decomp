#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_ThreadLocalRegion.h"

namespace nn {
namespace os {
namespace {

uptr CurrentThreadId()
{
    return reinterpret_cast<uptr>(detail::GetThreadLocalRegion());
}

} // namespace

// 0x00130840 | nintendogs:bytes [tier A]
void nn::os::CriticalSection::Initialize()
{
    mLock.Initialize();
    mOwner = 0;
    mLockCount = 0;
}

// 0x0013647C | libgarden [tier A]
void nn::os::CriticalSection::Enter()
{
    if (mOwner != CurrentThreadId()) {
        mLock.Lock();
        mOwner = CurrentThreadId();
    }
    mLockCount++;
}

// 0x00136520 | libgarden [tier A]
void nn::os::CriticalSection::Exit()
{
    if (--mLockCount == 0) {
        mOwner = 0;
        mLock.Unlock();
    }
}

// 0x0034C02C | nintendogs:bytes [tier A]
bool nn::os::CriticalSection::TryEnter()
{
    if (mOwner != CurrentThreadId()) {
        if (!mLock.TryLock()) {
            return false;
        }
        mOwner = CurrentThreadId();
    }
    mLockCount++;
    return true;
}

} // namespace os
} // namespace nn
