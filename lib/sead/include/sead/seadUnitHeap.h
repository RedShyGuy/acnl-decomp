#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadHeap.h"

namespace sead {
// RTTI N4sead8UnitHeapE @ 0x008D224C
// vtable 0x00906E80 (vptr 0x00906E88), offset_to_top 0, 25 entries
// vtable 0x00906EEC (vptr 0x00906EF4), offset_to_top -24, 1 entries
class UnitHeap : public ::sead::Heap
{
public:
    // (inline in the creation functions)
    UnitHeap(const SafeStringBase<char>& name, Heap* parent, void* start, u32 size, u32 blockSize, bool enableLock)
        : Heap(name, parent, start, size, HEAP_DIRECTION_FORWARD, enableLock), mBlockSize(blockSize), mAreaPtr(nullptr),
          mAreaSize(0), mFreeSize(0), mFreeListHead(nullptr), mFirstBlock(nullptr)
    {
    }

    // a heap of size bytes of parent with blocks of blockSize (the current heap for NULL; all that
    // parent can give for size 0)
    static UnitHeap* tryCreate(u32 size, const SafeStringBase<char>& name, u32 blockSize, s32 alignment, Heap* parent,
                               bool enableLock); // 0x0012CFA8 (name is ours)
    // tryCreate under the name of the variant that must not fail (as ExpHeap::create; name is ours)
    static UnitHeap* create(u32 size, const SafeStringBase<char>& name, u32 blockSize, s32 alignment, Heap* parent,
                            bool enableLock); // 0x001261E0 (name is ours)
    // a heap with blockNum blocks of blockSize
    static UnitHeap* tryCreateWithBlockNum(u32 blockSize, u32 blockNum, const SafeStringBase<char>& name, s32 alignment,
                                           Heap* parent, bool enableLock); // 0x002F6FC0 | manual:ref-verified
    // the blocks behind the heap object, aligned (with isWithBlockNum one alignment step more if the
    // start is aligned already); registers the heap with parent (name is ours)
    void initialize_(s32 alignment, bool isWithBlockNum, Heap* parent); // 0x0012CE8C
    virtual ~UnitHeap(); // 0x00561D1C slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00561CEC slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074FC68 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return Heap::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074FC08 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009835E4 (guard 0x009835E0)
        static const RuntimeTypeInfo::Derive<Heap> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void destroy(); // 0x00561AA0 slot 0x10
    virtual u32 adjust(); // 0x00561A98 slot 0x14
    virtual void* tryAlloc(u32 size, s32 alignment); // 0x00561BD0 slot 0x18
    virtual void free(void* ptr); // 0x00561A0C slot 0x1C
    virtual void* resizeFront(void* ptr, u32 newSize); // 0x00561A00 slot 0x20
    virtual void* resizeBack(void* ptr, u32 newSize); // 0x005619F8 slot 0x24
    virtual void freeAll(); // 0x00561B10 slot 0x2C
    virtual const void* getStartAddress() const; // 0x0074FC00 slot 0x30
    virtual const void* getEndAddress() const; // 0x0074FBF0 slot 0x34
    virtual u32 getSize() const; // 0x0074FD60 slot 0x38
    virtual u32 getFreeSize() const; // 0x0074FBD8 slot 0x3C
    virtual u32 getMaxAllocatableSize(s32 alignment) const; // 0x0074FC54 slot 0x40
    virtual bool isInclude(const void* ptr) const; // 0x0074FFCC slot 0x44
    virtual bool isFreeable() const; // 0x0074FBD0 slot 0x48
    virtual bool isResizable() const; // 0x0074FBE0 slot 0x4C
    virtual bool isAdjustable() const; // 0x0074FBE8 slot 0x50
    virtual void dump() const; // 0x0074FD1C slot 0x54
    virtual void dumpYAML(Stream* stream, s32 indent) const; // 0x0074FD68 slot 0x58
    virtual void vf_0x5C(); // 0x00561A08 slot 0x5C | virtual slot, introduced by sead::Heap

    // the free blocks as a list through their first words (inline; name is ours)
    void buildFreeList_()
    {
        u32 num = mAreaSize / mBlockSize;
        mFreeSize = num * mBlockSize;
        mFreeListHead = mAreaPtr;
        s32 step = static_cast<s32>(mBlockSize) / 4;
        u32* block = static_cast<u32*>(mAreaPtr);
        for (s32 i = 0; i < static_cast<s32>(num) - 1; i++) {
            block[i * step] = reinterpret_cast<u32>(&block[(i + 1) * step]);
        }
        block[(num - 1) * step] = 0;
        mFirstBlock = mAreaPtr;
    }

    u32 mBlockSize;      // 0x74
    void* mAreaPtr;      // 0x78, the first block
    u32 mAreaSize;       // 0x7C
    u32 mFreeSize;       // 0x80
    void* mFreeListHead; // 0x84
    void* mFirstBlock;   // 0x88
};
ASSERT_SIZE(UnitHeap, 0x8C);
} // namespace sead
