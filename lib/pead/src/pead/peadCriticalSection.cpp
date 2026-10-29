#include "pead/peadCriticalSection.h"

namespace pead {
// 0x00538928 | nintendogs:callgraph [tier A]
pead::CriticalSection::CriticalSection() : IDisposer(), mCriticalSection()
{
    mCriticalSection.Initialize();
}

// 0x00538970 slot 0x00 | nintendogs:callgraph
// 0x00538954 (deleting dtor)
pead::CriticalSection::~CriticalSection()
{
    // empty (in the original too: the critical section of nn is not finalized)
}

// 0x00538834 (name after sead)
void pead::CriticalSection::lock()
{
    mCriticalSection.Enter();
}

// 0x00538920 (name after sead)
void pead::CriticalSection::unlock()
{
    mCriticalSection.Exit();
}
} // namespace pead
