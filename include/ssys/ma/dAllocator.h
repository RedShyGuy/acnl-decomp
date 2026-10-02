#pragma once

#include "decomp.h"
#include "nw/os/os_IAllocator.h"

namespace ssys {
namespace ma {
// RTTI N4ssys2ma9AllocatorE @ 0x008D2500
// vtable 0x009073AC (vptr 0x009073B4), offset_to_top 0, 4 entries
class Allocator : public ::nw::os::IAllocator
{
public:
    Allocator(); // ctor candidate(s) 0x00120DD0 (unverified)
    virtual ~Allocator(); // 0x0056B23C slot 0x00 | libgarden
    virtual void vf_0x04(); // 0x0056B238 slot 0x04 | virtual slot, introduced by ssys::ma::Allocator
    virtual void vf_0x08(); // 0x0056B21C slot 0x08 | virtual slot, introduced by ssys::ma::Allocator
    virtual void vf_0x0C(); // 0x0056B20C slot 0x0C | virtual slot, introduced by ssys::ma::Allocator
};
} // namespace ma
} // namespace ssys
