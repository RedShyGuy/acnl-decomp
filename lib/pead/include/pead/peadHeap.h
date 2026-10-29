#pragma once

#include "decomp.h"
#include "pead/RuntimeTypeInfo/peadRoot.h"
#include "pead/hostio/peadReflexible.h"
#include "pead/peadCriticalSection.h"
#include "pead/peadIDisposer.h"
#include "pead/peadINamable.h"
#include "pead/peadListImpl.h"

namespace pead {
// RTTI N4pead4HeapE @ 0x008D124C
// vtable 0x00904C68 (vptr 0x00904C70), offset_to_top 0, 22 entries
//
// pia's copy of sead::Heap (an older one): a block of memory of the parent heap (or of the arena for the root
// heap) with child heaps and the objects that are destroyed with it. Unlike sead the hostio base has no vtable
// (the members start at 0x18), there is no heap id, the flags are a word, the constructor links the heap into
// its parent and the vtable has no tryRealloc/dumpYAML/formatMetaName. The names of adjust/tryAlloc/free/
// freeAll/getFreeSize/getMaxAllocatableSize/isInclude/appendDisposer_/removeDisposer_/dispose_/destruct_ are
// from the reference symbols; the other names are those of our sead.
class Heap : public ::pead::IDisposer, public ::pead::INamable, public ::pead::hostio::Reflexible
{
public:
    // where the heap is in its memory (1: at the start, the blocks after it; -1: at the end)
    enum HeapDirection : s8
    {
        HEAP_DIRECTION_FORWARD = 1,
        HEAP_DIRECTION_REVERSE = -1,
    };

    // the bits of mFlag
    enum Flag
    {
        FLAG_ENABLE_LOCK = 1 << 0,
        FLAG_DISPOSING = 1 << 1,
        FLAG_DEFAULT = 1 << 2, // set by the constructor (its meaning is unknown)
    };

    // (called by the constructor of ExpHeap, never inline)
    DECOMP_NOINLINE Heap(const SafeStringBase<char>& name, Heap* parent, void* start, u32 size, HeapDirection direction,
                         bool enableLock); // 0x0053B6EC (name after sead)
    virtual ~Heap(); // 0x0053B85C slot 0x00 | nintendogs:bytes
    // 0x0053B838 slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x007494D4 (name after sead)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x00749488 (name after sead)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name after sead)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098338C (guard 0x00983388)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void destroy() = 0;
    virtual u32 adjust() = 0;
    virtual void* tryAlloc(u32 size, s32 alignment) = 0;
    virtual void free(void* ptr) = 0;
    virtual void* resizeFront(void* ptr, u32 newSize) = 0;
    virtual void* resizeBack(void* ptr, u32 newSize) = 0;
    virtual void freeAll() = 0;
    virtual const void* getStartAddress() const = 0;
    virtual const void* getEndAddress() const = 0;
    virtual u32 getSize() const = 0;
    virtual u32 getFreeSize() const = 0;
    virtual u32 getMaxAllocatableSize(s32 alignment) const = 0;
    virtual bool isInclude(const void* ptr) const = 0;
    virtual bool isFreeable() const = 0;
    virtual bool isResizable() const = 0;
    virtual bool isAdjustable() const = 0;
    // (empty in the release build)
    virtual void dump() const; // 0x00749530 slot 0x50
    // (empty; name unknown, the slot 0x5C of sead)
    virtual void vf_0x54(); // 0x0053B43C slot 0x54

    void appendDisposer_(IDisposer* disposer); // 0x0053B3D0 | nintendogs:bytes [tier A]
    void removeDisposer_(IDisposer* disposer); // 0x0053B440 | nintendogs:bytes [tier A]
    // destroys the disposers in [begin, end) (all for NULL, NULL)
    void dispose_(const void* begin, const void* end); // 0x0053B59C | nintendogs:bytes [tier A]
    // the work of the destructors of the derived heaps: disposes everything, leaves the parent
    void destruct_(); // 0x0053B63C | nintendogs:bytes [tier A]
    // the deepest heap below this one that contains ptr, NULL if this one does not (under the lock of the heap
    // tree; name after sead)
    Heap* findContainHeap_(const void* ptr); // 0x0053B4A0

    bool isEnableLock() const { return (mFlag & FLAG_ENABLE_LOCK) != 0; }

    void* mStart;                        // 0x18
    u32 mSize;                           // 0x1C
    Heap* mParent;                       // 0x20
    OffsetList<Heap> mChildren;          // 0x24
    ListNode mListNode;                  // 0x34, in the children of the parent
    OffsetList<IDisposer> mDisposerList; // 0x3C
    HeapDirection mDirection;            // 0x4C
    CriticalSection mCS;                 // 0x50
    u32 mFlag;                           // 0x6C
};
ASSERT_OFFSET(Heap, mStart, 0x18);
ASSERT_OFFSET(Heap, mCS, 0x50);
ASSERT_SIZE(Heap, 0x70);
} // namespace pead
