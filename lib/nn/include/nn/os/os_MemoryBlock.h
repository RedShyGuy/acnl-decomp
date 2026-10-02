#pragma once

#include "decomp.h"
#include "nn/os/os_MemoryBlockBase.h"

namespace nn {
namespace os {

// A range of the memory block space (nn::os::detail::s_MemoryBlockSpace).
class MemoryBlock : public MemoryBlockBase
{
public:
    void Finalize(); // 0x00136454 | fefates:bytes [tier B]
};

} // namespace os
} // namespace nn
