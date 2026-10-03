#pragma once

#include "decomp.h"
#include "nn/os/os_SimpleLock.h"

namespace nn {
namespace os {

// A SimpleLock that the owning thread may enter again (recursive). The owner is the address of
// the thread's local region, which is unique per thread. Member names are ours.
class CriticalSection
{
public:
    CriticalSection() : mOwner(0), mLockCount(-1) {}

    // constructs an initialized critical section (inline, in the static initializers of global
    // ones, e.g. __sti___21_fs_UserFileSystem_cpp; the tag type is ours)
    struct InitializeTag {};
    explicit CriticalSection(InitializeTag) { Initialize(); }

    // empty; the static initializers register it with __aeabi_atexit
    ~CriticalSection() {} // 0x0034C070

    void Initialize(); // 0x00130840 | nintendogs:bytes [tier A]
    void Enter(); // 0x0013647C | libgarden [tier A]
    void Exit(); // 0x00136520 | libgarden [tier A]
    bool TryEnter(); // 0x0034C02C | nintendogs:bytes [tier A]
    // back to the state before Initialize (inline, e.g. in BlockingQueueBase::Finalize; name is ours)
    void Finalize() { mLockCount = -1; }

    // enters in the constructor, exits in the destructor (the class name is from the binary,
    // the member name is ours)
    class ScopedLock
    {
    public:
        explicit ScopedLock(CriticalSection& criticalSection) : mCriticalSection(criticalSection)
        {
            mCriticalSection.Enter();
        }
        ~ScopedLock() { mCriticalSection.Exit(); }

    private:
        CriticalSection& mCriticalSection;
    };

private:
    SimpleLock mLock;   // 0x0
    uptr mOwner;        // 0x4, thread local region of the owner, 0 if free
    s32 mLockCount;     // 0x8, how often the owner entered (-1 until Initialize)
};
ASSERT_SIZE(CriticalSection, 0xC);

} // namespace os
} // namespace nn
