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
    virtual void vf_0x00(); // 0x00541338 slot 0x00 | virtual slot, introduced by sead::AllocatorNw4c
    virtual void vf_0x04(); // 0x00541334 slot 0x04 | virtual slot, introduced by sead::AllocatorNw4c
    virtual void Alloc(unsigned, unsigned char); // 0x0012E3AC slot 0x08 | mk7dlp:bytes
    virtual void vf_0x0C(); // 0x00541318 slot 0x0C | virtual slot, introduced by sead::AllocatorNw4c
};
} // namespace sead
