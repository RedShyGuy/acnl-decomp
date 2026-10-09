#include "nn/os/os_MemoryBlock.h"
#include <new>
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/os/detail/detail_Api.h"

namespace nn {
namespace os {
namespace {

// no room in the memory block space, as in os_SharedMemoryBlock.cpp (name is ours)
const bit32 RESULT_OUT_OF_ADDRESS_SPACE = 0xD8601837;  // permanent, out of resource, 55

const size_t PAGE_SIZE = 0x1000;

} // namespace

// 0x0011DE60 (name is ours)
void nn::os::MemoryBlock::AllocateBlock(size_t size)
{
    if (detail::s_IsMemoryBlockEnabled && mAddress == 0) {
        uptr address = detail::s_MemoryBlockSpace.Allocate(this, (size + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE, 0);
        if (address == 0) {
            CTR::detail::HandleInternalError(nn::Result(RESULT_OUT_OF_ADDRESS_SPACE));
        }
        mIsReadOnly = false;
    }
}

// 0x00136454 | fefates:bytes [tier B]
DECOMP_NOINLINE void nn::os::MemoryBlock::Finalize()
{
    if (mAddress != 0) {
        detail::FreeToMemoryBlockSpace(this);
    }
}

// 0x0034C010 (the destructor of the MemoryBlock in os_AlarmThreadPool.cpp, registered by its
// static initializer)
nn::os::MemoryBlock::~MemoryBlock()
{
    Finalize();
}

} // namespace os
} // namespace nn

// 0x0011F3B0
extern "C" void nnosMemoryBlockAllocate(nn::os::MemoryBlock* block, size_t size)
{
    if (block == NULL) {
        return;
    }
    new (block) nn::os::MemoryBlock;
    block->AllocateBlock(size);
}
