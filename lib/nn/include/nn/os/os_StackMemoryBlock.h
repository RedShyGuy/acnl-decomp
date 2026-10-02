#pragma once

// nn::os::StackMemoryBlock - a thread stack in the memory block space, used through a C interface
// (nnosStackMemoryBlock*, names from the binary). The class name is from the signature
// nn::os::detail::Switch(nn::os::StackMemoryBlock*, nn::os::StackMemoryBlock*).

#include "decomp.h"
#include "nn/os/os_MemoryBlockBase.h"

namespace nn {
namespace os {

class StackMemoryBlock : public MemoryBlockBase
{
};

namespace detail {
// frees a stack block (the function DefaultAutoStackManager::Destruct runs on its own stack;
// the name is ours)
void FreeStackMemoryBlock(StackMemoryBlock* block); // 0x007B2BD8
// switches to the stack at stackTop and calls f(arg) with returnAddress as return address
void CallOnStack(uptr stackTop, void (*f)(StackMemoryBlock*), StackMemoryBlock* arg, uptr returnAddress); // 0x00151A24
} // namespace detail

} // namespace os
} // namespace nn

extern "C" {
void nnosStackMemoryBlockAllocate(nn::os::StackMemoryBlock* block, size_t size); // 0x007B2B60
void nnosStackMemoryBlockFree(nn::os::StackMemoryBlock* block); // 0x007B2BDC
uptr nnosStackMemoryBlockGetStackBottom(const nn::os::StackMemoryBlock* block); // 0x007B2BF0
void nnosStackMemoryBlockInitialize(nn::os::StackMemoryBlock* block); // 0x007B2C00
}
