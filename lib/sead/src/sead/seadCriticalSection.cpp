#include "sead/seadIDisposer.h"
#include "sead/seadCriticalSection.h"

namespace sead {
// 0x00138D4C slot 0x00 | slot vf_0x00 of sead::IDisposer
sead::CriticalSection::~CriticalSection()
{
}

// 0x00136474 | nintendogs:callseq-callee [tier A]
void sead::CriticalSection::lock()
{
}

// 0x00138D44 | libgarden [tier A]
void sead::CriticalSection::Exit()
{
}

// 0x0034C024 | nintendogs:callseq-callee [tier A]
void sead::CriticalSection::tryLock()
{
}

// 0x00538928 | nintendogs:callgraph [tier A]
sead::CriticalSection::CriticalSection()
{
}

} // namespace sead
