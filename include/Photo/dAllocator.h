#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_IAllocator.h"

namespace photo {
// RTTI N5photo9AllocatorE @ 0x008D2DC4
// vtable 0x009092C4 (vptr 0x009092CC), offset_to_top 0, 4 entries
class Allocator : public ::nn::fnd::IAllocator
{
public:
    Allocator(); // ctor address unknown
    virtual void vf_0x00(); // 0x005B48F4 slot 0x00 | virtual slot, introduced by photo::Allocator
    virtual void vf_0x04(); // 0x005B48D0 slot 0x04 | virtual slot, introduced by photo::Allocator
    virtual void vf_0x08(); // 0x005B4910 slot 0x08 | virtual slot, introduced by photo::Allocator
    virtual void vf_0x0C(); // 0x005B490C slot 0x0C | virtual slot, introduced by photo::Allocator
};
} // namespace photo
