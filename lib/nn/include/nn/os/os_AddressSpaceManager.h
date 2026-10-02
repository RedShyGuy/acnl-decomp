#pragma once

#include "decomp.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_MemoryBlockBase.h"

namespace nn {
namespace os {

// Hands out ranges of [mBegin, mEnd) to MemoryBlockBase objects. The blocks form a ring through
// mNext (to lower addresses, the lowest one wraps around to the highest); mHead is the lowest.
// Member names are ours.
class AddressSpaceManager
{
public:
    AddressSpaceManager() : mBegin(0), mEnd(0), mHead(0) {}

    // once (later calls are ignored)
    void Initialize(uptr begin, size_t size)
    {
        if (mBegin == 0 && mEnd == 0) {
            mLock.Initialize();
            mEnd = begin + size;
            mBegin = begin;
        }
    }

    // finds the highest gap of size + margin and puts block at its bottom (margin above the
    // block below); returns the address, or 0 if there is no room
    uptr Allocate(nn::os::MemoryBlockBase* block, size_t size, size_t margin); // 0x00129818 | nintendogs:bytes [tier A]
    void Free(nn::os::MemoryBlockBase* block); // 0x0013E810 | nintendogs:callgraph [tier A]
    // to takes over the range of from
    void Switch(nn::os::MemoryBlockBase* to, nn::os::MemoryBlockBase* from); // 0x0034C260 | fefates:bytes [tier B]

private:
    // block becomes the only one (a ring of one)
    void InsertFirst(MemoryBlockBase* block)
    {
        block->mPrev = block;
        block->mNext = block;
        mHead = block;
    }

    // block goes right after pos in the ring (just below it)
    static void InsertAfter(MemoryBlockBase* pos, MemoryBlockBase* block)
    {
        block->mPrev = pos;
        pos->mNext->mPrev = block;
        block->mNext = pos->mNext;
        pos->mNext = block;
    }

    void Remove(MemoryBlockBase* block)
    {
        if (block->mNext == block) {
            mHead = 0;
        } else {
            if (mHead == block) {
                mHead = block->mPrev;
            }
            block->mPrev->mNext = block->mNext;
            block->mNext->mPrev = block->mPrev;
        }
        block->mPrev = 0;
        block->mNext = 0;
    }

    uptr mBegin;                // 0x00
    uptr mEnd;                  // 0x04
    MemoryBlockBase* mHead;     // 0x08
    CriticalSection mLock;      // 0x0C
};
ASSERT_SIZE(AddressSpaceManager, 0x18);

} // namespace os
} // namespace nn

// C interface (constructs the manager in place and initializes it)
extern "C" void nnosAddressSpaceManagerInitialize(nn::os::AddressSpaceManager* manager, uptr begin, size_t size); // 0x00122B2C
