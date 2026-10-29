#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadHeap.h"
#include "sead/seadExpHeap.h"
#include "sead/seadHeapMgr.h"

#include <cstring>
#include <new>
#include "sead/seadFixedSafeString.h"

namespace sead {
// 0x00133578 (name is ours)
sead::ExpHeap::ExpHeap(const SafeStringBase<char>& name, Heap* parent, void* start, u32 size, HeapDirection direction,
                       bool enableLock)
    : Heap(name, parent, start, size, direction, enableLock), mAllocMode(0)
{
    ConditionalScopedLock lock(&mCS, isEnableLock());
    mFreeList.initOffset(offsetof(MemBlock, mListNode));
    mUseList.initOffset(offsetof(MemBlock, mListNode));
}

// 0x0013351C | libgarden [tier A]
sead::ExpHeap* sead::ExpHeap::CreateRoot(void* start, u32 size, const SafeStringBase<char>* name, bool enableLock)
{
    size = (size + 3) & ~3u;
    if (size <= sizeof(ExpHeap) + sizeof(MemBlock)) {
        return nullptr;
    }
    ExpHeap* heap = new (start) ExpHeap(*name, nullptr, start, size, HEAP_DIRECTION_FORWARD, enableLock);
    heap->initializeFreeList_();
    return heap;
}

// 0x0012CC60 | manual:ref-verified
sead::ExpHeap* sead::ExpHeap::tryCreate(u32 size, const SafeStringBase<char>& name, Heap* parent, HeapDirection direction,
                                        bool enableLock)
{
    if (parent == nullptr) {
        parent = HeapMgr::s_pInstance->getCurrentHeap();
        if (parent == nullptr) {
            return nullptr;
        }
    }
    if (size == 0) {
        size = parent->getMaxAllocatableSize(4) & ~3u;
    } else {
        size = (size + 3) & ~3u;
    }
    if (size <= sizeof(ExpHeap) + sizeof(MemBlock)) {
        return nullptr;
    }
    void* memory = parent->tryAlloc(size, direction * 4);
    if (memory == nullptr) {
        return nullptr;
    }
    // the heap object at the start or at the end of its memory (seen from the parent)
    if (parent->mDirection == HEAP_DIRECTION_REVERSE) {
        direction = static_cast<HeapDirection>(-direction);
    }
    void* heapAddress = direction == HEAP_DIRECTION_FORWARD ? memory : static_cast<u8*>(memory) + size - sizeof(ExpHeap);
    ExpHeap* heap = new (heapAddress) ExpHeap(name, parent, memory, size, direction, enableLock);
    heap->initializeFreeList_();
    parent->pushBackChild_(heap);
    return heap;
}

// 0x002F6D8C | manual:inferred
sead::ExpHeap* sead::ExpHeap::create(u32 size, const SafeStringBase<char>& name, Heap* parent, HeapDirection direction,
                                     bool enableLock)
{
    return tryCreate(size, name, parent, direction, enableLock);
}

// 0x00133484 (name is ours)
void sead::ExpHeap::initializeFreeList_()
{
    ConditionalScopedLock lock(&mCS, isEnableLock());
    void* address = mDirection == HEAP_DIRECTION_FORWARD ? static_cast<void*>(this + 1) : mStart;
    MemBlock* block = new (address) MemBlock();
    block->mSize = mSize - sizeof(ExpHeap) - sizeof(MemBlock);
    pushToFreeList_(block);
}

namespace {
bool isPowerOfTwo(u32 x)
{
    return (x & (x - 1)) == 0;
}

u32 absAlignment(s32 alignment)
{
    return alignment < 0 ? -alignment : alignment;
}

uptr alignUp(uptr value, u32 alignment)
{
    return (value + alignment - 1) & ~(alignment - 1);
}

uptr alignDown(uptr value, u32 alignment)
{
    return value & ~(alignment - 1);
}

// the size that the copies of tryRealloc move (name is ours)
void* moveTo(ExpHeap* heap, void* ptr, u8* memory, u32 newSize, s32 alignment, u32 copySize)
{
    void* newMemory = heap->tryAlloc(newSize, alignment);
    if (newMemory == nullptr) {
        return nullptr;
    }
    std::memcpy(newMemory, memory, copySize);
    heap->free(ptr);
    return newMemory;
}
} // namespace

// 0x00139208 (name after pead::ExpHeap)
void sead::ExpHeap::pushToFreeList_(MemBlock* block)
{
    MemBlock* prev = nullptr;
    for (MemBlock* it = mFreeList.front(); it != mFreeList.endObj(); it = mFreeList.next(it)) {
        if (it > block) {
            bool isMerged = false;
            if (prev != nullptr && prev->getMemoryEnd() == reinterpret_cast<u8*>(block)) {
                prev->mSize += block->mAlignOffset + block->mSize + sizeof(MemBlock);
                block = prev;
                isMerged = true;
            }
            if (block->getMemoryEnd() == reinterpret_cast<u8*>(it)) {
                if (!isMerged) {
                    mFreeList.insertBefore(it, block);
                }
                block->mSize += it->mAlignOffset + it->mSize + sizeof(MemBlock);
                mFreeList.erase(it);
            } else if (!isMerged) {
                mFreeList.insertBefore(it, block);
            }
            return;
        }
        prev = it;
    }
    if (mFreeList.mCount != 0 && prev->getMemoryEnd() == reinterpret_cast<u8*>(block)) {
        prev->mSize += block->mAlignOffset + block->mSize + sizeof(MemBlock);
        return;
    }
    mFreeList.pushBack(block);
}

// 0x0055E718 (name after pead::ExpHeap)
sead::ExpHeap::MemBlock* sead::ExpHeap::allocFromHead_(u32 size, s32 alignment)
{
    MemBlock* block = findFreeMemBlockFromHead_(size, alignment, static_cast<FindMode>(mAllocMode));
    if (block == nullptr) {
        return nullptr;
    }
    u8* memory = block->getMemory();
    u32 offset = alignUp(reinterpret_cast<uptr>(memory), alignment) - reinterpret_cast<uptr>(memory);
    u32 rest = block->mSize - (size + offset);
    if (offset >= 0x10000) {
        // the offset does not fit: the part in front stays a free block
        block->mAlignOffset = 0;
        block->mSize = offset - sizeof(MemBlock);
        block = new (reinterpret_cast<u8*>(block) + offset) MemBlock();
        block->mAlignOffset = 0;
        block->mSize = size;
    } else {
        block->setAlignOffset(offset);
        block->mSize = size;
        mFreeList.erase(block);
    }
    pushToUseList_(block);
    splitRest_(block, block->getMemory() + size, rest, size);
    return block;
}

// 0x0055E85C (name after pead::ExpHeap)
sead::ExpHeap::MemBlock* sead::ExpHeap::allocFromTail_(u32 size)
{
    MemBlock* block = findFreeMemBlockFromTail_(size, static_cast<FindMode>(mAllocMode));
    if (block == nullptr) {
        return nullptr;
    }
    u32 rest = block->mSize - size;
    if (rest > sizeof(MemBlock)) {
        // the used block at the end of the free one
        block->mSize = rest - sizeof(MemBlock);
        MemBlock* used = new (block->getMemory() + block->mSize) MemBlock();
        used->mSize = size;
        pushToUseList_(used);
        return used;
    }
    mFreeList.erase(block);
    pushToUseList_(block);
    return block;
}

// 0x0055E9A4 (name after pead::ExpHeap)
sead::ExpHeap::MemBlock* sead::ExpHeap::allocFromTail_(u32 size, s32 alignment)
{
    MemBlock* block = findFreeMemBlockFromTail_(size, alignment, static_cast<FindMode>(mAllocMode));
    if (block == nullptr) {
        return nullptr;
    }
    uptr start = reinterpret_cast<uptr>(block->getMemory()) + (block->mSize - size);
    u32 usedSize = size + (start - alignDown(start, alignment));
    u32 rest = block->mSize - usedSize;
    if (rest > sizeof(MemBlock)) {
        block->mSize = rest - sizeof(MemBlock);
        MemBlock* used = new (block->getMemory() + block->mSize) MemBlock();
        used->mSize = usedSize;
        pushToUseList_(used);
        return used;
    }
    block->setAlignOffset(rest);
    block->mSize = usedSize;
    mFreeList.erase(block);
    pushToUseList_(block);
    return block;
}

// 0x0055EA8C (name is ours)
void sead::ExpHeap::pushToUseList_(MemBlock* block)
{
    if (mDirection == HEAP_DIRECTION_FORWARD) {
        mUseList.pushBack(block);
    } else {
        mUseList.pushFront(block);
    }
}

// 0x0055EAD8 (name is ours)
s32 sead::ExpHeap::compareMemBlockAddress_(const void* a, const void* b)
{
    return static_cast<const u8*>(a) - static_cast<const u8*>(b) >= 0 ? 1 : -1;
}

// 0x0074F304 (name after pead::ExpHeap)
sead::ExpHeap::MemBlock* sead::ExpHeap::findFreeMemBlockFromHead_(u32 size, FindMode mode) const
{
    MemBlock* found = nullptr;
    for (MemBlock* it = mFreeList.front(); it != mFreeList.endObj(); it = mFreeList.next(it)) {
        if (size > it->mSize) {
            continue;
        }
        if (mode == FIND_MODE_FIRST_FIT) {
            return it;
        }
        if (found == nullptr) {
            found = it;
        } else if (mode == FIND_MODE_SMALLEST_FIT) {
            if (found->mSize > it->mSize) {
                found = it;
            }
        } else if (mode == FIND_MODE_LARGEST_FIT) {
            if (found->mSize < it->mSize) {
                found = it;
            }
        }
    }
    return found;
}

// 0x0074F3A8 (name after pead::ExpHeap)
sead::ExpHeap::MemBlock* sead::ExpHeap::findFreeMemBlockFromHead_(u32 size, s32 alignment, FindMode mode) const
{
    MemBlock* found = nullptr;
    for (MemBlock* it = mFreeList.front(); it != mFreeList.endObj(); it = mFreeList.next(it)) {
        if (it->mSize < size) {
            continue;
        }
        uptr memory = reinterpret_cast<uptr>(it->getMemory());
        if (alignUp(memory, alignment) - memory + size > it->mSize) {
            continue;
        }
        if (mode == FIND_MODE_FIRST_FIT) {
            return it;
        }
        if (found == nullptr) {
            found = it;
        } else if (mode == FIND_MODE_SMALLEST_FIT) {
            if (found->mSize > it->mSize) {
                found = it;
            }
        } else if (mode == FIND_MODE_LARGEST_FIT) {
            if (found->mSize < it->mSize) {
                found = it;
            }
        }
    }
    return found;
}

// 0x0074F478 (name after pead::ExpHeap)
sead::ExpHeap::MemBlock* sead::ExpHeap::findFreeMemBlockFromTail_(u32 size, s32 alignment, FindMode mode) const
{
    MemBlock* found = nullptr;
    for (MemBlock* it = mFreeList.backOrNull(); it != nullptr && it != mFreeList.endObj(); it = mFreeList.prev(it)) {
        if (it->mSize >= size) {
            uptr start = reinterpret_cast<uptr>(it->getMemory()) + (it->mSize - size);
            if (start - alignDown(start, alignment) + size <= it->mSize) {
                if (mode == FIND_MODE_FIRST_FIT) {
                    return it;
                }
                if (found == nullptr) {
                    found = it;
                } else if (mode == FIND_MODE_SMALLEST_FIT) {
                    if (found->mSize > it->mSize) {
                        found = it;
                    }
                } else if (mode == FIND_MODE_LARGEST_FIT) {
                    if (found->mSize < it->mSize) {
                        found = it;
                    }
                }
            }
        }
    }
    return found;
}

inline sead::ExpHeap::MemBlock* sead::ExpHeap::findFreeMemBlockFromTail_(u32 size, FindMode mode) const
{
    MemBlock* found = nullptr;
    for (MemBlock* it = mFreeList.backOrNull(); it != nullptr && it != mFreeList.endObj(); it = mFreeList.prev(it)) {
        if (size <= it->mSize) {
            if (mode == FIND_MODE_FIRST_FIT) {
                return it;
            }
            if (found == nullptr) {
                found = it;
            } else if (mode == FIND_MODE_SMALLEST_FIT) {
                if (found->mSize > it->mSize) {
                    found = it;
                }
            } else if (mode == FIND_MODE_LARGEST_FIT) {
                if (found->mSize < it->mSize) {
                    found = it;
                }
            }
        }
    }
    return found;
}

// 0x0074EF30 (name after pead::ExpHeap)
void sead::ExpHeap::dumpUseList() const
{
    ConditionalScopedLock lock(const_cast<CriticalSection*>(&mCS), isEnableLock());
    for (MemBlock* it = mUseList.front(); it != mUseList.endObj(); it = mUseList.next(it)) {
        // (the print of the block is not in the release build)
    }
}

// 0x0074F028 (name after pead::ExpHeap)
void sead::ExpHeap::dumpFreeList() const
{
    ConditionalScopedLock lock(const_cast<CriticalSection*>(&mCS), isEnableLock());
    for (MemBlock* it = mFreeList.front(); it != mFreeList.endObj(); it = mFreeList.next(it)) {
        // (the print of the block is not in the release build)
    }
}

// 0x0055F184 slot 0x00 | slot vf_0x00 of sead::IDisposer
// 0x0055F154 (deleting dtor)
sead::ExpHeap::~ExpHeap()
{
    destruct_();
}

// 0x0055EDF4 slot 0x10
void sead::ExpHeap::destroy()
{
    ScopedLock treeLock(HeapMgr::getHeapTreeLockCS_());
    Heap* parent = mParent;
    void* start = mStart;
    this->~ExpHeap();
    if (parent != nullptr && parent->isFreeable()) {
        parent->free(start);
    }
}

// 0x0055EBF0 slot 0x14 (name after pead::ExpHeap)
u32 sead::ExpHeap::adjust()
{
    if (mParent == nullptr) {
        return mSize;
    }
    ConditionalScopedLock lock(&mCS, isEnableLock());
    Heap* parent = mParent;
    if (parent->isEnableLock()) {
        parent->mCS.lock();
    }
    u32 newSize = mSize;
    mUseList.sort(compareMemBlockAddress_);
    if (mDirection == HEAP_DIRECTION_FORWARD) {
        // the free block at the end goes back to the parent
        MemBlock* lastFree = mFreeList.backOrNull();
        MemBlock* lastUsed = mUseList.backOrNull();
        if (reinterpret_cast<uptr>(lastFree) > reinterpret_cast<uptr>(lastUsed) && lastFree != nullptr) {
            u32 size = reinterpret_cast<u8*>(lastFree) - static_cast<u8*>(mStart);
            mFreeList.erase(lastFree);
            if (mParent->resizeBack(mStart, size) != nullptr) {
                mSize = size;
                newSize = size;
            }
        }
    } else {
        // the free block at the start goes back to the parent
        MemBlock* firstFree = mFreeList.frontOrNull();
        MemBlock* firstUsed = mUseList.frontOrNull();
        if ((firstUsed == nullptr || reinterpret_cast<uptr>(firstUsed) > reinterpret_cast<uptr>(firstFree)) && firstFree != nullptr) {
            u32 size = mSize - (firstFree->mAlignOffset + firstFree->mSize) - sizeof(MemBlock);
            mFreeList.erase(firstFree);
            void* start = mParent->resizeFront(mStart, size);
            if (start != nullptr) {
                mSize = size;
                mStart = start;
                newSize = size;
            }
        }
    }
    if (parent->isEnableLock()) {
        parent->mCS.unlock();
    }
    return newSize;
}

// 0x0055EEE0 slot 0x18 (name after pead::ExpHeap)
void* sead::ExpHeap::tryAlloc(u32 size, s32 alignment)
{
    HeapMgr* mgr = HeapMgr::s_pInstance;
    u32 allocSize = size < 4 ? 4 : size;
    if (mSize <= allocSize || !isPowerOfTwo(absAlignment(alignment))) {
        HeapMgr::callAllocFailedCallback_(mgr, this, size, alignment, allocSize, alignment);
        return nullptr;
    }
    allocSize = (allocSize + 3) & ~3u;
    ConditionalScopedLock lock(&mCS, isEnableLock());
    s32 align = mDirection * alignment;
    MemBlock* block;
    if (align >= 0) {
        if (align <= 4) {
            block = allocFromHead_(allocSize);
        } else {
            block = allocFromHead_(allocSize, align);
        }
    } else {
        align = -align;
        if (align <= 4) {
            block = allocFromTail_(allocSize);
        } else {
            block = allocFromTail_(allocSize, align);
        }
    }
    if (block == nullptr) {
        HeapMgr::callAllocFailedCallback_(mgr, this, size, alignment, allocSize, align);
        return nullptr;
    }
    block->mHeapCheckTag = mHeapId;
    return block->getMemory();
}

// 0x0055EAEC slot 0x1C (name after pead::ExpHeap)
void sead::ExpHeap::free(void* ptr)
{
    if (ptr == nullptr) {
        return;
    }
    if (!isInclude(ptr)) {
        return;
    }
    if ((mFlag & FLAG_DISPOSING) != 0) {
        return;
    }
    ConditionalScopedLock lock(&mCS, isEnableLock());
    MemBlock* block = MemBlock::fromMemory(ptr);
    if (block == nullptr) {
        dumpUseList();
        dumpFreeList();
        return;
    }
    if (block->mHeapCheckTag != mHeapId) {
        return;
    }
    mUseList.erase(block);
    block->mSize += block->mAlignOffset;
    block->mAlignOffset = 0;
    pushToFreeList_(block);
}

// 0x0055E588 slot 0x20
void* sead::ExpHeap::resizeFront(void* ptr, u32 newSize)
{
    if (!isInclude(ptr)) {
        return nullptr;
    }
    ConditionalScopedLock lock(&mCS, isEnableLock());
    MemBlock* block = MemBlock::fromMemory(ptr);
    newSize = (newSize + 3) & ~3u;
    if (block->mSize < newSize) {
        return nullptr;
    }
    if (block->mSize == newSize) {
        return block->getMemory();
    }
    u32 rest = block->mSize + block->mAlignOffset - newSize;
    if (rest < sizeof(MemBlock)) {
        block->setAlignOffset(rest);
        return block->getMemory();
    }
    // the used block at the end, the part in front becomes free
    MemBlock* used = new (reinterpret_cast<u8*>(block) + rest) MemBlock();
    used->mHeapCheckTag = mHeapId;
    used->mSize = newSize;
    pushToUseList_(used);
    mUseList.erase(block);
    block->mSize = rest - sizeof(MemBlock);
    block->mAlignOffset = 0;
    pushToFreeList_(block);
    return used->getMemory();
}

// 0x0055E19C slot 0x24
void* sead::ExpHeap::resizeBack(void* ptr, u32 newSize)
{
    if (!isInclude(ptr)) {
        return nullptr;
    }
    ConditionalScopedLock lock(&mCS, isEnableLock());
    MemBlock* block = MemBlock::fromMemory(ptr);
    newSize = (newSize + 3) & ~3u;
    if (block->mSize < newSize) {
        return nullptr;
    }
    if (block->mSize == newSize) {
        return block->getMemory();
    }
    u32 rest = block->mSize - newSize;
    if (rest < sizeof(MemBlock)) {
        return block->getMemory();
    }
    block->mSize = newSize;
    MemBlock* free = new (block->getMemory() + newSize) MemBlock();
    free->mSize = rest - sizeof(MemBlock);
    pushToFreeList_(free);
    return block->getMemory();
}

// 0x0055E2D0 slot 0x28
void* sead::ExpHeap::tryRealloc(void* ptr, u32 newSize, s32 alignment)
{
    if (ptr == nullptr) {
        return tryAlloc(newSize, alignment);
    }
    if (newSize == 0) {
        free(ptr);
        return nullptr;
    }
    if (!isInclude(ptr)) {
        return nullptr;
    }
    if (alignment < 0) {
        return nullptr;
    }
    ConditionalScopedLock lock(&mCS, isEnableLock());
    MemBlock* block = MemBlock::fromMemory(ptr);
    u32 size = (newSize + 3) & ~3u;
    u32 oldSize = block->mSize;
    if (oldSize < size) {
        if (alignment == 0) {
            alignment = 4;
        }
        return moveTo(this, ptr, block->getMemory(), size, alignment, oldSize);
    }
    if (oldSize == size) {
        if (alignment != 0 && (reinterpret_cast<uptr>(block->getMemory()) & (alignment - 1)) != 0) {
            return moveTo(this, ptr, block->getMemory(), size, alignment, oldSize);
        }
        return block->getMemory();
    }
    if (alignment != 0 && (reinterpret_cast<uptr>(block->getMemory()) & (alignment - 1)) != 0) {
        return moveTo(this, ptr, block->getMemory(), size, alignment, size);
    }
    u32 rest = oldSize - size;
    if (rest >= sizeof(MemBlock)) {
        block->mSize = size;
        MemBlock* free = new (block->getMemory() + size) MemBlock();
        free->mSize = rest - sizeof(MemBlock);
        pushToFreeList_(free);
    }
    return block->getMemory();
}

// 0x0055EE64 slot 0x2C (name after pead::ExpHeap)
void sead::ExpHeap::freeAll()
{
    ConditionalScopedLock lock(&mCS, isEnableLock());
    dispose_(nullptr, nullptr);
    mUseList.clear();
    mFreeList.clear();
    initializeFreeList_();
}

// 0x0074F0BC slot 0x30
const void* sead::ExpHeap::getStartAddress() const
{
    return mStart;
}

// 0x0074F0AC slot 0x34
const void* sead::ExpHeap::getEndAddress() const
{
    return static_cast<u8*>(mStart) + mSize;
}

// 0x0074F668 slot 0x38
u32 sead::ExpHeap::getSize() const
{
    return mSize;
}

// 0x0074EFA4 slot 0x3C (name after pead::ExpHeap)
u32 sead::ExpHeap::getFreeSize() const
{
    ConditionalScopedLock lock(const_cast<CriticalSection*>(&mCS), isEnableLock());
    u32 size = 0;
    for (MemBlock* it = mFreeList.front(); it != mFreeList.endObj(); it = mFreeList.next(it)) {
        size += it->mSize;
    }
    return size;
}

// 0x0074F110 slot 0x40 (name after pead::ExpHeap)
u32 sead::ExpHeap::getMaxAllocatableSize(s32 alignment) const
{
    if (!isPowerOfTwo(absAlignment(alignment))) {
        return 0;
    }
    ConditionalScopedLock lock(const_cast<CriticalSection*>(&mCS), isEnableLock());
    s32 align = mDirection * alignment;
    MemBlock* block;
    u32 offset = 0;
    if (align >= 0) {
        if (align <= 4) {
            block = findFreeMemBlockFromHead_(4, FIND_MODE_LARGEST_FIT);
        } else {
            block = findFreeMemBlockFromHead_(4, align, FIND_MODE_LARGEST_FIT);
            if (block == nullptr) {
                return 0;
            }
            uptr memory = reinterpret_cast<uptr>(block->getMemory());
            offset = alignUp(memory, align) - memory;
        }
    } else {
        align = -align;
        if (align <= 4) {
            block = findFreeMemBlockFromTail_(4, FIND_MODE_LARGEST_FIT);
        } else {
            block = findFreeMemBlockFromTail_(4, align, FIND_MODE_LARGEST_FIT);
            if (block == nullptr) {
                return 0;
            }
            uptr start = reinterpret_cast<uptr>(block->getMemory()) + block->mSize - 4;
            offset = start - alignDown(start, align);
        }
    }
    if (block == nullptr) {
        return 0;
    }
    return block->mSize - offset;
}

// 0x0074F904 slot 0x44 (name after pead::ExpHeap)
bool sead::ExpHeap::isInclude(const void* ptr) const
{
    const u8* start = static_cast<u8*>(mStart) + (mDirection == HEAP_DIRECTION_FORWARD ? sizeof(ExpHeap) : 0);
    const u8* end = static_cast<u8*>(mStart) + mSize - (mDirection == HEAP_DIRECTION_FORWARD ? 0 : sizeof(ExpHeap));
    return ptr >= start && ptr < end;
}

// 0x0074EF28 slot 0x48
bool sead::ExpHeap::isFreeable() const
{
    return true;
}

// 0x0074F020 slot 0x4C
bool sead::ExpHeap::isResizable() const
{
    return true;
}

// 0x0074F0A4 slot 0x50
bool sead::ExpHeap::isAdjustable() const
{
    return true;
}

// 0x0074F60C slot 0x54
void sead::ExpHeap::dump() const
{
    ConditionalScopedLock lock(const_cast<CriticalSection*>(&mCS), isEnableLock());
    dumpUseList();
    dumpFreeList();
}

// 0x0074F670 slot 0x58 (name is ours)
void sead::ExpHeap::dumpYAML(Stream* stream, s32 indent) const
{
    ConditionalScopedLock lock(const_cast<CriticalSection*>(&mCS), isEnableLock());
    Heap::dumpYAML(stream, indent);
    FixedSafeString<128> str(SafeStringBase<char>(""));
    str.append(' ', indent);
    str.appendWithFormat("  heap_type: ExpHeap\n");
    stream->writeDecorationText(str);
    str.clear();
    str.append(' ', indent);
    str.appendWithFormat("  alloc_mode: %s\n", getAllocMode() == FIND_MODE_FIRST_FIT ? "First Fit" : "Best Fit");
    stream->writeDecorationText(str);
    str.clear();
    str.append(' ', indent);
    str.appendWithFormat("  use_list_size: %d\n", mUseList.mCount);
    stream->writeDecorationText(str);
    str.clear();
    str.append(' ', indent);
    str.appendWithFormat("  free_list_size: %d\n", mFreeList.mCount);
    stream->writeDecorationText(str);
}

// 0x0054E1E0 slot 0x5C
void sead::ExpHeap::vf_0x5C()
{
    Heap::vf_0x5C();
}

// 0x0055E710 slot 0x64
void sead::ExpHeap::setAllocMode(FindMode mode)
{
    mAllocMode = mode;
}

// 0x0074F09C slot 0x68
sead::ExpHeap::FindMode sead::ExpHeap::getAllocMode() const
{
    return static_cast<FindMode>(mAllocMode);
}

} // namespace sead

// the type information of the classes below Heap
// 0x00829634 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::Heap>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
