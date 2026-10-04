#include "pead/hostio/peadNode.h"
#include "pead/peadHeapMgr.h"

namespace pead {
// 0x00AE82B4
PtrArray<Heap> HeapMgr::sRootHeaps;

// ctor candidate(s) 0x00793140 (unverified)
pead::HeapMgr::HeapMgr()
{
}

// 0x0053D80C slot 0x00 | virtual slot, introduced by pead::HeapMgr
void pead::HeapMgr::vf_0x00()
{
}

// 0x0053D808 slot 0x04 | virtual slot, introduced by pead::HeapMgr
void pead::HeapMgr::vf_0x04()
{
}

} // namespace pead
