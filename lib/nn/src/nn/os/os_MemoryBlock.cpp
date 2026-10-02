#include "nn/os/os_MemoryBlock.h"
#include "nn/os/detail/detail_Api.h"

namespace nn {
namespace os {

// 0x00136454 | fefates:bytes [tier B]
void nn::os::MemoryBlock::Finalize()
{
    if (mAddress != 0) {
        detail::FreeToMemoryBlockSpace(this);
    }
}

} // namespace os
} // namespace nn
