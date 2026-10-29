#pragma once

#include "decomp.h"
#include "nw/os/os_IAllocator.h"

namespace sead {
class Heap;

// RTTI N4sead13AllocatorNw4cE @ 0x008D15CC
// vtable 0x00905284 (vptr 0x0090528C), offset_to_top 0, 4 entries
//
// The allocator of nw that takes its memory from a sead heap (the member name is ours).
class AllocatorNw4c : public ::nw::os::IAllocator
{
public:
    explicit AllocatorNw4c(Heap* heap); // 0x00541320
    virtual ~AllocatorNw4c(); // 0x00541338 slot 0x00
    // 0x00541334 slot 0x04 (deleting dtor)
    // (0 bytes: as many bytes as the alignment)
    virtual void* Alloc(size_t size, u8 alignment); // 0x0012E3AC slot 0x08 | mk7dlp:bytes
    virtual void Free(void* p); // 0x00541318 slot 0x0C | slot Free of nw::os::IAllocator

    Heap* mHeap; // 0x04
};
ASSERT_SIZE(AllocatorNw4c, 0x8);
} // namespace sead
