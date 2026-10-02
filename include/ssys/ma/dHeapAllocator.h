#pragma once

#include "decomp.h"
#include "ssys/ma/dAllocator.h"

namespace ssys {
namespace ma {
// RTTI N4ssys2ma13HeapAllocatorE @ 0x008D2434
// vtable 0x00907244 (vptr 0x0090724C), offset_to_top 0, 4 entries
class HeapAllocator : public ::ssys::ma::Allocator
{
public:
    HeapAllocator(); // ctor candidate(s) 0x00567EB0 (unverified)
    virtual ~HeapAllocator(); // 0x00317FD4 slot 0x00 | slot vf_0x00 of ssys::ma::Allocator
    virtual void vf_0x04(); // 0x00567EC8 slot 0x04 | virtual slot, introduced by ssys::ma::Allocator
};
} // namespace ma
} // namespace ssys
