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

// taken by a writer -> free (the sign turns back)
struct ReleaseFromWrite {
    bool operator()(s32& counter) const
    {
        counter = -counter;
        return true;
    }
};

// free -> taken by a writer, only without readers
struct TakeForWriteIfFree {
    bool operator()(s32& counter) const
    {
        if (counter != 1) {
            return false;
        }
        counter = -1;
        return true;
    }
};

// one more reader, unless a writer has it
struct AddReader {
    bool operator()(s32& counter) const
    {
        if (counter <= 0) {
            return false;
        }
        counter++;
        return true;
    }
};

// one reader less (counted towards -1 while a writer waits)
struct RemoveReader {
    bool operator()(s32& counter) const
    {
        if (counter >= 0) {
            counter--;
        } else {
            counter++;
        }
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

// 0x0034C18C
nn::os::ReaderWriterLock::~ReaderWriterLock()
{
}

// 0x0034C174 (name is ours)
void nn::os::ReaderWriterLock::Finalize()
{
    detail::AtomicStore(&mCounter, 0);
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

// 0x00136540 (name is ours)
void nn::os::ReaderWriterLock::UnlockForWrite()
{
    detail::AtomicUpdateConditional(&mCounter, ReleaseFromWrite());
    detail::ArbitrateSignal(&mCounter, -1);
}

// 0x0013658C (name is ours)
bool nn::os::ReaderWriterLock::TryLockForWrite()
{
    return detail::AtomicUpdateConditional(&mCounter, TakeForWriteIfFree());
}

// 0x0034C0A8 (name is ours)
void nn::os::ReaderWriterLock::LockForRead()
{
    while (!detail::AtomicUpdateConditional(&mCounter, AddReader())) {
        WaitWhileLessThan(&mCounter, 0);
    }
}

// 0x0034C114 (name is ours)
void nn::os::ReaderWriterLock::UnlockForRead()
{
    detail::AtomicUpdateConditional(&mCounter, RemoveReader());
    // the last reader lets a waiting writer in
    if (mCounter == -1) {
        detail::ArbitrateSignal(&mCounter, -1);
    }
}

} // namespace os
} // namespace nn
