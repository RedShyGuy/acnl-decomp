#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
// Memory for sound data (the class name and Alloc are from the reference symbols).
// RTTI N2nw3snd22SoundMemoryAllocatableE @ 0x008D0954
class SoundMemoryAllocatable
{
public:
    // (inline)
    SoundMemoryAllocatable() {}
    virtual ~SoundMemoryAllocatable() {} // slot 0x00

    // size bytes (NULL: no memory)
    virtual void* Alloc(u32 size) = 0; // slot 0x08
};
} // namespace snd
} // namespace nw
