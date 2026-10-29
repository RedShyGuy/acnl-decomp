#include "sead/seadHeap.h"
#include "sead/seadBufferedSafeStringBase.h"
#include "sead/seadHeapMgr.h"
#include "sead/seadFixedSafeString.h"

namespace sead {
// 0x0013334C (name is ours)
sead::Heap::Heap(const SafeStringBase<char>& name, Heap* parent, void* start, u32 size, HeapDirection direction,
                 bool enableLock)
    : IDisposer(parent, HEAP_NULL_OPTION_FIND_CONTAIN_HEAP), INamable(name), mStart(start), mSize(size), mParent(parent),
      mDirection(direction), mCS(parent), mFlag(FLAG_DEFAULT), mHeapId(HeapMgr::s_HeapIdCounter.increment())
{
    if (enableLock) {
        mFlag |= FLAG_ENABLE_LOCK;
    } else {
        mFlag &= ~FLAG_ENABLE_LOCK;
    }
    ConditionalScopedLock lock(&mCS, isEnableLock());
    mChildren.initOffset(offsetof(Heap, mListNode));
    mDisposerList.initOffset(offsetof(IDisposer, mListNode));
}

// 0x0054E424 slot 0x00 | slot vf_0x00 of sead::IDisposer
// 0x0054E3F8 (deleting dtor)
sead::Heap::~Heap()
{
    // empty (in the original too: the derived heaps call destruct_)
}

// 0x0054E1D8 (name is ours)
void* sead::Heap::tryRealloc(void*, u32, s32)
{
    return nullptr;
}

// 0x0074E860 (name is ours)
void sead::Heap::dump() const
{
    // empty (in the original too)
}

// 0x0074E864 slot 0x58 (name is ours)
void sead::Heap::dumpYAML(Stream* stream, s32 indent) const
{
    ConditionalScopedLock lock(const_cast<CriticalSection*>(&mCS), isEnableLock());
    FixedSafeString<128> str(SafeStringBase<char>(""));
    str.append(' ', indent);
    str.appendWithFormat("- name: \"%s\"\n", getName().getCString());
    stream->writeDecorationText(str);
    str.clear();
    str.append(' ', indent);
    str.appendWithFormat("  start_address: 0x%08X\n", reinterpret_cast<uptr>(getStartAddress()));
    stream->writeDecorationText(str);
    str.clear();
    str.append(' ', indent);
    str.appendWithFormat("  end_address: 0x%08X\n", reinterpret_cast<uptr>(getEndAddress()));
    stream->writeDecorationText(str);
    str.clear();
    str.append(' ', indent);
    str.appendWithFormat("  parent: %s\n", mParent != nullptr ? mParent->getName().getCString() : "--");
    stream->writeDecorationText(str);
    str.clear();
    str.append(' ', indent);
    str.appendWithFormat("  direction: %s\n", mDirection == 1 ? "Forward" : "Reverse");
    stream->writeDecorationText(str);
    str.clear();
    str.append(' ', indent);
    str.appendWithFormat("  size: %d\n", getSize());
    stream->writeDecorationText(str);
    str.clear();
    str.append(' ', indent);
    str.appendWithFormat("  free_size: %d\n", getFreeSize());
    stream->writeDecorationText(str);
    str.clear();
    str.append(' ', indent);
    str.appendWithFormat("  max_allocatable_size: %d\n", getMaxAllocatableSize(4));
    stream->writeDecorationText(str);
}

// 0x0054E1E4
void sead::Heap::vf_0x5C()
{
    // empty (in the original too)
}

// 0x0054E1E8 (name is ours)
s32 sead::Heap::formatMetaName(BufferedSafeStringBase<char>* name) const
{
    f32 free = static_cast<f32>(getFreeSize());
    f32 size = static_cast<f32>(getSize());
    s32 used = static_cast<s32>((1.0f - free / size) * 10.0f);
    return name->format("$SEAD_META_HEAP_%03d", used * 10);
}

// 0x00138D80 (name after pead::Heap)
void sead::Heap::appendDisposer_(IDisposer* disposer)
{
    ConditionalScopedLock lock(&mCS, isEnableLock());
    mDisposerList.pushBack(disposer);
}

// 0x00141CBC (name after pead::Heap)
void sead::Heap::removeDisposer_(IDisposer* disposer)
{
    ConditionalScopedLock lock(&mCS, isEnableLock());
    mDisposerList.erase(disposer);
}

// 0x0054E280 (name after pead::Heap)
void sead::Heap::dispose_(const void* begin, const void* end)
{
    mFlag |= FLAG_DISPOSING;
    IDisposer* disposer = mDisposerList.front();
    while (disposer != mDisposerList.endObj()) {
        if (disposer->mDisposerHeap != nullptr &&
            ((begin == nullptr && end == nullptr) || (begin <= disposer && disposer < end))) {
            // the destructor takes the disposer out of the list
            disposer->~IDisposer();
            disposer = mDisposerList.front();
        } else {
            disposer = mDisposerList.next(disposer);
        }
    }
    mFlag &= ~FLAG_DISPOSING;
}

// 0x0054E320 (name after pead::Heap)
void sead::Heap::destruct_()
{
    ConditionalScopedLock lock(&mCS, isEnableLock());
    dispose_(nullptr, nullptr);
    HeapMgr::removeFromFindContainHeapCache_(this);
    if (mParent != nullptr) {
        ScopedLock treeLock(HeapMgr::getHeapTreeLockCS_());
        ConditionalScopedLock parentLock(&mParent->mCS, mParent->isEnableLock());
        mParent->mChildren.erase(this);
    }
}

// 0x001332D4 (name is ours)
void sead::Heap::pushBackChild_(Heap* child)
{
    ScopedLock treeLock(HeapMgr::getHeapTreeLockCS_());
    ConditionalScopedLock lock(&mCS, isEnableLock());
    mChildren.pushBack(child);
}

// 0x0013C6B8 (name is ours)
sead::Heap* sead::Heap::findContainHeap_(const void* ptr)
{
    if (!isInclude(ptr)) {
        return nullptr;
    }
    for (Heap* child = mChildren.front(); child != mChildren.endObj(); child = mChildren.next(child)) {
        if (child->isInclude(ptr)) {
            return child->findContainHeap_(ptr);
        }
    }
    return this;
}

} // namespace sead
