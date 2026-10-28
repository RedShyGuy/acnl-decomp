#include "pead/peadIDisposer.h"
#include "pead/peadCriticalSection.h"

namespace pead {
// 0x00538970 slot 0x00 | nintendogs:callgraph
pead::CriticalSection::~CriticalSection()
{
}

// 0x00538928 | nintendogs:callgraph [tier A]
pead::CriticalSection::CriticalSection()
{
}

} // namespace pead
