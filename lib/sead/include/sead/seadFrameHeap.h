#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadHeap.h"

namespace sead {
// RTTI N4sead9FrameHeapE @ 0x008D2368
// vtable 0x00907048 (vptr 0x00907050), offset_to_top 0, 25 entries
// vtable 0x009070B4 (vptr 0x009070BC), offset_to_top -24, 1 entries
//
// A heap that allocates from both ends of its free area and frees only all at once. The member
// names and the names of the static functions are ours (layout from tryCreate 0x005623E8).
class FrameHeap : public ::sead::Heap
{
public:
    // (inline in tryCreate)
    FrameHeap(const SafeStringBase<char>& name, Heap* parent, void* start, u32 size, HeapDirection direction, bool enableLock)
        : Heap(name, parent, start, size, direction, enableLock), mHeadPtr(nullptr), mTailPtr(nullptr)
    {
    }

    // a heap of size bytes of parent (the current heap for NULL; all that parent can give for size 0)
    static FrameHeap* tryCreate(u32 size, const SafeStringBase<char>& name, Heap* parent, HeapDirection direction,
                                bool enableLock); // 0x005623E8
    static FrameHeap* create(u32 size, const SafeStringBase<char>& name, Heap* parent, HeapDirection direction,
                             bool enableLock); // 0x005620CC
    virtual ~FrameHeap(); // 0x005625D0 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x005625A0 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x00750398 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return Heap::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x007502CC (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009835EC (guard 0x009835E8)
        static const RuntimeTypeInfo::Derive<Heap> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void destroy(); // 0x005620E0 slot 0x10
    virtual u32 adjust(); // 0x00561F88 slot 0x14
    virtual void* tryAlloc(u32 size, s32 alignment); // 0x005621C4 slot 0x18
    virtual void free(void* ptr); // 0x00561F84 slot 0x1C
    virtual void* resizeFront(void* ptr, u32 newSize); // 0x00561F78 slot 0x20
    virtual void* resizeBack(void* ptr, u32 newSize); // 0x00561F70 slot 0x24
    virtual void freeAll(); // 0x00562134 slot 0x2C
    virtual const void* getStartAddress() const; // 0x007502C4 slot 0x30
    virtual const void* getEndAddress() const; // 0x007502B4 slot 0x34
    virtual u32 getSize() const; // 0x00750490 slot 0x38
    virtual u32 getFreeSize() const; // 0x0075025C slot 0x3C
    virtual u32 getMaxAllocatableSize(s32 alignment) const; // 0x00750318 slot 0x40
    virtual bool isInclude(const void* ptr) const; // 0x00750858 slot 0x44
    virtual bool isFreeable() const; // 0x00750254 slot 0x48
    virtual bool isResizable() const; // 0x007502A4 slot 0x4C
    virtual bool isAdjustable() const; // 0x007502AC slot 0x50
    virtual void dump() const; // 0x0075044C slot 0x54
    virtual void vf_0x58(); // 0x00750498 slot 0x58 | virtual slot, introduced by sead::Heap
    virtual void vf_0x5C(); // 0x00561F80 slot 0x5C | virtual slot, introduced by sead::Heap

    // the free area: all the memory behind the heap object (inline)
    void initializeArea_()
    {
        mHeadPtr = static_cast<u8*>(mStart) + (mDirection == HEAP_DIRECTION_FORWARD ? sizeof(FrameHeap) : 0);
        mTailPtr = static_cast<u8*>(mStart) + mSize - (mDirection == HEAP_DIRECTION_FORWARD ? 0 : sizeof(FrameHeap));
    }

    u8* mHeadPtr; // 0x74, the free area starts here
    u8* mTailPtr; // 0x78, and ends here
};
ASSERT_SIZE(FrameHeap, 0x7C);
} // namespace sead
