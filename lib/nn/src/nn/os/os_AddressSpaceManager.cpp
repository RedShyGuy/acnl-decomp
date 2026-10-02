#include "nn/os/os_AddressSpaceManager.h"

#include <new>

namespace nn {
namespace os {

// 0x00129818 | nintendogs:bytes [tier A]
uptr nn::os::AddressSpaceManager::Allocate(nn::os::MemoryBlockBase* block, size_t size, size_t margin)
{
    mLock.Enter();

    // from the top down: the first block with room above it
    MemoryBlockBase* below = 0;
    if (mHead) {
        uptr limit = mEnd;
        for (MemoryBlockBase* cur = mHead->mNext; cur; cur = cur->mNext) {
            if (size + margin <= limit - (cur->mAddress + cur->mSize)) {
                below = cur;
                break;
            }
            if (cur == mHead) {
                break;
            }
            limit = cur->mAddress - margin;
        }
    }

    uptr address;
    if (below) {
        address = below->mAddress + below->mSize + margin;
        MemoryBlockBase* highest = mHead ? mHead->mNext : 0;
        MemoryBlockBase* above = (highest == below) ? 0 : below->mPrev;
        if (above == 0) {
            // above the highest block
            if (mHead) {
                InsertAfter(mHead, block);
            } else {
                InsertFirst(block);
            }
        } else if (above == mHead) {
            InsertAfter(mHead, block);
            mHead = block;
        } else {
            InsertAfter(above, block);
        }
    } else {
        // no gap above any block: at the bottom of the space
        address = mBegin;
        if (mHead) {
            if (mBegin + size + margin > mHead->mAddress) {
                mLock.Exit();
                return 0;
            }
            InsertAfter(mHead, block);
            mHead = block;
        } else {
            if (mEnd < mBegin + size) {
                mLock.Exit();
                return 0;
            }
            InsertFirst(block);
        }
    }
    block->mAddress = address;
    block->mSize = size;
    mLock.Exit();
    return address;
}

// 0x0013E810 | nintendogs:callgraph [tier A]
void nn::os::AddressSpaceManager::Free(nn::os::MemoryBlockBase* block)
{
    mLock.Enter();
    Remove(block);
    block->mAddress = 0;
    block->mSize = 0;
    mLock.Exit();
}

// 0x0034C260 | fefates:bytes [tier B]
void nn::os::AddressSpaceManager::Switch(nn::os::MemoryBlockBase* to, nn::os::MemoryBlockBase* from)
{
    mLock.Enter();
    to->mAddress = from->mAddress;
    to->mSize = from->mSize;
    if (mHead == from) {
        if (mHead) {
            InsertAfter(mHead, to);
        } else {
            InsertFirst(to);
        }
        mHead = to;
    } else if (from) {
        InsertAfter(from, to);
    } else if (mHead) {
        InsertAfter(mHead, to);
    } else {
        InsertFirst(to);
    }
    from->mAddress = 0;
    from->mSize = 0;
    Remove(from);
    mLock.Exit();
}

} // namespace os
} // namespace nn

// 0x00122B2C | nintendogs [tier A]
extern "C" void nnosAddressSpaceManagerInitialize(nn::os::AddressSpaceManager* manager, uptr begin, size_t size)
{
    if (manager) {
        new (manager) nn::os::AddressSpaceManager;
    }
    manager->Initialize(begin, size);
}
