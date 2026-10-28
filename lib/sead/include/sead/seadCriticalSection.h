#pragma once

#include "decomp.h"
#include "nn/os/os_CriticalSection.h"
#include "sead/seadIDisposer.h"

namespace sead {
// RTTI N4sead15CriticalSectionE @ 0x008D1790
// vtable 0x00905954 (vptr 0x0090595C), offset_to_top 0, 2 entries
//
// A critical section that its heap destroys (lock/unlock/tryLock are the names of the reference
// symbols; symbols_us calls unlock Exit). The member name is ours.
class CriticalSection : public ::sead::IDisposer
{
public:
    CriticalSection(); // 0x0012CA78 (name is ours)
    explicit CriticalSection(Heap* disposerHeap); // 0x0013C66C (name is ours)
    virtual ~CriticalSection(); // 0x00138D4C slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00545B8C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    void lock(); // 0x00136474 | nintendogs:callseq-callee [tier A]
    void unlock(); // 0x00138D44 | nintendogs:callseq-callee [tier A]
    bool tryLock(); // 0x0034C024 | nintendogs:callseq-callee [tier A]

    nn::os::CriticalSection mCriticalSection; // 0x10
};
ASSERT_SIZE(CriticalSection, 0x1C);

// locks lock if cond is true (inline; name is ours)
class ConditionalScopedLock
{
public:
    ConditionalScopedLock(CriticalSection* lock, bool cond) : mLock(nullptr)
    {
        if (cond) {
            mLock = lock;
            mLock->lock();
        }
    }
    ~ConditionalScopedLock()
    {
        if (mLock != nullptr) {
            mLock->unlock();
        }
    }

    CriticalSection* mLock;
};

// locks lock (inline; name is ours)
class ScopedLock
{
public:
    explicit ScopedLock(CriticalSection* lock) : mLock(lock) { mLock->lock(); }
    ~ScopedLock() { mLock->unlock(); }

    CriticalSection* mLock;
};
} // namespace sead
