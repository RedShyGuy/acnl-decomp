#pragma once

#include "decomp.h"
#include "nn/os/os_CriticalSection.h"
#include "pead/peadIDisposer.h"

namespace pead {
// RTTI N4pead15CriticalSectionE @ 0x008D11A8
// vtable 0x00904AB0 (vptr 0x00904AB8), offset_to_top 0, 2 entries
//
// pia's copy of sead::CriticalSection: a critical section that its heap destroys (the member name is ours).
class CriticalSection : public ::pead::IDisposer
{
public:
    CriticalSection(); // 0x00538928 | nintendogs:callgraph [tier A]
    virtual ~CriticalSection(); // 0x00538970 slot 0x00 | nintendogs:callgraph
    // 0x00538954 slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)

    void lock(); // 0x00538834 (name after sead)
    void unlock(); // 0x00538920 (name after sead)

    nn::os::CriticalSection mCriticalSection; // 0x10
};
ASSERT_SIZE(CriticalSection, 0x1C);

// locks lock if cond is true (inline, as in sead; name is ours)
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
} // namespace pead
