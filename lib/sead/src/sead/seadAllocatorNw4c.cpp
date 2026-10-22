#include "nw/os/os_IAllocator.h"
#include "sead/seadAllocatorNw4c.h"

namespace sead {
// ctor candidate(s) 0x00541320 (unverified)
sead::AllocatorNw4c::AllocatorNw4c()
{
}

// 0x00541338 slot 0x00 | slot of nw::os::IAllocator
// 0x00541334 (deleting dtor)
sead::AllocatorNw4c::~AllocatorNw4c()
{
}

// 0x0012E3AC slot 0x08 | mk7dlp:bytes
void* sead::AllocatorNw4c::Alloc(unsigned, unsigned char)
{
}

// 0x00541318 slot 0x0C | slot Free of nw::os::IAllocator
void sead::AllocatorNw4c::Free(void*)
{
}

} // namespace sead
