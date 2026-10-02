#pragma once

// nn::os::MemoryBlockBase - a range of the process address space that an AddressSpaceManager
// hands out (shared memory, memory blocks, transfer memory). The class name is from the binary
// (signatures like nn::os::detail::FreeToSharedMemorySpace(nn::os::MemoryBlockBase*)); the layout
// is from AddressSpaceManager::Allocate / Free and TransferMemoryBlock, the member names are ours.

#include "decomp.h"

namespace nn {
namespace os {

class MemoryBlockBase
{
public:
    MemoryBlockBase() : mNext(0), mPrev(0), mAddress(0), mSize(0), mIsReadOnly(false) {}

    uptr GetAddress() const { return mAddress; }
    size_t GetSize() const { return mSize; }
    bool IsReadOnly() const { return mIsReadOnly; }

protected:
    MemoryBlockBase* mNext;     // 0x00, ring of the blocks of an AddressSpaceManager (to lower addresses)
    MemoryBlockBase* mPrev;     // 0x04 (to higher addresses)
    uptr mAddress;              // 0x08, 0 while the block is not set up
    size_t mSize;               // 0x0C
    bool mIsReadOnly;           // 0x10

    friend class AddressSpaceManager;
};
ASSERT_SIZE(MemoryBlockBase, 0x14);

} // namespace os
} // namespace nn
