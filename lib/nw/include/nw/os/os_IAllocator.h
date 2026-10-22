#pragma once

#include "decomp.h"

namespace nw {
namespace os {
// RTTI N2nw2os10IAllocatorE @ 0x008D0528
// An allocator interface (no out-of-line functions of its own). The slots follow the calls of
// nw::lyt::Layout::AllocMemory / FreeMemory; the names of the functions are ours.
class IAllocator
{
public:
    // (inline)
    IAllocator() {}
    virtual ~IAllocator() {} // slot 0x00 (0x04 deleting)
    // size bytes aligned to alignment
    virtual void* Alloc(size_t size, u8 alignment) = 0; // slot 0x08
    virtual void Free(void* p) = 0; // slot 0x0C
};
} // namespace os
} // namespace nw
