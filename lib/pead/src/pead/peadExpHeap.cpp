#include "pead/peadExpHeap.h"
#include "pead/peadHeapMgr.h"

#include <new>

namespace pead {
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
} // namespace

// 0x0053883C slot 0x1C | nintendogs:callseq
void pead::ExpHeap::free(void* ptr)
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
    mUseList.erase(block);
    block->mSize += block->mAlignOffset;
    block->mAlignOffset = 0;
    pushToFreeList_(block);
}

// 0x0053B438 slot 0x54
void pead::ExpHeap::vf_0x54()
{
    // (the original falls into Heap::vf_0x54 with a nop)
    Heap::vf_0x54();
}

// 0x0053C6F0 slot 0x24 (name after sead)
void* pead::ExpHeap::resizeBack(void* ptr, u32 newSize)
{
    if (!isInclude(ptr)) {
        return nullptr;
    }
    ConditionalScopedLock lock(&mCS, isEnableLock());
    for (MemBlock* block = mUseList.front(); block != mUseList.endObj(); block = mUseList.next(block)) {
        if (block->getMemory() != ptr) {
            continue;
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
    return nullptr;
}

// 0x0053C81C slot 0x20 (name after sead)
void* pead::ExpHeap::resizeFront(void* ptr, u32 newSize)
{
    if (!isInclude(ptr)) {
        return nullptr;
    }
    ConditionalScopedLock lock(&mCS, isEnableLock());
    for (MemBlock* block = mUseList.front(); block != mUseList.endObj(); block = mUseList.next(block)) {
        if (block->getMemory() != ptr) {
            continue;
        }
        u32 rest = block->mAlignOffset + block->mSize - newSize;
        if (rest < sizeof(MemBlock)) {
            // (the original moves the start without changing the size)
            block->setAlignOffset(rest);
            return block->getMemory();
        }
        // the used block at the end, the part in front becomes free
        MemBlock* used = new (reinterpret_cast<u8*>(block) + rest) MemBlock();
        used->mSize = newSize;
        mUseList.pushBack(used);
        mUseList.erase(block);
        block->mSize = rest - sizeof(MemBlock);
        block->mAlignOffset = 0;
        pushToFreeList_(block);
        return used->getMemory();
    }
    return nullptr;
}

// 0x0053C9A4 slot 0x58 (name after sead)
void pead::ExpHeap::setAllocMode(FindMode mode)
{
    mAllocMode = mode;
}

// 0x0053C9AC | nintendogs:bytes [tier A]
pead::MemBlock* pead::ExpHeap::allocFromHead_(u32 size, s32 alignment)
{
    MemBlock* block = findFreeMemBlockFromHead_(size, alignment, static_cast<FindMode>(mAllocMode));
    if (block == nullptr) {
        return nullptr;
    }
    u8* memory = block->getMemory();
    u32 offset = alignUp(reinterpret_cast<uptr>(memory), alignment) - reinterpret_cast<uptr>(memory);
    u32 rest = block->mSize - (size + offset);
    block->setAlignOffset(offset);
    block->mSize = size;
    mFreeList.erase(block);
    mUseList.pushBack(block);
    splitRest_(block, block->getMemory() + size, rest, size);
    return block;
}

// 0x0053CAAC | nintendogs:bytes [tier A]
pead::MemBlock* pead::ExpHeap::allocFromTail_(u32 size)
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
        mUseList.pushBack(used);
        return used;
    }
    mFreeList.erase(block);
    mUseList.pushBack(block);
    return block;
}

// 0x0053CC14 | nintendogs:bytes [tier B]
pead::MemBlock* pead::ExpHeap::allocFromTail_(u32 size, s32 alignment)
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
        mUseList.pushBack(used);
        return used;
    }
    block->setAlignOffset(rest);
    block->mSize = usedSize;
    mFreeList.erase(block);
    mUseList.pushBack(block);
    return block;
}

// 0x0053CD1C | nintendogs:bytes [tier A]
void pead::ExpHeap::pushToFreeList_(MemBlock* block)
{
    MemBlock* it = mFreeList.front();
    while (it != mFreeList.endObj()) {
        if (it > block) {
            mFreeList.insertBefore(it, block);
            mergeFreeList_();
            return;
        }
        it = mFreeList.next(it);
    }
    mFreeList.pushBack(block);
    mergeFreeList_();
}

// 0x0053CDA4 | nintendogs:bytes [tier A]
void pead::ExpHeap::mergeFreeList_()
{
    MemBlock* block = mFreeList.front();
    while (block != mFreeList.endObj()) {
        MemBlock* next = mFreeList.next(block);
        u8* end = block->getMemory() + block->mSize;
        if (next == mFreeList.endObj()) {
            return;
        }
        if (end != reinterpret_cast<u8*>(next)) {
            block = next;
            continue;
        }
        block->mSize += next->mAlignOffset + next->mSize + sizeof(MemBlock);
        mFreeList.erase(next);
    }
}

// 0x0053CE50 (name after sead)
s32 pead::ExpHeap::compareMemBlockAddress_(const void* a, const void* b)
{
    return static_cast<const u8*>(a) - static_cast<const u8*>(b) >= 0 ? 1 : -1;
}

// 0x0053CE64 | nintendogs:bytes [tier A]
void pead::ExpHeap::createMaxSizeFreeMemBlock_(ExpHeap* heap)
{
    ConditionalScopedLock lock(&heap->mCS, heap->isEnableLock());
    void* address = heap->mDirection == HEAP_DIRECTION_FORWARD ? static_cast<void*>(heap + 1) : heap->mStart;
    MemBlock* block = new (address) MemBlock();
    block->mSize = heap->mSize - sizeof(ExpHeap) - sizeof(MemBlock);
    heap->pushToFreeList_(block);
}

// 0x0053CEF8 slot 0x14 | nintendogs:bytes-fuzzy
u32 pead::ExpHeap::adjust()
{
    if (mParent == nullptr) {
        return mSize;
    }
    ConditionalScopedLock lock(&mCS, isEnableLock());
    mUseList.sort(compareMemBlockAddress_);
    if (mDirection == HEAP_DIRECTION_FORWARD) {
        // the free block at the end goes back to the parent
        MemBlock* lastFree = mFreeList.backOrNull();
        MemBlock* lastUsed = mUseList.backOrNull();
        if (lastFree <= lastUsed || lastFree == nullptr) {
            return mSize;
        }
        u32 newSize = reinterpret_cast<u8*>(lastFree) - static_cast<u8*>(mStart);
        mFreeList.erase(lastFree);
        if (mParent->resizeBack(mStart, newSize) == nullptr) {
            return mSize;
        }
        mSize = newSize;
        return newSize;
    }
    // the free block at the start goes back to the parent
    MemBlock* firstFree = mFreeList.frontOrNull();
    MemBlock* firstUsed = mUseList.frontOrNull();
    if (firstFree >= firstUsed || firstFree == nullptr) {
        return mSize;
    }
    u32 newSize = mSize - (firstFree->mAlignOffset + firstFree->mSize) - sizeof(MemBlock);
    mFreeList.erase(firstFree);
    void* start = mParent->resizeFront(mStart, newSize);
    if (start == nullptr) {
        return mSize;
    }
    mStart = start;
    mSize = newSize;
    return newSize;
}

// 0x0053D0CC slot 0x10 (name after sead)
void pead::ExpHeap::destroy()
{
    ScopedLock treeLock(HeapMgr::getHeapTreeLockCS_());
    Heap* parent = mParent;
    void* start = mStart;
    this->~ExpHeap();
    if (parent != nullptr && parent->isFreeable()) {
        parent->free(start);
    }
}

// 0x0053D13C slot 0x28 | nintendogs:callseq
void pead::ExpHeap::freeAll()
{
    ConditionalScopedLock lock(&mCS, isEnableLock());
    dispose_(nullptr, nullptr);
    mUseList.clear();
    mFreeList.clear();
    createMaxSizeFreeMemBlock_(this);
}

// 0x0053D1B8 slot 0x18 | nintendogs:bytes-fuzzy
void* pead::ExpHeap::tryAlloc(u32 size, s32 alignment)
{
    HeapMgr::AllocFailedCallback* callback = nullptr;
    HeapMgr::AllocFailedCallbackArg arg;
    if (HeapMgr::s_pInstance != nullptr) {
        callback = HeapMgr::s_pInstance->mAllocFailedCallback;
    }
    if (callback != nullptr) {
        arg.heap = this;
        arg.requestSize = size;
        arg.requestAlignment = alignment;
    }
    if (size < 4) {
        size = 4;
    }
    if (mSize <= size || !isPowerOfTwo(absAlignment(alignment))) {
        if (callback != nullptr) {
            arg.allocSize = size;
            arg.allocAlignment = alignment;
            callback->invoke(&arg);
        }
        return nullptr;
    }
    size = (size + 3) & ~3u;
    ConditionalScopedLock lock(&mCS, isEnableLock());
    s32 align = mDirection * alignment;
    if (callback != nullptr) {
        arg.allocSize = size;
        arg.allocAlignment = align;
    }
    MemBlock* block;
    if (align >= 0) {
        if (align <= 4) {
            block = allocFromHead_(size);
        } else {
            block = allocFromHead_(size, align);
        }
    } else {
        align = -align;
        if (align <= 4) {
            block = allocFromTail_(size);
        } else {
            block = allocFromTail_(size, align);
        }
    }
    if (block != nullptr) {
        return block->getMemory();
    }
    if (callback != nullptr) {
        callback->invoke(&arg);
    }
    return nullptr;
}

// 0x0053D400 | nintendogs:callseq [tier A]
pead::ExpHeap* pead::ExpHeap::tryCreate(void* start, u32 size, const SafeStringBase<char>& name, bool enableLock)
{
    ScopedLock treeLock(HeapMgr::getHeapTreeLockCS_());
    size = (size + 3) & ~3u;
    if (size <= sizeof(ExpHeap) + sizeof(MemBlock)) {
        return nullptr;
    }
    ExpHeap* heap = new (start) ExpHeap(name, nullptr, start, size, HEAP_DIRECTION_FORWARD, enableLock);
    createMaxSizeFreeMemBlock_(heap);
    return heap;
}

// 0x0053D490 | nintendogs:callseq (parameter names are ours)
pead::ExpHeap* pead::ExpHeap::tryCreate(u32 size, const SafeStringBase<char>& name, Heap* parent,
                                        HeapDirection direction, bool enableLock)
{
    ScopedLock treeLock(HeapMgr::getHeapTreeLockCS_());
    if (parent == nullptr) {
        parent = HeapMgr::s_pInstance->getCurrentHeap();
        if (parent == nullptr) {
            return nullptr;
        }
    }
    if (size == 0) {
        size = parent->getMaxAllocatableSize(4);
    }
    size = (size + 3) & ~3u;
    if (size <= sizeof(ExpHeap) + sizeof(MemBlock)) {
        return nullptr;
    }
    void* memory = parent->tryAlloc(size, direction * 4);
    if (memory == nullptr) {
        return nullptr;
    }
    // the heap object at the start or at the end of its memory
    void* heapAddress =
        direction == HEAP_DIRECTION_FORWARD ? memory : static_cast<u8*>(memory) + size - sizeof(ExpHeap);
    ExpHeap* heap = new (heapAddress) ExpHeap(name, parent, memory, size, direction, enableLock);
    createMaxSizeFreeMemBlock_(heap);
    return heap;
}

// 0x0053D59C | nintendogs:bytes [tier A]
pead::ExpHeap::ExpHeap(const SafeStringBase<char>& name, Heap* parent, void* start, u32 size,
                       HeapDirection direction, bool enableLock)
    : Heap(name, parent, start, size, direction, enableLock), mAllocMode(FIND_MODE_FIRST_FIT)
{
    ConditionalScopedLock lock(&mCS, isEnableLock());
    mFreeList.initOffset(offsetof(MemBlock, mListNode));
    mUseList.initOffset(offsetof(MemBlock, mListNode));
}

// 0x0053D66C slot 0x00 | slot vf_0x00 of pead::IDisposer
// 0x0053D640 (deleting dtor)
pead::ExpHeap::~ExpHeap()
{
    destruct_();
}

// 0x007495C4 slot 0x44 (name after sead)
bool pead::ExpHeap::isFreeable() const
{
    return true;
}

// 0x007495CC | nintendogs:bytes [tier A]
void pead::ExpHeap::dumpUseList() const
{
    ConditionalScopedLock lock(const_cast<CriticalSection*>(&mCS), isEnableLock());
    for (MemBlock* it = mUseList.front(); it != mUseList.endObj(); it = mUseList.next(it)) {
        // (the output is not in the release build)
    }
}

// 0x00749638 slot 0x38 | nintendogs:bytes
u32 pead::ExpHeap::getFreeSize() const
{
    ConditionalScopedLock lock(const_cast<CriticalSection*>(&mCS), isEnableLock());
    u32 size = 0;
    for (MemBlock* it = mFreeList.front(); it != mFreeList.endObj(); it = mFreeList.next(it)) {
        size += it->mSize;
    }
    return size;
}

// 0x007496B0 slot 0x48 (name after sead)
bool pead::ExpHeap::isResizable() const
{
    return true;
}

// 0x007496B8 | nintendogs:bytes [tier A]
void pead::ExpHeap::dumpFreeList() const
{
    ConditionalScopedLock lock(const_cast<CriticalSection*>(&mCS), isEnableLock());
    for (MemBlock* it = mFreeList.front(); it != mFreeList.endObj(); it = mFreeList.next(it)) {
        // (the output is not in the release build)
    }
}

// 0x00749724 slot 0x5C (name after sead)
pead::ExpHeap::FindMode pead::ExpHeap::getAllocMode() const
{
    return static_cast<FindMode>(mAllocMode);
}

// 0x0074972C slot 0x4C (name after sead)
bool pead::ExpHeap::isAdjustable() const
{
    return true;
}

// 0x00749734 slot 0x30 (name after sead)
const void* pead::ExpHeap::getEndAddress() const
{
    return static_cast<u8*>(mStart) + mSize;
}

// 0x00749744 slot 0x2C (name after sead)
const void* pead::ExpHeap::getStartAddress() const
{
    return mStart;
}

// 0x00749798 slot 0x3C | nintendogs:bytes
u32 pead::ExpHeap::getMaxAllocatableSize(s32 alignment) const
{
    if (!isPowerOfTwo(absAlignment(alignment))) {
        return 0;
    }
    ConditionalScopedLock lock(const_cast<CriticalSection*>(&mCS), isEnableLock());
    s32 align = mDirection * alignment;
    MemBlock* block;
    if (align >= 0) {
        if (align <= 4) {
            block = findFreeMemBlockFromHead_(4, FIND_MODE_LARGEST_FIT);
        } else {
            block = findFreeMemBlockFromHead_(4, align, FIND_MODE_LARGEST_FIT);
        }
    } else {
        align = -align;
        if (align <= 4) {
            block = findFreeMemBlockFromTail_(4, FIND_MODE_LARGEST_FIT);
        } else {
            block = findFreeMemBlockFromTail_(4, align, FIND_MODE_LARGEST_FIT);
        }
    }
    if (block == nullptr) {
        return 0;
    }
    return block->mSize;
}

// 0x00749928 | nintendogs:bytes [tier A]
pead::MemBlock* pead::ExpHeap::findFreeMemBlockFromHead_(u32 size, FindMode mode) const
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

// 0x007499C4 | nintendogs:bytes [tier A]
pead::MemBlock* pead::ExpHeap::findFreeMemBlockFromHead_(u32 size, s32 alignment, FindMode mode) const
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

// 0x00749A90 | nintendogs:bytes [tier A]
pead::MemBlock* pead::ExpHeap::findFreeMemBlockFromTail_(u32 size, s32 alignment, FindMode mode) const
{
    MemBlock* found = nullptr;
    for (MemBlock* it = mFreeList.backOrNull(); it != nullptr && it != mFreeList.endObj(); it = mFreeList.prev(it)) {
        if (it->mSize < size) {
            continue;
        }
        uptr start = reinterpret_cast<uptr>(it->getMemory()) + (it->mSize - size);
        if (start - alignDown(start, alignment) + size > it->mSize) {
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

// 0x00749C24 slot 0x50 (name after sead)
void pead::ExpHeap::dump() const
{
    ConditionalScopedLock lock(const_cast<CriticalSection*>(&mCS), isEnableLock());
    dumpUseList();
    dumpFreeList();
}

// 0x00749C80 slot 0x34 (name after sead)
u32 pead::ExpHeap::getSize() const
{
    return mSize;
}

// 0x00749C88 | nintendogs:bytes
bool pead::ExpHeap::isInclude(const void* ptr) const
{
    const u8* start = static_cast<const u8*>(mStart);
    const u8* end = start + mSize;
    // the heap object is not part of the memory
    if (mDirection == HEAP_DIRECTION_FORWARD) {
        start += sizeof(ExpHeap);
    } else {
        end -= sizeof(ExpHeap);
    }
    return start <= ptr && ptr < end;
}
} // namespace pead

// the type information of the classes below Heap
// 0x008282B4
template bool pead::RuntimeTypeInfo::Derive<pead::Heap>::isDerived(const pead::RuntimeTypeInfo::Interface*) const;
