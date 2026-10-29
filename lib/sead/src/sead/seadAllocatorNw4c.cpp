#include "sead/seadAllocatorNw4c.h"
#include "sead/seadNew.h"

namespace sead {
// 0x00541320 (name is ours)
sead::AllocatorNw4c::AllocatorNw4c(Heap* heap) : IAllocator(), mHeap(heap)
{
}

// 0x00541338 slot 0x00
// 0x00541334 (deleting dtor)
sead::AllocatorNw4c::~AllocatorNw4c()
{
    // empty (in the original too)
}

// 0x0012E3AC slot 0x08 | mk7dlp:bytes
void* sead::AllocatorNw4c::Alloc(size_t size, u8 alignment)
{
    if (size == 0) {
        size = alignment;
    }
    return new (mHeap, alignment) u8[size];
}

// 0x00541318 slot 0x0C | slot Free of nw::os::IAllocator
void sead::AllocatorNw4c::Free(void* p)
{
    delete[] static_cast<u8*>(p);
}
} // namespace sead
