#include "nn/os/os_ReaderWriterLock.h"
#include "nn/os/os_Atomic.h"
#include "nn/os/os_WaitableCounter.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace os {
namespace {

// free for writing -> taken by a writer (the readers inside stay counted)
struct TakeForWrite {
    bool operator()(s32& counter) const
    {
        if (counter <= 0) {
            return false;
        }
        counter = -counter;
        return true;
    }
};

// sleeps while the counter is below value
void WaitWhileLessThan(volatile s32* counter, s32 value)
{
    nn::svc::ArbitrateAddress(WaitableCounter::s_ArbitrationObject, reinterpret_cast<uptr>(counter),
                              ARBITRATION_TYPE_WAIT_IF_LESS_THAN, value, 0);
}

} // namespace

// 0x0011F690 | fefates:bytes [tier B]
nn::os::ReaderWriterLock::ReaderWriterLock() : mCounter(0)
{
    detail::AtomicStore(&mCounter, 0);
}

// 0x0011F678 | tier C
void nn::os::ReaderWriterLock::Initialize()
{
    detail::AtomicStore(&mCounter, 1);
}

// 0x0013AF84 | fefates:bytes [tier B]
void nn::os::ReaderWriterLock::LockForWrite()
{
    // close the lock for new readers (or wait until another writer is done)
    while (!detail::AtomicUpdateConditional(&mCounter, TakeForWrite())) {
        WaitWhileLessThan(&mCounter, 0);
    }
    // wait until the readers inside have left
    while (mCounter != -1) {
        WaitWhileLessThan(&mCounter, -1);
    }
}

} // namespace os
} // namespace nn
