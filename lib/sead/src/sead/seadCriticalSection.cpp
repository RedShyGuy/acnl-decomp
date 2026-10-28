#include "sead/seadCriticalSection.h"

namespace sead {
// 0x0012CA78 (name is ours)
sead::CriticalSection::CriticalSection() : IDisposer(), mCriticalSection()
{
    mCriticalSection.Initialize();
}

// 0x0013C66C (name is ours)
sead::CriticalSection::CriticalSection(Heap* disposerHeap)
    : IDisposer(disposerHeap, HEAP_NULL_OPTION_FIND_CONTAIN_HEAP), mCriticalSection()
{
    mCriticalSection.Initialize();
}

// 0x00138D4C slot 0x00 | slot vf_0x00 of sead::IDisposer
// 0x00545B8C (deleting dtor)
sead::CriticalSection::~CriticalSection()
{
    // empty (in the original too: the critical section of nn is not finalized)
}

// 0x00136474 | nintendogs:callseq-callee [tier A]
void sead::CriticalSection::lock()
{
    mCriticalSection.Enter();
}

// 0x00138D44 | nintendogs:callseq-callee [tier A]
void sead::CriticalSection::unlock()
{
    mCriticalSection.Exit();
}

// 0x0034C024 | nintendogs:callseq-callee [tier A]
bool sead::CriticalSection::tryLock()
{
    return mCriticalSection.TryEnter();
}

} // namespace sead
