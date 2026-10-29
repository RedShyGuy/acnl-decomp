#include "pead/peadHeap.h"
#include "pead/peadHeapMgr.h"

#include <cstddef>

namespace pead {
// 0x0053B3D0 | nintendogs:bytes [tier A]
void pead::Heap::appendDisposer_(IDisposer* disposer)
{
    ConditionalScopedLock lock(&mCS, isEnableLock());
    mDisposerList.pushBack(disposer);
}

// 0x0053B43C slot 0x54
void pead::Heap::vf_0x54()
{
    // empty (in the original too)
}

// 0x0053B440 | nintendogs:bytes [tier A]
void pead::Heap::removeDisposer_(IDisposer* disposer)
{
    ConditionalScopedLock lock(&mCS, isEnableLock());
    mDisposerList.erase(disposer);
}

// 0x0053B4A0 (name after sead)
pead::Heap* pead::Heap::findContainHeap_(const void* ptr)
{
    ScopedLock treeLock(HeapMgr::getHeapTreeLockCS_());
    if (!isInclude(ptr)) {
        return nullptr;
    }
    ConditionalScopedLock lock(&mCS, isEnableLock());
    for (Heap* child = mChildren.front(); child != mChildren.endObj(); child = mChildren.next(child)) {
        if (child->isInclude(ptr)) {
            return child->findContainHeap_(ptr);
        }
    }
    return this;
}

// 0x0053B59C | nintendogs:bytes [tier A]
void pead::Heap::dispose_(const void* begin, const void* end)
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

// 0x0053B63C | nintendogs:bytes [tier A]
void pead::Heap::destruct_()
{
    ConditionalScopedLock lock(&mCS, isEnableLock());
    dispose_(nullptr, nullptr);
    if (mParent != nullptr) {
        ConditionalScopedLock parentLock(&mParent->mCS, mParent->isEnableLock());
        mParent->mChildren.erase(this);
    }
}

// 0x0053B6EC (name after sead)
pead::Heap::Heap(const SafeStringBase<char>& name, Heap* parent, void* start, u32 size, HeapDirection direction,
                 bool enableLock)
    : IDisposer(), INamable(name), mStart(start), mSize(size), mParent(parent), mDirection(direction), mCS(),
      mFlag(FLAG_DEFAULT)
{
    if (enableLock) {
        mFlag = FLAG_DEFAULT | FLAG_ENABLE_LOCK;
    }
    ConditionalScopedLock lock(&mCS, isEnableLock());
    mChildren.initOffset(offsetof(Heap, mListNode));
    mDisposerList.initOffset(offsetof(IDisposer, mListNode));
    if (parent != nullptr) {
        ConditionalScopedLock parentLock(&parent->mCS, parent->isEnableLock());
        parent->mChildren.pushBack(this);
    }
}

// 0x0053B85C slot 0x00 | nintendogs:bytes
// 0x0053B838 (deleting dtor)
pead::Heap::~Heap()
{
    // (only the members and bases are destroyed: ExpHeap calls destruct_)
}

// 0x00749530 slot 0x50
void pead::Heap::dump() const
{
    // empty (in the original too)
}
} // namespace pead
