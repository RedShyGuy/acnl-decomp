#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"
#include "sead/hostio/seadReflexible.h"
#include "sead/seadCriticalSection.h"
#include "sead/seadIDisposer.h"
#include "sead/seadINamable.h"
#include "sead/seadStream.h"
#include "sead/seadListImpl.h"

namespace sead {
template <typename T>
class BufferedSafeStringBase;

// RTTI N4sead4HeapE @ 0x008D1FDC
// vtable 0x00906A1C (vptr 0x00906A24), offset_to_top 0, 25 entries
// vtable 0x00906A88 (vptr 0x00906A90), offset_to_top -24, 1 entries
//
// A heap: a block of memory of the parent heap (or of the arena for the root heaps) with child heaps
// and the objects that are destroyed with it (disposers). Layout from the constructor 0x0013334C.
// The names of adjust/tryAlloc/free/freeAll/getFreeSize/getMaxAllocatableSize/isInclude and of
// appendDisposer_/removeDisposer_/dispose_/destruct_ are from the reference symbols (of pead, whose
// vtable lacks slot 0x28: its slots from 0x28 on are 4 bytes lower); the other names are ours.
class Heap : public ::sead::IDisposer, public ::sead::INamable, public ::sead::hostio::Reflexible
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

    // (called by the constructors of the derived heaps, never inline)
    DECOMP_NOINLINE Heap(const SafeStringBase<char>& name, Heap* parent, void* start, u32 size, HeapDirection direction,
                         bool enableLock); // 0x0013334C (name is ours)
    virtual ~Heap(); // 0x0054E424 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0054E3F8 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074E804 (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074E7B8 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009835A4 (guard 0x009835A0)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    // destroys the heap and gives its memory back to the parent (name is ours)
    virtual void destroy() = 0;
    // shrinks the heap to what is allocated; the new size
    virtual u32 adjust() = 0;
    virtual void* tryAlloc(u32 size, s32 alignment) = 0;
    virtual void free(void* ptr) = 0;
    // changes the size of the block ptr at its start or at its end; the memory of the block, NULL if
    // it failed (names are ours)
    virtual void* resizeFront(void* ptr, u32 newSize) = 0;
    virtual void* resizeBack(void* ptr, u32 newSize) = 0;
    // (only ExpHeap does it; name is ours)
    virtual void* tryRealloc(void* ptr, u32 newSize, s32 alignment); // 0x0054E1D8
    virtual void freeAll() = 0;
    // the memory of the heap (names are ours)
    virtual const void* getStartAddress() const = 0;
    virtual const void* getEndAddress() const = 0;
    virtual u32 getSize() const = 0;
    virtual u32 getFreeSize() const = 0;
    virtual u32 getMaxAllocatableSize(s32 alignment) const = 0;
    virtual bool isInclude(const void* ptr) const = 0;
    // what the heap can do (names are ours)
    virtual bool isFreeable() const = 0;
    virtual bool isResizable() const = 0;
    virtual bool isAdjustable() const = 0;
    // (empty in the release build; name is ours)
    virtual void dump() const; // 0x0074E860
    // writes the properties of the heap as YAML lines (indented by indent spaces) to the stream
    // (name is ours)
    virtual void dumpYAML(Stream* stream, s32 indent) const; // 0x0074E864 slot 0x58
    // (empty; name unknown)
    virtual void vf_0x5C(); // 0x0054E1E4 slot 0x5C
    // the name "$SEAD_META_HEAP_%03d" with the used part in 10 percent steps (name is ours)
    virtual s32 formatMetaName(BufferedSafeStringBase<char>* name) const; // 0x0054E1E8

    void appendDisposer_(IDisposer* disposer); // 0x00138D80 (name after pead::Heap)
    void removeDisposer_(IDisposer* disposer); // 0x00141CBC (name after pead::Heap)
    // destroys the disposers in [begin, end) (all for NULL, NULL)
    void dispose_(const void* begin, const void* end); // 0x0054E280 (name after pead::Heap)
    // the work of the destructors of the derived heaps: disposes everything, leaves the parent
    void destruct_(); // 0x0054E320 (name after pead::Heap)
    void pushBackChild_(Heap* child); // 0x001332D4 (name is ours)
    // the deepest heap below this one that contains ptr, NULL if this one does not (name is ours)
    Heap* findContainHeap_(const void* ptr); // 0x0013C6B8

    bool isEnableLock() const { return (mFlag & FLAG_ENABLE_LOCK) != 0; }

    void* mStart;                     // 0x1C
    u32 mSize;                        // 0x20
    Heap* mParent;                    // 0x24
    OffsetList<Heap> mChildren;       // 0x28
    ListNode mListNode;               // 0x38, in the children of the parent
    OffsetList<IDisposer> mDisposerList; // 0x40
    HeapDirection mDirection;         // 0x50
    CriticalSection mCS;              // 0x54
    u16 mFlag;                        // 0x70
    u16 mHeapId;                      // 0x72, from a counter of all heaps
};
ASSERT_OFFSET(Heap, mStart, 0x1C);
ASSERT_OFFSET(Heap, mCS, 0x54);
ASSERT_SIZE(Heap, 0x74);

} // namespace sead
