#include "sead/seadUnitHeap.h"
#include "sead/seadHeapMgr.h"

#include <new>

namespace sead {
namespace {
bool isPowerOfTwo(u32 x)
{
    return (x & (x - 1)) == 0;
}

u32 absAlignment(s32 alignment)
{
    return alignment < 0 ? -alignment : alignment;
}

u32 alignUp(u32 value, u32 alignment)
{
    return (value + alignment - 1) & ~(alignment - 1);
}
} // namespace

// 0x0012CFA8 (name is ours; symbols.json: PFID_FaceDetection)
sead::UnitHeap* sead::UnitHeap::tryCreate(u32 size, const SafeStringBase<char>& name, u32 blockSize, s32 alignment,
                                          Heap* parent, bool enableLock)
{
    s32 align = alignment < 0 ? -alignment : alignment;
    if (blockSize == 0) {
        return nullptr;
    }
    if (align < 4) {
        align = 4;
    }
    if (!isPowerOfTwo(absAlignment(align))) {
        return nullptr;
    }
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
    if (alignUp(blockSize, align) + sizeof(UnitHeap) > size) {
        return nullptr;
    }
    void* memory = parent->tryAlloc(size, 4);
    if (memory == nullptr) {
        return nullptr;
    }
    UnitHeap* heap = new (memory) UnitHeap(name, parent, memory, size, blockSize, enableLock);
    heap->initialize_(align, false, parent);
    return heap;
}

// 0x002F6FC0 | manual:ref-verified
sead::UnitHeap* sead::UnitHeap::tryCreateWithBlockNum(u32 blockSize, u32 blockNum, const SafeStringBase<char>& name,
                                                      s32 alignment, Heap* parent, bool enableLock)
{
    s32 align = alignment < 0 ? -alignment : alignment;
    if (blockNum == 0 || blockSize == 0) {
        return nullptr;
    }
    if (align < 4) {
        align = 4;
    }
    if (!isPowerOfTwo(absAlignment(align))) {
        return nullptr;
    }
    if (parent == nullptr) {
        parent = HeapMgr::s_pInstance->getCurrentHeap();
        if (parent == nullptr) {
            return nullptr;
        }
    }
    u32 size = alignUp(blockSize, align) * blockNum + align + sizeof(UnitHeap);
    void* memory = parent->tryAlloc(size, 4);
    if (memory == nullptr) {
        return nullptr;
    }
    UnitHeap* heap = new (memory) UnitHeap(name, parent, memory, size, blockSize, enableLock);
    heap->initialize_(align, true, parent);
    return heap;
}

// 0x0012CE8C (name is ours)
void sead::UnitHeap::initialize_(s32 alignment, bool isWithBlockNum, Heap* parent)
{
    ConditionalScopedLock lock(&mCS, isEnableLock());
    u8* start = reinterpret_cast<u8*>(this + 1);
    u8* aligned = reinterpret_cast<u8*>(alignUp(reinterpret_cast<u32>(start), alignment));
    mAreaPtr = start;
    if (isWithBlockNum && start == aligned) {
        mAreaPtr = aligned + alignment;
    } else {
        mAreaPtr = aligned;
    }
    u32 offset = static_cast<u8*>(mAreaPtr) - reinterpret_cast<u8*>(this);
    mBlockSize = alignUp(mBlockSize, alignment);
    mAreaSize = mSize - offset;
    buildFreeList_();
    parent->pushBackChild_(this);
}

// 0x00561D1C slot 0x00 | slot vf_0x00 of sead::IDisposer
// 0x00561CEC (deleting dtor)
sead::UnitHeap::~UnitHeap()
{
    destruct_();
}

// 0x00561AA0 slot 0x10
void sead::UnitHeap::destroy()
{
    ScopedLock treeLock(HeapMgr::getHeapTreeLockCS_());
    Heap* parent = mParent;
    void* start = mStart;
    this->~UnitHeap();
    if (parent != nullptr && parent->isFreeable()) {
        parent->free(start);
    }
}

// 0x00561A98 slot 0x14 (name after pead::ExpHeap)
u32 sead::UnitHeap::adjust()
{
    return mSize;
}

// 0x00561BD0 slot 0x18 (name after pead::ExpHeap)
void* sead::UnitHeap::tryAlloc(u32 size, s32 alignment)
{
    HeapMgr* mgr = HeapMgr::s_pInstance;
    if (alignment < 0 || (mBlockSize & (alignment - 1)) != 0 || size > mBlockSize) {
        HeapMgr::callAllocFailedCallback_(mgr, this, size, alignment, size, alignment);
        return nullptr;
    }
    ConditionalScopedLock lock(&mCS, isEnableLock());
    void* block = mFreeListHead;
    if (block == nullptr) {
        HeapMgr::callAllocFailedCallback_(mgr, this, size, alignment, mBlockSize, alignment);
        return nullptr;
    }
    mFreeListHead = *static_cast<void**>(block);
    mFreeSize -= mBlockSize;
    return block;
}

// 0x00561A0C slot 0x1C (name after pead::ExpHeap)
void sead::UnitHeap::free(void* ptr)
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
    *static_cast<void**>(ptr) = mFreeListHead;
    mFreeListHead = ptr;
    mFreeSize += mBlockSize;
}

// 0x00561A00 slot 0x20
void* sead::UnitHeap::resizeFront(void*, u32)
{
    return nullptr;
}

// 0x005619F8 slot 0x24
void* sead::UnitHeap::resizeBack(void*, u32)
{
    return nullptr;
}

// 0x00561B10 slot 0x2C (name after pead::ExpHeap)
void sead::UnitHeap::freeAll()
{
    ConditionalScopedLock lock(&mCS, isEnableLock());
    dispose_(nullptr, nullptr);
    buildFreeList_();
}

// 0x0074FC00 slot 0x30
const void* sead::UnitHeap::getStartAddress() const
{
    return mStart;
}

// 0x0074FBF0 slot 0x34
const void* sead::UnitHeap::getEndAddress() const
{
    return static_cast<u8*>(mStart) + mSize;
}

// 0x0074FD60 slot 0x38
u32 sead::UnitHeap::getSize() const
{
    return mSize;
}

// 0x0074FBD8 slot 0x3C (name after pead::ExpHeap)
u32 sead::UnitHeap::getFreeSize() const
{
    return mFreeSize;
}

// 0x0074FC54 slot 0x40 (name after pead::ExpHeap)
u32 sead::UnitHeap::getMaxAllocatableSize(s32) const
{
    return mFreeListHead != nullptr ? mBlockSize : 0;
}

// 0x0074FFCC slot 0x44 (name after pead::ExpHeap)
bool sead::UnitHeap::isInclude(const void* ptr) const
{
    return ptr >= mAreaPtr && ptr < static_cast<u8*>(mAreaPtr) + mAreaSize;
}

// 0x0074FBD0 slot 0x48
bool sead::UnitHeap::isFreeable() const
{
    return true;
}

// 0x0074FBE0 slot 0x4C
bool sead::UnitHeap::isResizable() const
{
    return false;
}

// 0x0074FBE8 slot 0x50
bool sead::UnitHeap::isAdjustable() const
{
    return false;
}

// 0x0074FD1C slot 0x54
void sead::UnitHeap::dump() const
{
    ConditionalScopedLock lock(const_cast<CriticalSection*>(&mCS), isEnableLock());
}

// 0x0074FD68 slot 0x58 | virtual slot, introduced by sead::Heap
void sead::UnitHeap::vf_0x58()
{
}

// 0x00561A08 slot 0x5C
void sead::UnitHeap::vf_0x5C()
{
    Heap::vf_0x5C();
}

} // namespace sead
