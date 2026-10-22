#pragma once

#include "decomp.h"
#include "nw/os/os_IAllocator.h"

namespace sead {
// RTTI N4sead13AllocatorNw4cE @ 0x008D15CC
// vtable 0x00905284 (vptr 0x0090528C), offset_to_top 0, 4 entries
class AllocatorNw4c : public ::nw::os::IAllocator
{
public:
    AllocatorNw4c(); // ctor candidate(s) 0x00541320 (unverified)
    virtual ~AllocatorNw4c();
    virtual void* Alloc(unsigned, unsigned char); // 0x0012E3AC slot 0x08 | mk7dlp:bytes
    virtual void Free(void*); // 0x00541318 slot 0x0C | slot Free of nw::os::IAllocator
};
} // namespace sead
