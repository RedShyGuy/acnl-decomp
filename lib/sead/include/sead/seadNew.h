#pragma once

#include "decomp.h"

#include <cstddef>
#include <new>

namespace sead {
class Heap;

// the allocation of the new operators: from heap (the current heap for NULL), with malloc before
// the heaps exist (isThrowing: false for the nothrow operator, not used here; name is ours)
DECOMP_NOINLINE void* allocFromHeap_(Heap* heap, size_t size, s32 alignment, bool isThrowing); // 0x001328EC

// tried first by the delete operators: the current heap (name is ours)
extern bool s_IsDeleteFromCurrentHeapFirst;
} // namespace sead

// The global new and delete operators of sead. The delete operators look for the heap that holds the
// memory; the two copies are equal (0x002F8958 is the one of delete[]; libgarden calls it
// "operator delete(void*, unsigned int)", probably folded with it there).
void* operator new(size_t size, sead::Heap* heap, int alignment); // 0x002FD0BC (name after symbols_us)
void* operator new[](size_t size, sead::Heap* heap, int alignment); // 0x0012E3C0 (name after symbols_us)
// (symbols_us gives 0x001328D8 both of the names above; it is the nothrow variant)
void* operator new(size_t size, sead::Heap* heap, int alignment, const std::nothrow_t&); // 0x001328D8
void* operator new[](size_t size); // 0x002FF7DC (name after the C++ operator)
void operator delete(void* p); // 0x002FF7F8
void operator delete[](void* p); // 0x002F8958
