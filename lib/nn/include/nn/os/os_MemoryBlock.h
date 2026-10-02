#pragma once

#include "decomp.h"
#include "nn/os/os_MemoryBlockBase.h"

namespace nn {
namespace os {

// A range of the memory block space (nn::os::detail::s_MemoryBlockSpace).
class MemoryBlock : public MemoryBlockBase
{
public:
    ~MemoryBlock();

    // takes size bytes (rounded up to pages) of the memory block space; does nothing if memory
    // blocks are not enabled or the block is set up already
    void AllocateBlock(size_t size);
    void Finalize(); // 0x00136454 | fefates:bytes [tier B]
};

} // namespace os
} // namespace nn
