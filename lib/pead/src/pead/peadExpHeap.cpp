#include "pead/peadHeap.h"
#include "pead/peadExpHeap.h"

namespace pead {
// 0x0053D66C slot 0x00 | slot vf_0x00 of pead::IDisposer
pead::ExpHeap::~ExpHeap()
{
}

// 0x00749B70 slot 0x08 | virtual slot, introduced by pead::Heap
void pead::ExpHeap::vf_0x08()
{
}

// 0x0074974C slot 0x0C | virtual slot, introduced by pead::Heap
void pead::ExpHeap::vf_0x0C()
{
}

// 0x0053D0CC slot 0x10 | virtual slot, introduced by pead::Heap
void pead::ExpHeap::destroy()
{
}

// 0x0053CEF8 slot 0x14 | nintendogs:bytes-fuzzy
void pead::ExpHeap::adjust()
{
}

// 0x0053D1B8 slot 0x18 | nintendogs:bytes-fuzzy
void* pead::ExpHeap::tryAlloc(unsigned, int)
{
}

// 0x0053883C slot 0x1C | nintendogs:callseq
void pead::ExpHeap::free(void*)
{
}

// 0x0053C81C slot 0x20 | virtual slot, introduced by pead::Heap
void pead::ExpHeap::vf_0x20()
{
}

// 0x0053C6F0 slot 0x24 | virtual slot, introduced by pead::Heap
void pead::ExpHeap::vf_0x24()
{
}

// 0x0053D13C slot 0x28 | nintendogs:callseq
void pead::ExpHeap::freeAll()
{
}

// 0x00749744 slot 0x2C | virtual slot, introduced by pead::Heap
void pead::ExpHeap::vf_0x2C()
{
}

// 0x00749734 slot 0x30 | virtual slot, introduced by pead::Heap
void pead::ExpHeap::vf_0x30()
{
}

// 0x00749C80 slot 0x34 | virtual slot, introduced by pead::Heap
void pead::ExpHeap::vf_0x34()
{
}

// 0x00749638 slot 0x38 | nintendogs:bytes
void pead::ExpHeap::getFreeSize() const
{
}

// 0x00749798 slot 0x3C | nintendogs:bytes
void pead::ExpHeap::getMaxAllocatableSize(int) const
{
}

// 0x00749C88 slot 0x40 | nintendogs:bytes
void pead::ExpHeap::isInclude(const void*) const
{
}

// 0x007495C4 slot 0x44 | virtual slot, introduced by pead::Heap
void pead::ExpHeap::vf_0x44()
{
}

// 0x007496B0 slot 0x48 | virtual slot, introduced by pead::Heap
void pead::ExpHeap::vf_0x48()
{
}

// 0x0074972C slot 0x4C | virtual slot, introduced by pead::Heap
void pead::ExpHeap::vf_0x4C()
{
}

// 0x00749C24 slot 0x50 | virtual slot, introduced by pead::Heap
void pead::ExpHeap::dump() const
{
}

// 0x0053B438 slot 0x54 | virtual slot, introduced by pead::Heap
void pead::ExpHeap::vf_0x54()
{
}

// 0x0053C9A4 slot 0x58 | virtual slot, introduced by pead::ExpHeap
void pead::ExpHeap::vf_0x58()
{
}

// 0x00749724 slot 0x5C | virtual slot, introduced by pead::ExpHeap
void pead::ExpHeap::vf_0x5C()
{
}

// 0x0053C9AC | nintendogs:bytes [tier A]
void pead::ExpHeap::allocFromHead_(unsigned, int)
{
}

// 0x0053CAAC | nintendogs:bytes [tier A]
void pead::ExpHeap::allocFromTail_(unsigned)
{
}

// 0x0053CC14 | nintendogs:bytes [tier B]
void pead::ExpHeap::allocFromTail_(unsigned, int)
{
}

// 0x0053CD1C | nintendogs:bytes [tier A]
void pead::ExpHeap::pushToFreeList_(pead::MemBlock*)
{
}

// 0x0053CDA4 | nintendogs:bytes [tier A]
void pead::ExpHeap::mergeFreeList_()
{
}

// 0x0053CE64 | nintendogs:bytes [tier A]
void pead::ExpHeap::createMaxSizeFreeMemBlock_(pead::ExpHeap*)
{
}

// 0x0053D400 | nintendogs:callseq [tier A]
void pead::ExpHeap::tryCreate(void*, unsigned, const pead::SafeStringBase<char>&, bool)
{
}

// 0x0053D59C | nintendogs:bytes [tier A]
pead::ExpHeap::ExpHeap(const pead::SafeStringBase<char>&, pead::Heap*, void*, unsigned, pead::Heap::HeapDirection, bool)
{
}

// 0x007495CC | nintendogs:bytes [tier A]
void pead::ExpHeap::dumpUseList() const
{
}

// 0x007496B8 | nintendogs:bytes [tier A]
void pead::ExpHeap::dumpFreeList() const
{
}

// 0x00749928 | nintendogs:bytes [tier A]
void pead::ExpHeap::findFreeMemBlockFromHead_(unsigned, pead::ExpHeap::FindMode) const
{
}

// 0x007499C4 | nintendogs:bytes [tier A]
void pead::ExpHeap::findFreeMemBlockFromHead_(unsigned, int, pead::ExpHeap::FindMode) const
{
}

// 0x00749A90 | nintendogs:bytes [tier A]
void pead::ExpHeap::findFreeMemBlockFromTail_(unsigned, int, pead::ExpHeap::FindMode) const
{
}

} // namespace pead
