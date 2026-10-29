#include "sead/seadFrameHeap.h"
#include "sead/seadHeapMgr.h"

#include <new>
#include "sead/seadFixedSafeString.h"

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
} // namespace

// 0x005623E8 (name is ours)
sead::FrameHeap* sead::FrameHeap::tryCreate(u32 size, const SafeStringBase<char>& name, Heap* parent, HeapDirection direction,
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
    if (size < sizeof(FrameHeap)) {
        return nullptr;
    }
    void* memory = parent->tryAlloc(size, direction * 4);
    if (memory == nullptr) {
        return nullptr;
    }
    if (parent->mDirection == HEAP_DIRECTION_REVERSE) {
        direction = static_cast<HeapDirection>(-direction);
    }
    void* heapAddress = direction == HEAP_DIRECTION_FORWARD ? memory : static_cast<u8*>(memory) + size - sizeof(FrameHeap);
    FrameHeap* heap = new (heapAddress) FrameHeap(name, parent, memory, size, direction, enableLock);
    {
        ConditionalScopedLock lock(&heap->mCS, heap->isEnableLock());
        heap->initializeArea_();
    }
    parent->pushBackChild_(heap);
    return heap;
}

// 0x005620CC (name is ours)
sead::FrameHeap* sead::FrameHeap::create(u32 size, const SafeStringBase<char>& name, Heap* parent, HeapDirection direction,
                                         bool enableLock)
{
    return tryCreate(size, name, parent, direction, enableLock);
}

// 0x005625D0 slot 0x00 | slot vf_0x00 of sead::IDisposer
// 0x005625A0 (deleting dtor)
sead::FrameHeap::~FrameHeap()
{
    destruct_();
}

// 0x005620E0 slot 0x10
void sead::FrameHeap::destroy()
{
    Heap* parent = mParent;
    void* start = mStart;
    this->~FrameHeap();
    if (parent != nullptr && parent->isFreeable()) {
        parent->free(start);
    }
}

// 0x00561F88 slot 0x14 (name after pead::ExpHeap)
u32 sead::FrameHeap::adjust()
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
    if (mDirection == HEAP_DIRECTION_FORWARD) {
        if (static_cast<u8*>(mStart) + mSize == mTailPtr) {
            u32 size = mHeadPtr - static_cast<const u8*>(getStartAddress());
            if (mParent->resizeBack(mStart, size) != nullptr) {
                mSize = size;
                mTailPtr = mHeadPtr;
                newSize = size;
            }
        }
    } else {
        if (mStart == mHeadPtr) {
            u32 size = static_cast<const u8*>(getEndAddress()) - mTailPtr;
            if (mParent->resizeFront(mStart, size) != nullptr) {
                mSize = size;
                mHeadPtr = mTailPtr;
                mStart = mTailPtr;
                newSize = size;
            }
        }
    }
    if (parent->isEnableLock()) {
        parent->mCS.unlock();
    }
    return newSize;
}

// 0x005621C4 slot 0x18 (name after pead::ExpHeap)
void* sead::FrameHeap::tryAlloc(u32 size, s32 alignment)
{
    u32 allocSize = ((size < 4 ? 4 : size) + 3) & ~3u;
    HeapMgr* mgr = HeapMgr::s_pInstance;
    if (!isPowerOfTwo(absAlignment(alignment))) {
        HeapMgr::callAllocFailedCallback_(mgr, this, size, alignment, allocSize, alignment);
        return nullptr;
    }
    ConditionalScopedLock lock(&mCS, isEnableLock());
    s32 align = mDirection * alignment;
    if (align >= 0) {
        u8* ptr = reinterpret_cast<u8*>((reinterpret_cast<uptr>(mHeadPtr) + align - 1) & ~(align - 1));
        u8* end = ptr + allocSize;
        if (end < ptr) {
            HeapMgr::callAllocFailedCallback_(mgr, this, size, alignment, allocSize, align);
            return nullptr;
        }
        if (mTailPtr < end) {
            HeapMgr::callAllocFailedCallback_(mgr, this, size, alignment, allocSize, align);
            return nullptr;
        }
        mHeadPtr = end;
        return ptr;
    }
    align = -align;
    u8* ptr = mTailPtr - allocSize;
    if (mTailPtr < ptr) {
        HeapMgr::callAllocFailedCallback_(mgr, this, size, alignment, allocSize, align);
        return nullptr;
    }
    ptr = reinterpret_cast<u8*>(reinterpret_cast<uptr>(ptr) & ~(align - 1));
    if (ptr < mHeadPtr) {
        HeapMgr::callAllocFailedCallback_(mgr, this, size, alignment, allocSize, align);
        return nullptr;
    }
    mTailPtr = ptr;
    return ptr;
}

// 0x00561F84 slot 0x1C (name after pead::ExpHeap)
void sead::FrameHeap::free(void*)
{
    // empty (in the original too: the blocks are only freed all at once)
}

// 0x00561F78 slot 0x20
void* sead::FrameHeap::resizeFront(void*, u32)
{
    return nullptr;
}

// 0x00561F70 slot 0x24
void* sead::FrameHeap::resizeBack(void*, u32)
{
    return nullptr;
}

// 0x00562134 slot 0x2C (name after pead::ExpHeap)
void sead::FrameHeap::freeAll()
{
    ConditionalScopedLock lock(&mCS, isEnableLock());
    dispose_(nullptr, nullptr);
    initializeArea_();
}

// 0x007502C4 slot 0x30
const void* sead::FrameHeap::getStartAddress() const
{
    return mStart;
}

// 0x007502B4 slot 0x34
const void* sead::FrameHeap::getEndAddress() const
{
    return static_cast<u8*>(mStart) + mSize;
}

// 0x00750490 slot 0x38
u32 sead::FrameHeap::getSize() const
{
    return mSize;
}

// 0x0075025C slot 0x3C (name after pead::ExpHeap)
u32 sead::FrameHeap::getFreeSize() const
{
    ConditionalScopedLock lock(const_cast<CriticalSection*>(&mCS), isEnableLock());
    return mTailPtr - mHeadPtr;
}

// 0x00750318 slot 0x40 (name after pead::ExpHeap)
u32 sead::FrameHeap::getMaxAllocatableSize(s32 alignment) const
{
    u32 align = absAlignment(alignment);
    if (!isPowerOfTwo(align)) {
        return 0;
    }
    ConditionalScopedLock lock(const_cast<CriticalSection*>(&mCS), isEnableLock());
    u8* ptr = reinterpret_cast<u8*>((reinterpret_cast<uptr>(mHeadPtr) + align - 1) & ~(align - 1));
    if (ptr > mTailPtr) {
        return 0;
    }
    return mTailPtr - ptr;
}

// 0x00750858 slot 0x44 (name after pead::ExpHeap)
bool sead::FrameHeap::isInclude(const void* ptr) const
{
    const u8* start = static_cast<u8*>(mStart) + (mDirection == HEAP_DIRECTION_FORWARD ? sizeof(FrameHeap) : 0);
    const u8* end = static_cast<u8*>(mStart) + mSize - (mDirection == HEAP_DIRECTION_FORWARD ? 0 : sizeof(FrameHeap));
    return ptr >= start && ptr < end;
}

// 0x00750254 slot 0x48
bool sead::FrameHeap::isFreeable() const
{
    return false;
}

// 0x007502A4 slot 0x4C
bool sead::FrameHeap::isResizable() const
{
    return false;
}

// 0x007502AC slot 0x50
bool sead::FrameHeap::isAdjustable() const
{
    return true;
}

// 0x0075044C slot 0x54
void sead::FrameHeap::dump() const
{
    ConditionalScopedLock lock(const_cast<CriticalSection*>(&mCS), isEnableLock());
}

// 0x00750498 slot 0x58 (name is ours)
void sead::FrameHeap::dumpYAML(Stream* stream, s32 indent) const
{
    ConditionalScopedLock lock(const_cast<CriticalSection*>(&mCS), isEnableLock());
    Heap::dumpYAML(stream, indent);
    FixedSafeString<128> str(SafeStringBase<char>(""));
    str.append(' ', indent);
    str.appendWithFormat("  heap_type: FrameHeap\n");
    stream->writeDecorationText(str);
    str.clear();
    str.append(' ', indent);
    str.appendWithFormat("  head_ptr: 0x%08X\n", reinterpret_cast<uptr>(mHeadPtr));
    stream->writeDecorationText(str);
    str.clear();
    str.append(' ', indent);
    str.appendWithFormat("  tail_ptr: 0x%08X\n", reinterpret_cast<uptr>(mTailPtr));
    stream->writeDecorationText(str);
}

// 0x00561F80 slot 0x5C
void sead::FrameHeap::vf_0x5C()
{
    Heap::vf_0x5C();
}

} // namespace sead
