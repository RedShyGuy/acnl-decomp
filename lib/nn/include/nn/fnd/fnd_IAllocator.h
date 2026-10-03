#pragma once

#include "decomp.h"

namespace nn {
namespace fnd {
// RTTI N2nn3fnd10IAllocatorE @ 0x008CDE38
// An allocator interface. The slots follow ExpHeapTemplate<...>::Allocator (Allocate 0x007D38D0,
// Free 0x007D38A0); IAllocator itself has no out-of-line functions.
class IAllocator
{
public:
    virtual void* Allocate(size_t size, s32 alignment) = 0; // slot 0x00
    virtual void Free(void* p) = 0; // slot 0x04
    virtual ~IAllocator() {} // slots 0x08, 0x0C
};
} // namespace fnd
} // namespace nn
