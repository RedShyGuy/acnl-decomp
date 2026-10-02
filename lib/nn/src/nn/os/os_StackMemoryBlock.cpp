// nn::os::StackMemoryBlock and its C interface.
#include "nn/os/os_StackMemoryBlock.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/os/detail/detail_Api.h"

#include <new>

namespace nn {
namespace os {
namespace detail {

// 0x007B2BD8
void FreeStackMemoryBlock(StackMemoryBlock* block)
{
    nnosStackMemoryBlockFree(block);
}

// 0x00151A24 | copied from the binary
__attribute__((naked)) void CallOnStack(uptr stackTop, void (*f)(StackMemoryBlock*), StackMemoryBlock* arg,
                                        uptr returnAddress)
{
    asm volatile(
        "mov sp, r0\n"
        "mov r0, r2\n"
        "mov lr, r3\n"
        "bx r1\n"
    );
}

} // namespace detail
} // namespace os
} // namespace nn

namespace {

// the address space ran out (permanent, out of resource, module os, 55)
const bit32 RESULT_OUT_OF_ADDRESS_SPACE = 0xD8601837;
const size_t PAGE_SIZE = 0x1000;

} // namespace

// 0x007B2B60
extern "C" void nnosStackMemoryBlockAllocate(nn::os::StackMemoryBlock* block, size_t size)
{
    if (block == 0) {
        return;
    }
    new (block) nn::os::StackMemoryBlock;
    if (nn::os::detail::IsMemoryBlockEnabled() && block->GetAddress() == 0) {
        if (nn::os::detail::AllocateFromMemoryBlockSpace(block, (size + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE) == 0) {
            nn::os::CTR::detail::HandleInternalError(nn::Result(RESULT_OUT_OF_ADDRESS_SPACE));
        }
    }
}

// 0x007B2BDC
extern "C" void nnosStackMemoryBlockFree(nn::os::StackMemoryBlock* block)
{
    if (block->GetAddress() != 0) {
        nn::os::detail::FreeToMemoryBlockSpace(block);
    }
}

// 0x007B2BF0
extern "C" uptr nnosStackMemoryBlockGetStackBottom(const nn::os::StackMemoryBlock* block)
{
    return block->GetAddress() + block->GetSize();
}

// 0x007B2C00
extern "C" void nnosStackMemoryBlockInitialize(nn::os::StackMemoryBlock* block)
{
    if (block) {
        new (block) nn::os::StackMemoryBlock;
    }
}
